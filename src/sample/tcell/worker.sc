;;; Copyright (c) 2009-2011 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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
#+tcell-gtk (c-exp "#include<gtk/gtk.h>")
(%cinclude "sock.h")

(%include "worker.sh")

(%if* VERBOSE
  (c-exp "#define NDEBUG"))
(c-exp "#include<assert.h>")

;;; �ǥХå���������Ѥ��ѿ�
(%ifdef* VERBOSE
         (static ext-cmd-status (array char 128) "") ; ������å�������������åɤξ���
         (static n-dreq-handler int 0)  ; ��ư���Ƥ���dreq-handler�ο�
         (static n-sending-dreq int 0)  ; ���� send-dreq-for-required-range��ʴޥ�å������Ԥ���
         (static n-sending-data int 0)  ; ���� data������ʴޥ�å������Ԥ���
         (static n-waiting-data int 0)) ; sending-data�Τ��� data�Ԥ���


(%defmacro xread (tp exp)
  `(mref (cast (ptr (volatile ,tp)) (ptr ,exp))))
;; exp�����������֡��ӥ�����������
(%defmacro pthread-cond-busywait (exp pmut)
  `(begin
    (csym::pthread-mutex-unlock ,pmut)
    (while ,exp)
    (csym::pthread-mutex-lock ,pmut)
    ))


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

;;; ���ߤ����л����ޥ�������ñ�̤�����������
(def (csym::get-universal-real-time) (csym::fn int)
  (def now (struct timeval))
  (csym::gettimeofday (ptr now) 0)
  (return (+ (* 1000 1000 now.tv-sec) now.tv-usec)))

;;; ���顼��å�����str�ȥ��ޥ�ɤ�stderr�˽���
(def (csym::proto-error str pcmd) (csym::fn void (ptr (const char)) (ptr (struct cmd)))
  (def i int)
  (def buf (array char BUFSIZE))
  (csym::serialize-cmd buf pcmd)
  (csym::fprintf stderr "(%d): %s> %s~%" (csym::get-universal-real-time) str buf))

;; �����ؤ�������å�
(def send-mut pthread-mutex-t)

;; �����Фؤ������������å�(<0:stdin/out)��main������
(def sv-socket int)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ���������

;; EOL�ޤǤ�̵��
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

;;; fgets��OUTSIDE����Υ�å������������� -> cmd-buf �˽񤭹���
;;; 1�Ԥ��ɤ߹�����塤���ڡ�����NULLʸ�����֤������뤳�Ȥ�ʸ�����ʬ�䤹�롥
;;; �ޤ���cmd.v[]��ʬ�䤵�줿��ʸ�����ؤ��褦�ˤ���
;;;   fgets �� 0-terminated buffer ���֤���
;;;   fgets returns 0 �ΤȤ����н�ɬ��?
;;; buf, cmd-buf �ϰ�ĤʤΤ���աʺ���main���ܥ���åɤ�������ʤ���
;; ��������Υ�å����������Хåե�
(def buf (array char BUFSIZE))
(def cmd-buf (struct cmd))

(def (csym::read-command) (csym::fn (ptr (struct cmd)))
  (defs char p c)
  (def b (ptr char) buf)
  (def cmdc int)

  (csym::receive-line b BUFSIZE sv-socket)
  (= cmd-buf.node OUTSIDE)
  (DEBUG-PRINT 1 "(%d): RECEIVED> %s" (csym::get-universal-real-time) b)
  ;; p:�������ʸ����c:���ߤ�ʸ��
  (csym::deserialize-cmd (ptr cmd-buf) b)
  (return (ptr cmd-buf)))

;;; struct cmd -> output��stdout�ء�
;;; task, rslt�Ǥ� body�����Ƥ�task-no�����ꤹ��ؿ�������
(def send-buf (array char BUFSIZE))
(def (csym::send-out-command pcmd body task-no)
    (csym::fn void (ptr (struct cmd)) (ptr void) int)
  (def ret int)
  (def w (enum command))
  (= w pcmd->w)
  ;; <--- sender-lock <---
  (csym::pthread-mutex-lock (ptr send-mut))
  ;; ���ޥ��̾
  (csym::serialize-cmd send-buf pcmd)
  (csym::send-string send-buf sv-socket)
  (csym::write-eol)
  ;; TASK, RSLT, DATA��body�����ؿ���Tascell�ץ��������ˤ�ƤӽФ�
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
    ;; data-mut�ϥ�å���
    (csym::data-send (aref pcmd->v 1 0) (aref pcmd->v 1 1))
    (csym::write-eol)))

  (csym::flush-send)
  (csym::pthread-mutex-unlock (ptr send-mut))
  ;; ---> sender-lock --->
  (if (and (== w RSLT) option.auto-exit)
      (csym::exit 0))
  )

