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
  (def speculative int)                 ; 投機的に外部へtreq
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
  (csym::fputs str stderr)
  (csym::fputc #\> stderr)
  (csym::fputc #\Space stderr)
  (csym::fputs buf stderr)
  (csym::fputc #\Newline stderr)
  )

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
  (cond
   ((== w TASK) (csym::recv-task pcmd body))
   ((== w RSLT) (csym::recv-rslt pcmd body))
   ((== w TREQ) (csym::recv-treq pcmd))
   ((== w NONE) (csym::recv-none pcmd))
   ((== w RACK) (csym::recv-rack pcmd))
   ((== w EXIT) (csym::exit 0))
   (else
    (csym::proto-error "wrong cmd" pcmd))))

;;; ノード内/外ワーカにコマンドを送信
;;; bodyはtask, rsltの本体．それ以外のコマンドではNULLに
(def (csym::send-command pcmd body task-no) (csym::fn void (ptr (struct cmd)) (ptr void) int)
  (if (== pcmd->node OUTSIDE)
      (csym::send-out-command pcmd body task-no) ; 外部に送信
    (csym::proc-cmd pcmd body)          ; 内部に送信（というかworker自身がコマンド処理）
    ))

(def threads (array (struct thread-data) 64))
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

;;; worker or wait-rslt から
;;; （後者は外に投げたタスクの結果待ち中に別の仕事をやろうとする時）
;;; タスク要求 -> 受け取り -> 計算 -> 結果送信
;;; thr->mut はロック済
;;; (treq-head,req-to): どこにタスク要求を出すか（any or 取り返し）
(def (recv-exec-send thr treq-head req-to)
    (fn void (ptr (struct thread-data)) (ptr (enum node)) (enum node))
  (def rcmd (struct cmd))
  (def tx (ptr (struct task)))
  (def delay long)
  (def old-ndiv int)

  ;; 前に送ったtreq（取り戻し）への none が届くまで待つ
  ;; （これから送るtreqに対するものと混同しないため）
  (while (> thr->w-none 0)
    (csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut))
    ;; rsltがきていたら自分の処理を再開できるのでreturn
    (if (and thr->sub
             (== thr->sub->stat TASK-HOME-DONE))
        (return)))

  ;; allocate
  (= tx thr->task-free)
  (= tx->stat TASK-ALLOCATED)
  (if (not tx)
      (csym::mem-error "Not enough task memory"))
  (= thr->task-top tx)
  (= thr->task-free tx->prev)

  ;;(= delay (* 2 1000 1000))
  (= delay 1000)

  ;; treqコマンド
  (= rcmd.c 2)
  ;; req-to は取り返しであれば取り返し先，そうでなければANY
  (= rcmd.node (if-exp (> num-thrs 1) req-to OUTSIDE))
  (= rcmd.w TREQ)
  (= (aref rcmd.v 0 0) thr->id)
  (= (aref rcmd.v 0 1) TERM)
  (csym::copy-address (aref rcmd.v 1) treq-head)
  (do-while (!= (fref tx -> stat) TASK-INITIALIZED)
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
           (goto Lnone)))
      (if (!= tx->stat TASK-ALLOCATED)
          (break))
      (csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut))
      )
    (if (== tx->stat TASK-NONE)
        (begin
         ;; 外への取り返しに失敗したのならしばらく待つ
         (if 1;; (and thr->sub
              ;;    (== PARENT (aref thr->sub->task-head 0)))
             (let ((t1 (struct timespec))
                   (now (struct timeval))
                   (nsec long))
               (csym::gettimeofday (ptr now) 0)
               (= nsec (* now.tv-usec 1000))
               (+= nsec delay)
               (+= delay delay)         ; 次回の待ち時間を増やす
               (if (> delay (* 40 1000 1000))
                   (= delay (* 40 1000 1000)))
               (= t1.tv-nsec (if-exp (> nsec 999999999)
                                     (- nsec 999999999)
                                     nsec))
               (= t1.tv-sec (+ now.tv-sec
                               (if-exp (> nsec 999999999) 1 0)))
               (csym::pthread-cond-timedwait (ptr thr->cond-r)
                                             (ptr thr->mut)
                                             (ptr t1))
               ))
         ;; rsltが到着していたら
         (if (and thr->sub
                  (== thr->sub->stat TASK-HOME-DONE))
             (goto Lnone))))
    )

  ;; ここで，statはTASK-INITIALIZED
  ;; ここからタスク開始
  (= tx->stat TASK-STARTED)
  (= old-ndiv thr->ndiv)
  (= thr->ndiv tx->ndiv)
  (csym::pthread-mutex-unlock (ptr thr->mut))

  ((aref task-doers tx->task-no) thr tx->body) ; タスク実行

  ;; taskの処理完了後は，そのtask-homeにsend-rsltする
  (= rcmd.w RSLT)
  (= rcmd.c 1)
  (= rcmd.node tx->rslt-to)             ; 外部or内部
  (csym::copy-address (aref rcmd.v 0) tx->rslt-head)
  (csym::send-command (ptr rcmd) tx->body tx->task-no)

  ;; ここでもtreqがたまっていたら noneを送る
  (csym::flush-treq-with-none thr)
  (csym::pthread-mutex-lock (ptr thr->rack-mut))
  (inc thr->w-rack)
  (csym::pthread-mutex-unlock (ptr thr->rack-mut))

  (csym::pthread-mutex-lock (ptr thr->mut))
  (= thr->ndiv old-ndiv)
  ;; タスクstackをpopしてフリーリストに返す
  (label Lnone
         (begin
          (= thr->task-free tx)
          (= thr->task-top tx->next)))
  )

