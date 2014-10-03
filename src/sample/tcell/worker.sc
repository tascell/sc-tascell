;;; Copyright (c) 2009-2014 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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


;;;; Tascell worker

;;; Affected by NF-TYPE (see worker.sh)
(%include "rule/nestfunc-setrule.sh")

;; Affectedy by USE-AFFINITY (see worker.sh)
(%if* (eq 'USE-AFFINITY 'SCHED)
  (c-exp "#define _GNU_SOURCE")
  (c-exp "#include<sched.h>"))
(%if* (eq 'USE-AFFINITY 'PBIND)
  (c-exp "#include <sys/types.h>")
  (c-exp "#include <sys/processor.h>")
  (c-exp "#include <sys/procset.h>"))

(c-exp "#include<stdio.h>")
(c-exp "#include<stdlib.h>")
(c-exp "#include<string.h>")
(c-exp "#include<math.h>")
(c-exp "#include<pthread.h>")
(c-exp "#include<sys/time.h>")
(c-exp "#include<getopt.h>")
(%ifdef* USEMPI (c-exp "#include <mpi.h>"))
(%cinclude "sock.h")

(%include "worker.sh")

(%if* VERBOSE
  (c-exp "#define NDEBUG"))
(c-exp "#include<assert.h>")

;;; Variables for debug information
(%ifdef* VERBOSE
         (static n-dreq-handler int 0)  ; # running dreq-handler threads
         (static n-sending-dreq int 0)  ; # dreq handlers executing send-dreq-for-required-range
         (static n-sending-data int 0)  ; # dreq handlers sending data
         (static n-waiting-data int 0)) ; # dreq handlers waiting data


(%defmacro xread (tp exp)
  `(mref (cast (ptr (volatile ,tp)) (ptr ,exp))))
;; busy wait while ,exp is satisfied
(%defmacro pthread-cond-busywait (exp pmut)
  `(begin
    (csym::pthread-mutex-unlock ,pmut)
    (while ,exp)
    (csym::pthread-mutex-lock ,pmut)
    ))


;;;; Worker threads
(def threads (array (struct thread-data) 128))
(def num-thrs unsigned-int)
;;;; Random number generator
(def random-seed1 double 0.2403703)
(def random-seed2 double 3.638732)
;;;; Start time of the first task execution
(PROF-CODE
 (def tp-strt (struct timeval)))

;; Random integer in [0, max-1]
(def (csym::my-random max pseed1 pseed2) (fn int int (ptr double) (ptr double))
  (= (mref pseed1) (+ (* (mref pseed1) 3.0) (mref pseed2)))
  (-= (mref pseed1) (cast int (mref pseed1)))
  (return (* max (mref pseed1))))
;; Random double number in [0, 1)
(def (csym::my-random-double pseed1 pseed2) (fn double (ptr double) (ptr double))
  (= (mref pseed1) (+ (* (mref pseed1) 3.0) (mref pseed2)))
  (-= (mref pseed1) (cast int (mref pseed1)))
  (return (mref pseed1)))
;; Random double number in [0, 1) (for probability)
(def (csym::my-random-probability thr) (fn double (ptr (struct thread-data)))
  (def d double (csym::erand48 thr->random-seed-probability))
  ;; (csym::fprintf stderr "random(%d): %lf~%" thr->id d)
  (return d))


;;; Command-line options
(def option (struct runtime-option))


(def (systhr-create p-tid start-func arg)
    (fn int (ptr pthread-t) (ptr (fn (ptr void) (ptr void))) (ptr void))
  (def status int 0)
  (def tid pthread-t)
  (def attr pthread-attr-t)
  (if (not p-tid) (= p-tid (ptr tid)))
  
  (csym::pthread-attr-init (ptr attr))
  (= status (csym::pthread-attr-setscope (ptr attr) PTHREAD-SCOPE-SYSTEM))
  (if (== status 0)
      (= status (pthread-create p-tid (ptr attr) start-func arg))
    (= status (pthread-create p-tid 0          start-func arg)))
  (return status))

