;; worker3から，ics08時に改良
;; * コマンドや引数（ワーカアドレス）を必要時以外文字列で扱わない
;;   - serialize, deserialize関数(in cmd-serial.sc)を作って相互変換をモジュール化
;;   - cmd.v[0]にコマンド名を入れない．以降前にずらす
;; * 外部へのsendを全ての種類についてsenderスレッドが担当
;; --> やめて全てworker自身が送信
;; * 内部へのsendについては inms-queueに追加 => 内部メッセージ処理スレッドが担当
;; --> これもやめて送信するworker自身が必要な処理を行う
;; * 内部のメッセージ処理については，文字列を組み立てるのをやめる
;; * timewait-rslt
;;   treq->noneがかえってきたとき，にしばらく待つか選択

;; *** 以下未処理の改良すべき点
;; [07/08/13]
;; * とりもどしのためのtreq（any以外のtreq）に対しては自分が空になるまで
;; （treq anyを送る状況になるまで）noneを返さない
;; 理由：a->b にtreqして仕事がないのは，b->cにtreqしているからである場合が多いので
;;         anyがtreqだったかどうかは受け取った時に覚えておく必要
;; 大林くんがいじる前のversion: ~yasugi/lang/c/loadb/

;; [08/01/16]
;; 継続ごとシリアライズして別ノードに送ると落ちても大丈夫

#|

・実装の話
・オブジェクト指向にするなら．．．
(def (struct hoge)
    indexとか入った構造体へのポインタ
  (def i int 0)
  (def ))

External data representation(XDR)

全ノードで同じ仕事を知っているが
別の部分を担当する方式も，
→ 一部落ちたときに "マージ処理部分" が冗長化されている

|#

(%include "rule/nestfunc-setrule.sh")

(c-exp "#include<stdio.h>")
(c-exp "#include<stdlib.h>")
(c-exp "#include<pthread.h>")
(c-exp "#include<sys/time.h>")
(c-exp "#include<getopt.h>")
#+tcell-gtk (c-exp "#include<gtk/gtk.h>")
(%include "worker4.sh")

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

(def threads (array (struct thread-data) 64)) ; ★ 64でよい？
(def num-thrs unsigned-int)

;;; リモートに treq してもらった task (copy)
;;; - treq まえに allocate
;;; x ときどき，リストから DONE となったものを除く
;;; (それなら，rack を得るまで DONE にしないようにする
;;;                 active なスレッド数は別管理すればよい?)
;;; x  ときどきでなくて，rack を得たときでよい?
;;; - rslt を送ったら自分で消える．

;;; 分割して作った task (home/orig) => task-home
;;; - thread-data の sub からのリストとなり，id は重複しないように付ける．
;;; x 最初から，STARTED，すぐに treq 元に送る．
;;; o treq の時点で ALLOCATED にしては?
;;; - rslt がきたら，DONE にして，rack を返す
;;; - DONE になっていたら，分割元task がマージして消去

;;; ?? task-home をそのまま同じノードで処理するケースもあるのか??

;;; task_top, task_free はこの形?
;;; 初期値は task_top = 0, task_free は左端(prevで次のfreeブロック)
;;; task_top を左から右に移動させれば，一つ前のタスクがnextに得られる．
;;; allocateは task_top = task_free, task_free = task_free->prev
;;; free は    task_free = task_top, task_top = task_top->next
;;; [ prev  ] -> [ prev  ] -> [ prev  ]  -> 
;;; <- [ next  ] <- [ next  ] <- [ next  ] <- 

