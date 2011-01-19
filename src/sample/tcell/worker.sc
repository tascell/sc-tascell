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

;;; デバッグ情報出力用の変数
(%ifdef* VERBOSE
         (static ext-cmd-status (array char 128) "") ; 外部メッセージ受信スレッドの状態
         (static n-dreq-handler int 0)  ; 起動しているdreq-handlerの数
         (static n-sending-dreq int 0)  ; うち send-dreq-for-required-range中（含ロック獲得待ち）
         (static n-sending-data int 0)  ; うち data送信中（含ロック獲得待ち）
         (static n-waiting-data int 0)) ; sending-dataのうち data待ち中


(%defmacro xread (tp exp)
  `(mref (cast (ptr (volatile ,tp)) (ptr ,exp))))
;; expが満たされる間，ビジーウェイト
(%defmacro pthread-cond-busywait (exp pmut)
  `(begin
    (csym::pthread-mutex-unlock ,pmut)
    (while ,exp)
    (csym::pthread-mutex-lock ,pmut)
    ))


;;; Command-line options
(def option (struct runtime-option))


(def (systhr-create start-func arg)
    (fn int (ptr (fn (ptr void) (ptr void))) (ptr void))
  (def status int 0)
  (def tid pthread-t)
  (def attr pthread-attr-t)

  (csym::pthread-attr-init (ptr attr))
  (= status (csym::pthread-attr-setscope (ptr attr) PTHREAD-SCOPE-SYSTEM))
  (if (== status 0)
      (= status (pthread-create (ptr tid) (ptr attr) start-func arg))
    (= status (pthread-create (ptr tid) 0          start-func arg)))
  (return status))

(def (csym::mem-error str) (csym::fn void (ptr (const char)))
  (csym::fputs str stderr)
  (csym::fputc #\Newline stderr)
  (csym::exit 1)
  )

;;; 現在の絶対時刻をマイクロ秒単位の整数で得る
(def (csym::get-universal-real-time) (csym::fn int)
  (def now (struct timeval))
  (csym::gettimeofday (ptr now) 0)
  (return (+ (* 1000 1000 now.tv-sec) now.tv-usec)))

;;; エラーメッセージstrとコマンドをstderrに出力
(def (csym::proto-error str pcmd) (csym::fn void (ptr (const char)) (ptr (struct cmd)))
  (def i int)
  (def buf (array char BUFSIZE))
  (csym::serialize-cmd buf pcmd)
  (csym::fprintf stderr "(%d): %s> %s~%" (csym::get-universal-real-time) str buf))

;; 外部への送信ロック
(def send-mut pthread-mutex-t)

;; サーバへの送受信ソケット(<0:stdin/out)．mainで設定
(def sv-socket int)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 入出力補助

;; EOLまでを無視
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

;;; fgetsでOUTSIDEからのメッセージを受け取る -> cmd-buf に書き込む
;;; 1行で読み込んだ後，スペースをNULL文字に置きかえることで文字列を分割する．
;;; また，cmd.v[]が分割された各文字列を指すようにする
;;;   fgets は 0-terminated buffer を返す．
;;;   fgets returns 0 のときの対処必要?
;;; buf, cmd-buf は一個なので注意（今はmainの本スレッドしか触らない）
;; 外部からのメッセージ受信バッファ
(def buf (array char BUFSIZE))
(def cmd-buf (struct cmd))

(def (csym::read-command) (csym::fn (ptr (struct cmd)))
  (defs char p c)
  (def b (ptr char) buf)
  (def cmdc int)

  (csym::receive-line b BUFSIZE sv-socket)
  (= cmd-buf.node OUTSIDE)
  (DEBUG-PRINT 1 "(%d): RECEIVED> %s" (csym::get-universal-real-time) b)
  ;; p:一個前の文字，c:現在の文字
  (csym::deserialize-cmd (ptr cmd-buf) b)
  (return (ptr cmd-buf)))

;;; struct cmd -> output（stdoutへ）
;;; task, rsltでは bodyの内容もtask-noが指定する関数で送る
(def send-buf (array char BUFSIZE))
(def (csym::send-out-command pcmd body task-no)
    (csym::fn void (ptr (struct cmd)) (ptr void) int)
  (def ret int)
  (def w (enum command))
  (= w pcmd->w)
  ;; <--- sender-lock <---
  (csym::pthread-mutex-lock (ptr send-mut))
  ;; コマンド名
  (csym::serialize-cmd send-buf pcmd)
  (csym::send-string send-buf sv-socket)
  (csym::write-eol)
  ;; TASK, RSLT, DATAのbody送信関数（Tascellプログラマ定義）を呼び出す
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
    ;; data-mutはロック済
    (csym::data-send (aref pcmd->v 1 0) (aref pcmd->v 1 1))
    (csym::write-eol)))

  (csym::flush-send)
  (csym::pthread-mutex-unlock (ptr send-mut))
  ;; ---> sender-lock --->
  (if (and (== w RSLT) option.auto-exit)
      (csym::exit 0))
  )

;;; （受信）cmdをもらってメッセージの種類に適した関数を呼出す
;;; bodyはタスクオブジェクト（内部通信のとき）
;;; または0（外部通信のとき，後でinput streamから情報を受け取って作る）
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
   (case BCAK) (csym::recv-bcak pcmd) (break)
   (case STAT) (csym::print-status pcmd) (break)
   (case VERB) (csym::set-verbose-level pcmd) (break)
   (case EXIT) (csym::recv-exit pcmd) (break)
   (default) (csym::proto-error "wrong cmd" pcmd) (break))
  )

