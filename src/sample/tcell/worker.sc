;;; Copyright (c) 2008 Tasuku Hiraishi <hiraisi@kuis.kyoto-u.ac.jp>
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
(%include "rule/nestfunc-setrule.sh")

(c-exp "#include<stdio.h>")
(c-exp "#include<stdlib.h>")
(c-exp "#include<string.h>")
(c-exp "#include<math.h>")
(c-exp "#include<pthread.h>")
(c-exp "#include<sys/time.h>")
(c-exp "#include<getopt.h>")
#+tcell-gtk (c-exp "#include<gtk/gtk.h>")
(%include "worker.sh")


;;; Command-line options
(%defconstant HOSTNAME-MAXSIZE 256)
(def (struct runtime-option)
  (def num-thrs int)                    ; worker数
  (def sv-hostname (array char HOSTNAME-MAXSIZE))
                                        ; Tascellサーバのホスト名．""ならstdout
  (def port unsigned-short)             ; Tascellサーバへの接続ポート番号
  (def prefetch int)                    ; 投機的に外部へtreq
  (def verbose int)                     ; verbose level
  )
(static option (struct runtime-option))


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
  (csym::fputc #\Newline stderr))

;;; エラーメッセージstrとコマンドをstderrに出力
(def (csym::proto-error str pcmd) (csym::fn void (ptr (const char)) (ptr (struct cmd)))
  (def i int)
  (def buf (array char BUFSIZE))
  (csym::serialize-cmd buf pcmd)
  (csym::fprintf stderr "%s> %s~%" str buf))

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
  (if (> option.verbose 0) (csym::fprintf stderr "RECEIVED> %s~%" b))
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
  ;; task, rsltのbody
  (if body
      (cond
       ((== w TASK)
        ((aref task-senders task-no) body)
        (csym::write-eol))
       ((== w RSLT)
        ((aref rslt-senders task-no) body)
        (csym::write-eol))))
  (csym::flush-send)
  (csym::pthread-mutex-unlock (ptr send-mut))
  ;; ---> sender-lock --->
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
   (case STAT) (csym::print-status pcmd) (break)
   (case VERB) (csym::set-verbose-level pcmd) (break)
   (case EXIT) (csym::exit 0) (break)
   (default) (csym::proto-error "wrong cmd" pcmd) (break))
  )

;;; ノード内/外ワーカにコマンドを送信
;;; bodyはtask, rsltの本体．それ以外のコマンドではNULLに
(def (csym::send-command pcmd body task-no) (csym::fn void (ptr (struct cmd)) (ptr void) int)
  (if (== pcmd->node OUTSIDE)
      (begin
       (csym::send-out-command pcmd body task-no) ; 外部に送信
       (if (> option.verbose 0)
           (csym::proto-error "OUTSIDE" pcmd)))
    (begin
     (csym::proc-cmd pcmd body)         ; 内部に送信（というかworker自身がコマンド処理）
     (if (> option.verbose 2)
         (csym::proto-error "INSIDE" pcmd)))))

(def threads (array (struct thread-data) 64))
(def prefetch-thr (ptr (struct thread-data)))
(def prefetch-thr-id int)
(def num-thrs unsigned-int)


;;; recv-exec-send および wait-rslt から
;;; treq（一旦treqを受け付けたが未初期化） がたまっていたら
;;; それらを破棄したうえで noneを送る．
;;; thr->mut と send_mut は lock 済
(def (csym::flush-treq-with-none thr) (csym::fn void (ptr (struct thread-data)))
  (def rcmd (struct cmd))
  (def hx (ptr (struct task-home)))
  (= rcmd.c 1)
  (= rcmd.w NONE)
  (while (= hx thr->treq-top)
    (= rcmd.node hx->req-from)          ; 外部or内部
    (csym::copy-address (aref rcmd.v 0) hx->task-head)
    (csym::send-command (ptr rcmd) 0 0)
    (= thr->treq-top hx->next)          ; treqスタックをpop
    (= hx->next thr->treq-free)         ; フリーリストに...
    (= thr->treq-free hx))              ; ...領域を返却
  )