;;; recv-exec-send および wait-rslt から
;;; treq（一旦treqを受け付けたが未初期化） がたまっていたら
;;; それらを破棄したうえで noneを送る．
;;; thr->mut と send_mut は lock 済
;;; ☆ 両方lock はだめなのでは?
(def (csym::flush-treq-with-none thr) (csym::fn void (ptr (struct thread-data)))
  (def rcmd (struct cmd))
  (def hx (ptr (struct task-home)))
  (= rcmd.c 1)                          ; 4でへらした
  (= rcmd.w NONE)
  (while (= hx thr->treq-top)
    (= rcmd.node hx->req-from)          ; 外部or内部
    (csym::copy-address (aref rcmd.v 0) hx->task-head) ; 4でへらした
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
  ;; 最初にtxをとる
  ;; タスクをとろうとしている間に，rslt が届いたら工夫するのか?
  ;; 単に，rslt 待ちについては，wait しないで，定期的にみる．
  ;; みたときに，なければ，task 待ちにしてしまうというのが簡単?
  ;; だって task 待ちにしたら treq をだすのだし．
  ;; ここでは，task 待ちのする場合
  ;; task については，treq 時点で場所を作っておく => ノード内並列にも
  ;; 同時スレッド数は超えないようには，別途セマフォで
  ;; ★ ↑遅延隠蔽のため余計にスレッドを作る場合のスレッド数

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
  (= rcmd.c 2)                          ; 4でへらした
  (if (> num-thrs 1)
      (= rcmd.node req-to)
    (= rcmd.node OUTSIDE))
  (= rcmd.w TREQ)
  (= (aref rcmd.v 0 0) thr->id)         ; 4でへらした
  (= (aref rcmd.v 0 1) TERM)
  (csym::copy-address (aref rcmd.v 1) treq-head) ; 4でへらした
  (do-while (!= (fref tx -> stat) TASK-INITIALIZED)
    ;; 最初にtreqがたまっていたら，noneを送る
    ;; これをやると treq が飛び交うかもしれないが，
    ;; none を送らないとだけすると，
    ;; 互いに none or task が送られているのを待つ
    ;; というタイプのデッドロックとなる．
    (csym::flush-treq-with-none thr)
    (= tx->stat TASK-ALLOCATED)         ; *-NONEにされてるかもしれないのを元に戻す
    ;; ☆ ここで，send_mut をとる前に，thr->mut を unlock すべきでは?
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
          ;; ★ ↓のほうにある，rack のタイミングによっては
          ;; この前提が崩れる可能性あり．前提が崩れた場合は，
          ;; w_none は使えないので，none か task が返されるのをこの場で
          ;; 待ち，task だった場合は，以下の do_task_body のほうに進むのでは?
          (begin
           (if (!= tx->stat TASK-NONE)
               (inc thr->w-none))
           (goto Lnone)))
      (if (!= tx->stat TASK-ALLOCATED)
          (break))
      (csym::pthread-cond-wait (ptr thr->cond) (ptr thr->mut))
      )
    ;; none を受け取ったときに，しばらく待つようにしてみてある．
    ;; 何か他の手は使えないか? (同時複数個所待ち相当の何かで)
    ;; ★ すくなくともリクエスト先が any でないなら，
    ;;    rslt とセットのnone を返す場合以外は，
    ;;    noneを返す必要はなく待たせておけばよい．
    ;; ★ any の場合，treq combining とか，複数個所待ちとか
    ;;    ここでは，delay を倍々としていっている．2ms からはじめて
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
               (if (> delay (* 40 1000 1000)) ; でも限度がある
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

  ;; none, task, rslt を同時に待てないか?
  ;; rack を使う限りでは，treq 中に rslt が届いた場合はその後
  ;; かならず none が戻ってくる．
  ;; none を待たずに，w-none を inc して，none が届いたときには
  ;; "struct task の書き込み/signal"せずに dec だけすればよさそう．
  ;; rack を使わないなら，rslt の後，task が届くかも．
  ;; 現在処理中の task が task-top でなくてもよいなら問題ない．
  ;; そのためには，task-top でないときに分かるようにするとともに
  ;; treq 時に thread番号に，task番号を加える必要があると思う．
  ;; ★ いまは，rack, w-none を使っているはず．

  ;; ここで，statはTASK-INITIALIZED
  ;; ここからタスク開始
  (= tx->stat TASK-STARTED)
  (= old-ndiv thr->ndiv)
  (= thr->ndiv tx->ndiv)
  (csym::pthread-mutex-unlock (ptr thr->mut))

  ((aref task-doers tx->task-no) thr tx->body) ; タスク実行
  
  ;; taskの処理完了後は，そのtask-homeにsend-rsltする
  (= rcmd.w RSLT)
  (= rcmd.c 1)                          ; 4でへらした
  (= rcmd.node tx->rslt-to)             ; 外部or内部
  (csym::copy-address (aref rcmd.v 0) tx->rslt-head) ; 送信先アドレス ; 4でへらした
  (csym::send-command (ptr rcmd) tx->body tx->task-no)

  ;; ここでもtreqがたまっていたら noneを送る
  (csym::flush-treq-with-none thr)
  (csym::pthread-mutex-lock (ptr thr->rack-mut))
  ;; w_rack == 0 になるまで止まる必要はなく，タスクを渡さないだけでよい
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
  ;; ★ 「unlockが標準」に変更すべき
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
  (if (< pcmd->c 4)                     ; 4でへらした
      (csym::proto-error "wrong-task" pcmd))
  ;; 外部からのメッセージの場合，コマンドに続くtask本体を受け取る
  ;; （内部からの場合は引数で与えられている）
  (= task-no (aref pcmd->v 3 0))        ; [3]: タスク番号 ; 4でへらした
  (if (== pcmd->node OUTSIDE)           ; 外部からのtaskの場合はここでbodyを受け取る
      (begin
       (= body ((aref task-receivers task-no)))
       (csym::read-to-eol)))
  ;; <task-head>を見て，タスクを実行するスレッドを決める．
  (= id (aref pcmd->v 2 0))             ; 4でへらした
  (if (not (< id num-thrs))
      (csym::proto-error "wrong task-head" pcmd))
  (= thr (+ threads id))                ; thr: taskを実行するスレッド

  ;; スレッドに実行すべきタスクを追加する
  (csym::pthread-mutex-lock (ptr thr->mut))
  (= tx thr->task-top)                  ; tx: thrがやるべき仕事リスト
  (= tx->rslt-to pcmd->node)            ; 結果の送り先種別 [INSIDE|OUTSIDE]
  (csym::copy-address tx->rslt-head (aref pcmd->v 1))
                                        ; [1]: 送り元＝結果の送信先 ; 4でへらした
  (= tx->ndiv (aref pcmd->v 0 0))       ; [0]: 分割回数 ; 4でへらした
  (= tx->task-no task-no)               ; タスク番号
  ;; タスクのパラメータ（task specificな構造体）の受け取り
  ;; ☆ thr->mut が unlock の状態で read すべきではないか?
  ;; ☆ do_task_body 以外は thr引数をなくす?
  (= tx->body body)
  ;; treqしてからtaskを受け取るまでの間はstatしかアクセスされない気がする
  ;; スレッド数の上限を超えるなら，後で(どれかのスレッドが終わるとき)
  ;; signal するべきか?
  ;; それよりは，別にセマフォで上限を管理するほうが簡単
  (= tx->stat TASK-INITIALIZED)         ; そもそもロックが必要なのはここだけかも
  (csym::pthread-mutex-unlock (ptr thr->mut))

  ;; 仕事待ちで眠っているワーカを起こす
  (csym::pthread-cond-signal (ptr thr->cond))
  )

(def (csym::recv-none pcmd) (csym::fn void (ptr (struct cmd)))
  (def thr (ptr (struct thread-data)))
  (def id unsigned-int)
  (def len size-t)
  (if (< pcmd->c 1)                     ; 4でへらした
      (csym::proto-error "Wrong none" pcmd))
  (= id (aref pcmd->v 0 0))             ; 4でへらした
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

;; rack について
;; rslt には <rack-head> はない => <task-head> を使う
;; treq 中なら，rack を返さないとしているが? (sp00b)
;; rack を返したときには，ワーカに結果を取り込ませようと試みる
;; 
;; treq 中に rslt をもらったら，すぐに rack を返さず，
;; その treq は必ず none されるはずだから，none に対応してrack を返す．
;; なぜ?  sp.memo に書いた SP: 以降 のように，FIFO性がないと，treq を
;;        rack が追い越してしまうことがあるから．
;; => とりあえずは，FIFO性を仮定してそのまま
;; ★ ↑treq を rack が抜かすと，treqが取り返しではないタスクを
;;    手にするかもしれない．treq combining なども微妙に影響か?
;;    そもそも，none 以外を受け取るとは想定されていないのでは?
;; ★ rack を treq が抜かす場合は?
;;    rack が着くまでは treq は待たせるか none を返すのみ
;;    -> 取り返しでないタスクが返されることはない．
(def (csym::recv-rslt pcmd body) (csym::fn void (ptr (struct cmd)) (ptr void))
  (def rcmd (struct cmd))               ; rackコマンド
  (def thr (ptr (struct thread-data)))
  (def hx (ptr (struct task-home)))
  (def tid unsigned-int)
  (def sid unsigned-int)
  ;; 引数の数チェック
  (if (< pcmd->c 1)                     ; 4でへらした
      (csym::proto-error "Wrong rslt" pcmd))
  ;; 結果受取人決定 "<thread-id>:<task-home-id>"
  (= tid (aref pcmd->v 0 0))            ; 4でへらした
  (if (not (< tid num-thrs))
      (csym::proto-error "wrong rslt-head" pcmd))
  (= sid (aref pcmd->v 0 1))            ; 4でへらした
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
    ;; ★ thr->mut を持ったままreadすべきではない
    ((aref rslt-receivers hx->task-no) hx->body)
    (csym::read-to-eol))
   ((== pcmd->node INSIDE)
    ;; ★ ここで local-rslt-copy
    (= hx->body body))
   (else
    (csym::proto-error "Wrong cmd.node" pcmd)))
  ;; rackを返す．もっと後のほうがよい？
  (= rcmd.c 1)                          ; 4でへらした
  (= rcmd.node pcmd->node)
  (= rcmd.w RACK)
  (csym::copy-address (aref rcmd.v 0) hx->task-head)
                                        ; 返答先．rsltではなく，もとのtaskコマンドのを覚えている
                                        ; 4でへらした
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
  ;; rackの条件はなぜ必要？
  ;; ->rsltを送った直後に取り返しのtreqが来たときに，仕事を
  ;;   分割してしまわないため
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
                                        ; 4でへらした
       (if (!= pcmd->node OUTSIDE)
           (= hx->req-from INSIDE)
         (= hx->req-from OUTSIDE))
       (= thr->treq-top hx)
       (= thr->req hx)                  ; ☆req は volatile, broadcast 不要
       ))
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (return avail))