;;; �ʼ�����cmd����äƥ�å������μ����Ŭ�����ؿ���ƽФ�
;;; body�ϥ��������֥������ȡ������̿��ΤȤ���
;;; �ޤ���0�ʳ����̿��ΤȤ������input stream�������������äƺ���
(def (csym::proc-cmd pcmd body) (csym::fn void (ptr (struct cmd)) (ptr void))
  (def w (enum command))
  (= w pcmd->w)
  (switch w
   (case TASK) (csym::recv-task pcmd body) (break)
   (case RSLT) (csym::recv-rslt pcmd body) (break)
   (case TREQ) (csym::recv-treq pcmd) (break)
   (case NONE) (csym::recv-none pcmd) (break)
   (case BACK) (csym::recv-back pcmd) (break)
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

;;; �Ρ�����/������˥��ޥ�ɤ�����
;;; body��task, rslt�����Ρ�����ʳ��Υ��ޥ�ɤǤ�NULL��
(def (csym::send-command pcmd body task-no) (csym::fn void (ptr (struct cmd)) (ptr void) int)
  (if (== pcmd->node INSIDE)
      (begin         ; �����������ʤȤ�����worker���Ȥ����ޥ�ɽ�����
       (DEBUG-STMTS 3 (if (or (>= option.verbose 4)
                              (and (!= TREQ pcmd->w) (!= NONE pcmd->w)))
                          (csym::proto-error "INSIDE" pcmd)))
       (csym::proc-cmd pcmd body))
    (begin
      (DEBUG-STMTS 1 (csym::proto-error "OUTSIDE" pcmd))
      (csym::send-out-command pcmd body task-no) ; ����������
      )
    ))

(def threads (array (struct thread-data) 128))
(def prefetch-thr (ptr (struct thread-data)))
(def prefetch-thr-id int)
(def num-thrs unsigned-int)


;;; Called by recv-exec-send and wait-rslt.
;;; The lock for 'thr' must have been acquried.
;;; Flushes treq messages that have been once accepted, and sends none messages for them.
;;; Stealing-back treq is not be flushed, unless the task of which the requester waiting
;;; for the result of equals to the specified by rslt-head and rslt-to
(def (csym::flush-treq-with-none thr rslt-head rslt-to)
    (csym::fn void (ptr (struct thread-data)) (ptr (enum addr)) (enum node))
  (def rcmd (struct cmd))
  (def pcur-hx (ptr (ptr (struct task-home))) (ptr thr->treq-top)) ; ref to task-home link to be updated
  (def hx (ptr (struct task-home)))
  (def flush int)
  (def ignored int 0)
  (def flushed-any-treq int 0)
  (def flushed-stealing-back-head (ptr (enum addr)) 0)
  (= rcmd.c 1)
  (= rcmd.w NONE)
  (while (= hx (mref pcur-hx))
    (cond 
     ((or option.always-flush-accepted-treq      ; flush if the option specified
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
        (begin
          (= rcmd.node hx->req-from)    ; ����or����
          (csym::copy-address (aref rcmd.v 0) hx->task-head)
          (csym::send-command (ptr rcmd) 0 0)
          (= (mref pcur-hx) hx->next)    ; treq�����å���pop
          (= hx->next thr->treq-free)   ; �ե꡼�ꥹ�Ȥ�...
          (= thr->treq-free hx))        ; ...�ΰ���ֵ�
      (begin
        ;; ignores stealing back treq
        (= pcur-hx (ptr hx->next)))))
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

;;; ptv-src(timeval) �� diff�ʥ��� ­���������pts-dst(timespec)�������
(def (csym::timeval-plus-nsec-to-timespec pts-dst ptv-src diff)
    (fn void (ptr (struct timespec)) (ptr (struct timeval)) long)
  (def nsec long (+ diff (* 1000 ptv-src->tv-usec)))
  (= pts-dst->tv-nsec (if-exp (> nsec 999999999)
                              (- nsec 999999999)
                              nsec))
  (= pts-dst->tv-sec (+ ptv-src->tv-sec
                        (if-exp (> nsec 999999999) 1 0)))
  )

;;; TREQ���������ơ�TASK�������줿�顤
;;; thr->task-top�λؤ�task��������TASK-INITIALIZED�ˡˤ���
;;; treq-head, req-to: �ɤ��˥������׵��Ф�����recv-exec-send �ΰ������Τޤޡ�
;;; �������treq�Υե�å���䡤rslt���褿�Ȥ����б���Ԥ�
;;; INITIALIZED�Ǥ���->1 �����Ǥʤ����->0 ���֤�
(def (csym::send-treq-to-initialize-task thr treq-head req-to)
    (fn int (ptr (struct thread-data)) (ptr (enum addr)) (enum node))
  (def rcmd (struct cmd))
  (def delay long 1000)     ; none ���֤äƤ����Ȥ��ԤĻ���[nsec]
  (def tx (ptr (struct task)) thr->task-top)

  ;; Treq���ޥ��
  (= rcmd.c 2)
  (= rcmd.node req-to) ; ����֤���μ��̡�INSIDE|OUTSIDE��
  (= rcmd.w TREQ)
  (= (aref rcmd.v 0 0) thr->id)
  (if (and (!= req-to ANY) thr->sub)
      (begin
       ;; ����֤��ʤ�С��Ԥ���碌�Ƥ��륿������id��ޤ��
       ;; ��try-take-back-prefetched-task �Ǥ�Ʊ������ǧ�Τ����
       (= (aref rcmd.v 0 1) thr->sub->id)
       (= (aref rcmd.v 0 2) TERM))
    (= (aref rcmd.v 0 1) TERM))
  (csym::copy-address (aref rcmd.v 1) treq-head) ; �׵���

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
     (csym::send-command (ptr rcmd) 0 0) ; treq����
     (csym::pthread-mutex-lock (ptr thr->mut)))
    ;; recv-task �� *tx ����������Τ��Ԥ�
    (loop ; none��å�������TASK-NONE or task��å�������TASK-INITIALIZED �ˤʤ�ޤǥ롼��
       ;; rslt �����夷�Ƥ����顤���̤���ˤ�����
       (if (and (!= tx->stat TASK-INITIALIZED)
                thr->sub                 ; rslt��....
                (== thr->sub->stat TASK-HOME-DONE)) ;...���夷������
           ;; rslt �����夹��褦�ʾ�硤�����Ǥ�treq�ϼ��ᤷ�Ǥ��ꡤ
           ;; ���μ��ᤷ�ϼ��Ԥ��ơ�none���֤����Ϥ��Ǥ��롥
           ;; ���Ǥ� none ���֤���Ƥ��뤫�ǡ��Ԥ����Υ����󥿤�
           ;; ���䤹���ɤ������ۤʤ�
           (begin
            (if (!= tx->stat TASK-NONE)
                (inc thr->w-none))
            (return 0)))
       (if (!= tx->stat TASK-ALLOCATED) (break))
       ;; tx->stat�ޤ��� thr->sub->stat���Ѳ����Ԥ�
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
         ;; ����֤����Ԥʤ餷�Ф餯�Ԥ�
         (if (%ifdef* BUSYWAIT 1 %else thr->sub)
             (let ((t-until (struct timespec))
                   (now (struct timeval)))
               (csym::gettimeofday (ptr now) 0)
               (csym::timeval-plus-nsec-to-timespec (ptr t-until) (ptr now) delay)
               (csym::pthread-cond-timedwait (ptr thr->cond-r)
                                             (ptr thr->mut)
                                             (ptr t-until))
               (+= delay delay)         ; ������Ԥ����֤����䤹
               (if (> delay DELAY-MAX) (= delay DELAY-MAX))
               ))
         ;; rslt�����夷�Ƥ����鼫ʬ��treq��ȥ饤������������ν�����ͥ��
         (if (and thr->sub
                  (== thr->sub->stat TASK-HOME-DONE))
             (return 0))))
    )
  (return 1))

;;; worker or wait-rslt ����
;;; �ʸ�Ԥϳ����ꤲ���������η���Ԥ�����̤λŻ������Ȥ������
;;; �������׵� -> ������� -> �׻� -> �������
;;; thr->mut �ϥ�å���
;;; treq-head, req-to: �ɤ��˥������׵��Ф�����any or ����֤����
(def (recv-exec-send thr treq-head req-to)
    (fn void (ptr (struct thread-data)) (ptr (enum addr)) (enum node))
  (def tx (ptr (struct task)))
  (def old-ndiv int)
  (def rcmd (struct cmd))               ; for RSLT command

  ;; �������ä�treq�ʼ���ᤷ�ˤؤ� none ���Ϥ��ޤ��Ԥ�
  ;; �ʤ��줫������treq���Ф����ΤȺ�Ʊ���ʤ������
  (while (> thr->w-none 0)
    (csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut))
    ;; rslt�����Ƥ����鼫ʬ�ν�����Ƴ��Ǥ���Τ�return
    (if (and thr->sub
             (== thr->sub->stat TASK-HOME-DONE))
        (return)))

  ;; allocate
  (= tx (csym::allocate-task thr))
  ;; treq����->�Ż��Ԥ�
  (if (csym::send-treq-to-initialize-task thr treq-head req-to)
      (begin
       ;; �������¹Գ���
       ;; �����ǡ�tx(=thr->task-top)->stat��TASK-INITIALIZED
       (= tx->stat TASK-STARTED)
       (= old-ndiv thr->ndiv)
       (= thr->ndiv tx->ndiv)
       (csym::pthread-mutex-unlock (ptr thr->mut))
       (DEBUG-PRINT 1 "(%d): (Thread %d) start %d<%p>.~%"
                    (csym::get-universal-real-time) thr->id tx->task-no tx->body)
       ((aref task-doers tx->task-no) thr tx->body) ; �������¹�
       (DEBUG-PRINT 1 "(%d): (Thread %d) end %d<%p>.~%"
                    (csym::get-universal-real-time) thr->id tx->task-no tx->body)
       ;; task�ν�����λ��ϡ�����task-home��send-rslt����
       (= rcmd.w RSLT)
       (= rcmd.c 1)
       (= rcmd.node tx->rslt-to)        ; ����or����
       (csym::copy-address (aref rcmd.v 0) tx->rslt-head)
       (csym::send-command (ptr rcmd) tx->body tx->task-no)
       (inc thr->w-rack)
       (csym::pthread-mutex-unlock (ptr thr->rack-mut))
       (csym::pthread-mutex-lock (ptr thr->mut))
       (= thr->ndiv old-ndiv)))

  ;; ������stack��pop���ƥե꡼�ꥹ�Ȥ��֤�
  (= tx->stat TASK-DONE)
  ;; Flushes all the once accepted treqs by sending nones.
  ;; Notice that stealing-back to the task of which just sent the result should be flushed, too.
  (csym::flush-treq-with-none thr tx->rslt-head tx->rslt-to)
  (csym::deallocate-task thr)
  )


;;; �������åɤ򥳥���Ž���դ���
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

;;; ����Υ롼��
(def (worker arg) (fn (ptr void) (ptr void))
  (def thr (ptr (struct thread-data)) arg)
  (= thr->wdptr (csym::malloc (sizeof (struct thread-data))))
  (%ifdef* USE-AFFINITY
    (if option.affinity
      (csym::worker-setaffinity thr->id)))
  (csym::worker-init thr)
  (csym::pthread-mutex-lock (ptr thr->mut))
  (loop
    (recv-exec-send thr (init (array (enum addr) 2) (array ANY TERM)) INSIDE))
  (csym::pthread-mutex-unlock (ptr thr->mut)))


;;; �굡Ū��treq�����ä�task��������륹��åɤΥ롼��
;;; ���ΤȤ����굡Ū�ˤȤ��Ż��ο���1�Ǹ���
(def (prefetcher thr0) (fn (ptr void) (ptr void))
  (def treq-head (array (enum addr) 2))
  (def req-to (enum node) OUTSIDE)
  (def thr (ptr (struct thread-data)) (cast (ptr (struct thread-data)) thr0))
  (= (aref treq-head 0) ANY) (= (aref treq-head 1) TERM)
  (csym::pthread-mutex-lock (ptr thr->mut))
  (loop
    ;;�ꥹ�Ȥ����ˤʤ�ޤ��Ե�
    (while thr->task-top
       (csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut)))
    (csym::allocate-task thr)
    ;; treq�����äƻŻ��������Ȥ��ޤǤ��
    (while (not (csym::send-treq-to-initialize-task thr treq-head req-to)))
    ;;�ꥹ�Ȥ����Ǥʤ��ʤä����Ȥ�����
    (csym::pthread-cond-broadcast (ptr thr->cond))
    )
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (return 0)
  )

;;; prefetch����task��pop���ơ�thr��task-top��INIITIALIZE����
(def (csym::pop-prefetched-task thr) (fn int (ptr (struct thread-data)))
  (defs (ptr (struct task)) tx-dst tx-src)
  (csym::pthread-mutex-lock (ptr prefetch-thr->mut))
  ;;�ꥹ�Ȥ����Ǥʤ��ʤ�ޤ��Ե�
  (while (not (and prefetch-thr->task-top
                   (== prefetch-thr->task-top->stat TASK-INITIALIZED)))
    (csym::pthread-cond-wait (ptr prefetch-thr->cond) (ptr prefetch-thr->mut)))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= tx-dst thr->task-top) (= tx-src prefetch-thr->task-top)
  (= tx-dst->task-no tx-src->task-no)
  (= tx-dst->body tx-src->body)
  (= tx-dst->ndiv tx-src->ndiv)
  (= tx-dst->rslt-to tx-src->rslt-to)
  (csym::copy-address tx-dst->rslt-head tx-src->rslt-head)
  (= tx-dst->stat TASK-INITIALIZED)
  (csym::deallocate-task prefetch-thr)
  ;;�Ż����Ǥ���褦�ˤʤä����Ȥ�����
  (csym::pthread-cond-broadcast (ptr thr->cond))
  ;;�ꥹ�Ȥ����ˤʤä����Ȥ�����
  (csym::pthread-cond-broadcast (ptr prefetch-thr->cond))
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::pthread-mutex-unlock (ptr prefetch-thr->mut))
  (return 1))

;;; ������prefetch����åɤؤ�treq�ʼ���֤��ˤ��Ф����ޤ����λŻ���Ϥ�Ƥʤ����
;;; ���Τޤ��ֵѤ��롥�ֵѤ���->1, ���ʤ��ä�->0 ���֤���
(def (csym::try-take-back-prefetched-task treq-head) (fn int (ptr (enum addr)))
  (def tx (ptr (struct task)))
  (def retval int)
  (csym::pthread-mutex-lock (ptr prefetch-thr->mut))
  (if (and (= tx prefetch-thr->task-top) ; prefetch����åɤ����äƤ��륿������treq�������ä���Τ�Ʊ�줫Ƚ��
           (== TASK-INITIALIZED tx->stat)
           (csym::address-equal tx->rslt-head treq-head))
      (let ((rcmd (struct cmd)))
        ;; �Ż��Υ���󥻥��������
        (= rcmd.w BACK) (= rcmd.c 1) (= rcmd.node OUTSIDE)
        (csym::copy-address (aref rcmd.v 0) treq-head)
        (csym::send-command (ptr rcmd) 0 0)
        ;; prefetch�����Ż����˴�
        (csym::deallocate-task prefetch-thr) ; !!! body�Τ��ߤ�ɤ����롩
        (csym::pthread-cond-broadcast (ptr prefetch-thr->cond)) ; ���ˤʤä����Ȥ�����
        (= retval 1))
    (= retval 0))
  (csym::pthread-mutex-unlock (ptr prefetch-thr->mut))
  (return retval))


