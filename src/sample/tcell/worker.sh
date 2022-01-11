;;; Copyright (c) 2009-2016 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
;;; All rights reserved.

;;; Redistribution and use in source and binary forms, with or without
;;; modification, are permitted provided that the following conditions
;;; are met:
;;; 1. Redistributions of source code must retain the above copyright
;;;    notice, this list of conditions and the following disclaimer.
;;; 2. Redistributions in binary form must reproduce the above copyright
;;;    notice, this list of conditions and the following disclaimer in the
;;;    documentation and/or other materials provided with the distribution.

;;; THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
;;; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;;; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;;; ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
;;; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;;; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
;;; OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
;;; HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
;;; LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
;;; OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
;;; SUCH DAMAGE.

;;;; Compile-time options
;;; Debug print
(%defconstant VERBOSE 1)
(%include "dprint.sh")

;;; Support MPI?
(%defconstant USEMPI 1)

;;; Implementation of nested functions
(%ifndef* NF-TYPE
  (%defconstant NF-TYPE GCC))           ; one of GCC LW-SC CL-SC XCC XCCCL

;;; Uncomment to use setaffinity to bind CPU cores to workers
;; (%defconstant USE-AFFINITY SCHED)    ; for Linux
;; (%defconstant USE-AFFINITY PBIND)    ; for SunOS

(c-exp "#include<sys/time.h>")

;;; 

;;; Sizes
(%defconstant BUFSIZE 1280)             ; buffer size for a command string from an external node
(%defconstant MAXCMDC 4)                ; maximum number of arguments of a command incl. a command name
(%defconstant ARG-SIZE-MAX 16)          ; maximum length of each argument
(%defconstant TASK-LIST-LENGTH (* 4 65536)) ; maximum size of a task stack and a subtask stack for each worker
(%defconstant TASK-MAX 256)             ; maximum number of the kinds of user defined tasks.
(%defconstant DUMMY-SIZE 1111)          ; size of padding for preventing false-sharing

(%defconstant DELAY-MAX (* 100 1000 1000))  ; max sleeping time before sending treq when receiving none [nsec]

;; NOTE: this BUSYWAIT implementation is incorrect now
;; Uncomment this to let worker wait for a reply to a treq by busywait rather than condwait
;; (%defconstant BUSYWAIT)

;; Declarations for external communication functionalities
;;(%cinclude "sock.h" (:macro))
(c-exp "#include \"sock.h\"")

;;;; Declarations
;; Special elements for address.
;; Non-negative numbers are uesd for worker-ID and node-ID
(def (enum addr) (ANY -3) (PARENT -4) (FORWARD -5) (TERM -99))
;; External or Internal
(def (enum node) INSIDE OUTSIDE)
;; Commands
(def (enum command)
    TASK RSLT TREQ NONE RACK
    BCST BCAK STAT VERB EXIT CNCL WRNG)
;; Strings corresponding to the commands above.
(static cmd-strings (array (ptr char))
  (array "task" "rslt" "treq" "none" "rack"
         "bcst" "bcak" "stat" "verb" "exit" "cncl" "wrng" 0))

;; How to determine the recipient of "treq any" (random or in-order)
(def (enum choose) CHS-RANDOM CHS-ORDER)
;; consistent with (enum choose)
(static choose-strings (array (ptr char))
  (array "CHS-RANDOM" "CHS-ORDER"))
(%defconstant NKIND-CHOOSE 2)           ; # of kinds of (enum choose)

;; Kind of affinity
(def (enum Affinity) COMPACT SCATTER SHAREDMEMORY)

;; A send msg struct
(def (struct send-block)
  (def buf (ptr char))
  (def len int)
  (def size int)
  (def rank int))

;; A message transferred among workers.
(def (struct cmd)
  (def w (enum command))                ; kind of command
  (def c int)                           ; # arguments including the command itself
  (def node (enum node))                ; internal or external message
  (def v (array (enum addr) MAXCMDC ARG-SIZE-MAX)) 
					; v[i]: i-th argument of the command
                                        ; Each argument is an array of (enum addr)|(int>=0)
					; that ends with TERM
  )

;; The body of task, rslt, and bcst.
(def (struct cmd-list)
  (def cmd (struct cmd))             ; corresponding command object
  (def body (ptr void))              ; task object
  (def task-no int)                  ; kind of task
  (def next (ptr (struct cmd-list)))
  )

(decl (struct task))
(decl (struct thread-data))