;;; ノード内/外ワーカにコマンドを送信
;;; bodyはtask, rsltの本体．それ以外のコマンドではNULLに
(def (csym::send-command pcmd body task-no) (csym::fn void (ptr (struct cmd)) (ptr void) int)
  (if (== pcmd->node INSIDE)
      (begin         ; 内部に送信（というかworker自身がコマンド処理）
       (DEBUG-STMTS 3 (if (or (>= option.verbose 4)
                              (and (!= TREQ pcmd->w) (!= NONE pcmd->w)))
                          (csym::proto-error "INSIDE" pcmd)))
       (csym::proc-cmd pcmd body))
    (begin
      (DEBUG-STMTS 1 (csym::proto-error "OUTSIDE" pcmd))
      (csym::send-out-command pcmd body task-no) ; 外部に送信
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
          (= rcmd.node hx->req-from)    ; 外部or内部
          (csym::copy-address (aref rcmd.v 0) hx->task-head)
          (csym::send-command (ptr rcmd) 0 0)
          (= (mref pcur-hx) hx->next)    ; treqスタックをpop
          (= hx->next thr->treq-free)   ; フリーリストに...
          (= thr->treq-free hx))        ; ...領域を返却
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

;;; ptv-src(timeval) に diffナノ秒 足した時刻をpts-dst(timespec)に入れる
(def (csym::timeval-plus-nsec-to-timespec pts-dst ptv-src diff)
    (fn void (ptr (struct timespec)) (ptr (struct timeval)) long)
  (def nsec long (+ diff (* 1000 ptv-src->tv-usec)))
  (= pts-dst->tv-nsec (if-exp (> nsec 999999999)
                              (- nsec 999999999)
                              nsec))
  (= pts-dst->tv-sec (+ ptv-src->tv-sec
                        (if-exp (> nsec 999999999) 1 0)))
  )

;;; TREQを送信して，TASKを受け取れたら，
;;; thr->task-topの指すtaskを初期化（TASK-INITIALIZEDに）する
;;; treq-head, req-to: どこにタスク要求を出すか（recv-exec-send の引数そのまま）
;;; 受付中のtreqのフラッシュや，rsltが来たときの対応も行う
;;; INITIALIZEDできた->1 そうでなければ->0 を返す
(def (csym::send-treq-to-initialize-task thr treq-head req-to)
    (fn int (ptr (struct thread-data)) (ptr (enum addr)) (enum node))
  (def rcmd (struct cmd))
  (def delay long 1000)     ; none が返ってきたとき待つ時間[nsec]
  (def tx (ptr (struct task)) thr->task-top)

  ;; Treqコマンド
  (= rcmd.c 2)
  (= rcmd.node req-to) ; 取り返し先の種別（INSIDE|OUTSIDE）
  (= rcmd.w TREQ)
  (= (aref rcmd.v 0 0) thr->id)
  (if (and (!= req-to ANY) thr->sub)
      (begin
       ;; 取り返しならば，待ち合わせているタスクのidも含める
       ;; （try-take-back-prefetched-task での同一性確認のため）
       (= (aref rcmd.v 0 1) thr->sub->id)
       (= (aref rcmd.v 0 2) TERM))
    (= (aref rcmd.v 0 1) TERM))
  (csym::copy-address (aref rcmd.v 1) treq-head) ; 要求先

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
     (csym::send-command (ptr rcmd) 0 0) ; treq送信
     (csym::pthread-mutex-lock (ptr thr->mut)))
    ;; recv-task が *tx を初期化するのを待つ
    (loop ; noneメッセージでTASK-NONE or taskメッセージでTASK-INITIALIZED になるまでループ
       ;; rslt が到着していたら，特別に先にさせる
       (if (and (!= tx->stat TASK-INITIALIZED)
                thr->sub                 ; rsltが....
                (== thr->sub->stat TASK-HOME-DONE)) ;...到着したか？
           ;; rslt が到着するような場合，ここでのtreqは取戻しであり，
           ;; その取戻しは失敗して，noneが返されるはずである．
           ;; すでに none が返されているかで，待ち数のカウンタを
           ;; 増やすかどうかが異なる
           (begin
            (if (!= tx->stat TASK-NONE)
                (inc thr->w-none))
            (return 0)))
       (if (!= tx->stat TASK-ALLOCATED) (break))
       ;; tx->statまたは thr->sub->statの変化を待つ
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
         ;; 取り返し失敗ならしばらく待つ
         (if (%ifdef* BUSYWAIT 1 %else thr->sub)
             (let ((t-until (struct timespec))
                   (now (struct timeval)))
               (csym::gettimeofday (ptr now) 0)
               (csym::timeval-plus-nsec-to-timespec (ptr t-until) (ptr now) delay)
               (csym::pthread-cond-timedwait (ptr thr->cond-r)
                                             (ptr thr->mut)
                                             (ptr t-until))
               (+= delay delay)         ; 次回の待ち時間を増やす
               (if (> delay DELAY-MAX) (= delay DELAY-MAX))
               ))
         ;; rsltが到着していたら自分のtreqリトライせず，そちらの処理を優先
         (if (and thr->sub
                  (== thr->sub->stat TASK-HOME-DONE))
             (return 0))))
    )
  (return 1))

;;; worker or wait-rslt から
;;; （後者は外に投げたタスクの結果待ち中に別の仕事をやろうとする時）
;;; タスク要求 -> 受け取り -> 計算 -> 結果送信
;;; thr->mut はロック済
;;; treq-head, req-to: どこにタスク要求を出すか（any or 取り返し先）
(def (recv-exec-send thr treq-head req-to)
    (fn void (ptr (struct thread-data)) (ptr (enum addr)) (enum node))
  (def tx (ptr (struct task)))
  (def old-ndiv int)
  (def rcmd (struct cmd))               ; for RSLT command

  ;; 前に送ったtreq（取り戻し）への none が届くまで待つ
  ;; （これから送るtreqに対するものと混同しないため）
  (while (> thr->w-none 0)
    (csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut))
    ;; rsltがきていたら自分の処理を再開できるのでreturn
    (if (and thr->sub
             (== thr->sub->stat TASK-HOME-DONE))
        (return)))

  ;; allocate
  (= tx (csym::allocate-task thr))
  ;; treq送信->仕事待ち
  (if (csym::send-treq-to-initialize-task thr treq-head req-to)
      (begin
       ;; タスク実行開始
       ;; ここで，tx(=thr->task-top)->statはTASK-INITIALIZED
       (= tx->stat TASK-STARTED)
       (= old-ndiv thr->ndiv)
       (= thr->ndiv tx->ndiv)
       (csym::pthread-mutex-unlock (ptr thr->mut))
       (DEBUG-PRINT 1 "(%d): (Thread %d) start %d<%p>.~%"
                    (csym::get-universal-real-time) thr->id tx->task-no tx->body)
       ((aref task-doers tx->task-no) thr tx->body) ; タスク実行
       (DEBUG-PRINT 1 "(%d): (Thread %d) end %d<%p>.~%"
                    (csym::get-universal-real-time) thr->id tx->task-no tx->body)
       ;; taskの処理完了後は，そのtask-homeにsend-rsltする
       (= rcmd.w RSLT)
       (= rcmd.c 1)
       (= rcmd.node tx->rslt-to)        ; 外部or内部
       (csym::copy-address (aref rcmd.v 0) tx->rslt-head)
       (csym::send-command (ptr rcmd) tx->body tx->task-no)
       (inc thr->w-rack)
       (csym::pthread-mutex-unlock (ptr thr->rack-mut))
       (csym::pthread-mutex-lock (ptr thr->mut))
       (= thr->ndiv old-ndiv)))

  ;; タスクstackをpopしてフリーリストに返す
  (= tx->stat TASK-DONE)
  ;; Flushes all the once accepted treqs by sending nones.
  ;; Notice that stealing-back to the task of which just sent the result should be flushed, too.
  (csym::flush-treq-with-none thr tx->rslt-head tx->rslt-to)
  (csym::deallocate-task thr)
  )


;;; ワーカスレッドをコアに貼り付ける
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

;;; ワーカのループ
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