;;;; recv-* �ϡ���������åɡ���������ӳ�����2�ġˤΤ߼¹�

;;; recv-task
;;; task <��������ʬ�䤵�줿���> <������> <������ʼ�ʬ��> <�������ֹ�>
(def (csym::recv-task pcmd body) (csym::fn void (ptr (struct cmd)) (ptr void))
  (def tx (ptr (struct task)))
  (def thr (ptr (struct thread-data)))
  (def id (enum addr))
  (def task-no int)
  (def len size-t)
  ;; �ѥ�᡼���������å�
  (if (< pcmd->c 4)
      (csym::proto-error "wrong-task" pcmd))
  ;; ��������Υ�å������ξ�硤���ޥ�ɤ�³��task���Τ�������
  ;; ����������ξ��ϰ�����Ϳ�����Ƥ����
  (= task-no (aref pcmd->v 3 0))
  (if (== pcmd->node OUTSIDE)           ; ���������task�ξ��Ϥ�����body��������
      (begin
       (= body ((aref task-receivers task-no)))
       (csym::read-to-eol)))
  ;; <task-head>�򸫤ơ���������¹Ԥ��륹��åɤ���롥
  (= id (aref pcmd->v 2 0))
  (if (not (< id (+ num-thrs (if-exp option.prefetch 1 0))))
      (csym::proto-error "wrong task-head" pcmd))
  (= thr (+ threads id))                ; thr: task��¹Ԥ��륹��å�

  ;; ����åɤ˼¹Ԥ��٤����������ɲä���
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= tx thr->task-top)                  ; tx: thr�����٤��Ż��ꥹ��
  (= tx->rslt-to pcmd->node)            ; ��̤���������� [INSIDE|OUTSIDE]
  (csym::copy-address tx->rslt-head (aref pcmd->v 1))
                                        ; [1]: ���긵���̤�������
  (= tx->ndiv (aref pcmd->v 0 0))       ; [0]: ʬ����
  (= tx->task-no task-no)               ; �������ֹ�
  ;; �������Υѥ�᡼����task specific�ʹ�¤�Ρˤμ������
  (= tx->body body)
  (= tx->stat TASK-INITIALIZED)
  (csym::pthread-mutex-unlock (ptr thr->mut))

  ;; �Ż��Ԥ���̲�äƤ������򵯤���
  (csym::pthread-cond-broadcast (ptr thr->cond))
  )


;;; none <������>
(def (csym::recv-none pcmd) (csym::fn void (ptr (struct cmd)))
  (def thr (ptr (struct thread-data)))
  (def id (enum addr))
  (def len size-t)
  (if (< pcmd->c 1) (csym::proto-error "Wrong none" pcmd))
  (= id (aref pcmd->v 0 0))
  (if (not (or (< id num-thrs)
               (and option.prefetch (== id num-thrs))))
      (csym::proto-error "Wrong task-head" pcmd))
  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (if (> thr->w-none 0)
      (dec thr->w-none)
    (= thr->task-top->stat TASK-NONE))
  (csym::pthread-cond-broadcast (ptr thr->cond)) ; TASK-NONE�ˤʤä����Ȥ�����
  (csym::pthread-mutex-unlock (ptr thr->mut))
  )


;;; task-home�Υꥹ�Ȥ���id�����ꤷ����Τ˰��פ������Ǥ�õ��
(def (csym::search-task-home-by-id id hx) (fn (ptr (struct task-home)) int (ptr (struct task-home)))
  (while (and hx (!= hx->id id))
    (= hx hx->next))
  (return hx))

;;; back <������>:<task-id>
;;; ���������Ż��Υ���󥻥����Ρ�task-home�ˤ���Ż���ʬ���ȤǤ��褦�ˤ��롥
;;; ����󥻥뤵�줿�Ż��� sub�����å��Υȥåפˤ����ΤʤΤǡ�����������������
(def (csym::recv-back pcmd) (csym::fn void (ptr (struct cmd)))
  (def thr (ptr (struct thread-data)))
  (def tx (ptr (struct task)))
  (def hx (ptr (struct task-home)))
  (def thr-id (enum addr))
  (def tsk-id int)
  (if (< pcmd->c 1) (csym::proto-error "Wrong back" pcmd))                  ; �����ο������å���0:������
  (= thr-id (aref pcmd->v 0 0)) (= tsk-id (aref pcmd->v 0 1))
  (if (not (< thr-id num-thrs)) (csym::proto-error "Wrong task-head" pcmd))
  (= thr (+ threads thr-id))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (if (not (= hx (csym::search-task-home-by-id tsk-id thr->sub)))
      (begin
       (csym::proto-error "Wrong rslt-head (specified task not exists)" pcmd)
       (csym::print-status 0)
       (csym::exit 1)
       ))
  (= tx thr->task-top) (= hx thr->sub)
  (= tx->task-no hx->task-no)
  (= tx->body hx->body)
  (= tx->ndiv thr->ndiv)
  (= tx->rslt-to INSIDE)
  (= (aref tx->rslt-head 0) thr-id)
  (= (aref tx->rslt-head 1) hx->id)
  (= (aref tx->rslt-head 2) TERM)
  (= hx->req-from INSIDE)               ; task�������������ѹ�
  (= (aref hx->task-head 0) thr-id)     ;
  (= (aref hx->task-head 1) TERM)       ;
  (= tx->stat TASK-INITIALIZED)
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::pthread-cond-broadcast (ptr thr->cond)) ; TASK-INITIALIZED�ˤʤä����Ȥ�����
  )


(def (csym::recv-rslt pcmd body) (csym::fn void (ptr (struct cmd)) (ptr void))
  (def rcmd (struct cmd))               ; rack���ޥ��
  (def thr (ptr (struct thread-data)))
  (def hx (ptr (struct task-home)))
  (def tid (enum addr))
  (def sid int)
  ;; �����ο������å�
  (if (< pcmd->c 1)
      (csym::proto-error "Wrong rslt" pcmd))
  ;; ��̼���ͷ��� "<thread-id>:<task-home-id>"
  (= tid (aref pcmd->v 0 0))
  (if (not (< tid num-thrs))
      (csym::proto-error "wrong rslt-head" pcmd))
  (= sid (aref pcmd->v 0 1))
  (if (== TERM sid)
      (csym::proto-error "Wrong rslt-head (no task-home-id)" pcmd))
  (= thr (+ threads tid))
  
  (csym::pthread-mutex-lock (ptr thr->mut))
  ;; hx = �֤äƤ���rslt���ԤäƤ���task-home��.id==sid�ˤ�õ��
  (if (not (= hx (csym::search-task-home-by-id sid thr->sub)))
      (csym::proto-error "Wrong rslt-head (specified task not exists)" pcmd))
  ;; ��������Υ�å������ξ�硤���ޥ�ɤ�³��rslt���Τ�������
  ;; ����������ξ��ϰ�����Ϳ�����Ƥ����
  (cond
   ((== pcmd->node OUTSIDE)
    ((aref rslt-receivers hx->task-no) hx->body)
    (csym::read-to-eol))
   ((== pcmd->node INSIDE)
    (= hx->body body))
   (else
    (csym::proto-error "Wrong cmd.node" pcmd)))
  ;; rack���֤�����äȸ�Τۤ����褤��
  (= rcmd.c 1)
  (= rcmd.node pcmd->node)
  (= rcmd.w RACK)
  (csym::copy-address (aref rcmd.v 0) hx->task-head)
                                        ; �����补rslt�ǤϤʤ�����Ȥ�task���ޥ�ɤΤ�Ф��Ƥ���
  ;; hx ��˵�Ͽ���줿 task-head �� rack ��������ʤ顤
  ;; �����ǤϤʤ������ޤ� free ���줿���ʤ��Τǡ��Ĥʤ��ʤ�������
  (= hx->stat TASK-HOME-DONE)
  (if (== hx thr->sub)
      (begin
       (csym::pthread-cond-broadcast (ptr thr->cond-r))
       (csym::pthread-cond-broadcast (ptr thr->cond)))
    )
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::send-command (ptr rcmd) 0 0))  ;rack����

;; The Thread 'thr' has the task specified by [<addr>,...,<ID>]x(INSIDE|OUTSIDE) ?
(def (csym::have-task thr task-spec task-from) 
    (csym::fn int (ptr (struct thread-data)) (ptr (enum addr)) (enum node))
  (def tx (ptr (struct task)))
  (= tx thr->task-top)
  (while tx
    (if (and (or (== tx->stat TASK-INITIALIZED)
                 (== tx->stat TASK-SUSPENDED)
                 (== tx->stat TASK-STARTED))
             (== tx->rslt-to task-from)
             (csym::address-equal tx->rslt-head task-spec))
        (return 1))
    (= tx tx->next))
  (return 0))