(def (worker arg) (fn (ptr void) (ptr void))
  (def thr (ptr (struct thread-data)) arg)
  (csym::pthread-mutex-lock (ptr thr->mut))
  (loop
    (recv-exec-send thr (init (array (enum node) 2) (array ANY TERM)) ANY))
  (csym::pthread-mutex-unlock (ptr thr->mut)))

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
  (if (not (< id num-thrs))
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

(def (csym::recv-none pcmd) (csym::fn void (ptr (struct cmd)))
  (def thr (ptr (struct thread-data)))
  (def id unsigned-int)
  (def len size-t)
  (if (< pcmd->c 1)
      (csym::proto-error "Wrong none" pcmd))
  (= id (aref pcmd->v 0 0))
  (if (not (< id num-thrs))
      (csym::proto-error "Wrong task-head" pcmd))
  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (if (> thr->w-none 0)
      (dec thr->w-none)
    (= thr->task-top->stat TASK-NONE))
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (csym::pthread-cond-signal (ptr thr->cond))
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

  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
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

(def (csym::choose-treq from-node) (fn int (enum node))
  (cond
    ((<= 0 from-node)
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
    ((== PARENT from-node)
     (return (csym::my-random num-thrs (ptr random-seed1) (ptr random-seed2))))
    (else
     (return 0))))

(def (csym::recv-treq pcmd) (csym::fn void (ptr (struct cmd)))
  (def rcmd (struct cmd))
  (def id unsigned-int)
  (if (< pcmd->c 2)                     ; 引数の数チェック 0:from, 1:to
      (csym::proto-error "Wrong treq" pcmd))
  ;; 仕事を要求するスレッドを決めて，要求を出す
  (if (== (aref pcmd->v 1 0) ANY)
      (let ((myid int) (start-id int) (d int))
        (= myid (aref pcmd->v 0 0))     ; 要求元
        (= start-id (csym::choose-treq myid))
        (for ((= d 0) (< d num-thrs) (inc d))
           (= id (% (+ d start-id) num-thrs)) ; 要求先id
          (if (and (!= pcmd->node OUTSIDE)
                   (== id myid))
              (continue))               ; 自分自身には要求を出さない
          (if (csym::try-treq pcmd id (aref pcmd->v 0)) (break)))
        (if (!= d num-thrs)             ; treqできた
            (return)))
    (begin                              ; "any"でない場合（取り返し）
     (= id (aref pcmd->v 1 0))
     (if (not (< id num-thrs))
         (csym::proto-error "Wrong task-head" pcmd))
     (if (csym::try-treq pcmd id (aref pcmd->v 0)) ; treqできた
         (return))))
  
  ;; 内部からのtreq anyに仕事を返せなかった場合
  (if (== pcmd->node ANY)
      (if (== (aref pcmd->v 0 0) 0)     ; v[0]:from
          ;; 0番workerからのtreqの場合は外部に問い合わせる
          (begin
           (= pcmd->node OUTSIDE)
           (csym::send-command pcmd 0 0)
           (return))
        ;; それ以外のworkerには単にnoneを返す
        (= pcmd->node INSIDE)))

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
  (def id unsigned-int)
  (def len size-t)
  (if (< pcmd->c 1)
      (csym::proto-error "Wrong rack" pcmd))
  ;; idを<task-head>に含める
  (= id (aref pcmd->v 0 0))
  (if (not (< id num-thrs))
      (csym::proto-error "Wrong task-head" pcmd))
  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr thr->rack-mut))
  (dec thr->w-rack)
  (csym::pthread-mutex-unlock (ptr thr->rack-mut)))

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

;; ワーカスレッドがサブタスクの結果待ち合わせ時に呼ぶ
(def (wait-rslt thr) (fn (ptr void) (ptr (struct thread-data)))
  (def body (ptr void))
  (def t1 (struct timespec))
  (def now (struct timeval))
  (def nsec long)
  (def sub (ptr (struct task-home)))
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= sub thr->sub)                      ; スレッドのサブタスク置き場
  (while (!= sub->stat TASK-HOME-DONE)
    ;; (csym::flush-treq-with-none thr)
    ;; (= (fref thr -> task-top -> stat) TASK-SUSPENDED)
    ;; 外部ノードに送った仕事ならちょっと待つ
    (if (== PARENT (aref sub->task-head 0))
        (begin
         (csym::gettimeofday (ptr now) 0)
         (= nsec (* 1000 (fref now tv-usec)))
         (+= nsec 1000);(* 5 1000 1000))      ; ちょっと待っている（ここの数字が待ち時間）
         ;; (+= nsec (* 10 1000 1000))
         (= (fref t1 tv-nsec) (if-exp (> nsec 999999999)
                                      (- nsec 999999999)
                                      nsec))
         (= (fref t1 tv-sec) (+ (fref now tv-sec)
                                (if-exp (> nsec 999999999) 1 0)))
         (csym::pthread-cond-timedwait (ptr thr->cond-r) (ptr thr->mut)
                                       (ptr t1))
         ))
    (= thr->task-top->stat TASK-STARTED)
    (if (== sub->stat TASK-HOME-DONE)
        (break))
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
                 "Usage: %s [-s hostname] [-p port-num] [-n n-threads] [-S]~%"
                 (aref argv 0))
  (csym::exit 1))