;;; スレッドthr(ロック済)の持つtaskのフリーリストから1つぶんallocateして，そこへのポインタを返す
(def (csym::allocate-task thr) (fn (ptr (struct task)) (ptr (struct thread-data)))
  (def tx (ptr (struct task)))
  (= tx thr->task-free)
  (= tx->stat TASK-ALLOCATED)
  (if (not tx) (csym::mem-error "Not enough task memory"))
  (= thr->task-top tx)
  (= thr->task-free tx->prev)
  (return tx))

;;; スレッドthr(ロック済)の持つtaskのリストから1つぶんフリーリストに返す
(def (csym::deallocate-task thr) (fn void (ptr (struct thread-data)))
  (def tx (ptr (struct task)) thr->task-top)
  (= thr->task-free tx)
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

;;; TREQを送信して thr->task-topの指すtaskをTASK-INITIALIZEDにする
;;; (treq-head,req-to): recv-exec-send の引数そのまま
;;; 受付中のtreqのフラッシュや，rsltが来たときの対応も行う
;;; INITIALIZEDできた->1 そうでなければ->0 を返す
(def (csym::send-treq-to-initialize-task thr treq-head req-to)
    (fn int (ptr (struct thread-data)) (ptr (enum node)) (enum node))
  (def rcmd (struct cmd))
  (def delay long 1000)     ; none が返ってきたとき待つ時間[nsec]
  (def delay-max long (* 40 1000 1000))
  (def tx (ptr (struct task)) thr->task-top)

  ;; treqコマンド
  (= rcmd.c 2)
  (= rcmd.node req-to) ; 取り返しであれば取り返し先，そうでなければANY
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

  (do-while (!= tx->stat TASK-INITIALIZED)
    ;; 最初にtreqがたまっていたら，noneを送る
    ;; これをやると treq が飛び交うかもしれないが，
    ;; none を送らないとだけすると，
    ;; 互いに none or task が送られているのを待つ
    ;; というタイプのデッドロックとなる．
    (csym::flush-treq-with-none thr)
    (= tx->stat TASK-ALLOCATED)         ; *-NONEにされてるかもしれないのを元に戻す
    (begin
     (csym::pthread-mutex-unlock (ptr thr->mut))
     (csym::send-command (ptr rcmd) 0 0) ; treq送信
     (csym::pthread-mutex-lock (ptr thr->mut)))
    ;; recv-task が *tx を初期化するのを待つ
    (loop
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
      ;; tx->statの変化を待つ
      (csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut))
      )
    (if (== tx->stat TASK-NONE)
        (begin
         ;; 外への取り返しに失敗したのならしばらく待つ
         (if 1;; (and thr->sub
              ;;    (== PARENT (aref thr->sub->task-head 0)))
             (let ((t-until (struct timespec))
                   (now (struct timeval)))
               (csym::gettimeofday (ptr now) 0)
               (csym::timeval-plus-nsec-to-timespec (ptr t-until) (ptr now) delay)
               (csym::pthread-cond-timedwait (ptr thr->cond-r)
                                             (ptr thr->mut)
                                             (ptr t-until))
               (+= delay delay)         ; 次回の待ち時間を増やす
               (if (> delay delay-max) (= delay delay-max))
               ))
         ;; rsltが到着していたらtreqリトライせず，そちらの処理を優先
         (if (and thr->sub
                  (== thr->sub->stat TASK-HOME-DONE))
             (return 0))))
    )
  (return 1))

;;; worker or wait-rslt から
;;; （後者は外に投げたタスクの結果待ち中に別の仕事をやろうとする時）
;;; タスク要求 -> 受け取り -> 計算 -> 結果送信
;;; thr->mut はロック済
;;; (treq-head,req-to): どこにタスク要求を出すか（any or 取り返し）
(def (recv-exec-send thr treq-head req-to)
    (fn void (ptr (struct thread-data)) (ptr (enum node)) (enum node))
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
       ((aref task-doers tx->task-no) thr tx->body) ; タスク実行
       ;; taskの処理完了後は，そのtask-homeにsend-rsltする
       (= rcmd.w RSLT)
       (= rcmd.c 1)
       (= rcmd.node tx->rslt-to)        ; 外部or内部
       (csym::copy-address (aref rcmd.v 0) tx->rslt-head)
       (csym::send-command (ptr rcmd) tx->body tx->task-no)
       ;; ここでもtreqがたまっていたら noneを送る
       (csym::flush-treq-with-none thr)
       (csym::pthread-mutex-lock (ptr thr->rack-mut))
       (inc thr->w-rack)
       (csym::pthread-mutex-unlock (ptr thr->rack-mut))
       (csym::pthread-mutex-lock (ptr thr->mut))
       (= thr->ndiv old-ndiv)))

  ;; タスクstackをpopしてフリーリストに返す
  (csym::deallocate-task thr)
  )