;;; 投機的にtreqを送ってtaskを受信するスレッドのループ
;;; 今のところ投機的にとれる仕事の数は1で固定
(def (prefetcher thr0) (fn (ptr void) (ptr void))
  (def treq-head (array (enum addr) 2))
  (def req-to (enum node) OUTSIDE)
  (def thr (ptr (struct thread-data)) (cast (ptr (struct thread-data)) thr0))
  (= (aref treq-head 0) ANY) (= (aref treq-head 1) TERM)
  (csym::pthread-mutex-lock (ptr thr->mut))
  (loop
    ;;リストが空になるまで待機
    (while thr->task-top
       (csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut)))
    (csym::allocate-task thr)
    ;; treqを送って仕事獲得：とれるまでやる
    (while (not (csym::send-treq-to-initialize-task thr treq-head req-to)))
    ;;リストが空でなくなったことを通知
    (csym::pthread-cond-broadcast (ptr thr->cond))
    )
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (return 0)
  )

;;; prefetchしたtaskをpopして，thrのtask-topをINIITIALIZEする
(def (csym::pop-prefetched-task thr) (fn int (ptr (struct thread-data)))
  (defs (ptr (struct task)) tx-dst tx-src)
  (csym::pthread-mutex-lock (ptr prefetch-thr->mut))
  ;;リストが空でなくなるまで待機
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
  ;;仕事ができるようになったことを通知
  (csym::pthread-cond-broadcast (ptr thr->cond))
  ;;リストが空になったことを通知
  (csym::pthread-cond-broadcast (ptr prefetch-thr->cond))
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::pthread-mutex-unlock (ptr prefetch-thr->mut))
  (return 1))

;;; 外からprefetchスレッドへのtreq（取り返し）に対し，まだその仕事を始めてなければ
;;; そのまま返却する．返却した->1, しなかった->0 を返す．
(def (csym::try-take-back-prefetched-task treq-head) (fn int (ptr (enum addr)))
  (def tx (ptr (struct task)))
  (def retval int)
  (csym::pthread-mutex-lock (ptr prefetch-thr->mut))
  (if (and (= tx prefetch-thr->task-top) ; prefetchスレッドが持っているタスクがtreq元が送ったものと同一か判定
           (== TASK-INITIALIZED tx->stat)
           (csym::address-equal tx->rslt-head treq-head))
      (let ((rcmd (struct cmd)))
        ;; 仕事のキャンセルを伝える
        (= rcmd.w BACK) (= rcmd.c 1) (= rcmd.node OUTSIDE)
        (csym::copy-address (aref rcmd.v 0) treq-head)
        (csym::send-command (ptr rcmd) 0 0)
        ;; prefetchした仕事を破棄
        (csym::deallocate-task prefetch-thr) ; !!! bodyのごみをどうする？
        (csym::pthread-cond-broadcast (ptr prefetch-thr->cond)) ; 空になったことを通知
        (= retval 1))
    (= retval 0))
  (csym::pthread-mutex-unlock (ptr prefetch-thr->mut))
  (return retval))


;;;; recv-* は，受信スレッド（内部および外部の2つ）のみ実行

;;; recv-task
;;; task <タスクが分割された回数> <送信元> <送信先（自分）> <タスク番号>
(def (csym::recv-task pcmd body) (csym::fn void (ptr (struct cmd)) (ptr void))
  (def tx (ptr (struct task)))
  (def thr (ptr (struct thread-data)))
  (def id (enum addr))
  (def task-no int)
  (def len size-t)
  ;; パラメータ数チェック
  (if (< pcmd->c 4)
      (csym::proto-error "wrong-task" pcmd))
  ;; 外部からのメッセージの場合，コマンドに続くtask本体を受け取る
  ;; （内部からの場合は引数で与えられている）
  (= task-no (aref pcmd->v 3 0))
  (if (== pcmd->node OUTSIDE)           ; 外部からのtaskの場合はここでbodyを受け取る
      (begin
       (= body ((aref task-receivers task-no)))
       (csym::read-to-eol)))
  ;; <task-head>を見て，タスクを実行するスレッドを決める．
  (= id (aref pcmd->v 2 0))
  (if (not (< id (+ num-thrs (if-exp option.prefetch 1 0))))
      (csym::proto-error "wrong task-head" pcmd))
  (= thr (+ threads id))                ; thr: taskを実行するスレッド

  ;; スレッドに実行すべきタスクを追加する
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= tx thr->task-top)                  ; tx: thrがやるべき仕事リスト
  (= tx->rslt-to pcmd->node)            ; 結果の送り先種別 [INSIDE|OUTSIDE]
  (csym::copy-address tx->rslt-head (aref pcmd->v 1))
                                        ; [1]: 送り元＝結果の送信先
  (= tx->ndiv (aref pcmd->v 0 0))       ; [0]: 分割回数
  (= tx->task-no task-no)               ; タスク番号
  ;; タスクのパラメータ（task specificな構造体）の受け取り
  (= tx->body body)
  (= tx->stat TASK-INITIALIZED)
  (csym::pthread-mutex-unlock (ptr thr->mut))

  ;; 仕事待ちで眠っているワーカを起こす
  (csym::pthread-cond-broadcast (ptr thr->cond))
  )


;;; none <送信先>
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
  (csym::pthread-cond-broadcast (ptr thr->cond)) ; TASK-NONEになったことを通知
  (csym::pthread-mutex-unlock (ptr thr->mut))
  )


;;; task-homeのリストからidが指定したものに一致する要素を探す
(def (csym::search-task-home-by-id id hx) (fn (ptr (struct task-home)) int (ptr (struct task-home)))
  (while (and hx (!= hx->id id))
    (= hx hx->next))
  (return hx))

;;; back <送信先>:<task-id>
;;; 送信した仕事のキャンセル通知．task-homeにある仕事を自分自身でやるようにする．
;;; キャンセルされた仕事は subスタックのトップにあるものなので，そこから情報獲得．
(def (csym::recv-back pcmd) (csym::fn void (ptr (struct cmd)))
  (def thr (ptr (struct thread-data)))
  (def tx (ptr (struct task)))
  (def hx (ptr (struct task-home)))
  (def thr-id (enum addr))
  (def tsk-id int)
  (if (< pcmd->c 1) (csym::proto-error "Wrong back" pcmd))                  ; 引数の数チェック．0:送信先
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
  (= hx->req-from INSIDE)               ; taskの送り先情報を変更
  (= (aref hx->task-head 0) thr-id)     ;
  (= (aref hx->task-head 1) TERM)       ;
  (= tx->stat TASK-INITIALIZED)
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::pthread-cond-broadcast (ptr thr->cond)) ; TASK-INITIALIZEDになったことを通知
  )