;; treq をためておくか問題
;;   自分も treq 中のときは，
;;     none を返すさないとデッドロックの危険．
;;     順序がつけられてループしなければ，またせてもよい．
;;   自分は動いているが渡すものがないとき
;;     none を返すと何度もいってくるかも．
;;     none を返さないと，他にいけばよいのに待つことになるかも．
;;     一度 none で拒否されたら覚えておき，treq 受け付けますと
;;     いう宣伝がくるまではそのノードには treq しないとかは...
;; ★ any に対する none，または 取り戻し(take bask)に対するnone か?
;;   取り戻し(take bask)に対するnoneはまたせればよい．
;;   treq 以外に tbak を設けるとよいかもしれない．メッセージはそのまま
;;   としても，trep_top とは別管理のものとして

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
  ;; task id が指定されている場合（とり返し）と，any の場合がある．
  ;; any の場合は，any でとってきたもののほうが大きいのでは?
  ;; => そもそも，すべての task が動いているわけではない．
  ;;    すくなくとも，rslt まちで，別 task を立ち上げた task に
  ;;    req してもだめ．
  ;; ★ 以下の二行わかりにくい
  ;; * thread 単位で req 先をもつならそれはそれで OK
  ;; * task 単位で req 先をもつなら，
  ;;   + task の req 先を見つけられるかという問題
  ;;   + 今動いているものに req すべき
  ;;     regulation (セマフォ)で，停止中の task に req してももらえないかも
  ;;      -> any だったら対象からはずす．
  ;;         または， 特定のtask なら，一時的に regu をはずす?
  ;;         regu しないならしないで，それは，進まないものが出る．
  ;;         一度，reguをはずしても，task-send後に regu を確認
  ;;      -> ちょっと余分に thread を使っても，タスク分割がその分
  ;;         進みが悪くなるなら，結局，遅延隠蔽にならない可能性が高い．
  ;;         遅延隠蔽のためには，thread が眠っていても task が分割可能
  ;;         だとよい．寝る前に分割しておくとか...
  ;;         -> スレッド数1で先に分割するのはあまり意味はないかも．
  ;;              => もぐらない効果はある?
  ;;            それに先に分割するぐらいなら，スレッド数を倍でよい?
  ;;  ★このあたりは，結局遅延隠蔽のためにスレッドを増やす場合の検討
  ;;     * スレッド数ちょうどの場合も先に分割は選択可
  ;;     * ストリームを作ることができるようにデータを保存(コピー)しておき
  ;;       スティールされなければ自分で処理
  ;;        -> log N 回くらい
  ;;     * この分割データを動かし始めてしまうと，(移動できないので
  ;;       移動できるタスクを作るために) 再分割に「実行」が必要となる．
  ;;       -> スレッドでの遅延隠蔽というのは失敗(よくないアイディア)
  (def rcmd (struct cmd))
  (def id unsigned-int)
  (if (< pcmd->c 2)                     ; 引数の数チェック 0:from, 1:to ; 4でへらした
      (csym::proto-error "Wrong treq" pcmd))
  ;; 仕事を要求するスレッドを決めて，要求を出す
  (if (== (aref pcmd->v 1 0) ANY)       ; 4でへらした
      (let ((myid int) (start-id int) (d int))
        (= myid (aref pcmd->v 0 0))     ; 要求元 ; 4でへらした
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
     (= id (aref pcmd->v 1 0))          ; 4でへらした
     (if (not (< id num-thrs))
         (csym::proto-error "Wrong task-head" pcmd))
     ;; ☆rslt を返した後以外は，取り戻しに対して none は送らず
     ;; またせておけばよいので，要修正
     ;; ↑try-treqが，rack待ちでなければ1を返すようになってるので大丈夫
     (if (csym::try-treq pcmd id (aref pcmd->v 0)) ; treqできた
         (return))))
  
  ;; 内部からのtreq anyに仕事を返せなかった場合
  (if (== pcmd->node ANY)
      (if (== (aref pcmd->v 0 0) 0)     ; v[0]:from ; 4でへらした
          ;; 0番workerからのtreqの場合は外部に問い合わせる
          (begin
           (= pcmd->node OUTSIDE)
           (csym::send-command pcmd 0 0)
           (return))
        ;; それ以外のworkerには単にnoneを返す
        (= pcmd->node INSIDE)))

  ;; noneを返す
  (= rcmd.c 1)                          ; 4でへらした
  (= rcmd.node pcmd->node)              ; [INSIDE|OUTSIDE]
  (= rcmd.w NONE)
  (csym::copy-address (aref rcmd.v 0) (aref pcmd->v 0)) ; 4でへらした
  (csym::send-command (ptr rcmd) 0 0))