;;; Arrays of task/result sender/receiver methods.
;;; Each method body is defined by a Tascell programmer or generated by Tascell compiler.
(extern-decl task-doers
      (array (ptr (fn void (ptr (struct thread-data)) (ptr void))) TASK-MAX))
(extern-decl task-senders
      (array (ptr (csym::fn void (ptr void))) TASK-MAX))
(extern-decl task-receivers
      (array (ptr (csym::fn (ptr void))) TASK-MAX))
(extern-decl rslt-senders
      (array (ptr (csym::fn void (ptr void))) TASK-MAX))
(extern-decl rslt-receivers
      (array (ptr (csym::fn void (ptr void))) TASK-MAX))

;;;; Worker local storage
;;; Declaration of the struct for worker local storage objects.
;;; Members are defined in each Tascell program.
(decl (struct worker-data))
;;; Initialize worker local stroage objects.
;;; Function body is defined in each Tascell program.
(decl (csym::worker-init) (csym::fn void (ptr (struct thread-data))))

;;;; Status of a task being executed by the owner worker
(def (enum task-stat)
  TASK-ALLOCATED   ; Allocated in task stack but uninitialized
  TASK-INITIALIZED ; Entry object is initialized
  TASK-STARTED     ; The task is running
  TASK-DONE        ; The task is completed
  TASK-NONE        ; Sent treq for ALLOCATED entry but received none
  TASK-SUSPENDED)  ; The task is suspended (due to waiting the result of a subtask)
(static task-stat-strings (array (ptr char))
  (array "TASK-ALLOCATED" "TASK-INITIALIZED" "TASK-STARTED"
	 "TASK-DONE" "TASK-NONE" "TASK-SUSPENDED"))
;; -(send treq)-> ALLOCATED -(receive task)-> INITIALIZED --> STARTED --> DONE -->
;;                 ^  |receive none                           ^     |receive the result
;;      resend treq|  V                 wait result of subtask|     V
;;                 NONE                                      SUSPENDED

;;;; Status of a subtask sent to another worker
(def (enum task-home-stat)
  TASK-HOME-ALLOCATED    ; Allocated to request queue, or then moved to subtask stack but uninitialized
  TASK-HOME-INITIALIZED  ; Initialized in subtask stack
  TASK-HOME-DONE         ; Completed (received and handled the result)
  TASK-HOME-EXCEPTION    ; Completed with an exception
  TASK-HOME-ABORTED      ; Aborted by a cancellation message
)
(static task-home-stat-strings (array (ptr char))
  (array "TASK-HOME-ALLOCATED" "TASK-HOME-INITIALIZED" "TASK-HOME-DONE"
	 "TASK-HOME-EXCEPTION" "TASK-HOME-ABORTED"))

;;;; The reason for the abnormal exit.
(def (enum exiting-rsn)
  EXITING-NORMAL         ; normal exit
  EXITING-EXCEPTION      ; exiting due to an exception
  EXITING-CANCEL         ; exiting due to a cancellation
  EXITING-SPAWN          ; temporary exiting to spawning a task
  )
(static exiting-rsn-strings (array (ptr char))
  (array "EXITING-NORMAL" "EXITING-EXCEPTION" "EXITING-CANCEL" "EXITING-SPAWN"))