(def (csym::recv-rslt pcmd body) (csym::fn void (ptr (struct cmd)) (ptr void))
  (def rcmd (struct cmd))               ; rackコマンド
  (def thr (ptr (struct thread-data)))
  (def hx (ptr (struct task-home)))
  (def tid (enum addr))
  (def sid int)
  ;; 引数の数チェック
  (if (< pcmd->c 1)
      (csym::proto-error "Wrong rslt" pcmd))
  ;; 結果受取人決定 "<thread-id>:<task-home-id>"
  (= tid (aref pcmd->v 0 0))
  (if (not (< tid num-thrs))
      (csym::proto-error "wrong rslt-head" pcmd))
  (= sid (aref pcmd->v 0 1))
  (if (== TERM sid)
      (csym::proto-error "Wrong rslt-head (no task-home-id)" pcmd))
  (= thr (+ threads tid))
  
  (csym::pthread-mutex-lock (ptr thr->mut))
  ;; hx = 返ってきたrsltを待っていたtask-home（.id==sid）を探す
  (if (not (= hx (csym::search-task-home-by-id sid thr->sub)))
      (csym::proto-error "Wrong rslt-head (specified task not exists)" pcmd))
  ;; 外部からのメッセージの場合，コマンドに続くrslt本体を受け取る
  ;; （内部からの場合は引数で与えられている）
  (cond
   ((== pcmd->node OUTSIDE)
    ((aref rslt-receivers hx->task-no) hx->body)
    (csym::read-to-eol))
   ((== pcmd->node INSIDE)
    (= hx->body body))
   (else
    (csym::proto-error "Wrong cmd.node" pcmd)))
  ;; rackを返す．もっと後のほうがよい？
  (= rcmd.c 1)
  (= rcmd.node pcmd->node)
  (= rcmd.w RACK)
  (csym::copy-address (aref rcmd.v 0) hx->task-head)
                                        ; 返答先．rsltではなく，もとのtaskコマンドのを覚えている
  ;; hx 中に記録された task-head に rack を後で送るなら，
  ;; ここではないが，まだ free されたくないので，つなぎなおすかも
  (= hx->stat TASK-HOME-DONE)
  (if (== hx thr->sub)
      (begin
       (csym::pthread-cond-broadcast (ptr thr->cond-r))
       (csym::pthread-cond-broadcast (ptr thr->cond)))
    )
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::send-command (ptr rcmd) 0 0))  ;rack送信

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
       (= thr->treq-free hx->next)      ; フリーリストから領域を確保
       (= hx->next thr->treq-top)       ; これよりnextはスタックのリンク
       (= hx->stat TASK-HOME-ALLOCATED)
       (if (== (aref dest-addr 0) ANY)
           (= (aref hx->waiting-head 0) TERM)
         (csym::copy-address hx->waiting-head from-addr))
       (csym::copy-address hx->task-head (aref pcmd->v 0)) ; v[0]: 返答先
       (if (!= pcmd->node OUTSIDE)
           (= hx->req-from INSIDE)
         (= hx->req-from OUTSIDE))
       (= thr->treq-top hx)
       (= thr->req hx)
       ))
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (return avail))

;; 親からのランダム用
(def random-seed1 double 0.2403703)
(def random-seed2 double 3.638732)
;; 0--max-1までの整数乱数を返す
(def (csym::my-random max pseed1 pseed2) (fn int int (ptr double) (ptr double))
  (= (mref pseed1) (+ (* (mref pseed1) 3.0) (mref pseed2)))
  (-= (mref pseed1) (cast int (mref pseed1)))
  (return (* max (mref pseed1))))

;;; treq any処理中に呼ばれ，要求元がどこかに応じて
;;; 適当な戦略で，最初にどのワーカに問い合わせるかを決める
(def (csym::choose-treq from-addr) (fn int (enum addr))
  (cond
   ((<= 0 from-addr)                    ; 内部から
    (let ((thr (ptr (struct thread-data)) (+ threads from-addr)))
      ;; 戦略を前回と変える
      (= thr->last-choose (% (+ 1 thr->last-choose) NKIND-CHOOSE))
      (cond
       ((== CHS-RANDOM thr->last-choose) ; ランダム
        (return (csym::my-random num-thrs
                                 (ptr thr->random-seed1)
                                 (ptr thr->random-seed2))))
       ((== CHS-ORDER thr->last-choose) ; 順番に
        (= thr->last-treq (% (+ 1 thr->last-treq) num-thrs))
        (return thr->last-treq))
       (else
        (return 0)))))
   ((== PARENT from-addr)               ; 外部から
    (return (csym::my-random num-thrs (ptr random-seed1) (ptr random-seed2))))
   (else
    (return 0))))

