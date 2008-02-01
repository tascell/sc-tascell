;; worker2から，ppopp08後に改良：dvblをはずす直前のバージョン
;; * 外部へのsendを全ての種類についてsenderスレッドが担当
;;   - 各ワーカはsend-queueに追加
;;   - 内部へのsendについては inms-queueに追加 => 内部メッセージ処理スレッドが担当
;; * いままでばらばらに書いていたロック->キュー追加の処理を統合 
;; * 引数の受け渡しの際の構造体コピーをできるだけポインタ渡しに変更

;; *** 以下未処理の改良すべき点
;; taskがきたらすぐnoneを返す or しばらく待つ or "dvbl"(<- divisible flagのチェックのオーバーヘッド)
;; areq: availability request （仕事を分割できる状態になったら返事をする）．
;; * サーバがnoneを返したノードに対して送る
;;   o noneを送ったらかならずareqがくるのだから送らなくてよい？
;; * 返事"dvbl"がくるまでtreqをそこには送らない
;; * treq_topに相当するareq_top：thread_data->reqにはフラグのみ（<- "req"の名前はかえるべき？）
;; treqとareqでリクエストキューを共有
;; [07/08/13]
;; * rslt_<>_body: とかが引数で取っているthread構造体はいらない
;; * dvblフラグのチェックはとるべき（↑のareqの機構で）
;; * とりもどしのためのtreq（any以外のtreq）に対しては自分が空になるまで
;; （treq anyを送る状況になるまで）noneを返さない
;; 理由：a->b にtreqして仕事がないのは，b->cにtreqしているからである場合が多いので
;;         anyがtreqだったかどうかは受け取った時に覚えておく必要
;; 大林くんがいじる前のversion: ~yasugi/lang/c/loadb/
(%ifndef* NF-TYPE
  (%defconstant NF-TYPE GCC)) ; one of (GCC LW-SC CL-SC XCC XCCCL)
(%include "rule/nestfunc-setrule.sh")

(c-exp "#include<stdio.h>")
(c-exp "#include<stdlib.h>")
(c-exp "#include<pthread.h>")
(c-exp "#include<sys/time.h>")
(c-exp "#include<getopt.h>")
#+tcell-gtk (c-exp "#include<gtk/gtk.h>")
(%include "worker3.sh")