(PROF-CODE
;;; Kinds of time counter (for profiling)
(%defconstant NKIND-TCOUNTER 10)
(def (enum tcounter)
  TCOUNTER-INIT          ; before execution
  TCOUNTER-EXEC          ; task execution time
  TCOUNTER-SPWN          ; task spawning time
  TCOUNTER-WAIT          ; waiting result (does not include time for stealing back)
  TCOUNTER-EXCP          ; exiting due to propagating an exception
  TCOUNTER-EXCP-WAIT     ; waiting result during propagating an exception
  TCOUNTER-ABRT          ; exiting due to abortion
  TCOUNTER-ABRT-WAIT     ; waiting result during abortion
  TCOUNTER-TREQ-BK       ; time for task request for stealing back
  TCOUNTER-TREQ-ANY      ; time for task request when having no task
)
(static tcounter-strings (array (ptr char))
  (array "TCOUNTER-INIT" "TCOUNTER-EXEC" "TCOUNTER-SPWN"
	 "TCOUNTER-WAIT" "TCOUNTER-EXCP" "TCOUNTER-EXCP-WAIT"
	 "TCOUNTER-ABRT" "TCOUNTER-ABRT-WAIT"
	 "TCOUNTER-TREQ-BK" "TCOUNTER-TREQ-ANY"))

;;; Kinds of events (for profiling)
(%defconstant NKIND-EV 12)
(def (enum event)
  EV-SEND-TASK-INSIDE                   ; task send to a worker in the same node (aux: recipient)
  EV-SEND-TASK-OUTSIDE                  ; task send to a worker in an external node (aux: recipient)
  EV-STRT-TASK-INSIDE                   ; start task from the same node (aux: task sender)
  EV-STRT-TASK-OUTSIDE                  ; start task from an exernal node (aux: task sender)
  EV-RSLT-TASK-INSIDE                   ; normally finish task from the same node (aux: task sender)
  EV-RSLT-TASK-OUTSIDE                  ; normally finish task from an external node (aux: rslt recipient)
  EV-EXCP-TASK-INSIDE                   ; finish task from the same node with an exception (aux: task sender)
  EV-EXCP-TASK-OUTSIDE                  ; finish task from an external node with an exception (aux: task sender)
  EV-ABRT-TASK-INSIDE                   ; aborted task from the same node (aux: rslt recipient)
  EV-ABRT-TASK-OUTSIDE                  ; aborted task from an external node (aux: rslt recipient)
  EV-SEND-CNCL-INSIDE                   ; send cncl to a worker in the same node (aux: recipient)
  EV-SEND-CNCL-OUTSIDE                  ; send cncl to a worker in an external node (aux: recipient)
  )
(static ev-strings (array (ptr char))
  (array "EV-SEND-TASK-INSIDE"
         "EV-SEND-TASK-OUTSIDE"
         "EV-STRT-TASK-INSIDE"
         "EV-STRT-TASK-OUTSIDE"
         "EV-RSLT-TASK-INSIDE"
         "EV-RSLT-TASK-OUTSIDE"
         "EV-EXCP-TASK-INSIDE"
         "EV-EXCP-TASK-OUTSIDE"
         "EV-ABRT-TASK-INSIDE"
         "EV-ABRT-TASK-OUTSIDE"
         "EV-SEND-CNCL-INSIDE"
         "EV-SEND-CNCL-OUTSIDE"
         ))

;;; Obj types of auxiliary data
(def (enum obj-type)
  OBJ-NULL
  OBJ-INT
  OBJ-ADDR
  OBJ-PADDR
)
(def (union aux-data-body)
  (def aux-int long)
  (def aux-addr (array (enum addr) ARG-SIZE-MAX))
  (def aux-paddr (ptr (enum addr)))
  )
(def (struct aux-data)
  (def type (enum obj-type))
  (def body (union aux-data-body))
  )
)   ; end of PROF-CODE


;; Entry in the task stack of a worker
(def (struct task)
  (def stat (enum task-stat))       ; task status
  (def next (ptr (struct task)))    ; next task (toward the bottom)
  (def prev (ptr (struct task)))    ; previous task (toward the top)
  (def task-no int)                 ; kind of the task
  (def body (ptr void))             ; task object
  (def ndiv int)                    ; # of task division
  (def cancellation int)            ; # of partial cancellation flags
  (def rslt-to (enum node))         ; task sender (= result recipient) is INSIDE/OUTSIDE of this node
  (def rslt-head (array (enum addr) ARG-SIZE-MAX)))
					; address of task sender (= result recipient)
					; including the subtask ID in the sender worker

;; Entry in the request queue or subtask stack of a worker
(def (struct task-home)
  (def stat (enum task-home-stat))      ; status
  (def id int)                          ; ID (unique in each worker)
  (def exception-tag int)               ; thrown exception value (when stat is TASK-HOME-EXCEPTION)
  (def msg-cncl int)                    ; a request flag for a cncl message
					; 1: a cncl message would be sent for this subtask
					; 2: the cncl message is already sent
  (def waiting-head (array (enum addr) ARG-SIZE-MAX))
                                        ; for stealing-back treq, the task head of which
                                        ; the requester is waiting for the result
  (def owner (ptr (struct task)))       ; the task that spawned this subtask
  (def eldest (ptr (struct task-home))) ; the task-home that is first spawned
					; in a parallel region
  (def task-no int)                     ; task number (corresponds to a task function)
  (def req-from (enum node))            ; where to send this subtask (INSIDE or OUTSIDE)
  (def task-head (array (enum addr) ARG-SIZE-MAX))
                                        ; the address of the worker which this task is sent to
                                        ; (referred when sending stealing back "treq" or "rack")
  (def next (ptr (struct task-home)))   ; link to the next (older) task-home
  (def prev (ptr (struct task-home)))   ; link to the prev (newer) task-home
  (def body (ptr void))                 ; task object
  )