;;; treqメッセージ（仕事要求）の処理
;;; 要求先のidのワーカ（ANYの場合は全てのワーカ）に仕事分割が可能か確認
;;; 可能なら要求キューに追加するので(try-treq内)そのうちtaskメッセージが送られてきて
;;; 要求元のタスクリストのアイテムが TASK-ALLOCATED => TASK-INITIALIZEDになる．
;;; 不可能ならNONEを返す．
;;; ただし，0番ワーカがANY要求をしている場合はtreqメッセージを外部に転送する．
(def (csym::recv-treq pcmd) (csym::fn void (ptr (struct cmd)))
  (def rcmd (struct cmd))
  (def dst0 (enum addr))
  (if (< pcmd->c 2)                     ; 引数の数チェック 0:from, 1:to
      (csym::proto-error "Wrong treq" pcmd))
  ;; 仕事を要求するスレッドを決めて，要求を出す
  (= dst0 (aref pcmd->v 1 0))
  (cond
   ;; ANY
   ((== dst0 ANY)
    (let ((myid int) (start-id int) (d int) (id int))
      (= myid (aref pcmd->v 0 0))       ; 要求元
      (= start-id (csym::choose-treq myid)) ; 要求先開始id
      (for ((= d 0) (< d num-thrs) (inc d))
        (= id (% (+ d start-id) num-thrs)) ; 要求先id
        (if (and (!= pcmd->node OUTSIDE)
                 (== id myid))
            (continue))                 ; 自分自身には要求を出さない
        (if (csym::try-treq pcmd id)
            (begin
             (DEBUG-PRINT 2 "(%d): treq(any) %d->%d... accepted.~%"
                          (csym::get-universal-real-time) myid id)
             (break)))
        (DEBUG-PRINT 4 "(%d): treq(any) %d->%d... refused.~%"
                     (csym::get-universal-real-time) myid id)
        )
      (if (!= d num-thrs)               ; treqできた
          (return))))
   ;; prefetchスレッドへの要求
   ((and option.prefetch (== dst0 prefetch-thr-id))
    (if (== pcmd->node OUTSIDE)         ; 外からの取り返し
        (if (csym::try-take-back-prefetched-task (aref pcmd->v 0)) ; まだタスクが開始されてなければそのまま取り返す
            (return)
          (begin                        ; 開始されていれば0番スレッドから取り返す(leapfrogging)
           (= (aref pcmd->v 1 0) 0)
           (csym::recv-treq pcmd)
           (return)))
      ;; (!= pcmd->node OUTSIDE): 内部ワーカからprefetchedタスクの要求
      (begin
       (DEBUG-PRINT 2 "Thread %d trying to pop prefetched task.~%" (aref pcmd->v 0 0))
       (if (csym::pop-prefetched-task (+ threads (aref pcmd->v 0 0)))
           (begin
            (DEBUG-PRINT 2 "Thread %d popped prefetched task.~%" (aref pcmd->v 0 0))
            (return))))))
   ;; 取り返し(leapfrogging)
   (else
    (if (not (and (<= 0 dst0) (< dst0 num-thrs)))
        (csym::proto-error "Wrong task-head" pcmd))
    (if (csym::try-treq pcmd dst0)      ; treqできた
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
  ;; 内部のワーカが，渡せる仕事がなかった場合のみここに来る
  (if (and (== dst0 ANY)
           (== pcmd->node INSIDE)
           (== (aref pcmd->v 0 0) 0))    ; v[0]:from
      ;; 0番workerからのtreqの場合は外部に問い合わせる
      (if option.prefetch
          (begin                        ; 投機treqスレッドに
           (= pcmd->node INSIDE)
           (= (aref pcmd->v 1 0) prefetch-thr-id) (= (aref pcmd->v 1 1) TERM)
           (csym::send-command pcmd 0 0)
           (return))
        (begin                          ; 外部に
         (= pcmd->node OUTSIDE)
         (csym::send-command pcmd 0 0)
         (return))))
  ;; 外へのtreq転送もしなかった場合のみここに来る
  ;; noneを返す
  (= rcmd.c 1)
  (= rcmd.node pcmd->node)              ; [INSIDE|OUTSIDE]
  (= rcmd.w NONE)
  (csym::copy-address (aref rcmd.v 0) (aref pcmd->v 0))
  (csym::send-command (ptr rcmd) 0 0))


;; rack <rack送信先header(ここではthread-id)>
(def (csym::recv-rack pcmd) (csym::fn void (ptr (struct cmd)))
  (def tx (ptr (struct task)))
  (def thr (ptr (struct thread-data)))
  (def id (enum addr))
  (def len size-t)
  (if (< pcmd->c 1)                     ; 引数の数チェック 0:返送先（スレッドid）
      (csym::proto-error "Wrong rack" pcmd))
  (= id (aref pcmd->v 0 0))
  ;; prefetchスレッドへのrackは0番に転送
  ;; （prefetchしたタスクは必ず0番が取るようにしているため）
  (if (and option.prefetch (== prefetch-thr-id id))
      (= id 0))
  (if (not (< id num-thrs))
      (csym::proto-error "Wrong task-head" pcmd))
  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr thr->rack-mut))
  (dec thr->w-rack)
  (csym::pthread-mutex-unlock (ptr thr->rack-mut)))


;; 存在フラグの配列の先頭：初期化は-setup-dataにて
(def data-flags (ptr (enum DATA-FLAG)) 0)
;; ロック，条件変数
(def data-mutex pthread-mutex-t)
(def data-cond  pthread-cond-t)

;; Tascellへの提供機能：dataを初期化 (n: size)
(def (csym::-setup-data n) (csym::fn void int)
  (def i int)
  (def tmp (ptr (enum DATA-FLAG)))

  (if data-flags (return))
  (csym::pthread-mutex-lock (ptr data-mutex))
  ;; data-flagsの初期化
  (if (not data-flags)
      (begin
       (= tmp (cast (ptr (enum DATA-FLAG)) (csym::malloc (* n (sizeof (enum DATA-FLAG))))))
       (for ((= i 0) (< i n) (inc i))
            (= (aref tmp i) DATA-NONE))
       (= data-flags tmp)))
  ;; dataの領域確保（Tascellプログラマ定義関数を呼出す）
  (csym::data-allocate n)
  (csym::pthread-mutex-unlock (ptr data-mutex))
  (return))

;; 要求時取得データのうち，NONEなものについてpcmd，REQUESTINGなものについてpcmd-fwdを送信
;; NONEをREQUESTING に更新する
;; data-mutexのロックはこの中でする．
;; pcmdとpcmd-fwdは データ範囲（3つめのパラメータ）以外がセットされたDREQコマンド
;; （送らなくていい場合はNULLを与える）
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
      ;; iからjまで要求 (pcmd) を出す
      (if pcmd
          (begin
            (= (aref pcmd->v 2 0) i)  (= (aref pcmd->v 2 1) j)
            (= (aref pcmd->v 2 2) TERM)
            (csym::send-command pcmd 0 0)))
      (= i (- j 1)))                    ; 要求を出した範囲の次からチェック再開
     ((== (aref data-flags i) DATA-REQUESTING)
      (for ((= j (+ i 1)) (and (< j end) (== (aref data-flags j) DATA-REQUESTING)) (inc j))
        )
      ;; iからjまで要求 (pcmd-fwd) を出す
      (if pcmd-fwd
          (begin
            (= (aref pcmd-fwd->v 2 0) i)  (= (aref pcmd-fwd->v 2 1) j)
            (= (aref pcmd-fwd->v 2 2) TERM)
            (csym::send-command pcmd-fwd 0 0)))
      (= i (- j 1)))                    ; 要求を出した範囲の次からチェック再開
     ))
  (csym::pthread-mutex-unlock (ptr data-mutex))
  )

;; thread-id=tid, id=sidのtask-homeを持つtaskから見て，
;; 最初の外部ノードにある祖先のtask-homeのアドレスをheadにコピー
(def (csym::get-first-outside-ancestor-task-address head tid sid) (csym::fn int (ptr (enum addr)) int int)
  (def thr (ptr (struct thread-data)))
  (def hx (ptr (struct task-home)))
  (def ok int)
  (do-while 1
    (= thr (+ threads tid))
    (csym::pthread-mutex-lock (ptr thr->mut))
    ;; hx = データ要求先のtask-home（.id==sid）を探す
    (if (not (= hx (csym::search-task-home-by-id sid thr->sub)))
        (csym::fprintf stderr "Error in get-first-outside-ancestor-task-address (specified task not exists)~%"))
    (csym::pthread-mutex-unlock (ptr thr->mut))
    
    (cond
     ((not hx->owner)                   ; 親がいなければエラー
      (csym::fprintf stderr "error in get-first-outside-ancestor-task-address: no owner found.~%")
      (csym::print-status 0)
      (csym::exit 1))
     ((== hx->owner->rslt-to OUTSIDE)   ; 次の親が外部ノードなら，そこが次の要求先
      (csym::copy-address head hx->owner->rslt-head)
      (break)))
    ;; 次の親が内部ノードなら，さらに親タスクをたどる
    (= tid (aref hx->owner->rslt-head 0))
    (= sid (aref hx->owner->rslt-head 1)))
  (return ok))