(decl task-stat-strings (array (ptr char)))
;;; Check if the id-th worker can spawn a task by the 'pcmd' treq message.
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
  (= avail (not fail-reason))
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
            (default)
            (csym::strcpy rsn-str "Unexpected reason") (break))
          (csym::fprintf 
           stderr "(%d): Thread %d refused treq from %s because %s.~%"
           (csym::get-universal-real-time) id from-str rsn-str))))
  (csym::pthread-mutex-unlock (ptr thr->rack-mut))

  ;; If succeeded, push the entry into the requestee's task-home queue.
  (if avail
      (begin
       (= hx thr->treq-free)
       (if (not hx)
           (csym::mem-error "Not enough task-home memory"))
       (= thr->treq-free hx->next)      ; �ե꡼�ꥹ�Ȥ����ΰ�����
       (= hx->next thr->treq-top)       ; ������next�ϥ����å��Υ��
       (= hx->stat TASK-HOME-ALLOCATED)
       (if (== (aref dest-addr 0) ANY)
           (= (aref hx->waiting-head 0) TERM)
         (csym::copy-address hx->waiting-head from-addr))
       (csym::copy-address hx->task-head (aref pcmd->v 0)) ; v[0]: ������
       (if (!= pcmd->node OUTSIDE)
           (= hx->req-from INSIDE)
         (= hx->req-from OUTSIDE))
       (= thr->treq-top hx)
       (= thr->req hx)
       ))
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (return avail))

;; �Ƥ���Υ�������
(def random-seed1 double 0.2403703)
(def random-seed2 double 3.638732)
;; 0--max-1�ޤǤ�����������֤�
(def (csym::my-random max pseed1 pseed2) (fn int int (ptr double) (ptr double))
  (= (mref pseed1) (+ (* (mref pseed1) 3.0) (mref pseed2)))
  (-= (mref pseed1) (cast int (mref pseed1)))
  (return (* max (mref pseed1))))

;;; treq any������˸ƤФ졤�׵ḵ���ɤ����˱�����
;;; Ŭ������ά�ǡ��ǽ�ˤɤΥ�����䤤��碌�뤫�����
(def (csym::choose-treq from-addr) (fn int (enum addr))
  (cond
   ((<= 0 from-addr)                    ; ��������
    (let ((thr (ptr (struct thread-data)) (+ threads from-addr)))
      ;; ��ά��������Ѥ���
      (= thr->last-choose (% (+ 1 thr->last-choose) NKIND-CHOOSE))
      (cond
       ((== CHS-RANDOM thr->last-choose) ; ������
        (return (csym::my-random num-thrs
                                 (ptr thr->random-seed1)
                                 (ptr thr->random-seed2))))
       ((== CHS-ORDER thr->last-choose) ; ���֤�
        (= thr->last-treq (% (+ 1 thr->last-treq) num-thrs))
        (return thr->last-treq))
       (else
        (return 0)))))
   ((== PARENT from-addr)               ; ��������
    (return (csym::my-random num-thrs (ptr random-seed1) (ptr random-seed2))))
   (else
    (return 0))))

;;; treq��å������ʻŻ��׵�ˤν���
;;; �׵����id�Υ����ANY�ξ������ƤΥ���ˤ˻Ż�ʬ�䤬��ǽ����ǧ
;;; ��ǽ�ʤ��׵ᥭ�塼���ɲä���Τ�(try-treq��)���Τ���task��å������������Ƥ���
;;; �׵ḵ�Υ������ꥹ�ȤΥ����ƥब TASK-ALLOCATED => TASK-INITIALIZED�ˤʤ롥
;;; �Բ�ǽ�ʤ�NONE���֤���
;;; ��������0�֥����ANY�׵�򤷤Ƥ������treq��å�����������ž�����롥
(def (csym::recv-treq pcmd) (csym::fn void (ptr (struct cmd)))
  (def rcmd (struct cmd))
  (def dst0 (enum addr))
  (if (< pcmd->c 2)                     ; �����ο������å� 0:from, 1:to
      (csym::proto-error "Wrong treq" pcmd))
  ;; �Ż����׵᤹�륹��åɤ���ơ��׵��Ф�
  (= dst0 (aref pcmd->v 1 0))
  (cond
   ;; ANY
   ((== dst0 ANY)
    (let ((myid int) (start-id int) (d int) (id int))
      (= myid (aref pcmd->v 0 0))       ; �׵ḵ
      (= start-id (csym::choose-treq myid)) ; �׵��賫��id
      (for ((= d 0) (< d num-thrs) (inc d))
        (= id (% (+ d start-id) num-thrs)) ; �׵���id
        (if (and (!= pcmd->node OUTSIDE)
                 (== id myid))
            (continue))                 ; ��ʬ���Ȥˤ��׵��Ф��ʤ�
        (if (csym::try-treq pcmd id)
            (begin
             (DEBUG-PRINT 2 "(%d): treq(any) %d->%d... accepted.~%"
                          (csym::get-universal-real-time) myid id)
             (break)))
        (DEBUG-PRINT 4 "(%d): treq(any) %d->%d... refused.~%"
                     (csym::get-universal-real-time) myid id)
        )
      (if (!= d num-thrs)               ; treq�Ǥ���
          (return))))
   ;; prefetch����åɤؤ��׵�
   ((and option.prefetch (== dst0 prefetch-thr-id))
    (if (== pcmd->node OUTSIDE)         ; ������μ���֤�
        (if (csym::try-take-back-prefetched-task (aref pcmd->v 0)) ; �ޤ������������Ϥ���Ƥʤ���Ф��Τޤ޼���֤�
            (return)
          (begin                        ; ���Ϥ���Ƥ����0�֥���åɤ������֤�(leapfrogging)
           (= (aref pcmd->v 1 0) 0)
           (csym::recv-treq pcmd)
           (return)))
      ;; (!= pcmd->node OUTSIDE): �����������prefetched���������׵�
      (begin
       (DEBUG-PRINT 2 "Thread %d trying to pop prefetched task.~%" (aref pcmd->v 0 0))
       (if (csym::pop-prefetched-task (+ threads (aref pcmd->v 0 0)))
           (begin
            (DEBUG-PRINT 2 "Thread %d popped prefetched task.~%" (aref pcmd->v 0 0))
            (return))))))
   ;; ����֤�(leapfrogging)
   (else
    (if (not (and (<= 0 dst0) (< dst0 num-thrs)))
        (csym::proto-error "Wrong task-head" pcmd))
    (if (csym::try-treq pcmd dst0)      ; treq�Ǥ���
        (begin
         (DEBUG-STMTS 2
                      (let ((buf1 (array char BUFSIZE)))
                        (csym::fprintf stderr "(%d): treq %s->%d (stealing back)... accepted.~%"
                                       (csym::get-universal-real-time)
                                       (exps (csym::serialize-arg buf1 (aref pcmd->v 0)) buf1) dst0)))
         (return)))
    (DEBUG-STMTS 2
                 (let ((buf1 (array char BUFSIZE)))
                   (csym::fprintf stderr "(%d): treq %s->%d (stealing back)... refused.~%"
                                  (csym::get-universal-real-time)
                                  (exps (csym::serialize-arg buf1 (aref pcmd->v 0)) buf1) dst0))))
   )
  ;; �����Υ�������Ϥ���Ż����ʤ��ä����Τߤ��������
  (if (and (== dst0 ANY)
           (== pcmd->node INSIDE)
           (== (aref pcmd->v 0 0) 0))    ; v[0]:from
      ;; 0��worker�����treq�ξ��ϳ������䤤��碌��
      (if option.prefetch
          (begin                        ; �굡treq����åɤ�
           (= pcmd->node INSIDE)
           (= (aref pcmd->v 1 0) prefetch-thr-id) (= (aref pcmd->v 1 1) TERM)
           (csym::send-command pcmd 0 0)
           (return))
        (begin                          ; ������
         (= pcmd->node OUTSIDE)
         (csym::send-command pcmd 0 0)
         (return))))
  ;; ���ؤ�treqž���⤷�ʤ��ä����Τߤ��������
  ;; none���֤�
  (= rcmd.c 1)
  (= rcmd.node pcmd->node)              ; [INSIDE|OUTSIDE]
  (= rcmd.w NONE)
  (csym::copy-address (aref rcmd.v 0) (aref pcmd->v 0))
  (csym::send-command (ptr rcmd) 0 0))


;; rack <rack������header(�����Ǥ�thread-id)>
(def (csym::recv-rack pcmd) (csym::fn void (ptr (struct cmd)))
  (def tx (ptr (struct task)))
  (def thr (ptr (struct thread-data)))
  (def id (enum addr))
  (def len size-t)
  (if (< pcmd->c 1)                     ; �����ο������å� 0:������ʥ���å�id��
      (csym::proto-error "Wrong rack" pcmd))
  (= id (aref pcmd->v 0 0))
  ;; prefetch����åɤؤ�rack��0�֤�ž��
  ;; ��prefetch������������ɬ��0�֤����褦�ˤ��Ƥ��뤿���
  (if (and option.prefetch (== prefetch-thr-id id))
      (= id 0))
  (if (not (< id num-thrs))
      (csym::proto-error "Wrong task-head" pcmd))
  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr thr->rack-mut))
  (dec thr->w-rack)
  (csym::pthread-mutex-unlock (ptr thr->rack-mut)))


;; ¸�ߥե饰���������Ƭ���������-setup-data�ˤ�
(def data-flags (ptr (enum DATA-FLAG)) 0)
;; ��å�������ѿ�
(def data-mutex pthread-mutex-t)
(def data-cond  pthread-cond-t)

;; Tascell�ؤ��󶡵�ǽ��data������ (n: size)
(def (csym::-setup-data n) (csym::fn void int)
  (def i int)
  (def tmp (ptr (enum DATA-FLAG)))

  (if data-flags (return))
  (csym::pthread-mutex-lock (ptr data-mutex))
  ;; data-flags�ν����
  (if (not data-flags)
      (begin
       (= tmp (cast (ptr (enum DATA-FLAG)) (csym::malloc (* n (sizeof (enum DATA-FLAG))))))
       (for ((= i 0) (< i n) (inc i))
            (= (aref tmp i) DATA-NONE))
       (= data-flags tmp)))
  ;; data���ΰ���ݡ�Tascell�ץ��������ؿ���ƽФ���
  (csym::data-allocate n)
  (csym::pthread-mutex-unlock (ptr data-mutex))
  (return))