(def (struct thread-data)
  (def id int)                          ; worker ID
  (def pthr-id pthread-t)               ; pthread assigned to the worker
  (def req (ptr (struct task-home)))    ; request to check task request queue and spawn tasks if needed
  (def req-cncl int)                    ; request to check subtask stack and send cncl messages if needed
  (def w-rack int)                      ; # of rack messages to be received
  (def w-none int)                      ; # of none messages to be received
  (def ndiv int)                        ; # of division of the task being executed by this worker
  (def probability double)              ; probability of accepting a task request
  (def last-treq int)                   ; the last recipient of internal "treq any"
  (def last-choose (enum choose))       ; the last used strategy for deciding recipient of internal "treq any"
  (def random-seed1 double)             ; random seed 1 for deciding recipient of treq any
  (def random-seed2 double)             ; random seed 2 for deciding recipient of treq any
  (def random-seed-probability (array unsigned-short 3))
                                        ; random seeds for probability guard
  (def task-free (ptr (struct task)))   ; free list for task stack
  (def task-top (ptr (struct task)))    ; task stack (tasks to be/being/have been executed by this worker)
  (def treq-free (ptr (struct task-home))) ; free list for both request queue and subtask stack
  (def treq-top (ptr (struct task-home)))  ; stack of subtasks to be initialized (corresponds accepted "treq")
  (def sub (ptr (struct task-home)))       ; stack of initialized subtasks (corresponds already sent "task")
  (def mut pthread-mutex-t)             ; mutex
  (def rack-mut pthread-mutex-t)        ; mutex for w-rack
  (def cond pthread-cond-t)             ; condition variable for notifying task/none messages
  (def cond-r pthread-cond-t)           ; condition variable for notifying rslt messages
  (def wdptr (ptr void))                ; worker local storage object
  (def w-bcak int)                      ; # of bcak messages to be recieved
  (def exiting (enum exiting-rsn))      ; the reason for abnormal exiting
  (def exception-tag int)               ; the exception tag to be catched
  (PROF-CODE
   ;; time counter (for profiling)
   (def tcnt-stat (enum tcounter))           ; the last state by tcounter-change-state
   (def tcnt (array double NKIND-TCOUNTER))  ; total time of each state
   (def tcnt-tp (array (struct timeval) NKIND-TCOUNTER))
					; start time of each state
   (def tc-aux (struct aux-data))       ; auxiliary data for time chart
   ;; event (for profiling)
   (def ev-cnt (array int NKIND-EV))    ; number of events
   ;; time chart / event output
   (def fp-tc (ptr FILE))               ; file pointer for time chart data output
   )
  ;; dummy
  (def dummy (array char DUMMY-SIZE))   ; padding for preventing false sharing
  )

;;;; NOTE: this functionality (on-demand data request) is incomplete now.
;;;; Declarations for on-demand data communications.
;; Status of data elements
(def (enum DATA-FLAG) DATA-NONE DATA-REQUESTING DATA-EXIST)

;; Parameters of dreq-handler() (entry point of dreq handler thread)
(def (struct dhandler-arg)
  (def data-to (enum node))                   ; whether data requester is INSIDE or OUTSIDE of this node
  (def head (array (enum addr) ARG-SIZE-MAX)) ; address of data requester
  (def dreq-cmd (struct cmd))           ; template of dreq for requesting to parent (for DATA-NONE data)
  (def dreq-cmd-fwd (struct cmd))       ; template of dreq for requesting to parent (for DATA-REQUESTING data)
  (def start int)                       ; requesting data range (start)
  (def end int)                         ; requesting data range (end)
  )

;;;; Declarations of functions in worker.sc
(decl (csym::make-and-send-task thr cur task-no body eldest-p)
      (csym::fn (ptr (struct task-home)) (ptr (struct thread-data)) (ptr (struct task-home)) int (ptr void) int))
(decl (wait-rslt thr cur stback) (fn (ptr void) (ptr (struct thread-data)) (ptr (struct task-home)) int))
(decl (csym::broadcast-task thr task-no body)
      (csym::fn void (ptr (struct thread-data)) int (ptr void)))
  
(decl (csym::proto-error str pcmd) (csym::fn void (ptr (const char)) (ptr (struct cmd))))
(decl (csym::read-to-eol) (csym::fn void void))