;; Tascellへの提供機能：dataを親タスクに要求 (start, end: データ範囲)
(def (csym::-request-data thr start end) (csym::fn void (ptr (struct thread-data)) int int)
  (def cmd (struct cmd))
  (def tx (ptr (struct task)))
  (DEBUG-PRINT 2 "request-data: %d--%d start~%" start end)
  (csym::pthread-mutex-lock (ptr thr->mut)) ; ロックは念のため
  (= tx thr->task-top)                  ; 今実行中のタスク
  (csym::pthread-mutex-unlock (ptr thr->mut))
  ;; コマンドの種類，引数の数
  (= cmd.w DREQ)
  (= cmd.c 3)
  ;; 要求元（thread番号は関係ないのでダミー番号でよい）
  (= (aref cmd.v 0 0) 0)  (= (aref cmd.v 0 1) TERM)
  ;; 要求先：直接の親が外部でなければ，さらに祖先を辿る
  (if (== OUTSIDE tx->rslt-to)
      (csym::copy-address (aref cmd.v 1) tx->rslt-head)
    (csym::get-first-outside-ancestor-task-address
     (aref cmd.v 1) (aref tx->rslt-head 0) (aref tx->rslt-head 1)))
  (= cmd.node OUTSIDE)

  (DEBUG-STMTS 2
               (= (aref cmd.v 2 0) TERM)
               (csym::proto-error "dreq template" (ptr cmd)))
  
  ;; 必要な範囲についてdreqを送信
  (csym::send-dreq-for-required-range start end (ptr cmd) 0)
  (DEBUG-PRINT 2 "request-data: %d--%d end~%" start end)
  (return)
  )

;; Tascellへの提供機能：dataが揃うまで待機 (start, end: データ範囲)
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

;; 指定された範囲のdata-flagsをDATA-EXISTにする（仕事開始ノード用）
(def (csym::-set-exist-flag start end) (csym::fn void int int)
  (def i int)
  (csym::pthread-mutex-lock (ptr data-mutex))
  (for ((= i start) (< i end) (inc i))
    (= (aref data-flags i) DATA-EXIST))
  (csym::pthread-mutex-unlock (ptr data-mutex))
  )



;; recv-dreqで作られるスレッドが実行する関数
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
  
  ;; NONEな範囲について，さらに親に要求を出す
  ;; REQUESTINGな範囲について，要求をforwardする
  (%ifdef* VERBOSE (inc n-sending-dreq))
  (csym::send-dreq-for-required-range start end pcmd pcmd-fwd)
  (%ifdef* VERBOSE (dec n-sending-dreq))
  
  ;; INSIDEからのdreqならデータを送る必要はないのでここで終わり
  (if (== parg->data-to INSIDE)
      (begin
       (%ifdef* VERBOSE (dec n-dreq-handler))
       (return)))

  ;; DATAコマンドの雛形を設定
  (= data-cmd.w DATA)
  (= data-cmd.c 2)
  (= data-cmd.node parg->data-to)       ; data送信先(OUTSIDE)
  (csym::copy-address (aref data-cmd.v 0) parg->head) ; data送信先
  
  ;; dataが送られてくるのをcond-waitで待って，順次要求元に送る
  (%ifdef* VERBOSE (inc n-sending-data))
  (csym::pthread-mutex-lock (ptr data-mutex))
  (for ((= i start) (< i end) (inc i))
    (while (!= (aref data-flags i) DATA-EXIST)
      (%ifdef* VERBOSE (inc n-waiting-data))
      (csym::pthread-cond-wait (ptr data-cond) (ptr data-mutex))
      (%ifdef* VERBOSE (dec n-waiting-data)) )
    (for ((= j (+ i 1)) (and (< j end) (== (aref data-flags j) DATA-EXIST)) (inc j))
      )
    ;; iからjまで送る（本体は，send-out-commandで送られる）
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

;; dreq <data要求元header> <data要求先(<thread-id>:<task-home-id>)> <data要求範囲(<data-start>:<data-end>)>
(def (csym::recv-dreq pcmd) (csym::fn void (ptr (struct cmd)))
  (def tx (ptr (struct task)))
  (def tid (enum addr))
  (def sid int)
  (def parg (ptr (struct dhandler-arg)))
  (def len size-t)
  ;; 引数の数チェック
  (if (< pcmd->c 3) (csym::proto-error "Wrong dreq" pcmd))
  
  ;; ;; dreq-handlerに渡す引数の設定
  ;; ;; ここで，dreq-cmdについては最後の引数（要求範囲）以外は設定しておく．
  (= parg (cast (ptr (struct dhandler-arg)) (csym::malloc (sizeof (struct dhandler-arg)))))
  (= parg->data-to pcmd->node)          ; data送信先（INSIDE|OUTSIDE)
  (csym::copy-address parg->head (aref pcmd->v 0)) ; data送信先（INSIDE|OUTSIDE)
  ;; parg->dreq-cmd: 自分のとこにもない(DATA-NONE)場合に出すDREQコマンドの雛形
  (= parg->dreq-cmd.w DREQ)
  (= parg->dreq-cmd.c 3)
  (= (aref parg->dreq-cmd.v 0 0) 0)     ; 要求元：ダミーの値
  (= (aref parg->dreq-cmd.v 0 1) TERM)  ; 要求元：ダミーの値
  (begin                                ; さらなるdreq要求をどこに出すか決定
   (= tid (aref pcmd->v 1 0))
   (if (not (< tid num-thrs)) (csym::proto-error "wrong dreq-head" pcmd))
   (= sid (aref pcmd->v 1 1))
   (if (== TERM sid) (csym::proto-error "Wrong dreq-head (no task-home-id)" pcmd))
   ;; 最初の外部ノードのtask-homeのアドレスを獲得
   (csym::get-first-outside-ancestor-task-address (aref parg->dreq-cmd.v 1) tid sid)
   )
  (= parg->dreq-cmd.node OUTSIDE)
  ;; parg->dreq-cmd: 自分のとこにもない(DATA-REQUESTING)場合に出すDREQコマンドの雛形
  (= parg->dreq-cmd-fwd.w DREQ)
  (= parg->dreq-cmd-fwd.c 3)
  (= (aref parg->dreq-cmd-fwd.v 0 0) FORWARD) ; 要求元：受け取ったDREQの要求元に偽装
  (csym::copy-address (ptr (aref parg->dreq-cmd-fwd.v 0 1))
                      (aref pcmd->v 0))
  (csym::copy-address (aref parg->dreq-cmd-fwd.v 1) ; 要求先：上で求めたものと同じ
                      (aref parg->dreq-cmd.v 1))
  (= parg->dreq-cmd-fwd.node OUTSIDE)
  ;; 要求範囲
  (= parg->start (aref pcmd->v 2 0))
  (= parg->end   (aref pcmd->v 2 1))
  
  ;; 別スレッドでdreq-handlerを呼出し
  (begin
    (def tid pthread-t)
    (csym::pthread-create (ptr tid) 0 csym::dreq-handler parg))
  (return))