;;; ワーカのループ
(def (worker arg) (fn (ptr void) (ptr void))
  (def thr (ptr (struct thread-data)) arg)
  (csym::pthread-mutex-lock (ptr thr->mut))
  (loop
    (recv-exec-send thr (init (array (enum node) 2) (array ANY TERM)) ANY))
  (csym::pthread-mutex-unlock (ptr thr->mut)))


;;; 投機的にtreqを送ってtaskを受信するスレッドのループ
;;; 今のところ投機的にとれる仕事の数は1で固定
(def (prefetcher thr0) (fn (ptr void) (ptr void))
  (def treq-head (array (enum node) 2) (array ANY TERM))
  (def req-to (enum node) OUTSIDE)
  (def thr (ptr (struct thread-data)) (cast (ptr (struct thread-data)) thr0))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (loop
    ;;リストが空になるまで待機
    (while thr->task-top
       (csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut)))
    (csym::allocate-task thr)
    ;; treqを送って仕事獲得：とれるまでやる
    (while (not (csym::send-treq-to-initialize-task thr treq-head req-to)))
    ;;リストが空でなくなったことを通知
    (csym::pthread-cond-signal (ptr thr->cond))
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
  (csym::pthread-cond-signal (ptr thr->cond))
  ;;リストが空になったことを通知
  (csym::pthread-cond-signal (ptr prefetch-thr->cond))
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::pthread-mutex-unlock (ptr prefetch-thr->mut))
  (return 1))

;;; 外からprefetchスレッドへのtreq（取り返し）に対し，まだその仕事を始めてなければ
;;; そのまま返却する．返却した->1, しなかった->0 を返す．
(def (csym::try-take-back-prefetched-task treq-head) (fn int (ptr (enum node)))
  (def tx (ptr (struct task)))
  (def retval int)
  (csym::pthread-mutex-lock (ptr prefetch-thr->mut))
  (if (and (= tx prefetch-thr->task-top) ; prefetchスレッドが持っているタスクがtreq元が送ったものと同一か判定
           (csym::address-equal tx->rslt-head treq-head))
      (let ((rcmd (struct cmd)))
        ;; 仕事のキャンセルを伝える
        (= rcmd.w BACK) (= rcmd.c 1) (= rcmd.node OUTSIDE)
        (csym::copy-address (aref rcmd.v 0) treq-head)
        (csym::send-command (ptr rcmd) 0 0)
        ;; prefetchした仕事を破棄
        (csym::deallocate-task prefetch-thr) ; !!! bodyのごみをどうする？
        (csym::pthread-cond-signal (ptr prefetch-thr->cond)) ; 空になったことを通知
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
  (def id unsigned-int)
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
  (csym::pthread-cond-signal (ptr thr->cond))
  )


;;; none <送信先>
(def (csym::recv-none pcmd) (csym::fn void (ptr (struct cmd)))
  (def thr (ptr (struct thread-data)))
  (def id unsigned-int)
  (def len size-t)
  (if (< pcmd->c 1) (csym::proto-error "Wrong none" pcmd))
  (= id (aref pcmd->v 0 0))
  (if (not (< id num-thrs)) (csym::proto-error "Wrong task-head" pcmd))
  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (if (> thr->w-none 0)
      (dec thr->w-none)
    (= thr->task-top->stat TASK-NONE))
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::pthread-cond-signal (ptr thr->cond)) ; TASK-NONEになったことを通知
  )


;;; back <送信先>:<task-id>
;;; 送信した仕事のキャンセル通知．task-homeにある仕事を自分自身でやるようにする．
;;; キャンセルされた仕事は subスタックのトップにあるものなので，そこから情報獲得．
(def (csym::recv-back pcmd) (csym::fn void (ptr (struct cmd)))
  (def thr (ptr (struct thread-data)))
  (def tx (ptr (struct task)))
  (def hx (ptr (struct task-home)))
  (def id unsigned-int)                 ; 引数の数チェック．0:送信先
  (if (< pcmd->c 1) (csym::proto-error "Wrong back" pcmd))
  (= id (aref pcmd->v 0 0))
  (if (not (< id num-thrs)) (csym::proto-error "Wrong task-head" pcmd))
  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= tx thr->task-top) (= hx thr->sub)
  (= tx->task-no hx->task-no)
  (= tx->body hx->body)
  (= tx->ndiv thr->ndiv)
  (= tx->rslt-to INSIDE)
  (= (aref tx->rslt-head 0) id)
  (= (aref tx->rslt-head 1) hx->id)
  (= (aref tx->rslt-head 2) TERM)
  (= hx->req-from INSIDE)               ; taskの送り先情報を変更
  (= (aref hx->task-head 0) id)         ;
  (= (aref hx->task-head 0) TERM)       ;
  (= tx->stat TASK-INITIALIZED)
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::pthread-cond-signal (ptr thr->cond)) ; TASK-INITIALIZEDになったことを通知
  )