(decl (csym::init-data-flag) (csym::fn void int))

(decl (csym::guard-task-request) (csym::fn void (ptr (struct thread-data)) (ptr (struct task-home))))
(decl (csym::guard-task-request-prob) (csym::fn int (ptr (struct thread-data)) (ptr (struct task-home)) double))
(decl (csym::guard-node-task-request) (csym::fn int (ptr (struct thread-data)) (ptr (struct task-home)) int))
(decl (csym::recv-rslt) (csym::fn void (ptr (struct cmd)) (ptr void)))
(decl (csym::recv-task) (csym::fn void (ptr (struct cmd)) (ptr void)))
(decl (csym::recv-treq) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-rack) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-none) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-back) (csym::fn void (ptr (struct cmd))))
(decl (csym::print-task-list task-top name) (csym::fn void (ptr (struct task)) (ptr char)))
(decl (csym::print-task-home-list treq-top name) (csym::fn void (ptr (struct task-home)) (ptr char)))
(decl (csym::print-thread-status thr) (csym::fn void (ptr (struct thread-data))))
(decl (csym::print-status) (csym::fn void (ptr (struct cmd))))
(decl (csym::set-verbose-level) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-exit) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-bcst) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-bcak) (csym::fn void (ptr (struct cmd))))
(decl (csym::recv-cncl) (csym::fn void (ptr (struct cmd))))

(PROF-CODE
 (decl (csym::initialize-tcounter) (fn void (ptr (struct thread-data))))
 (decl (csym::tcounter-start)
     (fn void (ptr (struct thread-data)) (enum tcounter)))
 (decl (csym::tcounter-end)
     (fn void (ptr (struct thread-data)) (enum tcounter)))
 (decl (csym::tcounter-change-state)
     (fn (enum tcounter) (ptr (struct thread-data)) (enum tcounter)
	 (enum obj-type) (ptr void)))
 (decl (csym::initialize-evcounter) (fn void (ptr (struct thread-data))))
 (decl (csym::evcounter-count)
     (fn int (ptr (struct thread-data)) (enum event) (enum obj-type) (ptr void)))
 (decl (csym::show-counters) (fn void))
 (decl (csym::finalize-tcounter) (fn void))
 )

;;;; Declarations of functions in cmd-serial.sc
(decl (csym::serialize-cmdname buf w) (fn int (ptr char) (enum command)))
(decl (csym::deserialize-cmdname buf str) (fn int (ptr (enum command)) (ptr char)))
(decl (csym::serialize-arg buf arg) (fn int (ptr char) (ptr (enum addr))))
(decl (csym::deserialize-addr str) (fn (enum addr) (ptr char)))
(decl (csym::deserialize-arg buf str) (fn int (ptr (enum addr)) (ptr char)))
(decl (csym::serialize-cmd buf pcmd) (fn int (ptr char) (ptr (struct cmd))))
(decl (csym::deserialize-cmd pcmd str) (fn int (ptr (struct cmd)) (ptr char)))
(decl (csym::copy-address dst src) (fn int (ptr (enum addr)) (ptr (enum addr))))
(decl (csym::address-equal adr1 adr2) (fn int (ptr (enum addr)) (ptr (enum addr))))

;;;; Command line options
(def (struct runtime-option)
  (def num-thrs int)                    ; # of workers
  (def sv-hostname (ptr char))          ; hostname of connecting Tascell server
                                        ; If the string is "", external messages are output to stdout
  (def port int)                        ; port # used to connect to Tascell server
  (def node-name (ptr char))            ; node name (used for debugging only)
  (def initial-task (ptr char))         ; string for arguments of initial task
  (def auto-exit int)                   ; When true, the process exits after sending external rslt message
  (def affinity int)                    ; use sched_setaffinity to assign a physical core/thread to each worker
  (def always-flush-accepted-treq int)  ; flush stealing back (accepted) treq message
  (def thread-affinity (enum Affinity)) ; worker-thread affinity
  (def cpu-num int)                     ; # cores / physical node
  (def thread-per-cpu int)              ; # threads / core
  (def verbose int)                     ; verbose level
  (PROF-CODE                            
   (def timechart-file (ptr char)))     ; postfix of timechart output file names
  )
(extern-decl option (struct runtime-option))
(extern-decl my-rank int)
(extern-decl num-procs int)
(extern-decl (csym::node-prob wid) (fn double int))
(extern-decl (csym::my-random-probability thr) (fn double (ptr (struct thread-data))))