;; �׵�������ǡ����Τ�����NONE�ʤ�ΤˤĤ���pcmd��REQUESTING�ʤ�ΤˤĤ���pcmd-fwd������
;; NONE��REQUESTING �˹�������
;; data-mutex�Υ�å��Ϥ�����Ǥ��롥
;; pcmd��pcmd-fwd�� �ǡ����ϰϡ�3�Ĥ�Υѥ�᡼���˰ʳ������åȤ��줿DREQ���ޥ��
;; ������ʤ��Ƥ�������NULL��Ϳ�����
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
      ;; i����j�ޤ��׵� (pcmd) ��Ф�
      (if pcmd
          (begin
            (= (aref pcmd->v 2 0) i)  (= (aref pcmd->v 2 1) j)
            (= (aref pcmd->v 2 2) TERM)
            (csym::send-command pcmd 0 0)))
      (= i (- j 1)))                    ; �׵��Ф����ϰϤμ���������å��Ƴ�
     ((== (aref data-flags i) DATA-REQUESTING)
      (for ((= j (+ i 1)) (and (< j end) (== (aref data-flags j) DATA-REQUESTING)) (inc j))
        )
      ;; i����j�ޤ��׵� (pcmd-fwd) ��Ф�
      (if pcmd-fwd
          (begin
            (= (aref pcmd-fwd->v 2 0) i)  (= (aref pcmd-fwd->v 2 1) j)
            (= (aref pcmd-fwd->v 2 2) TERM)
            (csym::send-command pcmd-fwd 0 0)))
      (= i (- j 1)))                    ; �׵��Ф����ϰϤμ���������å��Ƴ�
     ))
  (csym::pthread-mutex-unlock (ptr data-mutex))
  )

;; thread-id=tid, id=sid��task-home�����task���鸫�ơ�
;; �ǽ�γ����Ρ��ɤˤ��������task-home�Υ��ɥ쥹��head�˥��ԡ�
(def (csym::get-first-outside-ancestor-task-address head tid sid) (csym::fn int (ptr (enum addr)) int int)
  (def thr (ptr (struct thread-data)))
  (def hx (ptr (struct task-home)))
  (def ok int)
  (do-while 1
    (= thr (+ threads tid))
    (csym::pthread-mutex-lock (ptr thr->mut))
    ;; hx = �ǡ����׵����task-home��.id==sid�ˤ�õ��
    (if (not (= hx (csym::search-task-home-by-id sid thr->sub)))
        (csym::fprintf stderr "Error in get-first-outside-ancestor-task-address (specified task not exists)~%"))
    (csym::pthread-mutex-unlock (ptr thr->mut))
    
    (cond
     ((not hx->owner)                   ; �Ƥ����ʤ���Х��顼
      (csym::fprintf stderr "error in get-first-outside-ancestor-task-address: no owner found.~%")
      (csym::print-status 0)
      (csym::exit 1))
     ((== hx->owner->rslt-to OUTSIDE)   ; ���οƤ������Ρ��ɤʤ顤�����������׵���
      (csym::copy-address head hx->owner->rslt-head)
      (break)))
    ;; ���οƤ������Ρ��ɤʤ顤����˿ƥ������򤿤ɤ�
    (= tid (aref hx->owner->rslt-head 0))
    (= sid (aref hx->owner->rslt-head 1)))
  (return ok))

;; Tascell�ؤ��󶡵�ǽ��data��ƥ��������׵� (start, end: �ǡ����ϰ�)
(def (csym::-request-data thr start end) (csym::fn void (ptr (struct thread-data)) int int)
  (def cmd (struct cmd))
  (def tx (ptr (struct task)))
  (DEBUG-PRINT 2 "request-data: %d--%d start~%" start end)
  (csym::pthread-mutex-lock (ptr thr->mut)) ; ��å���ǰ�Τ���
  (= tx thr->task-top)                  ; ���¹���Υ�����
  (csym::pthread-mutex-unlock (ptr thr->mut))
  ;; ���ޥ�ɤμ��ࡤ�����ο�
  (= cmd.w DREQ)
  (= cmd.c 3)
  ;; �׵ḵ��thread�ֹ�ϴط��ʤ��Τǥ��ߡ��ֹ�Ǥ褤��
  (= (aref cmd.v 0 0) 0)  (= (aref cmd.v 0 1) TERM)
  ;; �׵��衧ľ�ܤοƤ������Ǥʤ���С�����������é��
  (if (== OUTSIDE tx->rslt-to)
      (csym::copy-address (aref cmd.v 1) tx->rslt-head)
    (csym::get-first-outside-ancestor-task-address
     (aref cmd.v 1) (aref tx->rslt-head 0) (aref tx->rslt-head 1)))
  (= cmd.node OUTSIDE)

  (DEBUG-STMTS 2
               (= (aref cmd.v 2 0) TERM)
               (csym::proto-error "dreq template" (ptr cmd)))
  
  ;; ɬ�פ��ϰϤˤĤ���dreq������
  (csym::send-dreq-for-required-range start end (ptr cmd) 0)
  (DEBUG-PRINT 2 "request-data: %d--%d end~%" start end)
  (return)
  )

;; Tascell�ؤ��󶡵�ǽ��data��·���ޤ��Ե� (start, end: �ǡ����ϰ�)
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

;; ���ꤵ�줿�ϰϤ�data-flags��DATA-EXIST�ˤ���ʻŻ����ϥΡ����ѡ�
(def (csym::-set-exist-flag start end) (csym::fn void int int)
  (def i int)
  (csym::pthread-mutex-lock (ptr data-mutex))
  (for ((= i start) (< i end) (inc i))
    (= (aref data-flags i) DATA-EXIST))
  (csym::pthread-mutex-unlock (ptr data-mutex))
  )



;; recv-dreq�Ǻ���륹��åɤ��¹Ԥ���ؿ�
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
  
  ;; NONE���ϰϤˤĤ��ơ�����˿Ƥ��׵��Ф�
  ;; REQUESTING���ϰϤˤĤ��ơ��׵��forward����
  (%ifdef* VERBOSE (inc n-sending-dreq))
  (csym::send-dreq-for-required-range start end pcmd pcmd-fwd)
  (%ifdef* VERBOSE (dec n-sending-dreq))
  
  ;; INSIDE�����dreq�ʤ�ǡ���������ɬ�פϤʤ��ΤǤ����ǽ����
  (if (== parg->data-to INSIDE)
      (begin
       (%ifdef* VERBOSE (dec n-dreq-handler))
       (return)))

  ;; DATA���ޥ�ɤο���������
  (= data-cmd.w DATA)
  (= data-cmd.c 2)
  (= data-cmd.node parg->data-to)       ; data������(OUTSIDE)
  (csym::copy-address (aref data-cmd.v 0) parg->head) ; data������
  
  ;; data�������Ƥ���Τ�cond-wait���Ԥäơ��缡�׵ḵ������
  (%ifdef* VERBOSE (inc n-sending-data))
  (csym::pthread-mutex-lock (ptr data-mutex))
  (for ((= i start) (< i end) (inc i))
    (while (!= (aref data-flags i) DATA-EXIST)
      (%ifdef* VERBOSE (inc n-waiting-data))
      (csym::pthread-cond-wait (ptr data-cond) (ptr data-mutex))
      (%ifdef* VERBOSE (dec n-waiting-data)) )
    (for ((= j (+ i 1)) (and (< j end) (== (aref data-flags j) DATA-EXIST)) (inc j))
      )
    ;; i����j�ޤ���������Τϡ�send-out-command���������
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

;; dreq <data�׵ḵheader> <data�׵���(<thread-id>:<task-home-id>)> <data�׵��ϰ�(<data-start>:<data-end>)>
(def (csym::recv-dreq pcmd) (csym::fn void (ptr (struct cmd)))
  (def tx (ptr (struct task)))
  (def tid (enum addr))
  (def sid int)
  (def parg (ptr (struct dhandler-arg)))
  (def len size-t)
  ;; �����ο������å�
  (if (< pcmd->c 3) (csym::proto-error "Wrong dreq" pcmd))
  
  ;; ;; dreq-handler���Ϥ�����������
  ;; ;; �����ǡ�dreq-cmd�ˤĤ��ƤϺǸ�ΰ������׵��ϰϡ˰ʳ������ꤷ�Ƥ�����
  (= parg (cast (ptr (struct dhandler-arg)) (csym::malloc (sizeof (struct dhandler-arg)))))
  (= parg->data-to pcmd->node)          ; data�������INSIDE|OUTSIDE)
  (csym::copy-address parg->head (aref pcmd->v 0)) ; data�������INSIDE|OUTSIDE)
  ;; parg->dreq-cmd: ��ʬ�ΤȤ��ˤ�ʤ�(DATA-NONE)���˽Ф�DREQ���ޥ�ɤο���
  (= parg->dreq-cmd.w DREQ)
  (= parg->dreq-cmd.c 3)
  (= (aref parg->dreq-cmd.v 0 0) 0)     ; �׵ḵ�����ߡ�����
  (= (aref parg->dreq-cmd.v 0 1) TERM)  ; �׵ḵ�����ߡ�����
  (begin                                ; ����ʤ�dreq�׵��ɤ��˽Ф�������
   (= tid (aref pcmd->v 1 0))
   (if (not (< tid num-thrs)) (csym::proto-error "wrong dreq-head" pcmd))
   (= sid (aref pcmd->v 1 1))
   (if (== TERM sid) (csym::proto-error "Wrong dreq-head (no task-home-id)" pcmd))
   ;; �ǽ�γ����Ρ��ɤ�task-home�Υ��ɥ쥹�����
   (csym::get-first-outside-ancestor-task-address (aref parg->dreq-cmd.v 1) tid sid)
   )
  (= parg->dreq-cmd.node OUTSIDE)
  ;; parg->dreq-cmd: ��ʬ�ΤȤ��ˤ�ʤ�(DATA-REQUESTING)���˽Ф�DREQ���ޥ�ɤο���
  (= parg->dreq-cmd-fwd.w DREQ)
  (= parg->dreq-cmd-fwd.c 3)
  (= (aref parg->dreq-cmd-fwd.v 0 0) FORWARD) ; �׵ḵ��������ä�DREQ���׵ḵ�˵���
  (csym::copy-address (ptr (aref parg->dreq-cmd-fwd.v 0 1))
                      (aref pcmd->v 0))
  (csym::copy-address (aref parg->dreq-cmd-fwd.v 1) ; �׵��衧��ǵ�᤿��Τ�Ʊ��
                      (aref parg->dreq-cmd.v 1))
  (= parg->dreq-cmd-fwd.node OUTSIDE)
  ;; �׵��ϰ�
  (= parg->start (aref pcmd->v 2 0))
  (= parg->end   (aref pcmd->v 2 1))
  
  ;; �̥���åɤ�dreq-handler��ƽФ�
  (begin
    (def tid pthread-t)
    (csym::pthread-create (ptr tid) 0 csym::dreq-handler parg))
  (return))