;; data <data送信先header> <data範囲(<data-start>:<data-end>)>
(def (csym::recv-data pcmd) (csym::fn void (ptr (struct cmd)))
  (def i int)
  (def start int (aref pcmd->v 1 0))
  (def end int (aref pcmd->v 1 1))
  ;; 引数の数チェック
  (if (< pcmd->c 2) (csym::proto-error "Wrong data" pcmd))
  ;; （起こらないはずだが）内部からのdataであれば無視
  (if (== pcmd->node INSIDE) (return))
  ;; データ受信関数（Tascellプログラマ定義）を呼び出す
  (csym::pthread-mutex-lock (ptr data-mutex))
  (csym::data-receive start end)
  (csym::read-to-eol)
  (for ((= i start) (< i end) (inc i))
    (= (aref data-flags i) DATA-EXIST))
  (csym::pthread-cond-broadcast (ptr data-cond))
  (csym::pthread-mutex-unlock (ptr data-mutex))
  (return))


;;; Tascellプログラマに提供する関数

;; 親タスクに指定された範囲のデータのdreqを発行する
(decl (csym::request-data) (csym::fn void (ptr (struct thread-data)) int int))
;; 指定された範囲のデータが揃うまで待つ
(decl (csym::wait-data) (csym::fn void int int))


;;; recv-bcst
;;; bcst  <送信元アドレス>  <ブロードキャスト種別>
(def (csym::recv-bcst pcmd) (csym::fn void (ptr (struct cmd)))
  (def rcmd (struct cmd))
  (def task-no int)
  (def body (ptr void))
  ;; パラメータ数チェック
  (if (< pcmd->c 2)
      (csym::proto-error "wrong-task" pcmd))
  ;; データの種別を読み取り
  (= task-no (aref pcmd->v 1 0))
  ;; データ受信部本体を呼ぶ
  ;;   receiver は内部でタスクオブジェクトをヒープに作って返してくる
  (= body ((aref task-receivers task-no)))
  (csym::read-to-eol)
  ;; task本体を実行する．第1引数（ワーカスレッド）にはNULLを渡しておく
  ((aref task-doers task-no) 0 body)
  (csym::free body)
  ;; bcak で送信元に返答
  (= rcmd.c 1)
  (= rcmd.node pcmd->node)
  (= rcmd.w BCAK)
  (csym::copy-address (aref rcmd.v 0) (aref pcmd->v 0))
  (csym::send-command (ptr rcmd) 0 task-no))


;;; recv-bcak
;;; bcak  <送信先アドレス>
(def (csym::recv-bcak pcmd) (csym::fn void (ptr (struct cmd)))
  (def thr (ptr (struct thread-data)))
  (def id (enum addr))
  ;; パラメータ数チェック
  (if (< pcmd->c 1)
    (csym::proto-error "wrong-task" pcmd))
  ;; bcak を待っているワーカをメッセージから特定
  (= id (aref pcmd->v 0 0))
  (= thr (+ threads id))
  ;; ワーカを起こす
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= thr->w-bcak 0)
  (csym::pthread-cond-broadcast (ptr thr->cond))
  (csym::pthread-mutex-unlock (ptr thr->mut)))