(def (csym::mem-error str) (csym::fn void (ptr (const char)))
  (csym::fputs str stderr)
  (csym::fputc #\Newline stderr)
  (csym::exit 1)
  )

;;; Get the current absolute time [msec]
(def (csym::get-universal-real-time) (csym::fn int)
  (def now (struct timeval))
  (csym::gettimeofday (ptr now) 0)
  (return (+ (* 1000 1000 now.tv-sec) now.tv-usec)))

;;; Get diff time between two timevals (tp1-tp2) as a double value.
(def (csym::diff-timevals tp1 tp2)
    (fn double (ptr (struct timeval)) (ptr (struct timeval)))
  (return (+ (- tp1->tv-sec tp2->tv-sec)
	     (* 0.000001 (- tp1->tv-usec tp2->tv-usec)))))

;;; Output the error message (str) to stderr
(def (csym::proto-error str pcmd) (csym::fn void (ptr (const char)) (ptr (struct cmd)))
  (def i int)
  (def buf (array char BUFSIZE))
  (csym::serialize-cmd buf pcmd)
  (csym::fprintf stderr "(%d): %s> %s~%" (csym::get-universal-real-time) str buf))

;; The lock for the send channel to external nodes (Tascell server)
(def send-mut pthread-mutex-t)

;; The socket for communications with the Tascell server.
;; sv-socket<0 indicates external communications are done via stdin/out
(def sv-socket int)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Auxiliary functions for external input/output

;; Ignore until an end of line/file
(def (csym::read-to-eol) (csym::fn void void)
  (def c int)
  (while (!= EOF (= c (csym::receive-char sv-socket)))
                                        ;(!= EOF (= c (csym::getc stdin)))
    (if (== c #\Newline) (break))))

(def (csym::write-eol) (csym::fn void void)
  (csym::send-char #\Newline sv-socket))

(def (csym::flush-send) (csym::fn void void)
  (if (< sv-socket 0) (csym::fflush stdout)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; Receive an external message and store it into buf.
;;; Then interpret (deserizlize) the string and set the result to cmd-buf.

;; the receive buffers
(def buf (array char BUFSIZE))
(def cmd-buf (struct cmd))

(def (csym::read-command) (csym::fn (ptr (struct cmd)))
  (def b (ptr char) buf)
  (def cp (ptr char) NULL)

  (= cp (csym::receive-line b BUFSIZE sv-socket))
  (if cp
    (begin
      (= cmd-buf.node OUTSIDE)
      (DEBUG-PRINT 1 "(%d): RECEIVED> %s" (csym::get-universal-real-time) b)
      (csym::deserialize-cmd (ptr cmd-buf) b)
      (return (ptr cmd-buf)))
    (begin
      (DEBUG-PRINT 1 "(%d): RECEIVED> (failed)" (csym::get-universal-real-time))
      (return NULL))))

;;; Send cmd to an external node (Tascell server)
;;; The body of task/rslt/bcst is also sent using task-senders[task-no]
;;; (task-senders[] are user-defined functinos)
(def send-buf (array char BUFSIZE))
(def (csym::send-out-command pcmd body task-no)
    (csym::fn void (ptr (struct cmd)) (ptr void) int)
  (def ret int)
  (def w (enum command))
  (= w pcmd->w)
  ;; <--- sender-lock <---
  (csym::pthread-mutex-lock (ptr send-mut))
  ;; Send command string
  (csym::serialize-cmd send-buf pcmd)
  (csym::send-string send-buf sv-socket)
  (csym::write-eol)
  ;; Send the body of task/rslt/bcst
  (cond
   (body
    (cond
     ((or (== w TASK) (== w BCST))
      ((aref task-senders task-no) body)
      (csym::write-eol))
     ((== w RSLT)
      ((aref rslt-senders task-no) body)
      (csym::write-eol)
      )))
   ((== w DATA)
    ;; data-mut has been already acquired
    (csym::data-send (aref pcmd->v 1 0) (aref pcmd->v 1 1))
    (csym::write-eol)))

  (csym::flush-send)
  (csym::pthread-mutex-unlock (ptr send-mut))
  ;; ---> sender-lock --->
  (if (and (== w RSLT) option.auto-exit)
      (begin
	(PROF-CODE
	 (let ((i int) (thr (ptr (struct thread-data))) (fp (ptr FILE)))
	   (for ((= i 0) (< i num-thrs) (inc i))
	     (= thr (+ threads i))
	     (csym::tcounter-change-state thr TCOUNTER-INIT OBJ-NULL 0)
	     (if thr->fp-tc
		 (begin
		   (= fp thr->fp-tc)
		   (= thr->fp-tc 0)
		   (csym::fclose fp)))))
	 (csym::show-counters))
	(csym::exit 0)))
  )

;;; Take cmd and call the function corresponding to its command name.
;;; For task/rslt command from a worker in the same node,
;;; the argument "body" is a pointer of task object.
;;; (The worker will receive the body later for external task/rslt.)
(def (csym::proc-cmd pcmd body) (csym::fn void (ptr (struct cmd)) (ptr void))
  (def w (enum command))
  (= w pcmd->w)
  (switch w
   (case TASK) (csym::recv-task pcmd body) (break)
   (case RSLT) (csym::recv-rslt pcmd body) (break)
   (case TREQ) (csym::recv-treq pcmd) (break)
   (case NONE) (csym::recv-none pcmd) (break)
   (case RACK) (csym::recv-rack pcmd) (break)
   (case DREQ) (csym::recv-dreq pcmd) (break)
   (case DATA) (csym::recv-data pcmd) (break)
   (case BCST) (csym::recv-bcst pcmd) (break)
   (case LEAV) (csym::recv-leav pcmd) (break)
   (case LACK) (csym::recv-lack pcmd) (break)
   (case ABRT) (csym::recv-abrt pcmd) (break)
   (case CNCL) (csym::recv-cncl pcmd) (break)
   (case BCAK) (csym::recv-bcak pcmd) (break)
   (case STAT) (csym::print-status pcmd) (break)
   (case VERB) (csym::set-verbose-level pcmd) (break)
   (case EXIT) (csym::recv-exit pcmd) (break)
   (default) (csym::proto-error "wrong cmd" pcmd) (break))
  )

;;; Send a command to another worker.
;;; For a task/rslt command, "body" is a pointer to a task object.
(def (csym::send-command pcmd body task-no) (csym::fn void (ptr (struct cmd)) (ptr void) int)
  (if (== pcmd->node INSIDE)
      (begin         ; An internal command is executed by the sender worker directly.
       (DEBUG-STMTS 3 (if (or (>= option.verbose 4)
                              (and (!= TREQ pcmd->w) (!= NONE pcmd->w)))
                          (csym::proto-error "INSIDE" pcmd)))
       (csym::proc-cmd pcmd body))
    (begin
      (DEBUG-STMTS 1 (csym::proto-error "OUTSIDE" pcmd))
      (csym::send-out-command pcmd body task-no) ; Send to external node
      )
    ))


;;; Flush the once accepted task request pointed by "(mref p-hx)"
;;; by sending back "none" and removing the treq entry from the thr's treq-top list.
;;; p-hx is the ponter to the "next" pointer in the previous treq entry.
;;; (or the pointer to the thr->treq-top when the "(mref p-hx)" is the top entry.)
;;; The lock for "thr" must have been acquired.
(def (csym::flush-treq-with-none-1 thr p-hx)
    (csym::fn void (ptr (struct thread-data)) (ptr (ptr (struct task-home))))
  (def hx (ptr (struct task-home)) (mref p-hx))
  (def rcmd (struct cmd) (struct ((fref-this c) 1) ((fref-this w) NONE)
                                 ((fref-this node) hx->req-from))) ; external or internal
  (csym::copy-address (aref rcmd.v 0) hx->task-head)
  (csym::send-command (ptr rcmd) 0 0)
  ;; Remove the top of the task request stack
  (= (mref p-hx) hx->next) 
  ;; Add the removed cell to the free list
  (= hx->next thr->treq-free)
  (= thr->treq-free hx)
  )

;;; Tascell user function:
;;; Flush the top entry of thr's once accepted task requests
;;; by sending back "none" and removing the treq entry from the thr's treq-top list.
;;; The lock for "thr" must have been acquried.
(def (csym::guard-task-request thr) (csym::fn void (ptr (struct thread-data)))
  (csym::flush-treq-with-none-1 thr (ptr thr->treq-top)))

;;; Tascell user function:
;;; Same as guard-task-request but it does nothing with the probability "1-prob"
(def (csym::guard-task-request-prob thr prob) (csym::fn int (ptr (struct thread-data)) double)
  (if (>= prob (csym::my-random-probability thr))
      (begin
        (csym::flush-treq-with-none-1 thr (ptr thr->treq-top))
        (return 1))
    (return 0)))

;;; Called by recv-exec-send and wait-rslt.
;;; The lock for 'thr' must have been acquried.
;;; Flushes treq messages that have been once accepted, and sends none messages for them.
;;; Stealing-back treq is not be flushed, unless the task of which the requester waiting
;;; for the result of equals to the specified by rslt-head and rslt-to
(def (csym::flush-treq-with-none thr rslt-head rslt-to)
    (csym::fn void (ptr (struct thread-data)) (ptr (enum addr)) (enum node))
  (def pcur-hx (ptr (ptr (struct task-home))) (ptr thr->treq-top)) ; ref to task-home link to be updated
  (def hx (ptr (struct task-home)))
  (def flush int)
  (def ignored int 0)
  (def flushed-any-treq int 0)
  (def flushed-stealing-back-head (ptr (enum addr)) 0)
  (while (= hx (mref pcur-hx))
    (cond 
     ((or option.always-flush-accepted-treq ; flush if the option specified
          (== TERM (aref hx->waiting-head 0))) ; flush if non-stealing-back treq
      (DEBUG-STMTS 2 (inc flushed-any-treq))
      (= flush 1))
     ((and rslt-head                    ; flush if stealing-back but specified to flush
           (== hx->req-from rslt-to)
           (csym::address-equal hx->waiting-head rslt-head))
      (DEBUG-STMTS 2
        (= flushed-stealing-back-head rslt-head)
        (= rslt-head 0))
      (= flush 1))
     (else
      (DEBUG-STMTS 2 (inc ignored))
      (= flush 0)))
    (if flush
        (csym::flush-treq-with-none-1 thr pcur-hx)
      (begin
        ;; ignores stealing back treq
        (= pcur-hx (ptr hx->next)))))
  ;; print debug message
  (DEBUG-STMTS 2
    (defs (array char BUFSIZE) buf0 buf1)
    (if (or (> flushed-any-treq 0) (> ignored 0) flushed-stealing-back-head)
        (csym::fprintf stderr "(%d): (Thread %d) flushed %d any %s and ignored %d stealing-back treqs in flush-treq-with-none~%"
          (get-universal-real-time) thr->id flushed-any-treq
          (if-exp flushed-stealing-back-head
              (exps
               (csym::serialize-arg buf1 flushed-stealing-back-head)
               (csym::sprintf buf0 "and stealing-back from %s" buf1)
               buf0)
            "")
          ignored)))
  )

;;; allocate a task in thr's task stack and return the pointer to the task.
;;; thr's lock must have been acquired before calling this function.
(def (csym::allocate-task thr) (fn (ptr (struct task)) (ptr (struct thread-data)))
  (def tx (ptr (struct task)))
  (= tx thr->task-free)
  (= tx->stat TASK-ALLOCATED)
  (if (not tx) (csym::mem-error "Not enough task memory"))
  (= thr->task-top tx)
  (= thr->task-free tx->prev)
  (return tx))

;;; Deallocate a task of the thread 'thr' in the task stack
(def (csym::deallocate-task thr) (fn void (ptr (struct thread-data)))
  (def tx (ptr (struct task)) thr->task-top)
  (= thr->task-free tx)                 ; return the space to the free lists.
  (= thr->task-top tx->next)
  (return))

;;; Set pts-dst(timespec) = ptv-src(timeval) + diff[nsec]
(def (csym::timeval-plus-nsec-to-timespec pts-dst ptv-src diff)
    (fn void (ptr (struct timespec)) (ptr (struct timeval)) long)
  (def nsec long (+ diff (* 1000 ptv-src->tv-usec)))
  (= pts-dst->tv-nsec (if-exp (> nsec 999999999)
                              (- nsec 999999999)
                              nsec))
  (= pts-dst->tv-sec (+ ptv-src->tv-sec
                        (if-exp (> nsec 999999999) 1 0)))
  )

;;; Send a treq message from the "thr" worker and wait for a reply.
;;; If a task message is returned, initialize the task in thr->task-top.
;;; If a none message is returned, send a treq message again.
;;; If a task request (stealing back) becomes unnecessary due to a rslt message,
;;; return without initializing the task.
;;; Before sending a treq, flush all task requests to the worker.
;;; Return 1 if the task is initialized.
;;; Return 0 if the task is not initialized.
;;; treq-head, req-to: recipient of the message
(def (csym::send-treq-to-initialize-task thr treq-head req-to)
    (fn int (ptr (struct thread-data)) (ptr (enum addr)) (enum node))
  (def rcmd (struct cmd))
  (def delay long 1000)     ; Waiting time after receiving none
  (def tx (ptr (struct task)) thr->task-top)

  ;; treq command
  (= rcmd.c 2)
  (= rcmd.node req-to) ; internal (INSIDE) or external (OUTSIDE)
  (= rcmd.w TREQ)
  (= (aref rcmd.v 0 0) thr->id)
  (if (and (!= req-to ANY) thr->sub)
      (begin
	;; If the task request is a stealing back, add the id of
	;; task that causes the waiting status.
	(= (aref rcmd.v 0 1) thr->sub->id)
	(= (aref rcmd.v 0 2) TERM))
    (= (aref rcmd.v 0 1) TERM))
  (csym::copy-address (aref rcmd.v 1) treq-head) ; Address of receipant

  ;; Send a treq message repeatedly until get a new task (task message)
  (do-while (!= tx->stat TASK-INITIALIZED)
    ;; Before sending treq, flushes all the once accepted treqs (except stealing-back ones)
    ;; by sending nones.
    ;; This is for preventing dead-lock by workers waiting for a none or task message
    ;; (as a reply to treq) each other.
    (csym::flush-treq-with-none thr 0 0)
    (= tx->stat TASK-ALLOCATED)
    (begin
     (csym::pthread-mutex-unlock (ptr thr->mut))
     (csym::send-command (ptr rcmd) 0 0) ; Send treq
     (csym::pthread-mutex-lock (ptr thr->mut)))
    ;; Wait for *tx being initialized in recv-task invoked by a sender worker thread
    ;; (internal task message) or the message handler (external task message)
    (loop
      ;; If the most recent spawned task has been completed,
      ;; stop getting a new task and resume the suspended task.
      (if (and (!= tx->stat TASK-INITIALIZED)
	       thr->sub
	       (or (== thr->sub->stat TASK-HOME-DONE)
                   (== thr->sub->stat TASK-HOME-EXCEPTION)
                   (== thr->sub->stat TASK-HOME-ABORTED)))
          ;; The worker sent a treq and its response may not have arrived.
	  ;; However, we can guarantee that the response is "none" because
	  ;; this treq is a stealing back and the result of the spawned task
	  ;; that caused the stealing back has been sent (and received).
	  (begin
	    ;; The worker can resume the suspended task before "none" is received.
	    ;; In this case, this worker does not send a new "treq" message until "none"
	    ;; is received. Otherwise, the worker cannot distinguish a response
	    ;; to this "treq" with a response to the new "treq".
	    ;; Thr->w-none is incremented to remember a "none" message will arrive later.
            (if (!= tx->stat TASK-NONE)
                (inc thr->w-none))
            (return 0)))
      (if (!= tx->stat TASK-ALLOCATED) (break))
      ;; Wait for tx->stat or thr->sub->stat being changed
      ;; NOTE: BUSYWAIT does not work now
      (%ifdef* BUSYWAIT
	(if thr->sub
	    (pthread-cond-busywait (and (== (xread (enum task-stat) tx->stat) TASK-ALLOCATED)
					(!= (xread (enum task-home-stat) thr->sub->stat) TASK-HOME-DONE))
				   (ptr thr->mut))
	  (pthread-cond-busywait (== (xread (enum task-stat) tx->stat) TASK-ALLOCATED)
				 (ptr thr->mut)))
	%else
	(csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut)))
      )
    (if (== tx->stat TASK-NONE)
        (begin
	  ;; When none is received, wait for a moment.
          (if 1                         ; NOTE: replace 1 to "thr->sub" to wait only when a stealing back
              (let ((t-until (struct timespec))
                    (now (struct timeval)))
                (csym::gettimeofday (ptr now) 0)
                (csym::timeval-plus-nsec-to-timespec (ptr t-until) (ptr now) delay)
                (csym::pthread-cond-timedwait (ptr thr->cond-r)
                                              (ptr thr->mut)
                                              (ptr t-until))
                (+= delay delay)        ; increase the next waiting time
                (if (> delay DELAY-MAX) (= delay DELAY-MAX)) ; limit the waiting time to DELAY-MAX
                ))
	  ;; If the most recent spawned task has been completed,
	  ;; stop getting a new task and resume the suspended task.
          (if (and thr->sub
                   (or (== thr->sub->stat TASK-HOME-DONE)
                       (== thr->sub->stat TASK-HOME-EXCEPTION)
                       (== thr->sub->stat TASK-HOME-ABORTED)))
              (return 0))))
    )
  (return 1))

;;; Invoked from worker() or wait-rslt()(stealing back).
;;; Request a new task ==> receive ==> execute ==> send the result
;;; The lock thr->mut must have been acquired when this function is called.
;;; treq-head, req-to: recipient of the task request (any or a worker address)
(def (recv-exec-send thr treq-head req-to)
    (fn void (ptr (struct thread-data)) (ptr (enum addr)) (enum node))
  (def tx (ptr (struct task)))
  (def old-ndiv int)
  (def old-probability double)
  (def rsn (enum exiting-rsn))
  (def reason int)
  (def rcmd (struct cmd))               ; for RSLT command

  ;; If there is a treq message that was sent by this worker
  ;; and the response to it have not arrived, wait for the response.
  ;; (See also the comment in send-treq-to-initialize-task())
  (while (> thr->w-none 0)
    (csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut))
    ;; If the result for the most recent suspended task has arrived,
    ;; stop requesting a new task and resume the suspended task.
    (if (and thr->sub
             (or (== thr->sub->stat TASK-HOME-DONE)
		 (== thr->sub->stat TASK-HOME-EXCEPTION)
		 (== thr->sub->stat TASK-HOME-ABORTED)))
        (return)))

  ;; Allocate task to be initialized
  (= tx (csym::allocate-task thr))
  ;; Send a "treq" and wait for the allocated task to be initizlied.
  (if (csym::send-treq-to-initialize-task thr treq-head req-to)
      (begin
	;; Execute the task
	(PROF-CODE
	 (csym::tcounter-change-state thr TCOUNTER-EXEC OBJ-NULL 0))
	(= tx->stat TASK-STARTED) ; TASK-INITIALIZED => TASK-STARTED
	(= tx->cancellation 0)    ; initialize # of cancellation flags
	(= old-ndiv thr->ndiv)
	(= old-probability thr->probability)
	(= thr->ndiv tx->ndiv)
	(= thr->probability 1.0)
	(csym::pthread-mutex-unlock (ptr thr->mut))
	(DEBUG-PRINT 1 "(%d): (Thread %d) start %d<%p> (body=%p).~%"
		     (csym::get-universal-real-time) thr->id tx->task-no tx tx->body)
	((aref task-doers tx->task-no) thr tx->body) ; Invoke the task body method
	(= rsn thr->exiting)
	(= thr->exiting EXITING-NORMAL)
	;; Set the reason for compliting the task
	(switch rsn
	  (case EXITING-NORMAL)
	  (DEBUG-PRINT 1 "(%d): (Thread %d) end %d<%p> (body=%p).~%" 
		       (csym::get-universal-real-time)
		       thr->id tx->task-no tx tx->body)
	  (= reason 0)
	  (break)
	  (case EXITING-EXCEPTION)
	  (DEBUG-PRINT 1 "(%d): (Thread %d) end %d<%p> (body=%p) with exception %d.~%" 
		       (csym::get-universal-real-time)
		       thr->id tx->task-no tx tx->body thr->exception-tag)
	  (= reason 1)
	  (break)
	  (case EXITING-CANCEL)
	  (DEBUG-PRINT 1 "(%d): (Thread %d) aborted %d<%p> (body=%p).~%" 
		       (csym::get-universal-real-time)
		       thr->id tx->task-no tx tx->body)
	  (= reason 2)
	  (break)
	  (default)
	  (csym::fprintf 
	   stderr "(%d) Warn: Thread %d ended with unexpected reason.~%"
	   (csym::get-universal-real-time) thr->id)
	  (= reason 0))
	;; Send the result to the task sender (== the recipient of the treq)
	(= rcmd.w RSLT)
	(= rcmd.c 3)
	(= rcmd.node tx->rslt-to)        ; internal or external
	(csym::copy-address (aref rcmd.v 0) tx->rslt-head)      ;[0]:recipient
	(= (aref rcmd.v 1 0) reason) (= (aref rcmd.v 1 1) TERM) ;[1]:reason
        (= (aref rcmd.v 2 0) thr->exception-tag) (= (aref rcmd.v 2 1) TERM)
                             ; [2]:exception-tag (meaningful only when [1]==1)
	(csym::send-command (ptr rcmd) tx->body tx->task-no)
	(csym::pthread-mutex-lock (ptr thr->rack-mut))
	(inc thr->w-rack) ; Increase w-rack counter. This is decreased when the worker
	                  ; receives a rack message as a reply to the rslt. While this
                          ; counter is larger than zero, the worker does not spawn a
	                  ; a new task. NOTE: the worker waiting for this rslt may send
	                  ; a stealing-back treq message to this worker, but since this
                          ; worker has sent the result, the reply to the treq should be
	                  ; "none". This w-rack mechanism realizes this. The fact w-rack=0
                          ; means no worker sends a stealing back treq to this worker.
                          ; Therefore it can spawn a task.
	(csym::pthread-mutex-unlock (ptr thr->rack-mut))
	(csym::pthread-mutex-lock (ptr thr->mut))
	(= thr->ndiv old-ndiv)
	(= thr->probability old-probability)
	))

  ;; TASK-STARTED => TASK-DONE
  (= tx->stat TASK-DONE)
  ;; Flushes all the once accepted treqs by sending nones.
  ;; Notice that stealing-back to the task of which just sent the result should be flushed, too.
  (csym::flush-treq-with-none thr tx->rslt-head tx->rslt-to)
  ;; Pop the task stack of the worker
  (csym::deallocate-task thr)
  )


;;; Assign a worker to a physical computing unit (core, HW thread, etc.)
(%if* (eq 'USE-AFFINITY 'SCHED)
  (def (csym::worker-setaffinity n) (csym::fn void int)
    (def mask cpu-set-t)
    (csym::CPU-ZERO (ptr mask))
    (csym::CPU-SET n (ptr mask))
    (if (== -1 (csym::sched-setaffinity 0 (sizeof mask) (ptr mask)))
        (begin
          (csym::perror "Failed to set CPU affinity")
          (csym::exit -1)))
    (if (>= option.verbose 1)
        (csym::fprintf stderr "Bind worker to core %d~%" n)))
  )

(%if* (eq 'USE-AFFINITY 'PBIND)
  (def (csym::worker-setaffinity pe) (csym::fn void int)
    (def pe0 int pe)
    (def p int)    
    (if (>= pe (csym::sysconf csym::-SC-NPROCESSORS-ONLN))
        (begin
         (csym::fprintf stderr "Error in worker-setaffnity: too large pe~%")
         (csym::exit -1)))
    (for ((= p 0) (< p 65536) (inc p))
      (if (and (== csym::P_ONLINE (csym::p_online p csym::P_STATUS))
               (== 0 (dec pe)))
          (break)))
    ;; (csym::fprintf stderr "Bind worker to core %d~%" pe0)
    (if (!= 0 (csym::processor-bind csym::P-LWPID csym::P-MYID p 0))
        (begin
          (csym::perror "Failed to set CPU affinity")
          (csym::exit -1)))
    (return))
  )

;;; The worker's main function
(def (worker arg) (fn (ptr void) (ptr void))
  (def thr (ptr (struct thread-data)) arg)
  (= thr->wdptr (csym::malloc (sizeof (struct thread-data))))
  (%ifdef* USE-AFFINITY
    (if option.affinity
      (csym::worker-setaffinity thr->id)))
  (csym::worker-init thr)  ; initialize app-defined worker local storage
  (csym::pthread-mutex-lock (ptr thr->mut))
  (loop
    (PROF-CODE
     (if (!= thr->tcnt-stat TCOUNTER-INIT)
	 (csym::tcounter-change-state thr TCOUNTER-TREQ-ANY OBJ-NULL 0)))
    (recv-exec-send thr (init (array (enum addr) 2) (array ANY TERM)) INSIDE))
  (csym::pthread-mutex-unlock (ptr thr->mut)))



;;;; recv-xxxx(): handler of the xxxx command.

;;; task message: a reply to a treq message that sends a spawned task
;;; task <# task divisions> <sender> <recipient> <kind of task>
(def (csym::recv-task pcmd body) (csym::fn void (ptr (struct cmd)) (ptr void))
  (def tx (ptr (struct task)))
  (def thr (ptr (struct thread-data)))
  (def id (enum addr))
  (def task-no int)
  (def len size-t)
  ;; Chceck # of arguments
  (if (< pcmd->c 4)
      (csym::proto-error "wrong-task" pcmd))
  ;; For an external task message, get the body of task by
  ;; invoking the user-defined receiver method.
  ;; (For an internal task message, body is given as the argument)
  (= task-no (aref pcmd->v 3 0))
  (if (== pcmd->node OUTSIDE)
      (begin
       (= body ((aref task-receivers task-no)))
       (csym::read-to-eol)))
  ;; Determine the task recipient worker from <recipient>
  (= id (aref pcmd->v 2 0))
  (if (not (< id num-thrs))
      (csym::proto-error "wrong task-head" pcmd))
  (= thr (+ threads id))                ; thr: worker to that the task is sent

  ;; Initialize the task.
  ;; The task entry has been allocated at the top of the task stack of the recipient
  ;; before it sends a treq message (see send-treq-to-initialize-task())
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= tx thr->task-top)                  ; tx: the top of the task stack
  (= tx->rslt-to pcmd->node)            ; set external/internal for the rslt message
					; after the task is completed
					; (the same to the value in the task message)
  (csym::copy-address tx->rslt-head (aref pcmd->v 1))
                                        ; set the address of the recipient of the result
					; (the same to <sender> of the task message)
  (= tx->ndiv (aref pcmd->v 0 0))       ; [0]: # task divisions
  (= tx->task-no task-no)               ; the kind of the task
  (= tx->body body)                     ; task object
  (= tx->stat TASK-INITIALIZED)         ; TASK-ALLOCATED => TASK-INITIALIZED
  (csym::pthread-mutex-unlock (ptr thr->mut))

  ;; Awake the worker thread sleeping to waiting for the task
  (csym::pthread-cond-broadcast (ptr thr->cond))
  )


;;; none message: a reply to a treq message to indicate a new task cannot be spawned
;;; none <recipient>
(def (csym::recv-none pcmd) (csym::fn void (ptr (struct cmd)))
  (def thr (ptr (struct thread-data)))
  (def id (enum addr))
  (def len size-t)
  (if (< pcmd->c 1) (csym::proto-error "Wrong none" pcmd))
  (= id (aref pcmd->v 0 0))
  (if (not (< id num-thrs))
      (csym::proto-error "Wrong task-head" pcmd))
  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (if (> thr->w-none 0)                ; When w-none>0, the recipient has stopped waiting
      (dec thr->w-none)                ; the response to treq
    (= thr->task-top->stat TASK-NONE)) ; TASK-ALLOCATED => TASK-NONE
  (csym::pthread-mutex-unlock (ptr thr->mut))

  ;; Awake the worker thread sleeping to waiting for the task
  (csym::pthread-cond-broadcast (ptr thr->cond))
  )


;;; Find a task-home entry whose id equals to the "id" argument
(def (csym::search-task-home-by-id id hx) (fn (ptr (struct task-home)) int (ptr (struct task-home)))
  (while (and hx (!= hx->id id))
    (= hx hx->next))
  (return hx))


;;; Set subtasks to be cancelled spawned by "owner" and newer than "eldest"
;;; If eldest is 0, all the subtasks spawned by "owner" are set to be cancelled.
;;; The mutex thr->mut must be acquired before calling this function.
;;; When there is any subtask set to be cancelled, set a cancel request flag to "thr".
;;; (This flag is detected by polling and cncl messages are sent for flagged subtasks)
;;; Returns # of subtasks that are set to be cancelled.
(def (csym::set-cancelled thr owner eldest)
    (csym::fn int (ptr (struct thread-data)) (ptr (struct task)) (ptr (struct task-home)))
  (def cur (ptr (struct task-home)))
  (def count int 0)
  (for ((= cur thr->sub) cur (= cur cur->next))
    (if (and (== cur->owner owner)
	     (== cur->stat TASK-HOME-INITIALIZED) ; currently running?
	     (== cur->msg-cncl 0))
	(begin
	  (= cur->msg-cncl 1)
	  (inc count)))
    (if (and eldest (== eldest cur->eldest))
	(break))
    )
  (if count (= thr->req-cncl 1))
  (return count))


;;; rslt message: a reply to a task message to send the result of the task
;;; rslt <recipient> <reason(0:normal,1:exception,2:abort)> <excption-tag>
(def (csym::recv-rslt pcmd body) (csym::fn void (ptr (struct cmd)) (ptr void))
  (def rcmd (struct cmd))               ; rack command
  (def thr (ptr (struct thread-data)))
  (def hx (ptr (struct task-home)))
  (def tid (enum addr))
  (def sid int)
  (def reason int)
  (def exception-tag int)
  ;; Check # of arguments
  (if (< pcmd->c 2)
      (csym::proto-error "Wrong rslt" pcmd))
  ;; Extract the recipient worker id and the task-home id from <recipient>
  ;; <recipient> = <worker-id>:<task-home-id>
  (= tid (aref pcmd->v 0 0))
  (if (not (< tid num-thrs))
      (csym::proto-error "wrong rslt-head" pcmd))
  (= sid (aref pcmd->v 0 1))
  (if (== TERM sid)
      (csym::proto-error "Wrong rslt-head (no task-home-id)" pcmd))
  (= thr (+ threads tid))
  ;; Get <reason>
  (= reason (aref pcmd->v 1 0))
  ;; Get <exception-tag> (ignored when <reason> is not 1)
  (= exception-tag (aref pcmd->v 2 0))

  (csym::pthread-mutex-lock (ptr thr->mut))
  ;; Determine the task-home entry whose id is <task-home-id>
  (if (not (= hx (csym::search-task-home-by-id sid thr->sub)))
      (csym::proto-error "Wrong rslt-head (specified task not exists)" pcmd))
  ;; If the rslt message is from external node and the task is normally exited,
  ;; receive the body of thea result by invoking the user-defined receiver method.
  ;; (for an internal rslt, the body is passed as the argument)
  (cond
   ((== pcmd->node OUTSIDE)
    ((aref rslt-receivers hx->task-no) hx->body)
    (csym::read-to-eol))
   ((== pcmd->node INSIDE)
    (= hx->body body))
   (else
    (csym::proto-error "Wrong cmd.node" pcmd)))
  ;; Make a rack message
  (= rcmd.c 1)
  (= rcmd.node pcmd->node)
  (= rcmd.w RACK)
  (csym::copy-address (aref rcmd.v 0) hx->task-head) 
					; Set the recipient address 
                                        ; (same to the recipient of the task message)

  ;; Change the status of the task-home entry 
  (cond
   ((== reason 0)
    (= hx->stat TASK-HOME-DONE))
   ((== reason 1)
    (= hx->stat TASK-HOME-EXCEPTION)
    (= hx->exception-tag exception-tag)
    (inc hx->owner->cancellation)
    (csym::set-cancelled thr hx->owner hx->eldest)
    )
   ((== reason 2)
    (= hx->stat TASK-HOME-ABORTED)))
  ;; Notify the worker waiting for the result.
  (if (== hx thr->sub)
      (begin
       (csym::pthread-cond-broadcast (ptr thr->cond-r))
       (csym::pthread-cond-broadcast (ptr thr->cond)))
    )
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::send-command (ptr rcmd) 0 0))  ; Send the rack command


;; If the worker 'thr' has the task whose return address is [<addr>,...,<ID>]x(INSIDE|OUTSIDE),
;; return the pointer to the task object. Return 0 otherwise.
(def (csym::have-task thr task-spec task-from) 
    (csym::fn (ptr (struct task)) (ptr (struct thread-data)) (ptr (enum addr)) (enum node))
  (def tx (ptr (struct task)))
  (= tx thr->task-top)
  (while tx
    (if (and (or (== tx->stat TASK-INITIALIZED)
                 (== tx->stat TASK-SUSPENDED)
                 (== tx->stat TASK-STARTED))
             (== tx->rslt-to task-from)
             (csym::address-equal tx->rslt-head task-spec))
        (return tx))
    (= tx tx->next))
  (return 0))


;;; Check if the id-th worker can accept the task request 'pcmd'
;;; If ok, the worker allocate a task-home.
(def (csym::try-treq pcmd id)
    (csym::fn int (ptr (struct cmd)) (enum addr))
  (def from-addr (ptr (enum addr)) (aref pcmd->v 0))
  (def dest-addr (ptr (enum addr)) (aref pcmd->v 1))
  (def hx (ptr (struct task-home)))
  (def thr (ptr (struct thread-data)))
  (def fail-reason int 0)
  (def avail int 0)

  (= thr (+ threads id))

  (csym::pthread-mutex-lock (ptr thr->mut))

  ;; Check whether worker can accept treq
  (csym::pthread-mutex-lock (ptr thr->rack-mut))
  (cond
   ((> thr->w-rack 0)                   ; waiting rack message (the treq sender is ill-prepared for
    (= fail-reason 1))                  ; a task message and, if received, may break data structures)
   ((not thr->task-top)                 ; having no task
    (= fail-reason 2))
   ((== (aref dest-addr 0) ANY)         ; * for 'any' request...
    (if (not (or (== thr->task-top->stat TASK-STARTED) ; the task is not prepared for being divided
                 (== thr->task-top->stat TASK-INITIALIZED)))
        (= fail-reason 3)))
   (else                                ; * for stealing-back request...
    (if (not (csym::have-task thr from-addr pcmd->node))
                                        ; the task is already finished
        (= fail-reason 4))))
  (if (and (not fail-reason) (== (aref dest-addr 0) ANY))
    (if (< thr->probability (csym::my-random-probability thr))
      (= fail-reason 5)))
  (= avail (not fail-reason))
  (csym::pthread-mutex-unlock (ptr thr->rack-mut))

  ;; (Debug) Show the reason for refusing the task request
  (DEBUG-STMTS 2
    (if (not avail)
        (let ((from-str (array char BUFSIZE))
              (buf1 (array char BUFSIZE))
              (rsn-str (array char BUFSIZE)))
          (csym::serialize-arg from-str from-addr)
          (switch fail-reason
            (case 1)
            (csym::sprintf rsn-str "w-rack=%d" thr->w-rack) (break)
            (case 2)
            (csym::strcpy rsn-str "of having no task") (break)
            (case 3)
            (csym::sprintf rsn-str "the task is %s" (aref task-stat-strings thr->task-top->stat)) (break)
            (case 4)
            (csym::serialize-arg buf1 from-addr)
            (csym::sprintf rsn-str "%s is already finished" buf1) (break)
            (case 5)
            (csym::sprintf rsn-str "of probability (%lf)" thr->probability) (break)
            (default)
            (csym::strcpy rsn-str "Unexpected reason") (break))
          (csym::fprintf 
           stderr "(%d): Thread %d refused treq from %s because %s.~%"
           (csym::get-universal-real-time) id from-str rsn-str))))

  ;; If succeeded, push a task-home entry to the requestee's request queue.
  (if avail
      (begin
	(= hx thr->treq-free)
	(if (not hx)
	    (csym::mem-error "Not enough task-home memory"))
	(= thr->treq-free hx->next)      ; Allocate an entry
	(= hx->next thr->treq-top)       ; Set the next of the new entry to the former top
	(= hx->stat TASK-HOME-ALLOCATED) ; Set the status of the new entry
	(if (== (aref dest-addr 0) ANY)
	    (= (aref hx->waiting-head 0) TERM)
	  (csym::copy-address hx->waiting-head from-addr))
	(csym::copy-address hx->task-head (aref pcmd->v 0)) ; Set the address of the recipient of the result
	(if (!= pcmd->node OUTSIDE)
	    (= hx->req-from INSIDE)
	  (= hx->req-from OUTSIDE))
	(= thr->treq-top hx)
	(= thr->req hx)
	))
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (return avail))

;;; Invoked while handling "treq any"
;;; Choose a worker to send a task request using some strategy.
(def (csym::choose-treq from-addr) (fn int (enum addr))
  (cond
   ;; treq from a worker in this node
   ((<= 0 from-addr)                   
    (let ((thr (ptr (struct thread-data)) (+ threads from-addr)))
      ;; Switch the strategy
      (= thr->last-choose (% (+ 1 thr->last-choose) NKIND-CHOOSE))
      (cond
       ((== CHS-RANDOM thr->last-choose) ; random strategy
        (return (csym::my-random num-thrs
                                 (ptr thr->random-seed1)
                                 (ptr thr->random-seed2))))
       ((== CHS-ORDER thr->last-choose)  ; in-order strategy
        (= thr->last-treq (% (+ 1 thr->last-treq) num-thrs))
        (return thr->last-treq))
       (else
        (return 0)))))
   ;; treq from a Tascell server
   ((== PARENT from-addr)
    (return (csym::my-random num-thrs (ptr random-seed1) (ptr random-seed2))))
   (else
    (return 0))))

;;; treq: a message to request a new task
;;; treq <sender> <recipient(or "ANY")> 
;;; Check whether the recipient can spawn a new task
;;; (If <recipient> is "ANY", find a worker that can spawn a task)
;;; If a task can be spawned, add a task request entry to the worker's request queue.
;;; (The worker will detect the request by polling and send a task message)
;;; If a task cannot be spawned, send a none message, or for "ANY" message from
;;; the 0-th worker in this node, forward the treq message to external nodes.
(def (csym::recv-treq pcmd) (csym::fn void (ptr (struct cmd)))
  (def rcmd (struct cmd))
  (def dst0 (enum addr))
  ;; Check # of arguments
  (if (< pcmd->c 2)
      (csym::proto-error "Wrong treq" pcmd))
  ;; Extract <recipient> from the message
  (= dst0 (aref pcmd->v 1 0))
  (cond
   ;; ANY
   ((== dst0 ANY)
    (let ((myid int) (start-id int) (d int) (id int))
      (= myid (aref pcmd->v 0 0))           ; extract <sender>
      (= start-id (csym::choose-treq myid)) ; choose the first candidate worker
      (for ((= d 0) (< d num-thrs) (inc d))
        (= id (% (+ d start-id) num-thrs))  ; the candidate worker id
        (if (and (!= pcmd->node OUTSIDE)
                 (== id myid))
            (continue))                     ; skip the treq sender
        (if (csym::try-treq pcmd id)        ; try and handle request for the id-th worker
            (begin
             (DEBUG-PRINT 2 "(%d): treq(any) %d->%d... accepted.~%"
                          (csym::get-universal-real-time) myid id)
             (break)))
        (DEBUG-PRINT 4 "(%d): treq(any) %d->%d... refused.~%"
                     (csym::get-universal-real-time) myid id)
        )
      (if (< d num-thrs)                    ; Return if the treq is accepted
          (return))))
   ;; An arbitrary worker (= stealing back request)
   (else
    (if (not (and (<= 0 dst0) (< dst0 num-thrs)))   ; Range check of <recipient>
        (csym::proto-error "Wrong task-head" pcmd))
    (if (csym::try-treq pcmd dst0)          ; try and handle request for the dst0-th worker
        (begin
         (DEBUG-STMTS 2
                      (let ((buf1 (array char BUFSIZE)))
                        (csym::fprintf stderr "(%d): treq %s->%d (stealing back)... accepted.~%"
                                       (csym::get-universal-real-time)
                                       (exps (csym::serialize-arg buf1 (aref pcmd->v 0)) buf1) dst0)))
         (return)))                         ; Return if the treq is accepted
    (DEBUG-STMTS 2
                 (let ((buf1 (array char BUFSIZE)))
                   (csym::fprintf stderr "(%d): treq %s->%d (stealing back)... refused.~%"
                                  (csym::get-universal-real-time)
                                  (exps (csym::serialize-arg buf1 (aref pcmd->v 0)) buf1) dst0))))
   )
  ;; The following is executed when no worker accepted the request
  (if (and (== dst0 ANY)                 ; <recipient> is "ANY"
           (== pcmd->node INSIDE)        ; internal treq
           (== (aref pcmd->v 0 0) 0))    ; treq from 0-th worker
      ;; Forward the treq to external nodes and return
      (begin
	(= pcmd->node OUTSIDE)
	(csym::send-command pcmd 0 0)
	(return)))
  ;; Reply with a none message
  (= rcmd.c 1)
  (= rcmd.node pcmd->node)              ; internal or external
  (= rcmd.w NONE)
  (csym::copy-address (aref rcmd.v 0) (aref pcmd->v 0))
  (csym::send-command (ptr rcmd) 0 0))


;; rack message: reply to a rslt message
;; rack <recipient>
(def (csym::recv-rack pcmd) (csym::fn void (ptr (struct cmd)))
  (def tx (ptr (struct task)))
  (def thr (ptr (struct thread-data)))
  (def id (enum addr))
  (def len size-t)
  ;; Check # of arguments
  (if (< pcmd->c 1)
      (csym::proto-error "Wrong rack" pcmd))
  (= id (aref pcmd->v 0 0))
  (if (not (< id num-thrs))
      (csym::proto-error "Wrong task-head" pcmd))
  (= thr (+ threads id))
  ;; Rack only decreases the w-rack counter of the recipient
  ;; See also the comment in recv-exec-send().
  (csym::pthread-mutex-lock (ptr thr->rack-mut))
  (dec thr->w-rack)
  (csym::pthread-mutex-unlock (ptr thr->rack-mut)))


;;;; NOTE: currentry the implementation of data-dreq mechanism is incomplete.

;; The pointer to thevarray of existence flags (initialozed in setup-data())
(def data-flags (ptr (enum DATA-FLAG)) 0)
;; The lock and condition variable for data-flags
(def data-mutex pthread-mutex-t)
(def data-cond  pthread-cond-t)

;; A built-in function of Tascell: initialize data (n: size)
(def (csym::-setup-data n) (csym::fn void int)
  (def i int)
  (def tmp (ptr (enum DATA-FLAG)))

  (if data-flags (return))
  (csym::pthread-mutex-lock (ptr data-mutex))
  ;; Initialize data-flags
  (if (not data-flags)
      (begin
       (= tmp (cast (ptr (enum DATA-FLAG)) (csym::malloc (* n (sizeof (enum DATA-FLAG))))))
       (for ((= i 0) (< i n) (inc i))
            (= (aref tmp i) DATA-NONE))
       (= data-flags tmp)))
  ;; Allocate data by invoking a user-defined method
  (csym::data-allocate n)
  (csym::pthread-mutex-unlock (ptr data-mutex))
  (return))

;; For each on-demand data element in the range [start,end), send pcmd if it is NONE and
;; pcmd-fwd if it is REQUESTING. Then, update the status of NONE elements to REQUESTING.
;; pcmd and pcmd-fwd are pointers to dreq commands to which arguments except the third
;; argument, which indicates the data range, are set.
;; If pcmd or pcmd-fwd is NULL, sending the corresponding message is skipped.
(def (csym::send-dreq-for-required-range start end pcmd pcmd-fwd)
    (csym::fn void int int (ptr (struct cmd)) (ptr (struct cmd)))
  (defs int i j)
  (csym::pthread-mutex-lock (ptr data-mutex))
  (for ((= i start) (< i end) (inc i))
    (cond
     ((== (aref data-flags i) DATA-NONE)
      (= (aref data-flags i) DATA-REQUESTING)
      (for ((= j (+ i 1)) (and (< j end) (== (aref data-flags j) DATA-NONE)) (inc j))
        (= (aref data-flags j) DATA-REQUESTING))
      ;; Send a data request (pcmd) for [i,j)
      (if pcmd
          (begin
            (= (aref pcmd->v 2 0) i)  (= (aref pcmd->v 2 1) j)
            (= (aref pcmd->v 2 2) TERM)
            (csym::send-command pcmd 0 0)))
      (= i (- j 1)))     ; Continue from the next location of the requested range
     ((== (aref data-flags i) DATA-REQUESTING)
      (for ((= j (+ i 1)) (and (< j end) (== (aref data-flags j) DATA-REQUESTING)) (inc j))
        )
      ;; Send a data request (pcmd-fwd) for [i,j)
      (if pcmd-fwd
          (begin
            (= (aref pcmd-fwd->v 2 0) i)  (= (aref pcmd-fwd->v 2 1) j)
            (= (aref pcmd-fwd->v 2 2) TERM)
            (csym::send-command pcmd-fwd 0 0)))
      (= i (- j 1)))     ; Continue from the next location of the requested range
     ))
  (csym::pthread-mutex-unlock (ptr data-mutex))
  )

;; Find the task of the tid-th worker that generated the task-home entry whose id is sid.
;; If the task is sent from external node, output its address to "head".
;; Otherwise, find the task-home entry in another worker and repeat this search step.
(def (csym::get-first-outside-ancestor-task-address head tid sid) (csym::fn int (ptr (enum addr)) int int)
  (def thr (ptr (struct thread-data)))
  (def hx (ptr (struct task-home)))
  (def ok int)
  (do-while 1
    ;; thr: the tid-th worker thread
    (= thr (+ threads tid))
    (csym::pthread-mutex-lock (ptr thr->mut))
    ;; hx: the task-home entry of the tid-th worker whose id is sid.
    (if (not (= hx (csym::search-task-home-by-id sid thr->sub)))
        (csym::fprintf stderr "Error in get-first-outside-ancestor-task-address (specified task not exists)~%"))
    ;; hx->owner: the task that generated the hx entry
    (cond
     ((not hx->owner)                   ; Error occurs if there is no parent
      (csym::fprintf stderr "error in get-first-outside-ancestor-task-address: no owner found.~%")
      (csym::print-status 0)
      (csym::exit 1))
     ((== hx->owner->rslt-to OUTSIDE)   ; If hx->owner is sent from external node, output its address
      (csym::copy-address head hx->owner->rslt-head)
      (break)))
    ;; If hx->owner is sent from a worker in the same node,
    ;; get its worker id and task-home id, and repeat the search step
    (= tid (aref hx->owner->rslt-head 0))
    (= sid (aref hx->owner->rslt-head 1))
    (csym::pthread-mutex-unlock (ptr thr->mut))
    )
  (return ok))

;; Tascell user function: request data of the range [start,end) to the parent task
(def (csym::-request-data thr start end) (csym::fn void (ptr (struct thread-data)) int int)
  (def cmd (struct cmd))
  (def tx (ptr (struct task)))
  (DEBUG-PRINT 2 "request-data: %d--%d start~%" start end)
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= tx thr->task-top)                  ; the task being executed by thr
  (csym::pthread-mutex-unlock (ptr thr->mut))
  ;; Generate a template for dreq commands
  (= cmd.w DREQ)
  (= cmd.c 3)
  ;; Sender address
  (= (aref cmd.v 0 0) 0)  (= (aref cmd.v 0 1) TERM)
  ;; Rcipient address: search ancestor tasks for the first task in an external node
  (if (== OUTSIDE tx->rslt-to)
      (csym::copy-address (aref cmd.v 1) tx->rslt-head)
    (csym::get-first-outside-ancestor-task-address
     (aref cmd.v 1) (aref tx->rslt-head 0) (aref tx->rslt-head 1)))
  (= cmd.node OUTSIDE)

  (DEBUG-STMTS 2
               (= (aref cmd.v 2 0) TERM)
               (csym::proto-error "dreq template" (ptr cmd)))
  
  ;; Send dreq commands for necessary ranges
  (csym::send-dreq-for-required-range start end (ptr cmd) 0)
  (DEBUG-PRINT 2 "request-data: %d--%d end~%" start end)
  (return)
  )

;; Tascell user function: wait until data of the range [stard,end) reach
(def (csym::-wait-data start end) (csym::fn void int int)
  (def i int)
  (DEBUG-PRINT 2 "wait-data: %d--%d start~%" start end)
  (csym::pthread-mutex-lock (ptr data-mutex))
  (for ((= i start) (< i end) (inc i))
    (while (!= (aref data-flags i) DATA-EXIST)
      (csym::pthread-cond-wait (ptr data-cond) (ptr data-mutex))))
  (csym::pthread-mutex-unlock (ptr data-mutex))
  (DEBUG-PRINT 2 "wait-data: %d--%d end~%" start end)
  )

;; Set the data-flag of the range [start,end) to DATA-EXIST
(def (csym::-set-exist-flag start end) (csym::fn void int int)
  (def i int)
  (csym::pthread-mutex-lock (ptr data-mutex))
  (for ((= i start) (< i end) (inc i))
    (= (aref data-flags i) DATA-EXIST))
  (csym::pthread-mutex-unlock (ptr data-mutex))
  )



;; The main function of a data request handler thread
(def (csym::dreq-handler parg0) (csym::fn (ptr void) (ptr void))
  (def parg (ptr (struct dhandler-arg)) parg0)
  (def start int parg->start)
  (def end int parg->end)
  (def pcmd (ptr (struct cmd)) (ptr parg->dreq-cmd))
  (def pcmd-fwd (ptr (struct cmd)) (ptr parg->dreq-cmd-fwd))
  (def data-cmd (struct cmd))
  (defs int i j)

  (%ifdef* VERBOSE (inc n-dreq-handler))
   
  #+comment
  (DEBUG-STMTS 1 (= (aref pcmd->v 2 0) TERM)
               (csym::fprintf stderr "dreq-handler: %d %d~%" start end)
               (csym::proto-error "template" pcmd))
  
  ;; Send pcmd for subranges whose flags are "NONE" (not exist in this node)
  ;; Send pcmd-fwd for for subranges whose flags are "REQUESTING" (not exist
  ;; in this node but already requesting to another node)
  (%ifdef* VERBOSE (inc n-sending-dreq))
  (csym::send-dreq-for-required-range start end pcmd pcmd-fwd)
  (%ifdef* VERBOSE (dec n-sending-dreq))
  
  ;; Ignore dreq command from a worker in the same node
  (if (== parg->data-to INSIDE)
      (begin
       (%ifdef* VERBOSE (dec n-dreq-handler))
       (return)))

  ;; Generate a template for "data" commands
  (= data-cmd.w DATA)
  (= data-cmd.c 2)
  (= data-cmd.node parg->data-to)       ; internal or external
  (csym::copy-address (aref data-cmd.v 0) parg->head) ; recipient address
  
  ;; Wait for data by cond-wait and send as "data" messages
  (%ifdef* VERBOSE (inc n-sending-data))
  (csym::pthread-mutex-lock (ptr data-mutex))
  (for ((= i start) (< i end) (inc i))
    (while (!= (aref data-flags i) DATA-EXIST)
      (%ifdef* VERBOSE (inc n-waiting-data))
      (csym::pthread-cond-wait (ptr data-cond) (ptr data-mutex))
      (%ifdef* VERBOSE (dec n-waiting-data)) )
    (for ((= j (+ i 1)) (and (< j end) (== (aref data-flags j) DATA-EXIST)) (inc j))
      )
    ;; Send a "data" message for [i,j) (the data body is sent in send-cmmand())
    (csym::assert (< i j))
    (= (aref data-cmd.v 1 0) i)  (= (aref data-cmd.v 1 1) j)
    (= (aref data-cmd.v 1 2) TERM)
    (csym::send-command (ptr data-cmd) 0 0)
    (= i (- j 1)))
  (csym::pthread-mutex-unlock (ptr data-mutex))
  (%ifdef* VERBOSE (dec n-sending-data))
  
  (csym::free parg)
  (%ifdef* VERBOSE (dec n-dreq-handler))
  (return))

;; dreq: request dreq handler of an external node to send data of the specified range
;; dreq <sender> <recipient> <range("start:end" indicating [start,end))>
(def (csym::recv-dreq pcmd) (csym::fn void (ptr (struct cmd)))
  (def tx (ptr (struct task)))
  (def tid (enum addr))
  (def sid int)
  (def parg (ptr (struct dhandler-arg)))
  (def len size-t)
  ;; Check # of arguments
  (if (< pcmd->c 3) (csym::proto-error "Wrong dreq" pcmd))
  
  ;; Set arguments for dreq-handler
  (= parg (cast (ptr (struct dhandler-arg)) (csym::malloc (sizeof (struct dhandler-arg)))))
  (= parg->data-to pcmd->node)          ; internal or external
  (csym::copy-address parg->head (aref pcmd->v 0)) ; the recipient address of "data"
  ;; parg->dreq-cmd: a template for dreq messages when requested data are "DATA-NONE".
  ;; <Range> is set later
  (= parg->dreq-cmd.w DREQ)
  (= parg->dreq-cmd.c 3)
  (= (aref parg->dreq-cmd.v 0 0) 0)     ; <sender> (dummy)
  (= (aref parg->dreq-cmd.v 0 1) TERM)  ; <sender> (dummy)
  (begin                                ; set the <recipient>
    (= tid (aref pcmd->v 1 0))
    (if (not (< tid num-thrs)) (csym::proto-error "wrong dreq-head" pcmd))
    (= sid (aref pcmd->v 1 1))
    (if (== TERM sid) (csym::proto-error "Wrong dreq-head (no task-home-id)" pcmd))
    ;; Search ancestors for the first external task and set the address of the task-home
    ;; entry corresponding to the task
    (csym::get-first-outside-ancestor-task-address (aref parg->dreq-cmd.v 1) tid sid)
    )
  (= parg->dreq-cmd.node OUTSIDE)
  ;; parg->dreq-cmd-fwd: a template for dreq messages when requested data are "DATA-REQUESTING".
  ;; <Range> is set later
  (= parg->dreq-cmd-fwd.w DREQ)
  (= parg->dreq-cmd-fwd.c 3)
  (= (aref parg->dreq-cmd-fwd.v 0 0) FORWARD)       ; <sender>
  (csym::copy-address (ptr (aref parg->dreq-cmd-fwd.v 0 1))
                      (aref pcmd->v 0))
  (csym::copy-address (aref parg->dreq-cmd-fwd.v 1) ; <recipient> (same as dreq-cmd)
                      (aref parg->dreq-cmd.v 1))
  (= parg->dreq-cmd-fwd.node OUTSIDE)
  ;; Extract start and end from <range>
  (= parg->start (aref pcmd->v 2 0))
  (= parg->end   (aref pcmd->v 2 1))
  
  ;; Invoke dreq-handler asynchronously
  (begin
    (def tid pthread-t)
    (csym::pthread-create (ptr tid) 0 csym::dreq-handler parg))
  (return))


;; data: reply to a dreq message with data
;; data <recipient> <range("<start>:<end>" indicating [start,end))>
(def (csym::recv-data pcmd) (csym::fn void (ptr (struct cmd)))
  (def i int)
  (def start int (aref pcmd->v 1 0))
  (def end int (aref pcmd->v 1 1))
  ;; Check # of arguments
  (if (< pcmd->c 2) (csym::proto-error "Wrong data" pcmd))
  ;; Ignore an internal "data" message
  (if (== pcmd->node INSIDE) (return))
  ;; Receive data by invoking a user-defined receiver method
  (csym::pthread-mutex-lock (ptr data-mutex))
  (csym::data-receive start end)
  (csym::read-to-eol)
  (for ((= i start) (< i end) (inc i))
    (= (aref data-flags i) DATA-EXIST))
  (csym::pthread-cond-broadcast (ptr data-cond))
  (csym::pthread-mutex-unlock (ptr data-mutex))
  (return))


;;; bcst: send data to all nodes
;;; bcst <sender> <data-kind>
(def (csym::recv-bcst pcmd) (csym::fn void (ptr (struct cmd)))
  (def rcmd (struct cmd))
  (def task-no int)
  (def body (ptr void))
  ;; Check # of arguments
  (if (< pcmd->c 2)
      (csym::proto-error "wrong-task" pcmd))
  ;; Extract <data-kind>
  (= task-no (aref pcmd->v 1 0))
  ;; (Allocate and) receive data body by invoking the user-defined 
  ;; receiver method associated with the <data-kind> number
  (= body ((aref task-receivers task-no)))
  (csym::read-to-eol)
  ;; NOTE: here the "body" object is deallocated immediately
  ;; after the receiver method finished. The data that need to be
  ;; accessed after bcst should be stored outside of the task 
  ;; object (global variables and so on)
  (csym::free body)
  ;; Generate and send a bcak message as the reply
  (= rcmd.c 1)
  (= rcmd.node pcmd->node)
  (= rcmd.w BCAK)
  (csym::copy-address (aref rcmd.v 0) (aref pcmd->v 0))
  (csym::send-command (ptr rcmd) 0 task-no))

;;; leav: a message from a computing node to indicate the node
;;; will leave from the Tascell network.
;;; Here, a message handler just outputs error message because
;;; a leav message from a Tascell server should not be sent.
(def (csym::recv-leav pcmd) (csym::fn void (ptr (struct cmd)))
  (csym::fprintf stderr "Leav from server is unexpected.~%"))


;;; Cancel all worker threads after acquiring all threads' locks
(def (csym::cancel-workers) (csym::fn void void)
  (def i int 0)
  (def thr (ptr (struct thread-data)))
  (for ((= i 0) (< i num-thrs) (inc i))
    (= thr (ptr (aref threads i)))
    (csym::pthread-mutex-lock (ptr thr->mut))
    (csym::pthread-mutex-lock (ptr thr->rack-mut)))
  (for ((= i 0) (< i num-thrs) (inc i))
    (= thr (ptr (aref threads i)))
   (csym::pthread-cancel thr->pthr-id)
   (csym::fprintf stderr "Cancelled worker %d~%" i))
  (return))

;;; lack: a reply to a leav message
(def (csym::recv-lack pcmd) (csym::fn void (ptr (struct cmd)))
  (def cur (ptr (struct task)))
  (def task-top (ptr (struct task))) 
  (def thr (ptr (struct thread-data)))
  (def i int)
  (def rcmd (struct cmd))
  ;; Prevent workers from sending out messages.
  (csym::pthread-mutex-lock (ptr send-mut))
  ;; Prevent workers from modifying their own information (task stacks etc.)
  (csym::cancel-workers)
  (for ((= i 0) (< i num-thrs) (inc i))
    (= thr (ptr (aref threads i)))
    (= task-top thr->task-top)
    (for ((= cur task-top) cur (= cur cur->next))
      (= rcmd.w ABRT)
      (= rcmd.c 1)
      (= rcmd.node cur->rslt-to)        ; external or internal
      (csym::copy-address (aref rcmd.v 0) cur->rslt-head)
      (csym::send-command (ptr rcmd) 0 0))
    (csym::print-thread-status thr))
  ;; all command check
  (csym::exit 0))

;;; abrt: reply to a task message without the result
;;; abrt <recipient("<worker-id>:<task-home-id>")>
(def (csym::recv-abrt pcmd) (csym::fn void (ptr (struct cmd)))
  (def rcmd (struct cmd))               ; rack message
  (def thr (ptr (struct thread-data)))
  (def hx (ptr (struct task-home)))
  (def tid (enum addr))
  (def sid int)
  ;; Check # of arguments
  (if (< pcmd->c 1)
      (csym::proto-error "Wrong abrt" pcmd))
  ;; Extract <worker-id> and <task-home-id> from <recipients>
  (= tid (aref pcmd->v 0 0))
  (if (not (< tid num-thrs))
      (csym::proto-error "Wrong abrt-head" pcmd))
  (= sid (aref pcmd->v 0 1))
  (if (== TERM sid)
      (csym::proto-error "Wrong abrt-head (no task-home-id)" pcmd))
  (= thr (+ threads tid))
  
  (csym::pthread-mutex-lock (ptr thr->mut))
  ;; Find the task-home entry corresponding to the abrt message.
  ;; (the task-home entry whose .id is sid)
  (if (not (= hx (csym::search-task-home-by-id sid thr->sub)))
      (csym::proto-error "Wrong abrt-head (specified task not exists)" pcmd))
  
  (= hx->stat TASK-HOME-ABORTED)
  (if (== hx thr->sub)
      (begin
	(csym::pthread-cond-broadcast (ptr thr->cond-r))
	(csym::pthread-cond-broadcast (ptr thr->cond)))
    )
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::send-command (ptr rcmd) 0 0))  ; Send rack


;;; cncl: set a cancellation to the recipient task
;;; cncl <sender> <recipient>
(def (csym::recv-cncl pcmd) (csym::fn void (ptr (struct cmd)))
  (def from-addr (ptr (enum addr)))    ; return address of the task to be cancelled (search key)
  (def dst0 (enum addr))               ; ID of recipient worker
  (def thr (ptr (struct thread-data))) ; recipient worker thread
  (def tx (ptr (struct task)))         ; task to be cancelled
  ;; Check # of arguments
  (if (< pcmd->c 2)
      (csym::proto-error "Wrong cncl" pcmd))
  ;; Get <sender> from the message
  (= from-addr (aref pcmd->v 0))
  ;; Extract <recipient> from the message
  (= dst0 (aref pcmd->v 1 0))
  (if (not (and (<= 0 dst0) (< dst0 num-thrs)))   ; Range check of <recipient>
      (csym::proto-error "Wrong cncl-head" pcmd))
  (= thr (+ threads dst0))

  (csym::pthread-mutex-lock (ptr thr->mut))
  (if (= tx (csym::have-task thr from-addr pcmd->node))  ; Check whether the recipient task exists
      (begin 
	(inc tx->cancellation)
	(csym::set-cancelled thr tx 0)
	(DEBUG-PRINT 1 "Task %p of worker %d is cancelled by cncl~%" tx dst0)))
  (csym::pthread-mutex-unlock (ptr thr->mut))
)

;;; bcak: ack message to bcst
;;; bcak <recipient>
(def (csym::recv-bcak pcmd) (csym::fn void (ptr (struct cmd)))
  (def thr (ptr (struct thread-data)))
  (def id (enum addr))
  ;; Check # of arguments
  (if (< pcmd->c 1)
    (csym::proto-error "wrong-task" pcmd))
  ;; extract <recipient> (= worker ID) from the 1st argument
  (= id (aref pcmd->v 0 0))
  (= thr (+ threads id))
  ;; decrease bcak counter and notify
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= thr->w-bcak 0)
  (csym::pthread-cond-broadcast (ptr thr->cond))
  (csym::pthread-mutex-unlock (ptr thr->mut)))


;;; Print information of a task entry in human-friendly style
(def (csym::node-to-string buf node) (csym::fn void (ptr char) (enum node))
  (switch node
    (case INSIDE)   (csym::strcpy buf "INSIDE")      (break)
    (case OUTSIDE)  (csym::strcpy buf "OUTSIDE")     (break)
    (default)       (csym::strcpy buf "wrong-value") (break)))
(def (csym::addr-to-string buf addr) (csym::fn void (ptr char) (enum addr))
  (switch addr
    (case ANY)     (csym::strcpy  buf "ANY")     (break)
    (case PARENT)  (csym::strcpy  buf "PARENT")  (break)
    (case TERM)    (csym::strcpy  buf "TERM")    (break)
    (default)      (csym::sprintf buf "%d" addr) (break)))
(def (csym::print-task-list task-top name) (csym::fn void (ptr (struct task)) (ptr char))
  (def cur (ptr (struct task)))
  (defs (array char BUFSIZE) buf1 buf2)
  (csym::fprintf stderr "%s= {" name)
  (for ((= cur task-top) cur (= cur cur->next))
    (csym::fprintf stderr "{stat=%s, task-no=%d, body=%p, ndiv=%d, cancellation=%d, rslt-to=%s, rslt-head=%s}, "
                   (aref task-stat-strings cur->stat) cur->task-no cur->body cur->ndiv cur->cancellation
                   (exps (csym::node-to-string buf1 cur->rslt-to) buf1)
                   (exps (csym::serialize-arg buf2 cur->rslt-head) buf2)))
  (csym::fprintf stderr "}, ")
  (return))

;;; Print information of a task-home entry in human-friendly style
(def (csym::print-task-home-list treq-top name) (csym::fn void (ptr (struct task-home)) (ptr char))
  (def cur (ptr (struct task-home)))
  (defs (array char BUFSIZE) buf0 buf1 buf2)
  (csym::fprintf stderr "%s= {" name)
  (for ((= cur treq-top) cur (= cur cur->next))
    (csym::fprintf stderr "{stat=%s, id=%d, exception-tag=%d, msg-cncl=%d, waiting=%s, owner=%p, eldest=%p(%d), task-no=%d, body=%p, req-from=%s, task-head=%s}, "
                   (aref task-home-stat-strings cur->stat) cur->id
		   cur->exception-tag cur->msg-cncl
                   (exps (csym::serialize-arg buf0 cur->waiting-head) buf0)
                   cur->owner
                   cur->eldest (if-exp cur->eldest cur->eldest->id 0)
		   cur->task-no cur->body
                   (exps (csym::node-to-string buf1 cur->req-from) buf1)
                   (exps (csym::serialize-arg buf2 cur->task-head) buf2)))
  (csym::fprintf stderr "}, ")
  (return))

;;; Print the status of a worker in human-friendly style
(def (csym::print-thread-status thr) (csym::fn void (ptr (struct thread-data)))
  (csym::fprintf stderr "*** Worker %d ***~%" thr->id)
  (csym::fprintf stderr "req=%p, " thr->req)
  (csym::fprintf stderr "w-rack=%d, " thr->w-rack)
  (csym::fprintf stderr "w-none=%d, " thr->w-none)
  (csym::fprintf stderr "ndiv=%d, " thr->ndiv)
  (csym::fprintf stderr "exiting=%s, " (aref exiting-rsn-strings thr->exiting))
  (csym::fprintf stderr "exception-tag=%d, " thr->exception-tag)
  (csym::fprintf stderr "probability=%lf, " thr->probability)
  (csym::fprintf stderr "last-treq=%d, " thr->last-treq)
  (csym::fprintf stderr "last-choose=%s, " (aref choose-strings thr->last-choose))
  (csym::fprintf stderr "random-seed(1,2)=(%f,%f), " thr->random-seed1 thr->random-seed2)
  (csym::fprintf stderr "~%")
  (csym::print-task-list thr->task-top "tasks")
  (csym::fprintf stderr "~%")
  (csym::print-task-home-list thr->treq-top "treq-top")
  (csym::fprintf stderr "~%")
  (csym::print-task-home-list thr->sub "sub")
  (csym::fprintf stderr "~%")
  (return)
  )

;;; (recv-stat)
;;; stat: print current information of this node. 
;;; No arguments.
(def (csym::print-status pcmd) (csym::fn void (ptr (struct cmd)))
  (def i int)
  (csym::fprintf stderr "worker-name: %s~%"
                 (if-exp option.node-name option.node-name "Unnamed"))
  (csym::fprintf stderr "num-thrs: %d~%" num-thrs)
  (csym::fprintf stderr "verbose-level: %d~%" option.verbose)
  (%ifdef* VERBOSE
    (csym::fprintf stderr
                   "active dreq-handlers: %d (%d sending dreq, %d sending data (%d waiting data))~%"
                   n-dreq-handler n-sending-dreq n-sending-data n-waiting-data))

  (for ((= i 0) (< i num-thrs) (inc i))
    (csym::print-thread-status (ptr (aref threads i))))
  (return)
  )


;;; (recv-verb)
;;; verb: change verbose level
;;; verb <level>
(def (csym::set-verbose-level pcmd) (csym::fn void (ptr (struct cmd)))
  
  (if (< pcmd->c 1)
      (csym::proto-error "Wrong verb" pcmd))
  (= option.verbose (aref pcmd->v 0 0))
  (return))



;;; exit: terminate this process
;;; No arguments
(def (csym::recv-exit pcmd) (csym::fn void (ptr (struct cmd)))
  (csym::fprintf stderr "Received \"exit\"... terminate.~%")
  (csym::exit 0))


;; Start temporary backtracking to spawn tasks
(def (handle-req -bk -thr)
    (fn void (ptr (NESTFN int void)) (ptr (struct thread-data)))
  (csym::pthread-mutex-lock (ptr -thr->mut))
  (if -thr->req
      (begin
	(= -thr->exiting EXITING-SPAWN)
	(PROF-CODE
	 (csym::tcounter-change-state -thr TCOUNTER-SPWN OBJ-NULL 0))
	(-bk)
	(PROF-CODE
	 (csym::tcounter-change-state -thr TCOUNTER-EXEC OBJ-NULL 0))
	(= -thr->exiting EXITING-NORMAL)
	(= -thr->req -thr->treq-top) ))
  (csym::pthread-mutex-unlock (ptr -thr->mut)))

;; Send a cncl message for each flagged subtask spawned by "thr"
;; The mutex thr->mut must be acquired before calling.
;; Returns # of sent cncl messages.
(def (csym::send-cncl-for-flagged-subtasks thr)
    (csym::fn int (ptr (struct thread-data)))
  (def rcmd (struct cmd))               ; for CNCL command
  (def cur (ptr (struct task-home)))
  (def count int 0)
  (= rcmd.w CNCL)
  (= rcmd.c 2)
  (= (aref rcmd.v 0 0) thr->id)
  (= (aref rcmd.v 0 2) TERM)
  (for ((= cur thr->sub) cur (= cur cur->next))
    (if (and (== cur->msg-cncl 1)
	     (== cur->stat TASK-HOME-INITIALIZED))
	(begin
	  (= rcmd.node cur->req-from)
	  (= (aref rcmd.v 0 1) cur->id)                       ; <sender>=<thread-id>:<task-home-id>
	  (csym::copy-address (aref rcmd.v 1) cur->task-head) ; <recipient>
	  (csym::send-command (ptr rcmd) 0 0)
	  (= cur->msg-cncl 2)
	  (inc count) )))
  (return count) )

;; Check -thr's subtask queue and send a cncl message for each flagged subtask
(def (handle-req-cncl -bk -thr)
    (fn void (ptr (NESTFN int void)) (ptr (struct thread-data)))
  (csym::pthread-mutex-lock (ptr -thr->mut))
  (if -thr->req-cncl
      (begin
	(DEBUG-PRINT 1 "(%d): (Thread %d) detected cncl message request~%"
		     (csym::get-universal-real-time) -thr->id)
	(csym::send-cncl-for-flagged-subtasks -thr)
	(= -thr->req-cncl 0) ))
  (csym::pthread-mutex-unlock (ptr -thr->mut)))

;; Start propagating an exception. Invoked by a Tascell throw statement.
(def (handle-exception -bk -thr excep)
    (fn void (ptr (NESTFN int void)) (ptr (struct thread-data)) int)
  (= -thr->exiting EXITING-EXCEPTION)
  (= -thr->exception-tag excep)
  (PROF-CODE
   (csym::tcounter-change-state -thr TCOUNTER-EXCP OBJ-INT (cast (ptr void) (cast long excep))))
  (-bk)) ; never returns

;; Check (partial) cancellation flags and abort if needed.
(def (handle-cancellation -bk -thr)
    (fn void (ptr (NESTFN int void)) (ptr (struct thread-data)))
  (csym::pthread-mutex-lock (ptr -thr->mut))
  (if -thr->task-top->cancellation
      (begin
	(DEBUG-PRINT 1 "(%d): (Thread %d) detected cancellation flag (%d)~%" 
		     (csym::get-universal-real-time) -thr->id -thr->task-top->cancellation)
	(= -thr->exiting EXITING-CANCEL)
	(PROF-CODE
	 (csym::tcounter-change-state -thr TCOUNTER-ABRT
				      OBJ-INT (cast (ptr void) (cast long -thr->task-top->cancellation))))
	(csym::pthread-mutex-unlock (ptr -thr->mut))
	(-bk)))  ; never returns
  (csym::pthread-mutex-unlock (ptr -thr->mut))
  )


;; Make a task message and send it.
;; (The recipient is determined by the top of the task request stack)
;; Called by a worker thread ("thr") after initializing the task object
;; in :put section ("body"). The thr->mut should have been acquired.
;; If "eldest-p" is non-zero, the task is the first one among tasks
;; spawned in a parallel region.
(def (csym::make-and-send-task thr task-no body eldest-p)
    (csym::fn void (ptr (struct thread-data)) int (ptr void) int)
  (def tcmd (struct cmd))
  (def hx (ptr (struct task-home)) thr->treq-top)
  ;; (csym::fprintf stderr "make-and-send-task(%d)~%" thr->id)
  ;; Pop a task-home entry from the worker's task request queue and push it to the worker's subtask stack
  (= thr->treq-top hx->next)            ; top of request queue <-- 2nd entry of the queue
  (= hx->next thr->sub)                 ; popped entry's next  <-- top of subtask stack
  (= thr->sub hx)                       ; top of subtask stack <-- the new task-home entry
  ;; Initialize the task-home entry
  (= hx->task-no task-no)               ; task-no: the kind of task
  (= hx->body body)
  (= hx->id (if-exp hx->next            ; the subtask ID (= height_of_the_stack + 1)
                    (+  hx->next->id 1)
                    0))
  (= hx->owner thr->task-top)           ; the parent task (= current top of the task stack)
  (= hx->eldest (if-exp eldest-p hx hx->next->eldest))
  (= hx->msg-cncl 0)                    ; initialize flag to be cancelled
  (= hx->stat TASK-HOME-INITIALIZED)    ; ALLOCATED => INITIALIZED
  ;; Make a task message
  (= tcmd.c 4)                          ; # of arguments
  (= tcmd.node hx->req-from)            ; internal or external
  (= tcmd.w TASK)                       ; message kind = TASK
  (= (aref tcmd.v 0 0) (++ thr->ndiv))  ; # of task division
  (= (aref tcmd.v 0 1) TERM)
  (= (aref tcmd.v 1 0) thr->id)         ; sender: <worker-id>
  (= (aref tcmd.v 1 1) hx->id)          ; sender: <subtask-id>
  (= (aref tcmd.v 1 2) TERM)
  (csym::copy-address (aref tcmd.v 2) hx->task-head) ; recipient: use the information in the task request
  (= (aref tcmd.v 3 0) task-no)         ; the kind of task
  (= (aref tcmd.v 3 1) TERM)
  (PROF-CODE
   (csym::evcounter-count thr EV-SEND-TASK OBJ-PADDR hx->task-head))
  (csym::send-command (ptr tcmd) body task-no))

;;; Wait for the result of the subtask thr->sub, remove it from the worker's subtask list,
;;; and returns the pointer to the task object of the subtask.
;;; If stback is non-zero, steal back another task during waiting the result.
(def (wait-rslt thr stback) (fn (ptr void) (ptr (struct thread-data)) int)
  (def body (ptr void))
  (def sub (ptr (struct task-home)))
  (PROF-CODE
   (def tcnt-stat (enum tcounter))
   (def tcnt-stat-w (enum tcounter)))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (PROF-CODE
   (= tcnt-stat thr->tcnt-stat))
  (= sub thr->sub)                           ; sub: the top of the worker's subtask stack
  (while (and (!= sub->stat TASK-HOME-DONE)  ; Until the subtask is done/aborted
	      (!= sub->stat TASK-HOME-EXCEPTION)
	      (!= sub->stat TASK-HOME-ABORTED))
    (PROF-CODE
     (= tcnt-stat-w (if-exp (== tcnt-stat TCOUNTER-EXEC)
			TCOUNTER-WAIT
		      (if-exp (== tcnt-stat TCOUNTER-ABRT)
			  TCOUNTER-ABRT-WAIT
			TCOUNTER-EXCP-WAIT)))
     (csym::tcounter-change-state thr tcnt-stat-w OBJ-NULL 0))
    (= thr->task-top->stat TASK-SUSPENDED)   ; STARTED => SUSPENDED (thr->task-top is the task being executed)
    ;; When propagating exception, send cncl messages for subtasks
    (if (== thr->exiting EXITING-EXCEPTION)
	(begin
	  (csym::set-cancelled thr thr->task-top sub->eldest)
	  (csym::send-cncl-for-flagged-subtasks thr)))
    ;; Wait for a moment if the victim is an external worker.
    (if (== OUTSIDE sub->req-from)
	(let ((now (struct timeval))
	      (t-until (struct timespec)))
	  (csym::gettimeofday (ptr now) 0)
	  (csym::timeval-plus-nsec-to-timespec (ptr t-until) (ptr now) 1000)
	  (csym::pthread-cond-timedwait (ptr thr->cond-r) (ptr thr->mut)
					(ptr t-until))
	  ))
    ;; Quit if the subtask is finished or aborted.
    (if (or (== sub->stat TASK-HOME-DONE)
	    (== sub->stat TASK-HOME-EXCEPTION)
	    (== sub->stat TASK-HOME-ABORTED)) 
	(break))
    (if stback
        ;; Steal and execute a task
	(begin
	  (PROF-CODE
	   (csym::tcounter-change-state thr TCOUNTER-TREQ-BK
					OBJ-ADDR sub->task-head))
	  (recv-exec-send thr sub->task-head sub->req-from)
	  (PROF-CODE
	   (csym::tcounter-change-state thr tcnt-stat-w
					OBJ-NULL 0))
	  )
      ;; Just wait for the subtask finishing
      (csym::pthread-cond-wait (ptr thr->cond-r) (ptr thr->mut)))
    )
  (PROF-CODE
   (csym::tcounter-change-state thr tcnt-stat OBJ-NULL 0))

  ;; When the subtask has thrown an exception, propagate it
  (if (== sub->stat TASK-HOME-EXCEPTION)
      (begin
	(= thr->exiting EXITING-EXCEPTION)
	(= thr->exception-tag sub->exception-tag)
	(dec sub->owner->cancellation)  ; take the partial cancellation flag
	))
  ;; When the subtask is abnormally exited, a task object is not returned as the result
  (if (or (== sub->stat TASK-HOME-EXCEPTION)
	  (== sub->stat TASK-HOME-ABORTED))
      (begin
	(csym::free body)
	(= body 0))
    (= body sub->body))
  ;; Pop the subtask stack
  (= thr->sub sub->next)                ; stack top <== 2nd entry
  (= sub->next thr->treq-free)          ; next of the popped entry <== top of free list
  (= thr->treq-free sub)                ; top of free list <== the popped entry
  ;; Resume the suspended task
  (= thr->task-top->stat TASK-STARTED)  ; SUSPENDED => STARTED
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (return body))


;;; Send bcst (broadcast) message to the connected Tascell server.
;;; Called by worker thread after initializing the task object ("body")
;;; in a :put section.
(def tcell-bcst-wait-bcak int 1)
(def (csym::broadcast-task thr task-no body)
   (csym::fn void (ptr (struct thread-data)) int (ptr void))
  (def bcmd (struct cmd))
  ;; A bcst message
  (= bcmd.c 2)                         ; # of argments
  (= bcmd.node OUTSIDE)                ; bcst is sent only to external node
  (= bcmd.w BCST)                      ; message kind = BCST
  (= (aref bcmd.v 0 0) thr->id)        ; sender: worker ID
  (= (aref bcmd.v 0 1) TERM)
  (= (aref bcmd.v 1 0) task-no)        ; kind of broadcast
  (= (aref bcmd.v 1 1) TERM)
  ;; Send the message.
  ;; The broadcast data is sent by the user-defined method (associated with task-no)
  ;; invoked in send-out-command().
  (csym::send-command (ptr bcmd) body task-no)

  ;; Increase w-bcak counter and waiting until a bcak message is returned and the counter is drecreased.
  (if tcell-bcst-wait-bcak
    (begin
      (csym::pthread-mutex-lock (ptr thr->mut))
      (= thr->w-bcak 1)
      (while thr->w-bcak
        (csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut)))
      (csym::pthread-mutex-unlock (ptr thr->mut))
      ))
  )
   

;;; Handling command-line options
(def (csym::usage argc argv) (csym::fn void int (ptr (ptr char)))
  (csym::fprintf stderr
                 (%string
                  "Usage: %s [-s hostname] [-p port-num] [-n n-threads] [-i initial-task-parms] [-a]"
                  (%if* VERBOSE " [-v verbosity]" %else "")
                  (%if* PROFILE " [-T timechart-prefix]" %else "")
                  "~%")
                 (aref argv 0))
  (csym::exit 1))

(def (set-option argc argv) (csym::fn void int (ptr (ptr char)))
  (def i int) (def ch int)
  ;; Default values
  (= option.sv-hostname 0)
  (= option.port 9865)
  (= option.num-thrs 1)
  (= option.node-name 0)
  (= option.initial-task 0)
  (= option.auto-exit 0)
  (= option.affinity 0)
  (= option.always-flush-accepted-treq 0)
  (= option.verbose 0)
  (PROF-CODE
   (= option.timechart-file 0))

  ;; Parse and set options
  (while (!= -1 (= ch (csym::getopt argc argv "n:s:p:N:i:xafP:v:T:h")))
    (switch ch
      (case #\n)                        ; number of threads
      (= option.num-thrs (csym::atoi optarg))
      (break)

      (case #\s)                        ; server name
      (= option.sv-hostname
	 (if-exp (csym::strcmp "stdout" optarg) optarg 0))
      (break)

      (case #\p)                        ; connection port number
      (= option.port (csym::atoi optarg))
      (break)

      (case #\N)
      (if option.node-name (csym::free option.node-name))
      (= option.node-name
         (cast (ptr char) (csym::malloc (* (+ 1 (csym::strlen optarg))
                                           (sizeof char)))))
      (csym::strcpy option.node-name optarg)
      (break)
      
      (case #\i)                        ; initial task
      (if option.initial-task
          (csym::free option.initial-task))
      (= option.initial-task
         (cast (ptr char) (csym::malloc (* (+ 1 (csym::strlen optarg))
                                           (sizeof char)))))
      (csym::strcpy option.initial-task optarg)
      (= option.auto-exit 1)            ; auto-exit set automatically
      (break)
      
      (case #\x)                        ; auto exit
      (= option.auto-exit 1)
      (break)
      
      (case #\a)                        ; set affinity
      (%ifdef* USE-AFFINITY
        (if (== 0 option.affinity)
            (csym::fprintf stderr "setaffinity enabled.~%")))
      (= option.affinity 1)
      (%ifndef* USE-AFFINITY
        (csym::fprintf stderr "-a is ignored (invalidated in compile time)~%"))
      (break)

      (case #\f)                        ; flush also stealing-back treq
      (= option.always-flush-accepted-treq 1)
      (break)
      
      (case #\v)                        ; verbose level
      (%if* VERBOSE
	(= option.verbose (csym::atoi optarg))
	%else
	(csym::fprintf stderr "Warning: -v option is invalidated at compile-time.~%"))
      (break)

      (case #\T)                        ; output time chart
      (%if* PROFILE
	(= option.timechart-file optarg)
	%else
	(csym::fprintf stderr "Warning: -T option is invalidated at compile-time.~%"))
      (break)

      (case #\h)                        ; usage
      (csym::usage argc argv)
      (break)

      (default)
      (csym::fprintf stderr "Unknown option: %c~%" ch)
      (csym::usage argc argv)
      (break)))
  (return)
  )


;;; Initialize task stack cells
(def (csym::initialize-task-list tlist len p-top p-free)
    (fn void (ptr (struct task)) int (ptr (ptr (struct task))) (ptr (ptr (struct task))))
  (def i int)
  (= (mref p-top) 0)          ; stack top
  (= (mref p-free) tlist)     ; free list
  (for ((= i 0) (< i (- len 1)) (inc i))
       (= (fref (aref tlist i) prev) (ptr (aref tlist (+ i 1))))
       (= (fref (aref tlist (+ i 1)) next) (ptr (aref tlist i))))
  (= (fref (aref tlist 0) next) 0)
  (= (fref (aref tlist (- len 1)) prev) 0)
  (for ((= i 0) (< i len) (inc i))
    (= (fref (aref tlist i) rslt-to) TERM)
    (= (aref (fref (aref tlist i) rslt-head) 0) TERM))
  (return))

;;; Initialize subtask stack cells
(def (csym::initialize-task-home-list hlist len p-top p-free)
    (fn void (ptr (struct task-home)) int (ptr (ptr (struct task-home))) (ptr (ptr (struct task-home))))
  (def i int)
  (= (mref p-top) 0)          ; stack top
  (= (mref p-free) hlist)     ; free list
  (for ((= i 0) (< i (- len 1)) (inc i))
       (= (fref (aref hlist i) next) (ptr (aref hlist (+ i 1))))
      (= (fref (aref hlist (- len 1)) next) 0))
  (return))


(PROF-CODE
;;; Initialize (struct aux-data) object pointed by paux by given type and value
(def (csym::set-aux-data paux aux-type aux-body)
    (fn void (ptr (struct aux-data)) (enum obj-type) (ptr void))
  (= paux->type aux-type)
  (switch aux-type
    (case OBJ-INT)
    (= paux->body.aux-int (cast long aux-body))
    (break)
    (case OBJ-ADDR)                     ; copy address
    (csym::copy-address paux->body.aux-addr
                        (cast (ptr (enum addr)) aux-body))
    (break)
    (case OBJ-PADDR)                    ; copy pointer to address
    (= paux->body.aux-paddr (cast (ptr (enum addr)) aux-body))
    (break))
  )

;;; Output (struct aux-data) object pointed by paux to fp.
(def (csym::print-aux-data fp paux)
    (fn void (ptr FILE) (ptr (struct aux-data)))
  (def buf (array char BUFSIZE))
  (switch paux->type
    (case OBJ-INT)
    (csym::fprintf fp " %d" paux->body.aux-int)
    (break)
    (case OBJ-ADDR)
    (csym::serialize-arg buf paux->body.aux-addr)
    (csym::fputc #\Space fp)
    (csym::fputs buf fp)
    (break)
    (case OBJ-PADDR)
    (csym::serialize-arg buf paux->body.aux-paddr)
    (csym::fputc #\Space fp)
    (csym::fputs buf fp)
    (break))
  )

;;; Initialize time counters
(def (csym::initialize-tcounter thr) (fn void (ptr (struct thread-data)))
  (def i int)
  (def tp (struct timeval))
  (= thr->tcnt-stat TCOUNTER-INIT)
  (csym::gettimeofday (ptr tp) 0)
  (for ((= i 0) (< i NKIND-TCOUNTER) (inc i))
    (= (aref thr->tcnt i) 0)
    (= (aref thr->tcnt-tp i) tp))
  (= thr->tc-aux.type OBJ-NULL)
  )

;;; Set the start a time of tcnt-stat to the current time
(def (csym::tcounter-start thr tcnt-stat)
    (fn void (ptr (struct thread-data)) (enum tcounter))
  (def tp (struct timeval))
  (csym::gettimeofday (ptr tp) 0)
  (= (aref thr->tcnt-tp tcnt-stat) tp))

;;; Add time from the last start time of tcnt-stat to the corresponding counter
;;; Set the start a time of tcnt-stat to the current time
(def (csym::tcounter-end thr tcnt-stat)
    (fn void (ptr (struct thread-data)) (enum tcounter))
  (def tp (struct timeval))
  (csym::gettimeofday (ptr tp) 0)
  (+= (aref thr->tcnt tcnt-stat)
      (csym::diff-timevals (ptr tp) (ptr (aref thr->tcnt-tp tcnt-stat))))
  (= (aref thr->tcnt-tp tcnt-stat) tp))

;;; (tcounter-end <current state>) and (tcounter-start tcnt-stat)
;;; at the same time and change the <current state> to tcnt-stat.
;;; Return the original state.
;;; aux-type/aux-body is auxiliary data type/body for timechart:
(def (csym::tcounter-change-state thr tcnt-stat aux-type aux-body)
    (fn (enum tcounter) (ptr (struct thread-data)) (enum tcounter)
	(enum obj-type) (ptr void))
  (def tp (struct timeval))
  (def tcnt-stat0 (enum tcounter))
  (defs double tcnt0 tcnt)
  (= tcnt-stat0 thr->tcnt-stat)               ; old state
  (if (!= tcnt-stat0 tcnt-stat)
      (begin
	(= tcnt0 (aref thr->tcnt tcnt-stat0)) ; total time of old state
	(csym::gettimeofday (ptr tp) 0)       ; end time of old state
	(= tcnt (+ tcnt0                      ; increase total time of old state
		   (csym::diff-timevals (ptr tp)
					(ptr (aref thr->tcnt-tp tcnt-stat0)))))
	(= (aref thr->tcnt tcnt-stat0) tcnt)  ; update total time of old state
	(= (aref thr->tcnt-tp tcnt-stat) tp)  ; set start time of new state
	(= thr->tcnt-stat tcnt-stat)          ; set the new state of the worker
	(if thr->fp-tc
	    ;; Write time chart
	    (let ((tp0 (ptr (struct timeval))))
	      (= tp0 (ptr (aref thr->tcnt-tp tcnt-stat0)))
	      (if (and (== tcnt-stat0 TCOUNTER-INIT)
		       (== thr->id 0))
		  (= tp-strt tp))             ; start time of whole execution
	      ;; Output the previous state name, and the time range of the state.
	      (csym::fprintf thr->fp-tc "%s %lf %lf"
			     (aref tcounter-strings tcnt-stat0)
			     (csym::diff-timevals tp0 (ptr tp-strt))
			     (csym::diff-timevals (ptr tp) (ptr tp-strt)))
              ;; Output auxiliary data of the previous state.
              (csym::fputc #\Space thr->fp-tc)
              (csym::print-aux-data thr->fp-tc (ptr thr->tc-aux))
	      (csym::fputc #\Newline thr->fp-tc)
              ;; Save the given aux data for the next output
              (csym::set-aux-data (ptr thr->tc-aux) aux-type aux-body)
	      ))
	))
  (return tcnt-stat0))

;;; Initialize event counters
(def (csym::initialize-evcounter thr) (fn void (ptr (struct thread-data)))
  (def i int)
  (for ((= i 0) (< i NKIND-EV) (inc i))
    (= (aref thr->ev-cnt i) 0))
  )

;;; Add event counter and output to log file
(def (csym::evcounter-count thr ev aux-type aux-body)
    (fn int (ptr (struct thread-data)) (enum event) (enum obj-type) (ptr void))
  (def tp (struct timeval))
  (def aux (struct aux-data))
  (inc (aref thr->ev-cnt ev))
  (if thr->fp-tc
      (begin
        (csym::gettimeofday (ptr tp) 0)
        (csym::fprintf thr->fp-tc "%s %lf"
                       (aref ev-strings ev)
                       (csym::diff-timevals (ptr tp) (ptr tp-strt)))
        ;; Output auxiliary data of the previous state.
        (csym::fputc #\Space thr->fp-tc)
        (csym::set-aux-data (ptr aux) aux-type aux-body)
        (csym::print-aux-data thr->fp-tc (ptr aux))
        (csym::fputc #\Newline thr->fp-tc)))
  (return))

;;; Show time / event counters
(def (csym::show-counters) (fn void)
  (defs int i j)
  (def thr (ptr (struct thread-data)))
  (for ((= i 0) (< i num-thrs) (inc i))
    (csym::fprintf stderr "*** Worker %d ***~%" i)
    (= thr (+ threads i))
    (for ((= j 0) (< j NKIND-TCOUNTER) (inc j))
      (csym::fprintf stderr "%s: %lf~%"
		     (aref tcounter-strings j) (aref thr->tcnt j)))
    (for ((= j 0) (< j NKIND-EV) (inc j))
         (csym::fprintf stderr "%s: %ld~%"
                        (aref ev-strings j) (aref thr->ev-cnt j))))
  (return))

)                                       ; end PROF-CODE


;; main (entry point)
;; Initialize and run all workers and wait for external messages.
(def (main argc argv) (fn int int (ptr (ptr char)))
  (defs int i j)
  (def pcmd (ptr (struct cmd)))         ; external command received

  (%ifdef* USEMPI
    (csym::MPI_Init (ptr argc) (ptr argv)))

  ;; Show compile-time option
  (fprintf stderr (%string "compile-time options: "
                           "VERBOSE=" VERBOSE " "
                           "PROFILE=" PROFILE " "
                           "NF-TYPE=" NF-TYPE " "
                           "USE-AFFINITY=" USE-AFFINITY
                           "~%"))
  
  ;; Read command-line options to initialize the "option" global object.
  (csym::set-option argc argv)

  ;; Connect to a Tascell server
  (= sv-socket (if-exp option.sv-hostname
		   (csym::connect-to option.sv-hostname option.port)
		 -1))

  ;; Initialized mutex attribute
  (def m-attr pthread-mutexattr-t)
  (csym::pthread-mutexattr-init (ptr m-attr))
  ;; NOTE: this may be necessary when implementing dropping out (uncompleted)
  ;; (csym::pthread-mutexattr-settype (ptr m-attr) PTHREAD-MUTEX-RECURSIVE-NP)  
  
  ;; Initialize send-mut (lock for sending out external messages)
  (csym::pthread-mutex-init (ptr send-mut) (ptr m-attr))

  ;; Initialize data-mut/data-cond (lock/condition variable for on-demand data)
  (csym::pthread-mutex-init (ptr data-mutex) (ptr m-attr))
  (csym::pthread-cond-init (ptr data-cond) 0)
  
  ;; Initialize thread-data objects (workers)
  (= num-thrs option.num-thrs)
  (for ((= i 0) (< i num-thrs) (inc i))
    (let ((thr (ptr (struct thread-data)) (+ threads i))
          (tx (ptr (struct task)))
          (hx (ptr (struct task-home))))
      (= thr->req 0)
      (= thr->req-cncl 0)
      (= thr->id i)
      (= thr->w-rack 0)
      (= thr->w-none 0)
      (= thr->w-bcak 0)
      (= thr->ndiv 0)
      (= thr->probability 1.0)
      (= thr->last-treq i)
      (= thr->last-choose CHS-RANDOM)
      (= thr->exiting EXITING-NORMAL)
      (= thr->exception-tag 0)
      (let ((r double) (q double))
        (= r (csym::sqrt (+ 0.5 i)))
        (= q (csym::sqrt (+ r i)))
        (-= r (cast int r))
        (= thr->random-seed1 r)
        (= thr->random-seed2 q))
      (= (aref thr->random-seed-probability 0) (+ 3 (* 3 i)))
      (= (aref thr->random-seed-probability 1) (+ 4 (* 3 i)))
      (= (aref thr->random-seed-probability 2) (+ 5 (* 3 i)))
      (csym::pthread-mutex-init (ptr thr->mut) (ptr m-attr))
      (csym::pthread-mutex-init (ptr thr->rack-mut) (ptr m-attr))
      (csym::pthread-cond-init (ptr thr->cond) 0)
      (csym::pthread-cond-init (ptr thr->cond-r) 0)

      ;; Initialize a task stack
      (= tx (cast (ptr (struct task))
              (csym::malloc (* (sizeof (struct task)) TASK-LIST-LENGTH))))
      (csym::initialize-task-list tx TASK-LIST-LENGTH
                                  (ptr thr->task-top) (ptr thr->task-free))

      ;; Initialize a subtask stack (also used for a task request queue)
      (= hx (cast (ptr (struct task-home))
              (csym::malloc (* (sizeof (struct task-home)) TASK-LIST-LENGTH))))
      (csym::initialize-task-home-list hx TASK-LIST-LENGTH
                                       (ptr thr->treq-top) (ptr thr->treq-free))
      (= thr->sub 0)

      ;; open timechart output file
      (PROF-CODE
       (if option.timechart-file
	   (let ((fname (ptr char)) (len int))
	     (= len (+ (csym::strlen option.timechart-file) 10))
	     (= fname (csym::malloc (* (sizeof char) len)))
	     (csym::snprintf fname len "%s-%04d.dat"
			     option.timechart-file thr->id)
	     (= thr->fp-tc (csym::fopen fname "w"))
	     (if (not thr->fp-tc) (csym::perror "Failed to open timechart-file for writing"))
	     (csym::free fname))
	 (= thr->fp-tc 0)))
      ))
  
  ;; Create and run worker threads
  (for ((= i 0) (< i num-thrs) (inc i))
    (let ((thr (ptr (struct thread-data)) (+ threads i)))
      (PROF-CODE                       ; initialize time counter
       (csym::initialize-evcounter thr)
       (csym::initialize-tcounter thr))
      (systhr-create (ptr thr->pthr-id) worker thr)))

  ;; If option.initial-task is given, send a task to the 0-th worker
  ;; as if this node received a "task" message as the first message.
  (if option.initial-task
      (begin
        (def p-src (ptr char))
        (def p-dst (ptr char))
        (def header (ptr char))
        (= header "task 0 0 0 ")
        (= receive-buf
           (cast (ptr char) (csym::malloc (* (+ 3
                                                (csym::strlen option.initial-task)
                                                (csym::strlen header))
                                             (sizeof char)))))
        (= receive-buf-p receive-buf)
        (csym::strcpy receive-buf header)
        (for ((exps (= p-src option.initial-task)
                    (= p-dst (+ receive-buf (csym::strlen header))))
              (mref p-src)
              (exps (inc p-src) (inc p-dst)))
          (= (mref p-dst) (if-exp (== #\Space (mref p-src)) #\Newline (mref p-src))))
        (= (mref (inc p-dst)) #\Newline)
        (= (mref (inc p-dst)) #\Newline)
        (= (mref p-dst) 0)
        (csym::sleep 1)
        (if (>= option.verbose 1)
            (csym::fprintf stderr "%s" receive-buf))
        ))
  ;; Loop for handling external messages
  (while 1
    (= pcmd (csym::read-command))
    (if pcmd
      (begin
        (csym::proc-cmd pcmd 0))
      (begin
        (while 1
          (csym::sleep 2147483647)))) )
  (csym::exit 0))