;; data <data������header> <data�ϰ�(<data-start>:<data-end>)>
(def (csym::recv-data pcmd) (csym::fn void (ptr (struct cmd)))
  (def i int)
  (def start int (aref pcmd->v 1 0))
  (def end int (aref pcmd->v 1 1))
  ;; �����ο������å�
  (if (< pcmd->c 2) (csym::proto-error "Wrong data" pcmd))
  ;; �ʵ�����ʤ��Ϥ����������������data�Ǥ����̵��
  (if (== pcmd->node INSIDE) (return))
  ;; �ǡ��������ؿ���Tascell�ץ��������ˤ�ƤӽФ�
  (csym::pthread-mutex-lock (ptr data-mutex))
  (csym::data-receive start end)
  (csym::read-to-eol)
  (for ((= i start) (< i end) (inc i))
    (= (aref data-flags i) DATA-EXIST))
  (csym::pthread-cond-broadcast (ptr data-cond))
  (csym::pthread-mutex-unlock (ptr data-mutex))
  (return))


;;; Tascell�ץ���ޤ��󶡤���ؿ�

;; �ƥ������˻��ꤵ�줿�ϰϤΥǡ�����dreq��ȯ�Ԥ���
(decl (csym::request-data) (csym::fn void (ptr (struct thread-data)) int int))
;; ���ꤵ�줿�ϰϤΥǡ�����·���ޤ��Ԥ�
(decl (csym::wait-data) (csym::fn void int int))


;;; recv-bcst
;;; bcst  <���������ɥ쥹>  <�֥��ɥ��㥹�ȼ���>
(def (csym::recv-bcst pcmd) (csym::fn void (ptr (struct cmd)))
  (def rcmd (struct cmd))
  (def task-no int)
  (def body (ptr void))
  ;; �ѥ�᡼���������å�
  (if (< pcmd->c 2)
      (csym::proto-error "wrong-task" pcmd))
  ;; �ǡ����μ��̤��ɤ߼��
  (= task-no (aref pcmd->v 1 0))
  ;; �ǡ������������Τ�Ƥ�
  ;;   receiver �������ǥ��������֥������Ȥ�ҡ��פ˺�ä��֤��Ƥ���
  (= body ((aref task-receivers task-no)))
  (csym::read-to-eol)
  ;; task���Τ�¹Ԥ��롥��1�����ʥ������åɡˤˤ�NULL���Ϥ��Ƥ���
  ((aref task-doers task-no) 0 body)
  (csym::free body)
  ;; bcak ��������������
  (= rcmd.c 1)
  (= rcmd.node pcmd->node)
  (= rcmd.w BCAK)
  (csym::copy-address (aref rcmd.v 0) (aref pcmd->v 0))
  (csym::send-command (ptr rcmd) 0 task-no))

;;; leav
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

;;; lack
(def (csym::recv-lack pcmd) (csym::fn void (ptr (struct cmd)))
  (def cur (ptr (struct task-home)))
  (def task-top (ptr (struct task))) 
  (def thr (ptr (struct thread-data)))
  (def i int)
  (def rcmd (struct cmd))
  ;; Prevent workers from sending out messages.
  (csym::pthread-mutex-lock (ptr send-mut))
  ;; Prevent workers from modifying their own information (task stacks etc.)
  (csym::cancel-workers)
  (for ((= i 0) (< i num-thrs) (inc i))
       (= thr (ptr (aref threads i))
          (= task-top thr->task-top)
          (for ((= cur task-top) cur (= cur cur->next))
               (= rcmd.w ABRT)
               (= rcmd.c 1)
               (= rcmd.node cur->rslt-to)        ; ����or����
               (csym::copy-address (aref rcmd.v 0) cur->rslt-head)
               (csym::send-command (ptr rcmd) 0 0))
          (csym::print-thread-status thr)))
  ;; all command check
  (csym::exit 0))

;;; abrt
(def (csym::recv-abrt pcmd) (csym::fn void (ptr (struct cmd)))
;  (def rcmd (struct cmd))               ; rack���ޥ��
  (def thr (ptr (struct thread-data)))
  (def hx (ptr (struct task-home)))
  (def tid (enum addr))
  (def sid int)
  ;; �����ο������å�
  (if (< pcmd->c 1)
      (csym::proto-error "Wrong abrt" pcmd))
  ;; ��̼���ͷ��� "<thread-id>:<task-home-id>"
  (= tid (aref pcmd->v 0 0))
  (if (not (< tid num-thrs))
      (csym::proto-error "Wrong abrt-head" pcmd))
  (= sid (aref pcmd->v 0 1))
  (if (== TERM sid)
      (csym::proto-error "Wrong abrt-head (no task-home-id)" pcmd))
  (= thr (+ threads tid))
  
  (csym::pthread-mutex-lock (ptr thr->mut))
  ;; hx = �֤äƤ���rslt���ԤäƤ���task-home��.id==sid�ˤ�õ��
  (if (not (= hx (csym::search-task-home-by-id sid thr->sub)))
      (csym::proto-error "Wrong abrt-head (specified task not exists)" pcmd))

  (= hx->stat TASK-HOME-ABORTED)
  (if (== hx thr->sub)
      (begin
       (csym::pthread-cond-broadcast (ptr thr->cond-r))
       (csym::pthread-cond-broadcast (ptr thr->cond)))
    )
  (csym::pthread-mutex-unlock (ptr thr->mut))
;  (csym::send-command (ptr rcmd) 0 0))  ;rack����

  (csym::exit 0))

;;; cncl
(def (csym::recv-cncl pcmd) (csym::fn void (ptr (struct cmd)))
  (csym::exit 0))

;;; recv-bcak
;;; bcak  <�����襢�ɥ쥹>
(def (csym::recv-bcak pcmd) (csym::fn void (ptr (struct cmd)))
  (def thr (ptr (struct thread-data)))
  (def id (enum addr))
  ;; �ѥ�᡼���������å�
  (if (< pcmd->c 1)
    (csym::proto-error "wrong-task" pcmd))
  ;; bcak ���ԤäƤ��������å�������������
  (= id (aref pcmd->v 0 0))
  (= thr (+ threads id))
  ;; ����򵯤���
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= thr->w-bcak 0)
  (csym::pthread-cond-broadcast (ptr thr->cond))
  (csym::pthread-mutex-unlock (ptr thr->mut)))


;;; task�ξ�������
(def task-stat-strings (array (ptr char)) ; enum task-stat���б�
  (array "TASK-ALLOCATED" "TASK-INITIALIZED" "TASK-STARTED" "TASK-DONE" "TASK-NONE" "TASK-SUSPENDED"))
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
    (csym::fprintf stderr "{stat=%s, task-no=%d, body=%p, ndiv=%d, rslt-to=%s, rslt-head=%s}, "
                   (aref task-stat-strings cur->stat) cur->task-no cur->body cur->ndiv
                   (exps (csym::node-to-string buf1 cur->rslt-to) buf1)
                   (exps (csym::serialize-arg buf2 cur->rslt-head) buf2)))
  (csym::fprintf stderr "}, ")
  (return))

;;; task-home�ξ�������
(def task-home-stat-strings (array (ptr char)) ; enum task-home-stat���б�
  (array "TASK-HOME-ALLOCATED" "TASK-HOME-INITIALIZED" "TASK-HOME-DONE"))
(def (csym::print-task-home-list treq-top name) (csym::fn void (ptr (struct task-home)) (ptr char))
  (def cur (ptr (struct task-home)))
  (defs (array char BUFSIZE) buf0 buf1 buf2)
  (csym::fprintf stderr "%s= {" name)
  (for ((= cur treq-top) cur (= cur cur->next))
    (csym::fprintf stderr "{stat=%s, id=%d, waiting=%s, owner=%p, task-no=%d, body=%p, req-from=%s, task-head=%s}, "
                   (aref task-home-stat-strings cur->stat) cur->id
                   (exps (csym::serialize-arg buf0 cur->waiting-head) buf0)
                   cur->owner cur->task-no cur->body
                   (exps (csym::node-to-string buf1 cur->req-from) buf1)
                   (exps (csym::serialize-arg buf2 cur->task-head) buf2)))
  (csym::fprintf stderr "}, ")
  (return))