;;; taskの情報を出力
(def task-stat-strings (array (ptr char)) ; enum task-statに対応
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

;;; task-homeの情報を出力
(def task-home-stat-strings (array (ptr char)) ; enum task-home-statに対応
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

;;; threadの情報を出力
(def choose-strings (array (ptr char)) (array "CHS-RANDOM" "CHS-ORDER")) ; enum chooseに対応
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

;;; statコマンド -> 状態を出力
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
    ;; よく考えたらstat処理中に決まっているので出力する意味がない
    ;; でもgdbのデバッグには有用なので変数自体は残す
    #+comment (csym::fprintf stderr "external command handler: %s~%" ext-cmd-status))

  (if option.prefetch 
      (begin (csym::print-task-list prefetch-thr->task-top "prefetched tasks")
             (csym::fputc #\Newline stderr)))
  (for ((= i 0) (< i num-thrs) (inc i))
    (csym::print-thread-status (ptr (aref threads i))))
  (return)
  )


;;; verbコマンド -> verbose levelを変更
(def (csym::set-verbose-level pcmd) (csym::fn void (ptr (struct cmd)))
  (if (< pcmd->c 1)                     ; 引数の数チェック 0:verbose-level
      (csym::proto-error "Wrong verb" pcmd))
  (= option.verbose (aref pcmd->v 0 0))
  (return))



;;; exitコマンド -> 終了
(def (csym::recv-exit pcmd) (csym::fn void (ptr (struct cmd)))
  (csym::fprintf stderr "Received \"exit\"... terminate.~%")
  (csym::exit 0))



;; ワーカスレッドが仕事分割開始時に呼ぶ
(def (handle-req -bk -thr)
    (fn void (ptr (NESTFN int void)) (ptr (struct thread-data)))
  (csym::pthread-mutex-lock (ptr -thr->mut))
  (if -thr->req
      (begin
       (-bk)
       (= -thr->req -thr->treq-top)))
  (csym::pthread-mutex-unlock (ptr -thr->mut)))

;; ワーカスレッドがput時に呼ぶ
;; thr->mut ロック済み
(def (csym::make-and-send-task thr task-no body) ; task-noをtcell追加
    (csym::fn void (ptr (struct thread-data)) int (ptr void))
  (def tcmd (struct cmd))
  (def hx (ptr (struct task-home)) thr->treq-top)
  ;; (csym::fprintf stderr "make-and-send-task(%d)~%" thr->id)
  (= thr->treq-top hx->next)            ; task要求スタックをpop
  (= hx->next thr->sub)                 ; これよりサブタスクstackのリンク
  (= thr->sub hx)                       ; サブタスクstackにpush
  (= hx->task-no task-no)
  (= hx->body body)
  (= hx->id (if-exp hx->next            ; サブタスクID = 底から何番目か
                    (+  hx->next->id 1)
                    0))
  (= hx->owner thr->task-top)           ; 親タスク
  (= hx->stat TASK-HOME-INITIALIZED)
  (= tcmd.c 4)
  (= tcmd.node hx->req-from)
  (= tcmd.w TASK)

  (= (aref tcmd.v 0 0) (++ thr->ndiv))  ; 分割回数
  (= (aref tcmd.v 0 1) TERM)
  (= (aref tcmd.v 1 0) thr->id)         ; 結果返答先 "<worker-id>:<subtask-id>"
  (= (aref tcmd.v 1 1) hx->id)
  (= (aref tcmd.v 1 2) TERM)
  (csym::copy-address (aref tcmd.v 2) hx->task-head) ; タスク送信先（try-treqで記憶済）
  (= (aref tcmd.v 3 0) task-no)         ; タスク番号
  (= (aref tcmd.v 3 1) TERM)
  (csym::send-command (ptr tcmd) body task-no))

;;; ワーカのdo-two, do-manyのところから呼ばれる．
;;; サブタスクの結果の受け取り完了を待ち，サブタスクリストから外す．
;;; （受け取り作業は rsltメッセージを受け取るルーチンにて実行される）
;;; しばらく返ってこなかったら仕事を取り返しにいく．
(def (wait-rslt thr) (fn (ptr void) (ptr (struct thread-data)))
  (def body (ptr void))
  (def sub (ptr (struct task-home)))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= sub thr->sub)                      ; スレッドのサブタスク置き場
  (while (!= sub->stat TASK-HOME-DONE)  ; iterate until the subtask is done
    (= thr->task-top->stat TASK-SUSPENDED)
    ;; 外部ノードに送った仕事ならちょっと待つ
    (if (== OUTSIDE sub->req-from)
        (let ((now (struct timeval))
              (t-until (struct timespec)))
          (csym::gettimeofday (ptr now) 0)
          (csym::timeval-plus-nsec-to-timespec (ptr t-until) (ptr now) 1000)
          (csym::pthread-cond-timedwait (ptr thr->cond-r) (ptr thr->mut)
                                        (ptr t-until))
          ))
    (if (== sub->stat TASK-HOME-DONE) (break))
    ;; 取り返しにいく (leapfrogging)
    (recv-exec-send thr sub->task-head sub->req-from))
  (= body sub->body)
  (= thr->sub sub->next)                ; サブタスクstackをpop
  (= sub->next thr->treq-free)          ; popした部分を...
  (= thr->treq-free sub)                ; ...フリーリストに返す
  (= thr->task-top->stat TASK-STARTED)
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (return body))


;;; ワーカがブロードキャストを実行するとき、put後に呼ばれる
(def (csym::broadcast-task thr task-no body)
   (csym::fn void (ptr (struct thread-data)) int (ptr void))
  (def bcmd (struct cmd))
  ;; まずは bcst を送る
  (= bcmd.c 2)
  (= bcmd.node OUTSIDE)
  (= bcmd.w BCST)
  (= (aref bcmd.v 0 0) thr->id)        ; bcst送信元アドレス (worker-id)
  (= (aref bcmd.v 0 1) TERM)
  (= (aref bcmd.v 1 0) task-no)        ; ブロードキャスト種別 (task-no)
  (= (aref bcmd.v 1 1) TERM)
  (csym::send-command (ptr bcmd) body task-no)

  ;; task-sender は send-out-command の中で呼ばれる

  ;; bcak 待ちフラグを立てて、bcak が来るまで待機
  ;; 待ちフラグが消えていたら、関数を抜ける（フラグは recv-bcak 関数内で消える）
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


;; 以下 tcell-gtkとあるのはデモ用の追加コード（不完全）
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

#+tcell-gtk                             ; 生成・サイズ変更時
(def (csym::configure-event widget event data)
  (fn void (ptr GtkWidget) (ptr GdkEventConfigure) gpointer)
  ;; 古いpixmapがあれば開放
  (if pixmap (csym::gdk-pixmap-unref pixmap))
  ;; 新しいサイズのpixmapを作成
  (= pixmap
     (csym::gdk-pixmap-new (fref (mref widget) window)
                           (fref (fref (mref widget) allocation) width)
                           (fref (fref (mref widget) allocation) height) (- 1))))

#+tcell-gtk                             ; 再描画
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

#+tcell-gtk                             ; 一定時間ごとの描画．アプリケーション側で定義
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
;;     /* 描画する (expose_eventを呼び出す) */
;;     gtk_widget_draw(drawing_area, NULL);
;;     return TRUE;
;; }

;;; (struct task)双方向リストの初期化
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

;;; (struct task-home)リストの初期化
(def (csym::initialize-task-home-list hlist len p-top p-free)
    (fn void (ptr (struct task-home)) int (ptr (ptr (struct task-home))) (ptr (ptr (struct task-home))))
  (def i int)
  (= (mref p-top) 0)
  (= (mref p-free) hlist)
  ;; フリーリストを構成
  (for ((= i 0) (< i (- len 1)) (inc i))
       (= (fref (aref hlist i) next) (ptr (aref hlist (+ i 1))))
      (= (fref (aref hlist (- len 1)) next) 0))
  (return))

;; main
;; データ・スレッドを初期化・起動してから
;; 外部メッセージの受信ループに入る
(def (main argc argv) (fn int int (ptr (ptr char)))
  (defs int i j)
  (def pcmd (ptr (struct cmd)))         ; 外部から受信したコマンド

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
                     (csym::systhr-create gtk-main 0)
                     )

  ;; サーバに接続
  (= sv-socket (if-exp (== #\NULL (aref option.sv-hostname 0))
                       -1
                       (csym::connect-to option.sv-hostname option.port)))

  ;; send-mut（外部送信ロック）の初期化
  (csym::pthread-mutex-init (ptr send-mut) 0)

  ;; data-mut, data-cond（必要時データのロック，条件変数）の初期化
  (csym::pthread-mutex-init (ptr data-mutex) 0)
  (csym::pthread-cond-init (ptr data-cond) 0)
  
  ;; thread-data の初期化, task の 双方向list も
  ;; 投機treqがonなら，それ用のスレッドをnum-thrs+1番目として作って初期化する
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
      (csym::pthread-mutex-init (ptr thr->mut) 0)
      (csym::pthread-mutex-init (ptr thr->rack-mut) 0)
      (csym::pthread-cond-init (ptr thr->cond) 0)
      (csym::pthread-cond-init (ptr thr->cond-r) 0)

      ;; taskの双方向リスト（スレッドが実行するべきタスク）の初期化
      (= tx (cast (ptr (struct task))
              (csym::malloc (* (sizeof (struct task)) TASK-LIST-LENGTH))))
      (csym::initialize-task-list tx TASK-LIST-LENGTH
                                  (ptr thr->task-top) (ptr thr->task-free))

      ;; task-homeのリスト（分割してできたタスク）
      (= hx (cast (ptr (struct task-home))
              (csym::malloc (* (sizeof (struct task-home)) TASK-LIST-LENGTH))))
      (csym::initialize-task-home-list hx TASK-LIST-LENGTH
                                       (ptr thr->treq-top) (ptr thr->treq-free))
      (= thr->sub 0)))
  ;; 投機treqスレッドを参照するポインタとidを設定
  (if option.prefetch
      (begin (= prefetch-thr (+ threads num-thrs))
             (= prefetch-thr-id num-thrs)))

  ;; ワーカスレッド生成
  (for ((= i 0) (< i num-thrs) (inc i))
    (systhr-create worker (+ threads i)))

  ;; 投機treqスレッド生成
  (if option.prefetch
      (systhr-create prefetcher prefetch-thr))

  ;; 本スレッドはOUTSIDEからのメッセージ処理
  (if option.initial-task               ; option.initial-task を入力文字列に変換
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