(def (csym::recv-rslt pcmd body) (csym::fn void (ptr (struct cmd)) (ptr void))
  (def rcmd (struct cmd))               ; rackコマンド
  (def thr (ptr (struct thread-data)))
  (def hx (ptr (struct task-home)))
  (def tid unsigned-int)
  (def sid unsigned-int)
  ;; 引数の数チェック
  (if (< pcmd->c 1)
      (csym::proto-error "Wrong rslt" pcmd))
  ;; 結果受取人決定 "<thread-id>:<task-home-id>"
  (= tid (aref pcmd->v 0 0))
  (if (not (< tid num-thrs))
      (csym::proto-error "wrong rslt-head" pcmd))
  (= sid (aref pcmd->v 0 1))
  (if (== TERM sid)
      (csym::proto-error "Wrong rslt-head (no task-id)" pcmd))
  (= thr (+ threads tid))

  (csym::pthread-mutex-lock (ptr thr->mut))
  ;; hx = 返ってきたrsltを待っていたtask-home（.id==sid）を探す
  (= hx thr->sub)
  (while (and hx (!= hx->id sid))
    (= hx hx->next))
  (if (not hx)
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
       (csym::pthread-cond-signal (ptr thr->cond-r))
       (csym::pthread-cond-signal (ptr thr->cond)))
    )
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::send-command (ptr rcmd) 0 0))  ;rack送信