;; (%cinclude "<stdio.h>" "<stdlib.h>" "<pthread.h>" "<sys/time.h>" "<getopt.h>"
;;            (:macro NULL EOF stdin stdout stderr PTHREAD-SCOPE-SYSTEM)
;; ;;            (:var FILE
;; ;;                  printf fprintf fputs fputc getc fflush atoi exit mallloc
;; ;;                  strcmp strstr strcpy strncpy
;; ;;                  pthread-cond-t pthread-condattr-t
;; ;;                  pthread-mutex-t pthread-mutexattr-t
;; ;;                  pthread-t pthread-attr-t
;; ;;                  pthread-create pthread-suspend pthread-cond-wait
;; ;;                  pthread-scope-system
;; ;;                  pthread-mutex-init pthread-cond-init
;; ;;                  suseconds-t time-t gettimeofday
;; ;;                  getopt optarg)
;; ;;            (:aggr timespec timeval timezone)
;;            )
;; (%defconstant NULL csym::NULL)
;; (%defconstant EOF csym::EOF)
;; (%defconstant stdin csym::stdin)
;; (%defconstant stdout csym::stdout)
;; (%defconstant stderr csym::stderr)
;; (%defconstant PTHREAD-SCOPE-SYSTEM csym::PTHREAD-SCOPE-SYSTEM)


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
  (csym::fputs str stderr)
  (csym::fputc #\> stderr)
  (csym::fputc #\Space stderr)
  (for ((= i 0) (< i (- (fref pcmd -> c) 1)) (inc i))
    (csym::fputs (aref (fref pcmd -> v) i) stderr)
    (csym::fputc #\Space stderr))
  (if (> (fref pcmd -> c) 0)
      (begin
       (csym::fputs (aref (fref pcmd -> v)
                          (- (fref pcmd -> c) 1))
                    stderr)
       (csym::fputc #\Newline stderr))))

;; コマンドのキュー．ロック・条件変数付き
(def (struct cmd-queue)
  (def len int)
  (def queue (ptr (struct cmd-list)))   ; queueの配列
  (def arg-buf (ptr char))              ; (* len MAXCMDC ARG-SIZE-MAX) 各引数の文字列保存領域
  (def arg-size-max int)
  (def in (ptr (struct cmd-list)))
  (def out (ptr (struct cmd-list)))
  (def mut pthread-mutex-t)
  (def cond pthread-cond-t))

;; キュー作成・初期化
(def (csym::make-cmd-queue length maxcmdc arg-size-max) (csym::fn (ptr (struct cmd-queue)) int int int)
  (def p-newq (ptr (struct cmd-queue)))
  (defs int i j)
  (= p-newq (cast (ptr (struct cmd-queue))
              (csym::malloc (sizeof (struct cmd-queue)))))
  (= (fref p-newq -> len) length)
  (= (fref p-newq -> queue) (cast (ptr (struct cmd-list))
                              (csym::malloc (* length (sizeof (struct cmd-list))))))
  (= (fref p-newq -> arg-buf)
     (cast (ptr char) (csym::malloc (* length maxcmdc arg-size-max (sizeof char)))))
  (= (fref p-newq -> arg-size-max) arg-size-max)
  (= (fref p-newq -> in) (fref p-newq -> queue))
  (= (fref p-newq -> out) (fref p-newq -> queue))
  (csym::pthread-mutex-init (ptr (fref p-newq -> mut)) 0)
  (csym::pthread-cond-init (ptr (fref p-newq -> cond)) 0)
  ;; キューの中身の初期化
  (for ((= i 0) (< i length) (inc i))
    ;; リンク
    (= (fref (aref (fref p-newq -> queue) i) next)
       (ptr (aref (fref p-newq -> queue) (% (+ i 1) length))))
    ;; queue[i].cmd.v[j] にバッファの適切な位置へのポインタ
    ;; （i番目のコマンド文字列のj番目の引数の文字列）
    (for ((= j 0) (< j maxcmdc) (inc j))
      (= (aref (fref (aref (fref p-newq -> queue) i) cmd v) j)
         (+ (fref p-newq -> arg-buf)
            (* maxcmdc arg-size-max i)
            (* arg-size-max j)))))
  (return p-newq))

;; queueへのメッセージ追加（構造体および文字列のコピーを伴う）
(def (csym::enqueue-command pcmd body task-no pq)
    (csym::fn void (ptr (struct cmd)) (ptr void) int (ptr (struct cmd-queue)))
  (def i int)
  (def len size-t)
  (def q (ptr (struct cmd-list)))
  (def len-max int (fref pq -> arg-size-max))
  (csym::pthread-mutex-lock (ptr (fref pq -> mut)))
  (= q (fref pq -> in))
  (if (== (fref q -> next) (fref pq -> out))
      (begin
       (csym::perror "queue overflow~%")
       (csym::exit 1)))
  (= (fref pq -> in) (fref q -> next))
  ;; cmdのコピー
  (= (fref q -> cmd c) (fref pcmd -> c))
  (= (fref q -> cmd node) (fref pcmd -> node))
  (for ((= i 0) (< i (fref pcmd -> c)) (inc i))
    (= len (csym::strlen (aref (fref pcmd -> v) i)))
    (if (>= len len-max)
        (csym::proto-error "too long cmd" pcmd))
    (csym::strncpy (aref (fref q -> cmd v) i)
                   (aref (fref pcmd -> v) i)
                   (+ len 1)))
  (= (fref q -> body) body)             ; task, rsltのbody
  (= (fref q -> task-no) task-no)       ; task番号
  (csym::pthread-mutex-unlock (ptr (fref pq -> mut)))
  ;; 追加したことを通知
  (csym::pthread-cond-signal (ptr (fref pq -> cond)))
  )

;; 内部受信キュー
(def inms-queue (ptr (struct cmd-queue)))
;; 外部への送信依頼キュー
(def send-queue (ptr (struct cmd-queue)))

;; 今度do-two等に来たら"dvbl"を送ろうフラグ
(def divisibility-flag int 1)           ; ★消してしまいたい

;; 外部からのメッセージ受信バッファ
(def buf (array char BUFSIZE))
(def cmd-buf (struct cmd))

;; サーバへの送受信ソケット(<0:stdin/out)．mainで設定
(def sv-socket int)

;;; fgetsでOUTSIDEからのメッセージを受け取る -> struct cmd
;;; 1行で読み込んだ後，スペースをNULL文字に置きかえることで文字列を分割する．
;;; また，cmd.v[]が分割された各文字列を指すようにする
;;;   fgets は 0-terminated buffer を返す．
;;;   fgets returns 0 のときの対処必要?
;;; buf, cmd-buf は一個なので注意（今はmainの本スレッドしか触らない）
(def (csym::recv-command) (csym::fn (ptr (struct cmd)))
  (defs char p c)
  (def b (ptr char) buf)
  (def cmdc int)

  (csym::receive-line b BUFSIZE sv-socket) ;; (csym::fgets b BUFSIZE stdin)
  (= (fref cmd-buf node) OUTSIDE)
  ;; p:一個前の文字，c:現在の文字
  (= cmdc 0)
  (for ((exps (= p #\NULL) (= c (mref b)))
        c
        (exps (= p c)      (= c (mref (++ b)))))
    (if (or (== c #\Space) (== c #\Newline))
        (begin (= c #\NULL)
               (= (mref b) #\NULL))
      (if (and (== p #\NULL) (< cmdc MAXCMDC))
          (= (aref (fref cmd-buf v) (inc cmdc)) b))))
  (= (fref cmd-buf c) cmdc)
  (return (ptr cmd-buf)))

;;; struct cmd -> output（stdoutへ）
;;; senderスレッドのsender-loopのみから呼ばれる
(def (csym::send-out-command pcmd) (csym::fn void (ptr (struct cmd)))
  (def i int)
  (def narg int (fref pcmd -> c))
  (for ((= i 0) (< i (- narg 1)) (inc i))
    (csym::send-string (aref (fref pcmd -> v) i) sv-socket)
    (csym::send-char #\Space sv-socket))
  (if (> narg 0)
      (begin
       (csym::send-string (aref (fref pcmd -> v) (- narg 1)) sv-socket)
       (csym::send-char #\Newline sv-socket))))

;;; ノード内/外ワーカにコマンドを送信
;;; bodyはtask, rsltの本体．それ以外のコマンドではNULLに
(def (csym::send-command pcmd body task-no) (csym::fn void (ptr (struct cmd)) (ptr void) int)
  (if (== (fref pcmd -> node) OUTSIDE)
      (csym::enqueue-command pcmd body task-no send-queue) ; 外部に送信
    (csym::enqueue-command pcmd body task-no inms-queue) ; 内部に送信
    ))

;; ノード内メッセージ用queue にたまったメッセージ処理
;; 内部メッセージ処理スレッドはここをぐるぐる
(def (exec-queue-cmd arg) (fn (ptr void) (ptr void))
  (def pcmd (ptr (struct cmd)))
  (def body (ptr void))
  (def p-mut (ptr pthread-mutex-t) (ptr (fref inms-queue -> mut)))
  (def p-cond (ptr pthread-cond-t) (ptr (fref inms-queue -> cond)))
  (loop
    ;; ・内容が書き潰されないようにキューからの削除はあとで
    ;; ・追加中のcmdの内容を参照しないようにロックは必要
    ;; ・取り出しが一人だけなのでrecv-*中のロックは不要だろう
    (csym::pthread-mutex-lock p-mut)
    (while (== (fref inms-queue -> in)
               (fref inms-queue -> out))
      (csym::pthread-cond-wait p-cond p-mut))
    (= pcmd (ptr (fref inms-queue -> out -> cmd)))
    (= body (fref inms-queue -> out -> body))
    (csym::pthread-mutex-unlock p-mut)
    ;; (csym::proto-error "INSIDE" pcmd)   ; debug print
    (cond
     ((== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "task"))
      (csym::proto-error "INSIDE" pcmd) ; debug print
      (csym::recv-task pcmd body))
     ((== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "rslt"))
      (csym::proto-error "INSIDE" pcmd) ; debug print
      (csym::recv-rslt pcmd body))
     ((== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "treq"))
      (csym::recv-treq pcmd))
     ((== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "none"))
      (csym::recv-none pcmd))
     ((== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "rack"))
      (csym::recv-rack pcmd))
     (else
      (csym::proto-error "wrong cmd" pcmd)
      (csym::exit 0)))
    (csym::pthread-mutex-lock p-mut)
    (= (fref inms-queue -> out)   ; dequeue
       (fref inms-queue -> out -> next))
    (csym::pthread-mutex-unlock p-mut)
    ))

;; EOLまでを無視
(def (csym::read-to-eol) (csym::fn void void)
  (def c int)
  (while (!= EOF (= c (csym::receive_char sv-socket)))
                                        ;(!= EOF (= c (csym::getc stdin)))
    (if (== c #\Newline) (break))))

(def (csym::write-eol) (csym::fn void void)
  (csym::send-char #\Newline sv-socket))

(def (csym::flush-send) (csym::fn void void)
  (csym::fflush stdout))

;; senderスレッドはここをぐるぐる
;; 出力ストリームをロックしていないので注意
;; （senderスレッドしか使わないと仮定）
(def (sender-loop arg) (fn (ptr void) (ptr void))
  (def pcmd (ptr (struct cmd)))
  (def p-mut (ptr pthread-mutex-t) (ptr (fref send-queue -> mut)))
  (def p-cond (ptr pthread-cond-t) (ptr (fref send-queue -> cond)))
  (def body (ptr void))
  (loop
    (csym::pthread-mutex-lock p-mut)
    (while (== (fref send-queue -> in)
               (fref send-queue -> out))
      ;; キューが空なら待つ
      (csym::pthread-cond-wait p-cond p-mut))
    (= pcmd (ptr (fref send-queue -> out -> cmd)))
    (csym::pthread-mutex-unlock p-mut)
    (csym::send-out-command pcmd)
    (= body (fref send-queue -> out -> body))
    (if body
        (begin
         (cond
          ((== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "task"))
           ((aref task-senders (fref send-queue -> out -> task-no)) body)
           (csym::write-eol))
          ((== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "rslt"))
           ((aref rslt-senders (fref send-queue -> out -> task-no)) body)
           (csym::write-eol)))
         (= (fref send-queue -> out -> body) 0)))
    (csym::flush-send)
    (if (== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "none"))
        (= divisibility-flag 1))        ; noneを送ったらdvblフラグをon
    (csym::pthread-mutex-lock p-mut)
    (= (fref send-queue -> out)
       (fref send-queue -> out -> next)) ; キューから追い出す
    (csym::pthread-mutex-unlock p-mut)
    (csym::flush-send)
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

;;; treq <task-head> <treq-head>
;;; <task-head>  タスク返答先
;;; <treq-head>  要求送信先

;;; task <ndiv> <rslt-head> <task-head>
;;; <ndiv>       分割回数 負荷のサイズの目安 (sp2が子の判断に使う)
;;; <rslt-head>  結果返答先
;;; <task-head>  タスク送信先

;;; rslt <rslt-head>
;;; <rslt-head>  結果送信先

;;; rack <task-head>
;;; <task-head>  rack送信先
;;; (w-rack カウンタを使うべき)

;;; none <task-head>
;;; <task-head>  (no)タスク送信先

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
  (= (fref rcmd c) 2)
  (= (aref (fref rcmd v) 0) "none")
  (while (= hx (fref thr -> treq-top))
    (= (fref rcmd node) (fref hx -> req-from))
    (= (aref (fref rcmd v) 1) (fref hx -> task-head))
    (csym::send-command (ptr rcmd) 0 0)
    (= (fref thr -> treq-top) (fref hx -> next))  ; treqスタックをpop
    (= (fref hx -> next) (fref thr -> treq-free)) ; フリーリストに...
    (= (fref thr -> treq-free) hx))               ; ...領域を返却
  )

;;; worker or wait-rslt から
;;; （後者は外に投げたタスクの結果待ち中に別の仕事をやろうとする時）
;;; タスク要求 -> 受け取り -> 計算 -> 結果送信
;;; thr->mut はロック済
;;; (treq-head,req-to): どこにタスク要求を出すか（any or 取り返し）
(def (recv-exec-send thr treq-head req-to)
    (fn void (ptr (struct thread-data)) (ptr char) int)
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
  (while (> (fref thr -> w-none) 0)
    (csym::pthread-cond-wait (ptr (fref thr -> cond))
                             (ptr (fref thr -> mut)))
    ;; rsltの到着チェック
    (if (and (fref thr -> sub)
             (== (fref thr -> sub -> stat) TASK-HOME-DONE))
        (return)))

  ;; allocate
  (= tx (fref thr -> task-free))
  (= (fref tx -> stat) TASK-ALLOCATED)
  (if (not tx)
      (csym::mem-error "Not enough task memory"))
  (= (fref thr -> task-top) tx)
  (= (fref thr -> task-free) (fref tx -> prev))

  (= delay (* 2 1000 1000))

  ;; treqコマンド
  (= (fref rcmd c) 3)
  (if (> num-thrs 1)
      (= (fref rcmd node) req-to)
    (= (fref rcmd node) OUTSIDE))
  (= (aref (fref rcmd v) 0) "treq")
  (= (aref (fref rcmd v) 1) (fref thr -> id-str))
  (= (aref (fref rcmd v) 2) treq-head)
  (do-while (!= (fref tx -> stat) TASK-INITIALIZED)
    ;; 最初にtreqがたまっていたら，noneを送る
    ;; これをやると treq が飛び交うかもしれないが，
    ;; none を送らないとだけすると，
    ;; 互いに none or task が送られているのを待つ
    ;; というタイプのデッドロックとなる．
    (csym::flush-treq-with-none thr)
    (= (fref tx -> stat) TASK-ALLOCATED) ; *-NONEにされてるかもしれないのを元に戻す
    ;; ☆ ここで，send_mut をとる前に，thr->mut を unlock すべきでは?
    (begin
     (csym::pthread-mutex-unlock (ptr (fref thr -> mut)))
     (csym::send-command (ptr rcmd) 0 0) ; treq送信
     (csym::pthread-mutex-lock (ptr (fref thr -> mut))))
    ;; recv-task が *tx を初期化するのを待つ
    (loop
      ;; rslt が到着していたら，特別に先にさせる
      (if (and (!= (fref tx -> stat) TASK-INITIALIZED)
               (fref thr -> sub)
               (== (fref thr -> sub -> stat) TASK-HOME-DONE))
          ;; rslt が到着するような場合，ここでのtreqは取戻しであり，
          ;; その取戻しは失敗して，noneが返されるはずである．
          ;; すでに none が返されているかで，待ち数のカウンタを
          ;; 増やすかどうかが異なる
          ;; ★ ↓のほうにある，rack のタイミングによっては
          ;; この前提が崩れる可能性あり．前提が崩れた場合は，
          ;; w_none は使えないので，none か task が返されるのをこの場で
          ;; 待ち，task だった場合は，以下の do_task_body のほうに進むのでは?
          (begin
           (if (!= (fref tx -> stat) TASK-NONE)
               (inc (fref thr -> w-none)))
           (goto Lnone)))
      (if (!= (fref tx -> stat) TASK-ALLOCATED)
          (break))
      (csym::pthread-cond-wait (ptr (fref thr -> cond))
                               (ptr (fref thr -> mut)))
      )
    ;; none を受け取ったときに，しばらく待つようにしてみてある．
    ;; 何か他の手は使えないか? (同時複数個所待ち相当の何かで)
    ;; ★ すくなくともリクエスト先が any でないなら，
    ;;    rslt とセットのnone を返す場合以外は，
    ;;    noneを返す必要はなく待たせておけばよい．
    ;; ★ any の場合，treq combining とか，複数個所待ちとか
    ;;    ここでは，delay を倍々としていっている．2ms からはじめて
    (if (== (fref tx -> stat) TASK-NONE)
        (let ((t1 (struct timespec))
              (now (struct timeval))
              (nsec long))
          (csym::gettimeofday (ptr now) 0)
          (= nsec (* (fref now tv-usec) 1000))
          ;; nsec += 10 * 1000 * 1000
          (+= nsec delay)
          (+= delay delay)
          (if (> delay (* 40 1000 1000))
              (= delay (* 40 1000 1000)))
          (= (fref t1 tv-nsec)
             (if-exp (> nsec 999999999)
                     (- nsec 999999999)
                     nsec))
          (= (fref t1 tv-sec)
             (+ (fref now tv-sec)
                (if-exp (> nsec 999999999) 1 0)))
          (csym::pthread-cond-timedwait
           (ptr (fref thr -> cond-r))
           (ptr (fref thr -> mut))
           (ptr t1))
          ;; rsltが到着していたら
          (if (and (fref thr -> sub)
                   (== (fref thr -> sub -> stat)
                       TASK-HOME-DONE))
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
  (= (fref tx -> stat) TASK-STARTED)
  (= old-ndiv (fref thr -> ndiv))
  (= (fref thr -> ndiv) (fref tx -> ndiv))
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut)))
  ((aref task-doers (fref tx -> task-no)) thr (fref tx -> body))
  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))

  ;; taskの処理完了後は，そのtask-homeにsend-rsltする
  (= (fref rcmd c) 2)
  (= (fref rcmd node) (fref tx -> rslt-to))
  (= (aref (fref rcmd v) 0) "rslt")
  (= (aref (fref rcmd v) 1) (fref tx -> rslt-head))
  ;; ☆ 両方lockはだめなのでは？
  (csym::send-command (ptr rcmd) (fref tx -> body) (fref tx -> task-no))

  ;; ここでもtreqがたまっていたら noneを送る
  ;; ☆両方lock はだめなのでは?
  (csym::flush-treq-with-none thr)
  (csym::pthread-mutex-lock (ptr (fref thr -> rack-mut)))
  ;; w_rack == 0 になるまで止まる必要はなく，タスクを渡さないだけでよい
  (inc (fref (mref thr) w-rack))
  (csym::pthread-mutex-unlock (ptr (fref thr -> rack-mut)))
  (= (fref thr -> ndiv) old-ndiv)
  ;; タスクstackをpopしてフリーリストに返す
  (label Lnone
         (begin
          (= (fref thr -> task-free) tx)
          (= (fref thr -> task-top) (fref tx -> next))))
  )

(def (worker arg) (fn (ptr void) (ptr void))
  (def thr (ptr (struct thread-data)) arg)
  ;; ★ 「unlockが標準」に変更すべき
  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
  (loop
    (recv-exec-send thr "any" ANY))
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut))))

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
  (if (< (fref pcmd -> c) 5)
      (csym::proto-error "wrong-task" pcmd))
  ;; 外部からのメッセージの場合，コマンドに続くtask本体を受け取る
  ;; （内部からの場合は引数で与えられている）
  (= task-no (csym::atoi (aref (fref pcmd -> v) 4))) ; [4]: タスク番号
  (if (== (fref pcmd -> node) OUTSIDE)
      (begin
       (= body ((aref task-receivers task-no)))
       (csym::read-to-eol)))
  ;; <task-head>を見て，タスクを実行するスレッドを決める．
  (= id (csym::atoi (aref (fref pcmd -> v) 3)))
  (if (not (< id num-thrs))
      (csym::proto-error "wrong task-head" pcmd))
  (= thr (+ threads id))                ; thr: taskを実行するスレッド

  ;; スレッドに実行すべきタスクを追加する
  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
  (= tx (fref thr -> task-top))         ; tx: thrがやるべき仕事リスト
  (= (fref tx -> rslt-to) (fref pcmd -> node)) ; 結果の送り先種別
  (= len (csym::strlen (aref (fref pcmd -> v) 2))) ; [2]: 送り元（=結果の送信先）
  (if (>= len ARG-SIZE-MAX)
      (csym::proto-error "Too long rslt-head for task" pcmd))
  (csym::strncpy (fref tx -> rslt-head)
                 (aref (fref pcmd -> v) 2)
                 (+ len 1))
  (= (fref tx -> ndiv) (csym::atoi (aref (fref pcmd -> v) 1))) ; [1]: 分割回数
  (= (fref tx -> task-no) task-no) ; タスク番号
  ;; タスクのパラメータ（task specificな構造体）の受け取り
  ;; ☆ thr->mut が unlock の状態で read すべきではないか?
  ;; ☆ do_task_body 以外は thr引数をなくす?
  (= (fref tx -> body) body)
  ;; treqしてからtaskを受け取るまでの間はstatしかアクセスされない気がする
  ;; スレッド数の上限を超えるなら，後で(どれかのスレッドが終わるとき)
  ;; signal するべきか?
  ;; それよりは，別にセマフォで上限を管理するほうが簡単
  (= (fref tx -> stat) TASK-INITIALIZED) ; そもそもロックが必要なのはここだけかも
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut))) 

  ;; 仕事待ちで眠っているワーカを起こす
  (csym::pthread-cond-signal (ptr (fref thr -> cond)))
  )

(def (csym::recv-none pcmd) (csym::fn void (ptr (struct cmd)))
  (def thr (ptr (struct thread-data)))
  (def id unsigned-int)
  (def len size-t)
  (if (< (fref pcmd -> c) 2)
      (csym::proto-error "Wrong none" pcmd))
  (= id (csym::atoi (aref (fref pcmd -> v) 1)))
  (if (not (< id num-thrs))
      (csym::proto-error "Wrong task-head" pcmd))
  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
  (if (> (fref thr -> w-none) 0)
      (dec (fref thr -> w-none))
    (= (fref thr -> task-top -> stat) TASK-NONE))
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut)))
  (csym::pthread-cond-signal (ptr (fref thr -> cond)))
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
  (def b (ptr char))
  (def h-buf (array char ARG-SIZE-MAX))
  ;; 引数の数チェック
  (if (< (fref pcmd -> c) 2)
      (csym::proto-error "Wrong rslt" pcmd))
  ;; 結果受取人決定 "<thread-id>:<task-home-id>"
  (= b (aref (fref pcmd -> v) 1))
  (= tid (csym::atoi b))
  (if (not (< tid num-thrs))
      (csym::proto-error "wrong rslt-head" pcmd))
  (= b (csym::strchr b #\:))
  (if (not b)
      (csym::proto-error "Wrong rslt-head" pcmd))
  (= sid (csym::atoi (+ b 1)))
  (= thr (+ threads tid))

  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
  ;; hx = 返ってきたrsltを待っていたtask-home（.id==sid）を探す
  (= hx (fref thr -> sub))
  (while (and hx (!= (fref hx -> id) sid))
    (= hx (fref hx -> next)))
  (if (not hx)
      (csym::proto-error "Wrong rslt-head" pcmd))
  ;; 外部からのメッセージの場合，コマンドに続くrslt本体を受け取る
  ;; （内部からの場合は引数で与えられている）
  (cond
   ((== (fref pcmd -> node) OUTSIDE)
    ;; ★ thr->mut を持ったままreadすべきではない
    ((aref rslt-receivers (fref hx -> task-no)) (fref hx -> body))
    (csym::read-to-eol))
   ((== (fref pcmd -> node) INSIDE)
    (= (fref hx -> body) body))
   (else
    (csym::proto-error "Wrong cmd.node" pcmd)))
  ;; rackを返す．もっと後のほうがよい？
  (= (fref rcmd c) 2)
  (= (fref rcmd node) (fref pcmd -> node))
  (= (aref (fref rcmd v) 0) "rack")
  (csym::strncpy                        ; 返答先．rsltではなく，もとのtaskコマンドのを覚えている
   h-buf (fref hx -> task-head) ARG-SIZE-MAX)
  (= (aref (fref rcmd v) 1) h-buf)
  ;; hx 中に記録された task-head に rack を後で送るなら，
  ;; ここではないが，まだ free されたくないので，つなぎなおすかも
  (= (fref hx -> stat) TASK-HOME-DONE)
  (if (== hx (fref thr -> sub))
      (begin
       (csym::pthread-cond-signal (ptr (fref thr -> cond-r)))
       (csym::pthread-cond-signal (ptr (fref thr -> cond))))
    )
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut)))

  (csym::send-command (ptr rcmd) 0 0))

;; worker がしばらく受け取ってくれなくても，
;; treq は受け取っておくほうがよさそう．
;; 複数の treq をリストにして受け取らせてもよいのでは?
;; treq は task-home の形にしておいてもよいかも．
;; task-home は スタックアロケートのつもりだったが...
;; ★ ↑この形にすでになっている
;; ★ treq 以外にすぐにnone を返さず(結局noneは返さず)，available になったら
;;   それを返すような task query というか availability requenst (areq) はどうか?
;;   返事は able とか．aval とか．
;; ★ これを最適化すると大林版の dvbl になるはずだが，dvbl が最適なのかは不明

;;; threads[id] にtreqを試みる
(def (csym::try-treq pcmd id) (csym::fn int (ptr (struct cmd)) unsigned-int)
  (def hx (ptr (struct task-home)))
  (def thr (ptr (struct thread-data)))
  (def len size-t)
  (def avail int 0)

  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
  (csym::pthread-mutex-lock (ptr (fref thr -> rack-mut)))
  ;; workerが仕事をしていて，rack待ちでもなければtreq可能とみなす
  ;; rackの条件はなぜ必要？
  ;; ->rsltを送った直後に取り返しのtreqが来たときに，仕事を
  ;;   分割してしまわないため
  (if (and (fref thr -> task-top)
           (or (== (fref thr -> task-top -> stat) TASK-STARTED)
               (== (fref thr -> task-top -> stat) TASK-INITIALIZED))
           (== (fref thr -> w-rack) 0))
      (= avail 1))
  (csym::pthread-mutex-unlock (ptr (fref thr -> rack-mut)))

  ;; 成功ならtask-home スタックにpush
  (if avail
      (begin
       (= hx (fref thr -> treq-free))
       (if (not hx)
           (csym::mem-error "Not enough task-home memory"))
       (= (fref thr -> treq-free) (fref hx -> next)) ; フリーリストから領域を確保
       (= (fref hx -> next) (fref thr -> treq-top))  ; これよりnextはスタックのリンク
       (= (fref hx -> stat) TASK-HOME-ALLOCATED)
       (= len (csym::strlen (aref (fref pcmd -> v) 1))) ; v[1]: 返答先
       (if (>= len ARG-SIZE-MAX)
           (csym::proto-error "Too long task-head for treq" pcmd))
       (csym::strncpy (fref hx -> task-head)
                      (aref (fref pcmd -> v) 1)
                      (+ len 1))
       (if (!= (fref pcmd -> node) OUTSIDE)
           (= (fref hx -> req-from) INSIDE)
         (= (fref hx -> req-from) OUTSIDE))
       (= (fref thr -> treq-top) hx)
       (= (fref thr -> req) hx) ; ☆req は volatile, broadcast 不要
       ))
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut)))
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
;; ★ 宣伝 (able等) は上で追加した areq とかで...
;; ★ any に対する none，または 取り戻し(take bask)に対するnone か?
;;   取り戻し(take bask)に対するnoneはまたせればよい．
;;   treq 以外に tbak を設けるとよいかもしれない．メッセージはそのまま
;;   としても，trep_top とは別管理のものとして

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
  (if (< (fref pcmd -> c) 3)            ; 引数の数チェック 0:"treq", 1:from, 2:to
      (csym::proto-error "Wrong treq" pcmd))
  ;; 仕事を要求するスレッドを決めて，要求を出す
  (if (== (csym::strcmp (aref (fref pcmd -> v) 2) "any") 0)
      (let ((myid int))
        (= myid (csym::atoi (aref (fref pcmd -> v) 1)))
        (for ((= id 0) (< id num-thrs) (inc id))
          (if (and (!= (fref pcmd -> node) OUTSIDE)
                   (== id myid))
              (continue))               ; 自分自身には要求を出さない
          (if (csym::try-treq pcmd id) (break)))
        (if (!= id num-thrs)            ; treqできた
            (return)))
    (begin                              ; "any"でない場合（取り返し）
     (= id (csym::atoi (aref (fref pcmd -> v) 2)))
     (if (not (< id num-thrs))
         (csym::proto-error "Wrong task-head" pcmd))
     ;; ☆rslt を返した後以外は，取り戻しに対して none は送らず
     ;; またせておけばよいので，要修正
     ;; ↑try-treqが，rack待ちでなければ1を返すようになってるので大丈夫
     (if (csym::try-treq pcmd id)        ; treqできた
         (return))))
  
  ;; 内部からのtreq anyに仕事を返せなかった場合
  (if (== (fref pcmd -> node) ANY)
      (if (== (csym::atoi (aref (fref pcmd -> v) 1)) 0) ; v[1]:from
          ;; 0番workerからのtreqの場合は外部に問い合わせる
          (begin
           (= (fref pcmd -> node) OUTSIDE)
           (csym::send-command pcmd 0 0)
           (= divisibility-flag 1)      ; ★ 消してしまいたい
           (return))
        ;; それ以外のworkerには単にnoneを返す
        (= (fref pcmd -> node) INSIDE)))

  ;; noneを返す
  ;; ☆ receiverスレッドが自分で none を送っているのはよくない．
  ;;    適当なスレッドに任せるべきでは?
  (= (fref rcmd c) 2)
  (= (fref rcmd node) (fref pcmd -> node))
  (= (aref (fref rcmd v) 0) "none")
  (= (aref (fref rcmd v) 1) (aref (fref pcmd -> v) 1))
  (csym::send-command (ptr rcmd) 0 0))

;; rack <rack送信先header(ここではthread-id)>
(def (csym::recv-rack pcmd) (csym::fn void (ptr (struct cmd)))
  (def tx (ptr (struct task)))
  (def thr (ptr (struct thread-data)))
  (def id unsigned-int)
  (def len size-t)
  (if (< (fref pcmd -> c) 2)
      (csym::proto-error "Wrong rack" pcmd))
  ;; idを<task-head>に含める
  (= id (csym::atoi (aref (fref pcmd -> v) 1)))
  (if (not (< id num-thrs))
      (csym::proto-error "Wrong task-head" pcmd))
  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr (fref thr -> rack-mut)))
  (dec (fref thr -> w-rack))
  (csym::pthread-mutex-unlock (ptr (fref thr -> rack-mut))))

(def (csym::send-divisible) (csym::fn void void)
  (def cmd (struct cmd))
  ;;   (if (csym::pthread-mutex-trylock (ptr send-mut))
  ;;       (return))
  (= divisibility-flag 0) ; ★ 消してしまいたい
  (= (fref cmd c) 1)
  (= (fref cmd node) OUTSIDE)
  (= (aref (fref cmd v) 0) "dvbl")
  (csym::send-command (ptr cmd) 0 0))

;; ワーカスレッドが仕事分割開始時に呼ぶ
(def (handle-req -bk -thr)
    (fn void (ptr (NESTFN int void)) (ptr (struct thread-data)))
  (csym::pthread-mutex-lock (ptr (fref -thr -> mut)))
  (if (fref -thr -> req)
      (begin
       (-bk)
       (= (fref -thr -> req) (fref -thr -> treq-top))))
  (csym::pthread-mutex-unlock (ptr (fref -thr -> mut))))

;; ワーカスレッドがput時に呼ぶ
;; -thr->mut ロック済み
(def (csym::make-and-send-task thr task-no body) ; task-noをtcell追加
    (csym::fn void (ptr (struct thread-data)) int (ptr void))
  (def tcmd (struct cmd))
  (def hx (ptr (struct task-home)) (fref thr -> treq-top))
  (= (fref thr -> treq-top) (fref hx -> next)) ; task要求スタックをpop
  (= (fref hx -> next) (fref thr -> sub)) ; これよりサブタスクstackのリンク
  (= (fref thr -> sub) hx)              ; サブタスクstackにpush
  (= (fref hx -> task-no) task-no)
  (= (fref hx -> body) body)
  (= (fref hx -> id) (if-exp (fref hx -> next) ; サブタスクID = 底から何番目か
                             (+ (fref hx -> next -> id) 1)
                             0))
  (= (fref hx -> stat) TASK-HOME-INITIALIZED)
  (= (fref tcmd c) 5)
  (= (fref tcmd node) (fref hx -> req-from))
  (= (aref (fref tcmd v) 0) "task")
  ;; ズルなので注意
  ;; ★ ↑スペースで区切っているので，数が一つすくなくずれている点と
  ;;    sprintf で バッファあふれ未確認だから? 
  (csym::sprintf (fref thr -> ndiv-buf) "%d"
                 (++ (fref thr -> ndiv)))
  (csym::sprintf (fref thr -> buf) "%s:%d"
                 (fref thr -> id-str) (fref hx -> id))
  (csym::sprintf (fref thr -> tno-buf) "%d" task-no)
  (= (aref (fref tcmd v) 1) (fref thr -> ndiv-buf)) ; 分割回数
  (= (aref (fref tcmd v) 2) (fref thr -> buf)) ; 結果返答先 "<worker-id>:<subtask-id>"
  (= (aref (fref tcmd v) 3) (fref hx -> task-head)) ; タスク送信先（try-treqで準備済）
  (= (aref (fref tcmd v) 4) (fref thr -> tno-buf)) ; タスク番号
  (csym::send-command (ptr tcmd) body task-no))

;; ワーカスレッドがサブタスクの結果待ち時に呼ぶ
(def (wait-rslt thr) (fn (ptr void) (ptr (struct thread-data)))
  (def body (ptr void))
  (def t1 (struct timespec))
  (def now (struct timeval))
  (def nsec long)
  (def sub (ptr (struct task-home)))
  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
  (= sub (fref thr -> sub))
  (while (!= (fref sub -> stat) TASK-HOME-DONE)
    ;; ★ 以下二行コメントになっている
    ;; -> しばらくrsltがなければ recv-exec-sendでflushするので不要？
    ;; (csym::flush-treq-with-none thr)
    ;; (= (fref thr -> task-top -> stat) TASK-SUSPENDED)
    (csym::gettimeofday (ptr now) 0)
    (= nsec (* 1000 (fref now tv-usec)))
    (+= nsec (* 5 1000 1000))          ; ちょっと待っている（ここの数字が待ち時間）
    ;; (+= nsec (* 10 1000 1000))
    (= (fref t1 tv-nsec) (if-exp (> nsec 999999999)
                                 (- nsec 999999999)
                                 nsec))
    (= (fref t1 tv-sec) (+ (fref now tv-sec)
                           (if-exp (> nsec 999999999) 1 0)))
    (csym::pthread-cond-timedwait
     (ptr (fref thr -> cond-r)) (ptr (fref thr -> mut))
     (ptr t1))
    ;; ☆ 以下一行コメントになっている
    (= (fref thr -> task-top -> stat) TASK-STARTED)
    (if (== (fref sub -> stat) TASK-HOME-DONE)
        (break))
    ;; fprintf(stderr, "sub %d\n", sub);
    ;; しばらくまってもあかんかったら仕事をとり返しに行く
    (recv-exec-send
     thr (fref sub -> task-head) (fref sub -> req-from)))
  (= body (fref sub -> body))
  (= (fref thr -> sub) (fref sub -> next))       ; サブタスクstaskをpop
  (= (fref sub -> next) (fref thr -> treq-free)) ; popした部分を...
  (= (fref thr -> treq-free) sub)                ; ...フリーリストに返す
  ;; fprintf(stderr, "nsub %d\n", thr->sub);
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut)))
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
  (def dummy (ptr void))
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

  ;; inms-queue（内部通信メッセージキュー） の初期化
  (= inms-queue (csym::make-cmd-queue CMD-QUEUE-LENGTH MAXCMDC ARG-SIZE-MAX))
  (systhr-create exec-queue-cmd NULL)   ; 内部メッセージ処理スレッド
  ;; send-queue（外部送信キュー）の初期化
  (= send-queue (csym::make-cmd-queue CMD-QUEUE-LENGTH MAXCMDC ARG-SIZE-MAX))
  (systhr-create sender-loop NULL)   ; 送信スレッド
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
      (csym::pthread-mutex-init (ptr (fref thr -> mut)) 0)
      (csym::pthread-mutex-init (ptr (fref thr -> rack-mut)) 0)
      (csym::pthread-cond-init (ptr (fref thr -> cond)) 0)
      (csym::pthread-cond-init (ptr (fref thr -> cond-r)) 0)
      (csym::sprintf (fref thr -> id-str) "%d" i)

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
  (= pcmd (csym::recv-command))
  (while (and (> (fref pcmd -> c) 0)
              (!= 0 (csym::strcmp
                     (aref (fref pcmd -> v) 0) "exit")))
    (cond 
     ((== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "task"))
      (csym::recv-task pcmd dummy))
     ((== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "rslt"))
      (csym::recv-rslt pcmd dummy))
     ((== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "treq"))
      (csym::recv-treq pcmd))
     ((== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "none"))
      (csym::recv-none pcmd))
     ((== 0 (csym::strcmp (aref (fref pcmd -> v) 0) "rack"))
      (csym::recv-rack pcmd))
     (else
      (csym::proto-error "wrong cmd" pcmd)))
    (= pcmd (csym::recv-command))
    )
  (csym::exit 0))