(def (set-option argc argv) (csym::fn void int (ptr (ptr char)))
  (def i int) (def ch int)
  ;; Default values
  (= option.num-thrs 1)
  (= (aref option.sv-hostname 0) #\NULL)
  (= option.port 8888)
  (= option.speculative 0)
  ;; Parse and set options
  (while (!= -1 (= ch (csym::getopt argc argv "n:s:p:S")))
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

        (case #\S)                      ; turn-on speculative task receipt from external nodes
        (= option.speculative 1)
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
  (csym::gdk-draw-pixmap (fref widget -> window)
                         (aref (fref widget -> style -> fg-gc)
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
                     (= gc (csym::gdk-gc-new (fref window -> window)))
                     (csym::gtk-signal-connect (csym::GTK-OBJECT darea) "configure_event"
                                               (csym::GTK-SIGNAL-FUNC csym::configure-event) 0)
                     (csym::gtk-signal-connect (csym::GTK-OBJECT darea) "expose_event"
                                               (csym::GTK-SIGNAL-FUNC csym::expose-event) 0)
                     (csym::gtk-timeout-add 33 repaint (cast gpointer darea))
                     (csym::gtk-widget-show-all window)
                     (csym::systhr-create gtk-main 0)
                     )
  
  ;; サーバに接続
  (= sv-socket (if-exp (== #\NULL (aref (fref option sv-hostname) 0))
                       -1
                       (csym::connect-to (fref option sv-hostname)
                                         (fref option port))))

  ;; send-mut（外部送信ロック）の初期化
  (csym::pthread-mutex-init (ptr send-mut) 0)

  ;; thread-data の初期化, task の 双方向list も
  (= num-thrs (fref (fref option num-thrs)))
  (for ((= i 0) (< i num-thrs) (inc i))
    (let ((thr (ptr (struct thread-data)) (+ threads i))
          (tx (ptr (struct task)))
          (hx (ptr (struct task-home))))
      (= (fref thr -> req) 0)
      (= (fref thr -> id) i)
      (= (fref thr -> w-rack) 0)
      (= (fref thr -> w-none) 0)
      (= (fref thr -> ndiv) 0)
      (= (fref thr -> last-treq) i)
      (= (fref thr -> last-choose) CHS-RANDOM)
      (let ((r double) (q double))
        (= r (csym::sqrt (+ 0.5 i)))
        (= q (csym::sqrt (+ r i)))
        (-= r (cast int r))
        (= (fref thr -> random-seed1) r)
        (= (fref thr -> random-seed2) q))
      (csym::pthread-mutex-init (ptr (fref thr -> mut)) 0)
      (csym::pthread-mutex-init (ptr (fref thr -> rack-mut)) 0)
      (csym::pthread-cond-init (ptr (fref thr -> cond)) 0)
      (csym::pthread-cond-init (ptr (fref thr -> cond-r)) 0)

      ;; taskの双方向リスト（スレッドが実行するべきタスク）
      (= tx (cast (ptr (struct task))
              (csym::malloc (* (sizeof (struct task)) TASK-LIST-LENGTH))))
      (= (fref thr -> task-top) 0)
      (= (fref thr -> task-free) tx)
      (for ((= j 0) (< j (- TASK-LIST-LENGTH 1)) (inc j))
        (= (fref (aref tx j) prev) (ptr (aref tx (+ j 1))))
        (= (fref (aref tx (+ j 1)) next) (ptr (aref tx j))))
      (= (fref (aref tx 0) next) 0)
      (= (fref (aref tx (- TASK-LIST-LENGTH 1)) prev) 0)

      ;; task-homeのリスト（分割してできたタスク）
      (= hx (cast (ptr (struct task-home))
              (csym::malloc (* (sizeof (struct task-home)) TASK-LIST-LENGTH))))
      (= (fref thr -> treq-top) 0)
      (= (fref thr -> treq-free) hx)
      (= (fref thr -> sub) 0)
      ;; フリーリストを構成
      (for ((= j 0) (< j (- TASK-LIST-LENGTH 1)) (inc j))
        (= (fref (aref hx j) next) (ptr (aref hx (+ j 1)))))
      (= (fref (aref hx (- TASK-LIST-LENGTH 1)) next) 0)))
  ;; ワーカスレッド生成
  (for ((= i 0) (< i num-thrs) (inc i))
    (let ((thr (ptr (struct thread-data)) (+ threads i)))
      (systhr-create worker thr)))

  ;; 本スレッドはOUTSIDEからのメッセージ処理
  (while 1
    (= pcmd (csym::read-command))
    (csym::proc-cmd pcmd 0))
  (csym::exit 0))