;;; threads[id] にtreqを試みる
(def (csym::try-treq pcmd id from-addr)
    (csym::fn int (ptr (struct cmd)) unsigned-int (ptr (enum node)))
  (def hx (ptr (struct task-home)))
  (def thr (ptr (struct thread-data)))
  (def avail int 0)
  (def from-head (enum node) (aref from-addr 0))

  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (csym::pthread-mutex-lock (ptr thr->rack-mut))
  (if (and (== thr->w-rack 0)           ; rack待ちならだめ
           (or ;;(and (!= PARENT from-head) ; 弟からならとりあえず受理して待たせておく
               ;;   (< id from-head))
               (and thr->task-top       ; 仕事中なら普通に受理
                    (or (== thr->task-top->stat TASK-STARTED)
                        (== thr->task-top->stat TASK-INITIALIZED)))))
      (= avail 1))
  (csym::pthread-mutex-unlock (ptr thr->rack-mut))

  ;; 成功ならtask-home（仕事の結果待ち） スタックにpush
  (if avail
      (begin
       (= hx thr->treq-free)
       (if (not hx)
           (csym::mem-error "Not enough task-home memory"))
       (= thr->treq-free hx->next)      ; フリーリストから領域を確保
       (= hx->next thr->treq-top)       ; これよりnextはスタックのリンク
       (= hx->stat TASK-HOME-ALLOCATED)
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
(def (csym::choose-treq from-node) (fn int (enum node))
  (cond
   ((<= 0 from-node)                    ; 内部から
    (let ((thr (ptr (struct thread-data)) (+ threads from-node)))
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
   ((== PARENT from-node)               ; 外部から
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
  (def dst0 (enum node))
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
        (if (csym::try-treq pcmd id (aref pcmd->v 0))
            (begin
             (if (> option.verbose 1) (fprintf stderr "try-treq %d->%d... accepted.~%" myid id))
             (break)))
        (if (> option.verbose 1) (fprintf stderr "try-treq to %d->%d... refused.~%" myid id))
        )
      (if (!= d num-thrs)               ; treqできた
          (return))))
   ;; prefetchスレッドへの要求
   ((== dst0 prefetch-thr-id)
    (if (== pcmd->node OUTSIDE)         ; 外からの取り返し
        (if (csym::try-take-back-prefetched-task (aref pcmd->v 0)) ; まだタスクが開始されてなければそのまま取り返す
            (return)
          (begin                        ; 開始されていれば0番スレッドから取り返す(leapfrogging)
           (= (aref pcmd->v 1 0) 0)
           (csym::recv-treq pcmd)
           (return)))
      ;; (!= pcmd->node OUTSIDE): 内部ワーカからprefetchedタスクの要求
      (if (csym::pop-prefetched-task (+ threads (aref pcmd->v 0 0)))
          (return))))
   ;; 取り返し(leapfrogging)
   (else
    (if (not (and (< 0 dst0) (< dst0 num-thrs)))
        (csym::proto-error "Wrong task-head" pcmd))
    (if (csym::try-treq pcmd dst0 (aref pcmd->v 0)) ; treqできた
        (return))))
  ;; 内部のワーカが，渡せる仕事がなかった場合のみここに来る
  (if (and (== pcmd->node ANY)
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
  (= rcmd.node (if-exp (== pcmd->node ANY) INSIDE pcmd->node)) ; [INSIDE|OUTSIDE]
  (= rcmd.w NONE)
  (csym::copy-address (aref rcmd.v 0) (aref pcmd->v 0))
  (csym::send-command (ptr rcmd) 0 0))


;; rack <rack送信先header(ここではthread-id)>
(def (csym::recv-rack pcmd) (csym::fn void (ptr (struct cmd)))
  (def tx (ptr (struct task)))
  (def thr (ptr (struct thread-data)))
  (def id unsigned-int)
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


;;; taskの情報を出力
(def task-stat-strings (array (ptr char)) ; enum task-statに対応
  (array "TASK-ALLOCTED" "TASK-INITIALIZED" "TASK-STARTED" "TASK-DONE" "TASK-NONE" "TASK-SUSPENDED"))
(def (csym::node-to-string buf node) (csym::fn void (ptr char) (enum node)) ; enum nodeに対応
  (switch node
    (case OUTSIDE) (sprintf buf "OUTSIDE") (break)
    (case INSIDE)  (sprintf buf "INSIDE")  (break)
    (case ANY)     (sprintf buf "ANY")     (break)
    (case PARENT)  (sprintf buf "PARENT")  (break)
    (case TERM)    (sprintf buf "TERM")    (break)
    (default)      (sprintf buf "%d" node) (break)))
(def (csym::print-task-list task-top) (csym::fn void (ptr (struct task)))
  (def cur (ptr (struct task)))
  (defs (array char BUFSIZE) buf1 buf2)
  (csym::fprintf stderr "task= {")
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
  (defs (array char BUFSIZE) buf1 buf2)
  (csym::fprintf stderr "%s= {" name)
  (for ((= cur treq-top) cur (= cur cur->next))
    (csym::fprintf stderr "{stat=%s, id=%d, task-no=%d, body=%p, req-from=%s, task-head=%s}, "
                   (aref task-home-stat-strings cur->stat) cur->id cur->task-no cur->body
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
  (csym::print-task-list thr->task-top)
  (csym::print-task-home-list thr->treq-top "treq-top")
  (csym::print-task-home-list thr->sub "sub")
  (csym::fprintf stderr "~%")
  (return)
  )

;;; statコマンド -> 状態を出力
(def (csym::print-status pcmd) (csym::fn void (ptr (struct cmd)))
  (def i int)
  (csym::fprintf stderr "num-thrs: %d~%" num-thrs)
  (csym::fprintf stderr "prefetches: %d~%" option.prefetch)
  (csym::fprintf stderr "verbose-level: %d~%" option.verbose)
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
;; -thr->mut ロック済み
(def (csym::make-and-send-task thr task-no body) ; task-noをtcell追加
    (csym::fn void (ptr (struct thread-data)) int (ptr void))
  (def tcmd (struct cmd))
  (def hx (ptr (struct task-home)) thr->treq-top)
  (= thr->treq-top hx->next)            ; task要求スタックをpop
  (= hx->next thr->sub)                 ; これよりサブタスクstackのリンク
  (= thr->sub hx)                       ; サブタスクstackにpush
  (= hx->task-no task-no)
  (= hx->body body)
  (= hx->id (if-exp hx->next            ; サブタスクID = 底から何番目か
                    (+  hx->next->id 1)
                    0))
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
  (while (!= sub->stat TASK-HOME-DONE)
    ;; (csym::flush-treq-with-none thr)
    ;; (= (fref thr -> task-top -> stat) TASK-SUSPENDED)
    ;; 外部ノードに送った仕事ならちょっと待つ
    (if (== PARENT (aref sub->task-head 0))
        (let ((now (struct timeval))
              (t-until (struct timespec)))
          (csym::gettimeofday (ptr now) 0)
          (csym::timeval-plus-nsec-to-timespec (ptr t-until) (ptr now) 1000)
          (csym::pthread-cond-timedwait (ptr thr->cond-r) (ptr thr->mut)
                                        (ptr t-until))
          ))
    (= thr->task-top->stat TASK-STARTED)
    (if (== sub->stat TASK-HOME-DONE) (break))
    ;; 取り返しにいく (leapfrogging)
    (recv-exec-send thr sub->task-head sub->req-from))
  (= body sub->body)
  (= thr->sub sub->next)                ; サブタスクstackをpop
  (= sub->next thr->treq-free)          ; popした部分を...
  (= thr->treq-free sub)                ; ...フリーリストに返す
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (return body))


;;; Handling command-line options
(def (csym::usage argc argv) (csym::fn void int (ptr (ptr char)))
  (csym::fprintf stderr
                 "Usage: %s [-s hostname] [-p port-num] [-n n-threads] [-P n-prefetches] [-v verbose]~%"
                 (aref argv 0))
  (csym::exit 1))

(def (set-option argc argv) (csym::fn void int (ptr (ptr char)))
  (def i int) (def ch int)
  ;; Default values
  (= option.num-thrs 1)
  (= (aref option.sv-hostname 0) #\NULL)
  (= option.port 8888)
  (= option.prefetch 0)
  (= option.verbose 0)
  ;; Parse and set options
  (while (!= -1 (= ch (csym::getopt argc argv "n:s:p:P:v:h")))
    (for ((= i 0) (< i argc) (inc i))
      (switch ch
        (case #\n)                      ; number of threads
        (= option.num-thrs (csym::atoi optarg))
        (break)

        (case #\s)                      ; server name
        (if (csym::strcmp "stdout" optarg)
            (begin
             (csym::strncpy option.sv-hostname optarg
                            HOSTNAME-MAXSIZE)
             (= (aref option.sv-hostname (- HOSTNAME-MAXSIZE 1)) 0))
          (= (aref option.sv-hostname 0) #\NULL))
        (break)

        (case #\p)                      ; connection port number
        (= option.port (csym::atoi optarg))
        (break)

        (case #\P)                      ; the number of speculative tasks from external nodes
        (= option.prefetch (csym::atoi optarg))
        (break)

        (case #\v)                      ; verbose level
        (= option.verbose (csym::atoi optarg))
        (break)

        (case #\h)                      ; usage
        (csym::usage argc argv)
        (break)

        (default)
        (csym::fprintf stderr "Unknown option: %c~%" ch)
        (csym::usage argc argv)
        (break))))
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
  
  ;; コマンドラインオプション
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

  ;; thread-data の初期化, task の 双方向list も
  ;; 冬季treqがonなら，それ用のスレッドをnum-thrs+1番目として作って初期化する
  (= num-thrs option.num-thrs)
  (for ((= i 0) (< i (+ num-thrs (if-exp option.prefetch 1 0))) (inc i))
    (let ((thr (ptr (struct thread-data)) (+ threads i))
          (tx (ptr (struct task)))
          (hx (ptr (struct task-home))))
      (= thr->req 0)
      (= thr->id i)
      (= thr->w-rack 0)
      (= thr->w-none 0)
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
  (while 1
    (= pcmd (csym::read-command))
    (csym::proc-cmd pcmd 0))
  (csym::exit 0))