;; rack <rack送信先header(ここではthread-id)>
(def (csym::recv-rack pcmd) (csym::fn void (ptr (struct cmd)))
  (def tx (ptr (struct task)))
  (def thr (ptr (struct thread-data)))
  (def id unsigned-int)
  (def len size-t)
  (if (< pcmd->c 1)                     ; 4でへらした
      (csym::proto-error "Wrong rack" pcmd))
  ;; idを<task-head>に含める
  (= id (aref pcmd->v 0 0))             ; 4でへらした
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
  (= tcmd.c 4)                          ; 4でへらした
  (= tcmd.node hx->req-from)
  (= tcmd.w TASK)
  ;; 以下，4でへらした
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
    ;; ★ 以下二行コメントになっている
    ;; -> しばらくrsltがなければ recv-exec-sendでflushするので不要？
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
    ;; ☆ 以下一行コメントになっている
    (= thr->task-top->stat TASK-STARTED)
    (if (== sub->stat TASK-HOME-DONE)
        (break))
    ;; fprintf(stderr, "sub %d\n", sub);
    ;; しばらくまってもあかんかったら仕事をとり返しに行く
    (recv-exec-send thr sub->task-head sub->req-from))
  (= body sub->body)
  (= thr->sub sub->next)                ; サブタスクstackをpop
  (= sub->next thr->treq-free)          ; popした部分を...
  (= thr->treq-free sub)                ; ...フリーリストに返す
  ;; fprintf(stderr, "nsub %d\n", thr->sub);
  (csym::pthread-mutex-unlock (ptr thr->mut))
  (return body))