;;; thread�ξ�������
(def choose-strings (array (ptr char)) (array "CHS-RANDOM" "CHS-ORDER")) ; enum choose���б�
(def (csym::print-thread-status thr) (csym::fn void (ptr (struct thread-data)))
  (csym::fprintf stderr "<Thread %d>~%" thr->id)
  (csym::fprintf stderr "req=%p, " thr->req)
  (csym::fprintf stderr "w-rack=%d, " thr->w-rack)
  (csym::fprintf stderr "w-none=%d, " thr->w-none)
  (csym::fprintf stderr "ndiv=%d, " thr->ndiv)
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

;;; stat���ޥ�� -> ���֤����
(def (csym::print-status pcmd) (csym::fn void (ptr (struct cmd)))
  (def i int)
  (csym::fprintf stderr "worker-name: %s~%"
                 (if-exp option.node-name option.node-name "Unnamed"))
  (csym::fprintf stderr "num-thrs: %d~%" num-thrs)
  (csym::fprintf stderr "prefetches: %d~%" option.prefetch)
  (csym::fprintf stderr "verbose-level: %d~%" option.verbose)
  (%ifdef* VERBOSE
    (csym::fprintf stderr
                   "active dreq-handlers: %d (%d sending dreq, %d sending data (%d waiting data))~%"
                   n-dreq-handler n-sending-dreq n-sending-data n-waiting-data)
    ;; �褯�ͤ�����stat������˷�ޤäƤ���Τǽ��Ϥ����̣���ʤ�
    ;; �Ǥ�gdb�ΥǥХå��ˤ�ͭ�ѤʤΤ��ѿ����ΤϻĤ�
    #+comment (csym::fprintf stderr "external command handler: %s~%" ext-cmd-status))

  (if option.prefetch 
      (begin (csym::print-task-list prefetch-thr->task-top "prefetched tasks")
             (csym::fputc #\Newline stderr)))
  (for ((= i 0) (< i num-thrs) (inc i))
    (csym::print-thread-status (ptr (aref threads i))))
  (return)
  )


;;; verb���ޥ�� -> verbose level���ѹ�
(def (csym::set-verbose-level pcmd) (csym::fn void (ptr (struct cmd)))
  (if (< pcmd->c 1)                     ; �����ο������å� 0:verbose-level
      (csym::proto-error "Wrong verb" pcmd))
  (= option.verbose (aref pcmd->v 0 0))
  (return))



;;; exit���ޥ�� -> ��λ
(def (csym::recv-exit pcmd) (csym::fn void (ptr (struct cmd)))
  (csym::fprintf stderr "Received \"exit\"... terminate.~%")
  (csym::exit 0))


;; �������åɤ��Ż�ʬ�䳫�ϻ��˸Ƥ�
(def (handle-req -bk -thr)
    (fn void (ptr (NESTFN int void)) (ptr (struct thread-data)))
  (csym::pthread-mutex-lock (ptr -thr->mut))
  (if -thr->req
      (begin
       (-bk)
       (= -thr->req -thr->treq-top)))
  (csym::pthread-mutex-unlock (ptr -thr->mut)))

;; �������åɤ�put���˸Ƥ�
;; thr->mut ��å��Ѥ�
(def (csym::make-and-send-task thr task-no body) ; task-no��tcell�ɲ�
    (csym::fn void (ptr (struct thread-data)) int (ptr void))
  (def tcmd (struct cmd))
  (def hx (ptr (struct task-home)) thr->treq-top)
  ;; (csym::fprintf stderr "make-and-send-task(%d)~%" thr->id)
  (= thr->treq-top hx->next)            ; task�׵᥹���å���pop
  (= hx->next thr->sub)                 ; �����ꥵ�֥�����stack�Υ��
  (= thr->sub hx)                       ; ���֥�����stack��push
  (= hx->task-no task-no)
  (= hx->body body)
  (= hx->id (if-exp hx->next            ; ���֥�����ID = �줫�鲿���ܤ�
                    (+  hx->next->id 1)
                    0))
  (= hx->owner thr->task-top)           ; �ƥ�����
  (= hx->stat TASK-HOME-INITIALIZED)
  (= tcmd.c 4)
  (= tcmd.node hx->req-from)
  (= tcmd.w TASK)

  (= (aref tcmd.v 0 0) (++ thr->ndiv))  ; ʬ����
  (= (aref tcmd.v 0 1) TERM)
  (= (aref tcmd.v 1 0) thr->id)         ; ��������� "<worker-id>:<subtask-id>"
  (= (aref tcmd.v 1 1) hx->id)
  (= (aref tcmd.v 1 2) TERM)
  (csym::copy-address (aref tcmd.v 2) hx->task-head) ; �������������try-treq�ǵ����ѡ�
  (= (aref tcmd.v 3 0) task-no)         ; �������ֹ�
  (= (aref tcmd.v 3 1) TERM)
  (csym::send-command (ptr tcmd) body task-no))

;;; �����do-two, do-many�ΤȤ�����ƤФ�롥
;;; ���֥������η�̤μ�����괰λ���Ԥ������֥������ꥹ�Ȥ��鳰����
;;; �ʼ�������Ȥ� rslt��å�������������롼����ˤƼ¹Ԥ�����
;;; ���Ф餯�֤äƤ��ʤ��ä���Ż������֤��ˤ�����
(def (wait-rslt thr) (fn (ptr void) (ptr (struct thread-data)))
  (def body (ptr void))
  (def sub (ptr (struct task-home)))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= sub thr->sub)                      ; ����åɤΥ��֥������֤���
  (while (and (!= sub->stat TASK-HOME-DONE)  ; iterate until the subtask is done
             (!= sub->stat TASK-HOME-ABORTED))
    (= thr->task-top->stat TASK-SUSPENDED)
    ;; �����Ρ��ɤ����ä��Ż��ʤ����ä��Ԥ�
    (if (== OUTSIDE sub->req-from)
        (let ((now (struct timeval))
              (t-until (struct timespec)))
          (csym::gettimeofday (ptr now) 0)
          (csym::timeval-plus-nsec-to-timespec (ptr t-until) (ptr now) 1000)
          (csym::pthread-cond-timedwait (ptr thr->cond-r) (ptr thr->mut)
                                        (ptr t-until))
          ))
    (if (or (== sub->stat TASK-HOME-DONE)
            (== sub->stat TASK-HOME-ABORTED)) 
        (break))  
    ;; ����֤��ˤ��� (leapfrogging)
    (recv-exec-send thr sub->task-head sub->req-from))

  (if (== sub->stat TASK-HOME-ABORTED) 
      (= body 0)
    (= body sub->body))
  (= thr->sub sub->next)                ; ���֥�����stack��pop
  (= sub->next thr->treq-free)          ; pop������ʬ��...
  (= thr->treq-free sub)                ; ...�ե꡼�ꥹ�Ȥ��֤�
  (= thr->task-top->stat TASK-STARTED)
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (return body))


;;; ������֥��ɥ��㥹�Ȥ�¹Ԥ���Ȥ���put��˸ƤФ��
(def (csym::broadcast-task thr task-no body)
   (csym::fn void (ptr (struct thread-data)) int (ptr void))
  (def bcmd (struct cmd))
  ;; �ޤ��� bcst ������
  (= bcmd.c 2)
  (= bcmd.node OUTSIDE)
  (= bcmd.w BCST)
  (= (aref bcmd.v 0 0) thr->id)        ; bcst���������ɥ쥹 (worker-id)
  (= (aref bcmd.v 0 1) TERM)
  (= (aref bcmd.v 1 0) task-no)        ; �֥��ɥ��㥹�ȼ��� (task-no)
  (= (aref bcmd.v 1 1) TERM)
  (csym::send-command (ptr bcmd) body task-no)

  ;; task-sender �� send-out-command ����ǸƤФ��

  ;; bcak �Ԥ��ե饰��Ω�Ƥơ�bcak �����ޤ��Ե�
  ;; �Ԥ��ե饰���ä��Ƥ����顢�ؿ���ȴ����ʥե饰�� recv-bcak �ؿ���Ǿä����
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= thr->w-bcak 1)
  (while thr->w-bcak
    (csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut)))
  (csym::pthread-mutex-unlock (ptr thr->mut)))
   

;;; Handling command-line options
(def (csym::usage argc argv) (csym::fn void int (ptr (ptr char)))
  (csym::fprintf stderr
                 "Usage: %s [-s hostname] [-p port-num] [-n n-threads] [-i initial-task-parms] [-a] [-P n-prefetches] [-v verbosity]~%"
                 (aref argv 0))
  (csym::exit 1))

(def (set-option argc argv) (csym::fn void int (ptr (ptr char)))
  (def i int) (def ch int)
  ;; Default values
  (= (aref option.sv-hostname 0) #\NULL)
  (= option.port 9865)
  (= option.num-thrs 1)
  (= option.node-name 0)
  (= option.initial-task 0)
  (= option.auto-exit 0)
  (= option.affinity 0)
  (= option.always-flush-accepted-treq 0)
  (= option.prefetch 0)
  (= option.verbose 0)
  ;; Parse and set options
  (while (!= -1 (= ch (csym::getopt argc argv "n:s:p:N:i:xafP:v:h")))
    (switch ch
      (case #\n)                        ; number of threads
      (= option.num-thrs (csym::atoi optarg))
      (break)

      (case #\s)                        ; server name
      (if (csym::strcmp "stdout" optarg)
          (begin
            (csym::strncpy option.sv-hostname optarg
                           HOSTNAME-MAXSIZE)
            (= (aref option.sv-hostname (- HOSTNAME-MAXSIZE 1)) 0))
        (= (aref option.sv-hostname 0) #\NULL))
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
      
      (case #\P)                        ; the number of speculative tasks from external nodes
      (= option.prefetch (csym::atoi optarg))
      (break)

      (case #\v)                        ; verbose level
      (= option.verbose (csym::atoi optarg))
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


;; �ʲ� tcell-gtk�Ȥ���Τϥǥ��Ѥ��ɲå����ɡ��Դ�����
#+tcell-gtk (def window (ptr GtkWidget))
#+tcell-gtk (def darea (ptr GtkWidget))
#+tcell-gtk (def gc (ptr GdkGC) 0)
#+tcell-gtk (def pixmap (ptr GdkPixmap) 0)

#+tcell-gtk
(def (csym::set-color r g b) (fn (ptr GdkGC) gushort gushort gushort)
  (decl color GdkColor) 
  (= (fref color red) r) 
  (= (fref color green) g)
  (= (fref color blue) b)
  (csym::gdk-color-alloc (csym::gdk-colormap-get-system) (ptr color))
  (csym::gdk-gc-set-foreground gc (ptr color))
  (return gc))

#+tcell-gtk                             ; �������������ѹ���
(def (csym::configure-event widget event data)
  (fn void (ptr GtkWidget) (ptr GdkEventConfigure) gpointer)
  ;; �Ť�pixmap������г���
  (if pixmap (csym::gdk-pixmap-unref pixmap))
  ;; ��������������pixmap�����
  (= pixmap
     (csym::gdk-pixmap-new (fref (mref widget) window)
                           (fref (fref (mref widget) allocation) width)
                           (fref (fref (mref widget) allocation) height) (- 1))))

#+tcell-gtk                             ; ������
(def (csym::expose-event widget event data)
  (fn void (ptr GtkWidget) (ptr GdkEventExpose) gpointer)
  (csym::gdk-draw-pixmap widget->window
                         (aref widget->style->fg-gc
                               (csym::GTK-WIDGET-STATE widget))
                         pixmap
                         (fref (fref (mref event) area) x)
                         (fref (fref (mref event) area) y)
                         (fref (fref (mref event) area) x)
                         (fref (fref (mref event) area) y)
                         (fref (fref (mref event) area) width)
                         (fref (fref (mref event) area) height)))

#+tcell-gtk                             ; ������֤��Ȥ����补���ץꥱ�������¦�����
(extern-decl (csym::repaint) (fn gint gpointer))
;; gint repaint(gpointer data){
;;     static x;
;;     GtkWidget *drawing_area = GTK_WIDGET (data);
;;     gdk_draw_rectangle(pixmap,
;;                        set_color(0xffff, 0x0, 0x0),
;;                        TRUE,
;;                        0, 0,
;;                        drawing_area->allocation.width,
;;                        drawing_area->allocation.height);
;;     x++;
;;     gdk_draw_rectangle(pixmap,
;;                        set_color(0xffff, 0xffff, 0x0),
;;                        TRUE,
;;                        x, x,
;;                        30, 30);
;;     /* ���褹�� (expose_event��ƤӽФ�) */
;;     gtk_widget_draw(drawing_area, NULL);
;;     return TRUE;
;; }

;;; (struct task)�������ꥹ�Ȥν����
(def (csym::initialize-task-list tlist len p-top p-free)
    (fn void (ptr (struct task)) int (ptr (ptr (struct task))) (ptr (ptr (struct task))))
  (def i int)
  (= (mref p-top) 0)
  (= (mref p-free) tlist)
  (for ((= i 0) (< i (- len 1)) (inc i))
       (= (fref (aref tlist i) prev) (ptr (aref tlist (+ i 1))))
       (= (fref (aref tlist (+ i 1)) next) (ptr (aref tlist i))))
  (= (fref (aref tlist 0) next) 0)
  (= (fref (aref tlist (- len 1)) prev) 0)
  (for ((= i 0) (< i len) (inc i))
    (= (fref (aref tlist i) rslt-to) TERM)
    (= (aref (fref (aref tlist i) rslt-head) 0) TERM))
  (return))

;;; (struct task-home)�ꥹ�Ȥν����
(def (csym::initialize-task-home-list hlist len p-top p-free)
    (fn void (ptr (struct task-home)) int (ptr (ptr (struct task-home))) (ptr (ptr (struct task-home))))
  (def i int)
  (= (mref p-top) 0)
  (= (mref p-free) hlist)
  ;; �ե꡼�ꥹ�Ȥ���
  (for ((= i 0) (< i (- len 1)) (inc i))
       (= (fref (aref hlist i) next) (ptr (aref hlist (+ i 1))))
      (= (fref (aref hlist (- len 1)) next) 0))
  (return))

;; main
;; �ǡ���������åɤ���������ư���Ƥ���
;; ������å������μ����롼�פ�����
(def (main argc argv) (fn int int (ptr (ptr char)))
  (defs int i j)
  (def pcmd (ptr (struct cmd)))         ; ������������������ޥ��

  ;; show compile-time option
  (fprintf stderr (%string "compile-time options: "
                           "VERBOSE=" VERBOSE " "
                           "NF-TYPE=" NF-TYPE " "
                           "USE-AFFINITY=" USE-AFFINITY
                           "~%"))
  
  ;; get command-line options
  #+tcell-gtk (csym::gtk-init (ptr argc) (ptr argv))
  (csym::set-option argc argv)

  #+tcell-gtk (begin (= window (csym::gtk-window-new GTK-WINDOW-TOPLEVEL))
                     (gtk-widget-show window)
                     (= darea (csym::gtk-drawing-area-new))
                     (csym::gtk-drawing-area-size (csym::GTK-DRAWING-AREA darea) 300 200)
                     (csym::gtk-container-add (csym::GTK-CONTAINER window) darea)
                     (= gc (csym::gdk-gc-new window->window))
                     (csym::gtk-signal-connect (csym::GTK-OBJECT darea) "configure_event"
                                               (csym::GTK-SIGNAL-FUNC csym::configure-event) 0)
                     (csym::gtk-signal-connect (csym::GTK-OBJECT darea) "expose_event"
                                               (csym::GTK-SIGNAL-FUNC csym::expose-event) 0)
                     (csym::gtk-timeout-add 33 repaint (cast gpointer darea))
                     (csym::gtk-widget-show-all window)
                     (csym::systhr-create 0 gtk-main 0)
                     )

  ;; �����Ф���³
  (= sv-socket (if-exp (== #\NULL (aref option.sv-hostname 0))
                       -1
                       (csym::connect-to option.sv-hostname option.port)))

  ;; mutex°���ν������æ����ν����ǡ���Ť˥�å����������Τ�recursive
  (def m-attr pthread-mutexattr-t)
  (csym::pthread-mutexattr-init (ptr m-attr))
  (csym::pthread-mutexattr-settype (ptr m-attr) PTHREAD-MUTEX-RECURSIVE-NP)  
  
  ;; send-mut�ʳ���������å��ˤν����
  (csym::pthread-mutex-init (ptr send-mut) (ptr m-attr))

  ;; data-mut, data-cond��ɬ�׻��ǡ����Υ�å�������ѿ��ˤν����
  (csym::pthread-mutex-init (ptr data-mutex) (ptr m-attr))
  (csym::pthread-cond-init (ptr data-cond) 0)
  
  ;; thread-data �ν����, task �� ������list ��
  ;; �굡treq��on�ʤ顤�����ѤΥ���åɤ�num-thrs+1���ܤȤ��ƺ�äƽ��������
  (= num-thrs option.num-thrs)
  (for ((= i 0) (< i (+ num-thrs (if-exp option.prefetch 1 0))) (inc i))
    (let ((thr (ptr (struct thread-data)) (+ threads i))
          (tx (ptr (struct task)))
          (hx (ptr (struct task-home))))
      (= thr->req 0)
      (= thr->id i)
      (= thr->w-rack 0)
      (= thr->w-none 0)
      (= thr->w-bcak 0)
      (= thr->ndiv 0)
      (= thr->last-treq i)
      (= thr->last-choose CHS-RANDOM)
      (let ((r double) (q double))
        (= r (csym::sqrt (+ 0.5 i)))
        (= q (csym::sqrt (+ r i)))
        (-= r (cast int r))
        (= thr->random-seed1 r)
        (= thr->random-seed2 q))
      (csym::pthread-mutex-init (ptr thr->mut) (ptr m-attr))
      (csym::pthread-mutex-init (ptr thr->rack-mut) (ptr m-attr))
      (csym::pthread-cond-init (ptr thr->cond) 0)
      (csym::pthread-cond-init (ptr thr->cond-r) 0)

      ;; task���������ꥹ�ȡʥ���åɤ��¹Ԥ���٤��������ˤν����
      (= tx (cast (ptr (struct task))
              (csym::malloc (* (sizeof (struct task)) TASK-LIST-LENGTH))))
      (csym::initialize-task-list tx TASK-LIST-LENGTH
                                  (ptr thr->task-top) (ptr thr->task-free))

      ;; task-home�Υꥹ�ȡ�ʬ�䤷�ƤǤ�����������
      (= hx (cast (ptr (struct task-home))
              (csym::malloc (* (sizeof (struct task-home)) TASK-LIST-LENGTH))))
      (csym::initialize-task-home-list hx TASK-LIST-LENGTH
                                       (ptr thr->treq-top) (ptr thr->treq-free))
      (= thr->sub 0)))
  ;; �굡treq����åɤ򻲾Ȥ���ݥ��󥿤�id������
  (if option.prefetch
      (begin (= prefetch-thr (+ threads num-thrs))
             (= prefetch-thr-id num-thrs)))

  ;; �������å�����
  (for ((= i 0) (< i num-thrs) (inc i))
    (let ((thr (ptr (struct thread-data)) (+ threads i)))
      (systhr-create (ptr thr->pthr-id) worker thr)))

  ;; �굡treq����å�����
  (if option.prefetch
      (systhr-create 0 prefetcher prefetch-thr))

  ;; �ܥ���åɤ�OUTSIDE����Υ�å���������
  (if option.initial-task               ; option.initial-task ������ʸ������Ѵ�
      (begin
        (def p-src (ptr char))
        (def p-dst (ptr char))
        (def header (array char 30))
        (csym::strcpy header "task 0 0 0 ")
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
  (while 1
    (%ifdef* VERBOSE
             (csym::sprintf ext-cmd-status "Waiting for an external message."))
    (= pcmd (csym::read-command))
    (%ifdef* VERBOSE
             (csym::sprintf ext-cmd-status "Processing a %s command."
                            (aref cmd-strings pcmd->w)))
    (csym::proc-cmd pcmd 0))
  (csym::exit 0))