(%defconstant HOSTNAME-MAXSIZE 256)
(def (struct runtime-option)
  (def num-thrs int)                    ; スレッド数
  (def sv-hostname (array char HOSTNAME-MAXSIZE))
                                        ; Tascellサーバのホスト名．""ならstdout
  (def port unsigned-short)             ; Tascellサーバへの接続ポート番号
  )
(static option (struct runtime-option))

(def (csym::usage argc argv) (csym::fn void int (ptr (ptr char)))
  (csym::fprintf stderr
                 "Usage: %s [-s hostname] [-p port-num] [-n n-threads]~%"
                 (aref argv 0))
  (csym::exit 1))

(def (set-option argc argv) (csym::fn void int (ptr (ptr char)))
  (def i int) (def ch int)
  ;; Default values
  (= (fref option num-thrs) 1)
  (= (aref (fref option sv-hostname) 0) #\NULL)
  (= (fref option port) 8888)
  (while (!= -1 (= ch (csym::getopt argc argv "n:s:p:")))
    (for ((= i 0) (< i argc) (inc i))
      (switch ch
        (case #\n)                      ; number of threads
        (= (fref option num-thrs) (csym::atoi optarg))
        (break)
        
        (case #\s)                      ; server name
        (if (csym::strcmp "stdout" optarg)
            (begin
             (csym::strncpy (fref option sv-hostname) optarg
                            HOSTNAME-MAXSIZE)
             (= (aref (fref option sv-hostname) (- HOSTNAME-MAXSIZE 1)) 0))
          (= (aref (fref option sv-hostname) 0) #\NULL))
        (break)
        
        (case #\p)                      ; connection port number
        (= (fref option port) (csym::atoi optarg))
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
  ;; 同一ノード内（共有メモリ）でもやりくりできるか？
  ;; ☆ 初期化がすべて終わってから fork すべきでは?
  (for ((= i 0) (< i num-thrs) (inc i))
    (let ((thr (ptr (struct thread-data)) (+ threads i)))
      (systhr-create worker thr)))

  ;; 本スレッドはOUTSIDEからのメッセージ処理
  (while 1
    (= pcmd (csym::read-command))
    (csym::proc-cmd pcmd 0))
  (csym::exit 0))
