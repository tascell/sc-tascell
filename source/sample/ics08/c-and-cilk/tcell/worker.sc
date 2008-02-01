(c-exp "#include<stdio.h>")
(c-exp "#include<pthread.h>")
(%include "worker.sh")

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

(def (csym::systhr-create start-func arg)
     (fn int (ptr (fn (ptr void) (ptr void))) (ptr void))
  (def status int 0)
  (def tid pthread-t)
  (def attr pthread-attr-t)

  (csym::pthread-attr-init (ptr attr))
  (= status (csym::pthread-attr-setscope (ptr attr) PTHREAD-SCOPE-SYSTEM))
  (if (== status 0)
      (= status (csym::pthread-create (ptr tid) (ptr attr) start-func arg))
      (= status (csym::pthread-create (ptr tid) 0          start-func arg)))
  (return status))

(def (csym::mem-error str) (fn void (ptr (const char)))
  (csym::fputs str stderr)
  (csym::fputc #\Newline stderr))

(def cmd-queue (array (struct cmd-list) 512))
(def cmd-v-buf (array char 512 4 256))
(def cmd-in (ptr (struct cmd-list)))
(def cmd-out (ptr (struct cmd-list)))
(def buf (array char BUFSIZE))
(def divisibility-flag int 0)

;;; fgetsでOUTSIDEからのメッセージを受け取る -> struct cmd
(def (recv-command) (fn (struct cmd))
  (def r (struct cmd))
  (defs char p c)
  (def b (ptr char) buf)
  
  (csym::fgets b BUFSIZE stdin)
  (= (fref r c) 0)
  (= (fref r node) OUTSIDE)
  (for ((exps (= p #\NULL)
	      (= c (mref b)))
	c
	(exps (= p c)
	      (= c (mref (++ b)))))
    (if (or (== c #\Space)
	    (== c #\Newline))
	(begin (= c #\NULL)
	  (= (mref b) #\NULL))
      (if (and (== p #\NULL)
	       (< (fref r c) MAXCMDC))
	  (= (aref (fref r v) (inc (fref r c))) b))))
  
  (return r) )

(def send-mut pthread-mutex-t PTHREAD-MUTEX-INITIALIZER)
(def queue-mut pthread-mutex-t PTHREAD-MUTEX-INITIALIZER)
(def cond-q pthread-cond-t PTHREAD-COND-INITIALIZER)

;;; struct cmd -> output（stdoutへ）
(def (csym::send-command cmd) (fn void (struct cmd))
  (def i int)
  (for ((= i 0) (< i (- (fref cmd c) 1)) (inc i))
    (csym::fputs (aref (fref cmd v) i) stdout)
    (csym::fputc #\Space stdout))
  (if (> (fref cmd c) 0)
      (begin
	(csym::fputs (aref (fref cmd v)
		       (- (fref cmd c) 1))
		     stdout)
	(csym::fputc #\Newline stdout))))

;;; エラーメッセージstrをstderrへ
(def (csym::proto-error str cmd) (fn void (ptr (const char)) (struct cmd))
  (def i int)
  (csym::fputs str stderr)
  (csym::fputc #\Newline stderr)
  (for ((= i 0) (< i (- (fref cmd c) 1)) (inc i))
    (csym::fputs (aref (fref cmd v) i) stderr)
    (csym::fputc #\Newline stderr))
  (if (> (fref cmd c) 0)
      (begin
	(csym::fputs (aref (fref cmd v)
		       (- (fref cmd c) 1))
		     stderr)
	(csym::fputc #\Newline stderr))))

;; INSIDE transmission用queueへのメッセージ追加
(def (csym::enqueue-command cmd body)
  (fn void (struct cmd) (ptr void))
  (def i int)
  (def len size-t)
  (def q (ptr (struct cmd-list)))
  
  ;; cmd-queue はlock済み
  (= q cmd-in)
  (if (== (fref cmd-in -> next) cmd-out)
      (begin
	(csym::perror "cmd-queue overflow~%")
	(csym::exit 0)))
  (= cmd-in (fref cmd-in -> next))
  (= (fref q -> cmd c) (fref cmd c))
  (= (fref q -> cmd node) (fref cmd node))
  (for ((= i 0) (< i (fref cmd c)) (inc i))
    (= len (csym::strlen (aref (fref cmd v) i)))
    (if (> len 254)
	(csym::proto-error "too long cmd" cmd))
    (csym::strncpy (aref (fref q -> cmd v) i)
		   (aref (fref cmd v) i)
		   (+ len 1)))
  (= (fref q -> body) body))

;; INSIDE transmission用queue にたまったメッセージ処理
(def (exec-queue-cmd arg) (fn (ptr void) (ptr void))
  (def cmd (struct cmd))
  (def body (ptr void))
  (loop
    (csym::pthread-mutex-lock (ptr queue-mut))
    (while (== cmd-in cmd-out)
      (csym::pthread-cond-wait
       (ptr cond-q) (ptr queue-mut)))
    (= cmd (fref cmd-out -> cmd))
    (= body (fref cmd-out -> body))
    (csym::pthread-mutex-unlock (ptr queue-mut))
    (if (== 0 (csym::strcmp (aref (fref cmd v) 0) "task"))
	(csym::recv-task cmd body)
      (if (== 0 (csym::strcmp (aref (fref cmd v) 0) "rslt"))
	  (csym::recv-rslt cmd body)
	(if (== 0 (csym::strcmp (aref (fref cmd v) 0) "treq"))
	    (csym::recv-treq cmd)
	  (if (== 0 (csym::strcmp (aref (fref cmd v) 0) "none"))
	      (recv-none cmd)
	    (if (== 0 (csym::strcmp (aref (fref cmd v) 0) "rack"))
		(csym::recv-rack cmd)
	      (begin
		(csym::proto-error "wrong cmd" cmd)
		(csym::exit 0)))))))
    (= cmd-out (fref cmd-out -> next))
    ))

;; EOLまでを無視
(def (csym::read-to-eol) (fn void void)
  (def c int)
  (while (!= EOF (= c (csym::getc stdin)))
    (if (== c #\Newline) (break))))

(def (csym::write-eol) (fn void void)
  (csym::putc #\Newline stdout))

(def (csym::flush-send) (fn void void)
  (csym::fflush stdout))


(def snr-mut pthread-mutex-t PTHREAD-MUTEX-INITIALIZER)
(def cond-snr pthread-cond-t PTHREAD-COND-INITIALIZER)
(def snr-queue (array (struct cmd-list) 32))
(def snr-v-buf (array char 32 2 256))
(def snr-in (ptr (struct cmd-list)))
(def snr-out (ptr (struct cmd-list)))

(def (csym::enqueue-snr cmd) (fn void (struct cmd))
  (def i int)
  (def len size-t)
  (def q (ptr (struct cmd-list)))

  (= q snr-in)
  (if (== (fref snr-in -> next) snr-out)
      (begin
	(csym::perror "snr-queue overflow~%")
	(csym::exit 0)))
  (= snr-in (fref snr-in -> next))
  (= (fref q -> cmd c) (fref cmd c))
  (= (fref q -> cmd node) (fref cmd node))
  (for ((= i 0) (< i (fref cmd c)) (inc i))
    (= len (csym::strlen (aref (fref cmd v) i)))
    (if (> len 254)
	(csym::proto-error "too long cmd" cmd))
    (csym::strncpy (aref (fref q -> cmd v) i)
		   (aref (fref cmd v) i)
		   (+ len 1))))

(def (send-none-rack arg) (fn (ptr void) (ptr void))
  (def cmd (struct cmd))
  (loop
    (csym::pthread-mutex-lock (ptr snr-mut))
    (while (== snr-in snr-out)  ; キューが空なら登録したよメッセージを待つ
      (csym::pthread-cond-wait (ptr cond-snr) (ptr snr-mut)))
    (= cmd (fref snr-out -> cmd))
    (csym::pthread-mutex-unlock (ptr snr-mut))
    (csym::pthread-mutex-lock (ptr send-mut))
    (csym::send-command cmd)
    (csym::flush-send)
    (if (== 0 (csym::strcmp (aref (fref cmd v) 0) "none"))
	(= divisibility-flag 1))  ; noneを送ったらdvblフラグをon（？）
    (csym::pthread-mutex-unlock (ptr send-mut))
    (= snr-out (fref snr-out -> next)))) ; 送ったものをキューから外す

(def threads (array (struct thread-data) 64))
(def num-thrs unsigned-int)

#|
  リモートに treq してもらった task (copy)
    - treq まえに allocate
    x ときどき，リストから DONE となったものを除く
      (それなら，rack を得るまで DONE にしないようにする
       active なスレッド数は別管理すればよい?)
    x  ときどきでなくて，rack を得たときでよい?
    - rslt を送ったら自分で消える．

  分割して作った task (home/orig) => task-home
    - thread-data の sub からのリストとなり，id は重複しないように付ける．
    x 最初から，STARTED，すぐに treq 元に送る．
    o treq の時点で ALLOCATED にしては?
    - rslt がきたら，DONE にして，rack を返す
    - DONE になっていたら，分割元task がマージして消去

  ?? task-home をそのまま同じノードで処理するケースもあるのか??

    treq <task-head> <treq-head>
      <task-head>  タスク返答先
      <treq-head>  要求送信先

    task <ndiv> <rslt-head> <task-head>
      <ndiv>       分割回数 負荷のサイズの目安 (sp2が子の判断に使う)
      <rslt-head>  結果返答先
      <task-head>  タスク送信先

    rslt <rslt-head>
      <rslt-head>  結果送信先

    rack <task-head>
      <task-head>  rack送信先
      (w-rack カウンタを使うべき)

    none <task-head>
      <task-head>  (no)タスク送信先

   [ prev  ] -> [ prev  ] -> [ prev  ]  -> 
<- [ next  ] <- [ next  ] <- [ next  ] <- 

|#  

(def (csym::flush-treq-with-none-1 thr) (fn void (ptr (struct thread-data)))
  (def rcmd (struct cmd))
  (def hx (ptr (struct task-home)))
  (while (= hx (fref thr -> treq-top))
    (= (fref rcmd c) 2)
    (= (fref rcmd node) (fref hx -> req-from))
    (= (aref (fref rcmd v) 0) "none")
    (= (aref (fref rcmd v) 1) (fref hx -> task-head))
    (if (== (fref rcmd node) INSIDE)
	(begin
	  (csym::pthread-mutex-lock (ptr queue-mut))
	  (csym::enqueue-command rcmd NULL)
	  (csym::pthread-cond-signal (ptr cond-q))
	  (csym::pthread-mutex-unlock (ptr queue-mut)))
      (if (== (fref rcmd node) OUTSIDE)
	  (begin
	    (csym::send-command rcmd)
	    (= divisibility-flag 1))
	(begin
	  (csym::perror "Invalid rcmd.node in flush-treq-with-none-1~%")
	  (csym::fprintf stderr "%d~%" (fref rcmd node))
	  (csym::exit 0))))
    (= (fref thr -> treq-top) (fref hx -> next))
    (= (fref hx -> next) (fref thr -> treq-free))
    (= (fref thr -> treq-free) hx)))

(def (csym::flush-treq-with-none thr) (fn void (ptr (struct thread-data)))
  ;; treq がたまっていたら noneを送る
  ;; thr->mut は lock済
  (if (fref thr -> treq-top)
      (begin
	(csym::pthread-mutex-lock (ptr send-mut))
	(csym::flush-treq-with-none-1 thr)
	(csym::flush-send)
	(csym::pthread-mutex-unlock (ptr send-mut)))))

;;; タスク要求 -> 受け取り -> 結果送信
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

  ;; あとでとどく none が届くまで待つ
  (while (> (fref thr -> w-none) 0)
    (csym::pthread-cond-wait (ptr (fref thr -> cond))
			     (ptr (fref thr -> mut)))
    ;; rsltが到着していたら
    (if (and (fref thr -> sub)
	     (== (fref thr -> sub -> stat) TASK-HOME-DONE))
	(return)))

  ;; allocate
  (= tx (fref thr -> task-free))
  (if (not tx)
      (csym::mem-error "Not enough task memory"))
  (= (fref thr -> task-top) tx)
  (= (fref thr -> task-free) (fref tx -> prev))

  (= delay (* 2 1000 1000))

  (do-while (!= (fref tx -> stat) TASK-INITIALIZED)
    ;; 最初にtreqがたまっていたら，noneを送る
    (csym::flush-treq-with-none thr)

    (= (fref tx -> stat) TASK-ALLOCATED)
    (csym::pthread-mutex-unlock (ptr (fref thr -> mut)))
    (= (fref rcmd c) 3)
    (if (> num-thrs 1)
	(= (fref rcmd node) req-to)
      (= (fref rcmd node) OUTSIDE))
    (= (aref (fref rcmd v) 0) "treq")
    (= (aref (fref rcmd v) 1) (fref thr -> id-str))
    (= (aref (fref rcmd v) 2) treq-head)

    (if (!= (fref rcmd node) OUTSIDE)
	(begin
	  (csym::pthread-mutex-lock (ptr queue-mut)) 
	  (csym::enqueue-command rcmd NULL)
	  (csym::pthread-cond-signal (ptr cond-q))
	  (csym::pthread-mutex-unlock (ptr queue-mut)))
      (begin
	(csym::pthread-mutex-lock (ptr send-mut))
	(csym::send-command rcmd)
	(csym::flush-send)
	(= divisibility-flag 1)
	(csym::pthread-mutex-unlock (ptr send-mut))))

    (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
    ;; recv-task で初期化されるのを待つ
    (loop
      ;; rslt が到着していたら，特別に先にさせる
      (if (and (!= (fref tx -> stat) TASK-INITIALIZED)
	       (fref thr -> sub)
	       (== (fref thr -> sub -> stat) TASK-HOME-DONE))
	  (begin
	    (if (== (fref tx -> stat) TASK-NONE)
		(goto Lnone))
	    (inc (fref thr -> w-none))
	    (goto Lnone)))
      (if (!= (fref tx -> stat) TASK-ALLOCATED)
	  (break))
      (csym::pthread-cond-wait (ptr (fref thr -> cond))
			       (ptr (fref thr -> mut)))
      )

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

  ;; TASK-INITIALIZED
  (= (fref tx -> stat) TASK-STARTED)
  (= old-ndiv (fref thr -> ndiv))
  (= (fref thr -> ndiv) (fref tx -> ndiv))
  ;; ここにセマフォ？
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut)))
  (do-task-body thr (fref tx -> body))    ; タスク実行
  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))

  ;; taskの処理完了後は，そのtask-homeにsend-rsltする
  (= (fref rcmd c) 2)
  (= (fref rcmd node) (fref tx -> rslt-to))
  (= (aref (fref rcmd v) 0) "rslt")
  (= (aref (fref rcmd v) 1) (fref tx -> rslt-head))

  (if (== (fref rcmd node) INSIDE)
      (begin
	(csym::pthread-mutex-lock (ptr queue-mut))
	(csym::enqueue-command rcmd (fref tx -> body))
	(csym::pthread-cond-signal (ptr cond-q))
	(csym::pthread-mutex-unlock (ptr queue-mut))
	(csym::pthread-mutex-lock (ptr send-mut)) ; flush-treqのため
	)
    (if (== (fref rcmd node) OUTSIDE)
	(begin
	  (csym::pthread-mutex-lock (ptr send-mut))
	  (csym::pthread-mutex-unlock (ptr (fref thr -> mut)))
	  (csym::send-command rcmd)
	  ;; body ではなく，do-task-body の return value にするのはどうか?
	  (send-rslt-body thr (fref tx -> body))
	  ;; または，x->body.h.class->send-rslt-body(x->body);
	  (csym::write-eol)
	  (csym::pthread-mutex-lock (ptr (fref thr -> mut))))
      (begin
	(csym::perror "Invalid rcmd.node in recv-exec-send~%")
	(csym::fprintf stderr "%d~%" (fref rcmd node))
	(csym::exit 0))))

  ;; 最後にもtreqがたまっていたら noneを送る
  (csym::flush-treq-with-none-1 thr)
  (csym::flush-send)
  (csym::pthread-mutex-unlock (ptr send-mut))
  (csym::pthread-mutex-lock (ptr (fref thr -> rack-mut)))
  (inc (fref (mref thr) w-rack))
  (csym::pthread-mutex-unlock (ptr (fref thr -> rack-mut)))
  (= (fref thr -> ndiv) old-ndiv)

  (label Lnone
    (begin
      (= (fref thr -> task-free) tx)
      (= (fref thr -> task-top) (fref tx -> next))))
  )

(def (worker arg) (fn (ptr void) (ptr void))
  (def thr (ptr (struct thread-data)) arg)
  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
  (loop
    (recv-exec-send thr "any" ANY))
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut))))

;;; recv-task <タスクが分割された回数> <送信元> <送信先>
(def (csym::recv-task cmd body) (fn void (struct cmd) (ptr void))
  (def tx (ptr (struct task)))
  (def thr (ptr (struct thread-data)))
  (def id unsigned-int)
  (def len size-t)
  (if (< (fref cmd c) 4)   ; パラメータ数チェック
      (csym::proto-error "wrong-task" cmd))
  ;; idを<task-head>に含める
  (= id (csym::atoi (aref (fref cmd v) 3)))
  (if (not (< id num-thrs))
      (csym::proto-error "wrong task-head" cmd))
  (= thr (+ threads id))    ; thr: taskを実行するスレッド
  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
  (= tx (fref thr -> task-top)) ; この場所にやってほしいtask情報を書き込む
  (= (fref tx -> rslt-to) (fref cmd node))  ; 結果の送り先がどこか
  (= len (csym::strlen (aref (fref cmd v) 2)))
  (if (> len 254)
      (csym::proto-error "Too long rslt-head for task" cmd))
  (csym::strncpy (fref tx -> rslt-head) ; "rslt" の送信先に使うアドレス
		 (aref (fref cmd v) 2)
		 (+ len 1))
  (= (fref tx -> ndiv) (csym::atoi (aref (fref cmd v) 1))) ; 分割回数
  ;; タスクのパラメータ（task specificな構造体）の受け取り
  (if (== (fref cmd node) INSIDE)
      (= (fref tx -> body) body)      ; INSIDEだったらポインタを渡すだけ
    (if (== (fref cmd node) OUTSIDE)
	(begin                        ; OUTSIDEだったら文字列で読み込む
	  (= (fref tx -> body)
	     (csym::recv-task-body thr)) ; 読み出しはロック獲得の前のほうがいいかも
	  (csym::read-to-eol))
      (begin
	(csym::perror "Invalid cmd.node in recv-task~%")
	(csym::fprintf stderr "%d~%" (fref cmd node))
	(csym::exit 0))))

  ;; task を受け取った後，ノード内に待っているスレッド(ワーカ)を起こす
  (= (fref tx -> stat) TASK-INITIALIZED) ; そもそもロックが必要なのはここだけかも
  ;; treqしてからtaskを受け取るまでの間はstatしかアクセスされない気がする
  
  ;; スレッド数の上限を超えるなら，後で(どれかのスレッドが終わるとき)
  ;; signal するべきか?
  ;; それよりは，別にセマフォで上限を管理するほうが簡単
  (csym::pthread-cond-broadcast (ptr (fref thr -> cond)))
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut))))

(def (recv-none cmd) (fn void (struct cmd))
  (def thr (ptr (struct thread-data)))
  (def id unsigned-int)
  (def len size-t)
  (if (< (fref cmd c) 2)
      (csym::proto-error "Wrong none" cmd))
  (= id (csym::atoi (aref (fref cmd v) 1)))
  (if (not (< id num-thrs))
      (csym::proto-error "Wrong task-head" cmd))
  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
  (if (> (fref thr -> w-none) 0)
      (dec (fref thr -> w-none))
    (= (fref thr -> task-top -> stat) TASK-NONE))
  (csym::pthread-cond-broadcast (ptr (fref thr -> cond)))
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut))))

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

(def (csym::recv-rslt cmd rbody) (fn void (struct cmd) (ptr void))
  (def rcmd (struct cmd))
  (def thr (ptr (struct thread-data)))
  (def hx (ptr (struct task-home)))
  (def tid unsigned-int)
  (def sid unsigned-int)
  (def b (ptr char))
  (def h-buf (array char 256))
  (if (< (fref cmd c) 2)  ; 引数の数チェック
      (csym::proto-error "Wrong rslt" cmd))
  (= b (aref (fref cmd v) 1)) ; 結果送信先 (<tid>:<sid>)
  (= tid (csym::atoi b))
  (= b (csym::strchr b #\:))
  (if (not b)
      (csym::proto-error "Wrong rslt-head" cmd))
  (= sid (csym::atoi (+ b 1)))
  (= thr (+ threads tid))

  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
  (= hx (fref thr -> sub))
  ;; hx = 返ってきたrsltを待っていたtask-home（.id==sid）を探す
  (while (and hx (!= (fref hx -> id) sid))
    (= hx (fref hx -> next)))
  (if (not hx)
      (csym::proto-error "Wrong rslt-head" cmd))
  (if (== (fref cmd node) INSIDE)  ; 結果の受け取り
      (= (fref hx -> body) rbody)  ; INSIDEならbodyのポインタ代入
    (if (== (fref cmd node) OUTSIDE) ; OUTSIDEならストリームから
	(begin
	  (csym::recv-rslt-body thr (fref hx -> body))
	  (csym::read-to-eol))
      (begin
	(csym::perror "Invalid cmd.node in recv-rslt~%")
	(csym::fprintf stderr "%d~%" (fref cmd node))
	(csym::exit 0))))

  ;; rackを返す．もっと後のほうがよい？
  (= (fref rcmd c) 2)
  (= (fref rcmd node) (fref cmd node))
  (= (aref (fref rcmd v) 0) "rack")
  (csym::strncpy
   h-buf (fref hx -> task-head)
   (+ 1 (csym::strlen (fref hx -> task-head))))
  (= (aref (fref rcmd v) 1) h-buf)
  ;; hx 中に記録された task-head に rack を後で送るなら，
  ;; ここではないが，まだ free されたくないので，つなぎなおすかも
  (= (fref hx -> stat) TASK-HOME-DONE)
  (if (== hx (fref thr -> sub))
      (begin
	(csym::pthread-cond-broadcast (ptr (fref thr -> cond-r)))
	(csym::pthread-cond-broadcast (ptr (fref thr -> cond))))
    )
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut)))
  (if (fref cmd node)
      (begin
	(csym::pthread-mutex-lock (ptr queue-mut))
	(csym::enqueue-command rcmd NULL)
	(csym::pthread-cond-signal (ptr cond-q))
	(csym::pthread-mutex-unlock (ptr queue-mut)))
    (begin
      ;; デッドロック防止のため読み出しをするスレッドで書き込みをしないようにしている
      (csym::pthread-mutex-lock (ptr snr-mut))
      (csym::enqueue-snr rcmd)
      (csym::pthread-cond-signal (ptr cond-snr))
      (csym::pthread-mutex-unlock (ptr snr-mut)))))

;; worker がしばらく受け取ってくれなくても，
;; treq は受け取っておくほうがよさそう．
;; 複数の treq をリストにして受け取らせてもよいのでは?
;; treq は task-home の形にしておいてもよいかも．
;; task-home は スタックアロケートのつもりだったが...

;;; threads[id] にtreqを試みる
(def (csym::try-treq cmd id) (fn int (struct cmd) unsigned-int)
  (def hx (ptr (struct task-home)))
  (def thr (ptr (struct thread-data)))
  (def len size-t)
  (def avail int 0)

  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
  (csym::pthread-mutex-lock (ptr (fref thr -> rack-mut)))
  (if (and (fref thr -> task-top)
	   (or (== (fref thr -> task-top -> stat) TASK-STARTED)
	       (== (fref thr -> task-top -> stat) TASK-INITIALIZED))
	   (== (fref thr -> w-rack) 0))
      (= avail 1))
  (csym::pthread-mutex-unlock (ptr (fref thr -> rack-mut)))

  ;; 成功ならタスク作成待ちスタックにpush
  (if avail
      (begin
	(= hx (fref thr -> treq-free))
	(if (not hx)
	    (csym::mem-error "Not enough task-home memory"))
	(= (fref thr -> treq-free) (fref hx -> next))
	(= (fref hx -> next) (fref thr -> treq-top))
	(= (fref hx -> stat) TASK-HOME-ALLOCATED)
	(= len (csym::strlen (aref (fref cmd v) 1)))
	(if (> len 254)
	    (csym::proto-error "Too long task-head for treq" cmd))
	(csym::strncpy (fref hx -> task-head)
		       (aref (fref cmd v) 1)
		       (+ len 1))
	(if (!= (fref cmd node) OUTSIDE)
	    (= (fref hx -> req-from) INSIDE)
	  (= (fref hx -> req-from) OUTSIDE))
	(= (fref thr -> treq-top) hx)
	(= (fref thr -> req) hx)
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

(def (csym::recv-treq cmd) (fn void (struct cmd))
  ;;   task id が指定されている場合と，any の場合がある．
  ;; any の場合は，any でとってきたもののほうが大きいのでは?
  ;; => そもそも，すべての task が動いているわけではない．
  ;;    すくなくとも，rslt まちで，別 task を立ち上げた task に
  ;;    req してもだめ．
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
  (def rcmd (struct cmd))
  (def id unsigned-int)
  (if (< (fref cmd c) 3)  ; 引数の数チェック
      (csym::proto-error "Wrong treq" cmd))
  ;; id を<task-head>に含める
  (if (== (csym::strcmp (aref (fref cmd v) 2) "any") 0)
      (let ((myid int))  ; 送信元id
	(= myid (csym::atoi (aref (fref cmd v) 1)))
	(for ((= id 0) (< id num-thrs) (inc id))
	  (if (and (!= (fref cmd node) OUTSIDE)
		   (== id myid))
	      (continue))  ; 自分自身に要求を出すようなポカはしない
	  (if (csym::try-treq cmd id) (break)))
	(if (!= id num-thrs) ; treqできた
	    (return)))
    (begin  ; "any"じゃない場合
      (= id (csym::atoi (aref (fref cmd v) 2)))
      (if (not (< id num-thrs))
	  (csym::proto-error "Wrong task-head" cmd))
      (if (csym::try-treq cmd id)
	  (return))))

  ;; treqできなかった場合
  (if (== (fref cmd node) ANY)
      (if (== (csym::atoi (aref (fref cmd v) 1)) 0)
	  (begin
	    (csym::pthread-mutex-lock (ptr send-mut))
	    (csym::send-command cmd)
	    (csym::flush-send)
	    (= divisibility-flag 1)
	    (csym::pthread-mutex-unlock (ptr send-mut))
	    (return))
	(= (fref cmd node) INSIDE)))

  ;; noneを返す
  (= (fref rcmd c) 2)
  (= (fref rcmd node) (fref cmd node))
  (= (aref (fref rcmd v) 0) "none")
  (= (aref (fref rcmd v) 1) (aref (fref cmd v) 1))
  (if (== (fref rcmd node) INSIDE)
      (begin
	(csym::pthread-mutex-lock (ptr queue-mut))
	(csym::enqueue-command rcmd NULL)
	(csym::pthread-cond-signal (ptr cond-q))
	(csym::pthread-mutex-unlock (ptr queue-mut)))
    (if (== (fref rcmd node) OUTSIDE)
	(begin
	  ;; デッドロック防止のため読み出しをするスレッドで書き込まないようにしている
	  (csym::pthread-mutex-lock (ptr snr-mut))
	  (csym::enqueue-snr rcmd)
	  (csym::pthread-cond-signal (ptr cond-snr))
	  (csym::pthread-mutex-unlock (ptr snr-mut)))
      (begin
	(csym::perror "Invalid rcmd.node in recv-treq~%")
	(csym::fprintf stderr "%d~%" (fref rcmd node))
	(csym::exit 0)))))

(def (csym::recv-rack cmd) (fn void (struct cmd))
  (def tx (ptr (struct task)))
  (def thr (ptr (struct thread-data)))
  (def id unsigned-int)
  (def len size-t)
  (if (< (fref cmd c) 2)
      (csym::proto-error "Wrong rack" cmd))
  ;; idを<task-head>に含める
  (= id (csym::atoi (aref (fref cmd v) 1)))
  (if (not (< id num-thrs))
      (csym::proto-error "Wrong task-head" cmd))
  (= thr (+ threads id))
  (csym::pthread-mutex-lock (ptr (fref thr -> rack-mut)))
  (dec (fref thr -> w-rack))
  (csym::pthread-mutex-unlock (ptr (fref thr -> rack-mut))))

(def (csym::send-divisible) (fn void void)
  (def cmd (struct cmd))
  (if (csym::pthread-mutex-trylock (ptr send-mut))
      (return))
  (= divisibility-flag 0)
  (= (fref cmd c) 1)
  (= (aref (fref cmd v) 0) "dvbl")
  (csym::send-command cmd)
  (csym::flush-send)
  (csym::pthread-mutex-unlock (ptr send-mut)))


(def (handle-req -bk -thr)
  (fn void (ptr (FN int void)) (ptr (struct thread-data)))

  (csym::pthread-mutex-lock (ptr (fref -thr -> mut)))
  (if (fref -thr -> req)
      (begin
	(-bk)
	(= (fref -thr -> req) (fref -thr -> treq-top))))
  (csym::pthread-mutex-unlock (ptr (fref -thr -> mut))))

(def (csym::make-and-send-task thr body)
  (fn void (ptr (struct thread-data)) (ptr void))

  (def tcmd (struct cmd))
  (def hx (ptr (struct task-home))
    (fref thr -> treq-top))
  (= (fref thr -> treq-top) (fref hx -> next))
  (= (fref hx -> next) (fref thr -> sub))
  (= (fref thr -> sub) hx)
  (= (fref hx -> body) body)
  (= (fref hx -> id) (if-exp (fref hx -> next)
			 (+ (fref hx -> next -> id) 1)
		       0))
  (= (fref hx -> stat) TASK-HOME-INITIALIZED)
  (= (fref tcmd c) 4)
  (= (fref tcmd node) (fref hx -> req-from))
  (= (aref (fref tcmd v) 0) "task")
  ;; ズルなので注意
  (csym::sprintf (fref thr -> ndiv-buf) "%d"
		 (++ (fref thr -> ndiv)))
  (csym::sprintf (fref thr -> buf) "%s:%d"
		 (fref thr -> id-str) (fref hx -> id))
  (= (aref (fref tcmd v) 1) (fref thr -> ndiv-buf))
  (= (aref (fref tcmd v) 2) (fref thr -> buf))
  (= (aref (fref tcmd v) 3) (fref hx -> task-head))

  (if (== (fref tcmd node) INSIDE)
      (begin
	(csym::pthread-mutex-lock (ptr queue-mut))
	(csym::enqueue-command tcmd body)
	(csym::pthread-cond-signal (ptr cond-q))
	(csym::pthread-mutex-unlock (ptr queue-mut)))
    (if (== (fref tcmd node) OUTSIDE)
	(begin
	  (csym::pthread-mutex-lock (ptr send-mut))
	  (csym::pthread-mutex-unlock (ptr (fref thr -> mut)))
	  (csym::send-command tcmd)
	  (csym::send-task-body thr body)
	  (csym::write-eol)
	  (csym::flush-send)
	  (csym::pthread-mutex-unlock (ptr send-mut))
	  (csym::pthread-mutex-lock (ptr (fref thr -> mut))))
      (begin
	(csym::perror "Invalid tcmd.node in make-and-send-task~%")
	(csym::fprintf stderr "%d~%" (fref tcmd node))
	(csym::exit 0)))))

(def (wait-rslt thr) (fn (ptr void) (ptr (struct thread-data)))
  (def body (ptr void))
  (def t1 (struct timespec))
  (def now (struct timeval))
  (def nsec long)
  (def sub (ptr (struct task-home)))
  (csym::pthread-mutex-lock (ptr (fref thr -> mut)))
  (= sub (fref thr -> sub))
  (while (!= (fref sub -> stat) TASK-HOME-DONE)
    (csym::gettimeofday (ptr now) 0)
    (= nsec (* 1000 (fref now tv-usec)))
    (+= nsec (* 5 1000 1000))
    ;; (+= nsec (* 10 1000 1000))
    (= (fref t1 tv-nsec) (if-exp (> nsec 999999999)
			     (- nsec 999999999)
			   nsec))
    (= (fref t1 tv-sec) (+ (fref now tv-sec)
			   (if-exp (> nsec 999999999) 1 0)))
    (csym::pthread-cond-timedwait
     (ptr (fref thr -> cond-r)) (ptr (fref thr -> mut))
     (ptr t1))
    (if (== (fref sub -> stat) TASK-HOME-DONE)
	(break))
    ;; fprintf(stderr, "sub %d\n", sub);

    (recv-exec-send
     thr (fref sub -> task-head) (fref sub -> req-from)))
  (= body (fref sub -> body))
  (= (fref thr -> sub) (fref sub -> next))
  (= (fref sub -> next) (fref thr -> treq-free))
  (= (fref thr -> treq-free) sub)
  ;; fprintf(stderr, "nsub %d\n", thr->sub);
  (csym::pthread-mutex-unlock (ptr (fref thr -> mut)))
  (return body))



;; main
(def (main argc argv) (fn int int (ptr (ptr char)))
  (defs int i j)
  (def dummy (ptr void))
  (def cmd (struct cmd))

  ;; cmd-queue（受信メッセージキュー） の初期化
  (for ((= i 0) (< i 512) (inc i))
    (= (fref (aref cmd-queue i) next)
       (ptr (aref cmd-queue (% (+ i 1) 512))))
    (for ((= j 0) (< j 4) (inc j))
      (= (aref (fref (aref cmd-queue i) cmd v) j)
	 (aref cmd-v-buf i j))))
  (= cmd-in (ptr (aref cmd-queue 0)))
  (= cmd-out (ptr (aref cmd-queue 0)))
  (csym::systhr-create exec-queue-cmd NULL) ; メッセージ受信スレッド

  ;; snr-queue（none, rackメッセージ送信キュー）の初期化
  (for ((= i 0) (< i 32) (inc i))
    (= (fref (aref  snr-queue i) next)
       (ptr (aref snr-queue (% (+ i 1) 32))))
    (for ((= j 0) (< j 2) (inc j))
      (= (aref (fref (aref snr-queue i) cmd v) j)
	 (aref snr-v-buf i j))))
  (= snr-in (ptr (aref snr-queue 0)))
  (= snr-out (ptr (aref snr-queue 0)))
  (csym::systhr-create send-none-rack NULL)  ; none, rack送信スレッド
  
  (= num-thrs (if-exp (> argc 1)
		  (csym::atoi (aref argv 1))
		1))
  ;; ここで thread-data の初期化, task の 双方向list も
  ;; num-thrs = 1;
  (for ((= i 0) (< i num-thrs) (inc i))
    (let ((thr (ptr (struct thread-data)) (+ threads i))
	  (tx (ptr (struct task)))
	  (hx (ptr (struct task-home))))
      (= (fref thr -> id) i)
      (= (fref thr -> w-rack) 0)
      (= (fref thr -> w-none) 0)
      (= (fref thr -> ndiv) 0)
      (csym::pthread-mutex-init (ptr (fref thr -> mut)) 0)
      (csym::pthread-mutex-init (ptr (fref thr -> rack-mut)) 0)
      (csym::pthread-cond-init (ptr (fref thr -> cond)) 0)
      (csym::pthread-cond-init (ptr (fref thr -> cond-r)) 0)
      (csym::sprintf (fref thr -> id-str) "%d" i)

      ;; taskの双方向リスト
      (= tx (cast (ptr (struct task))
	      (csym::malloc (* (sizeof (struct task)) 512))))
      (= (fref thr -> task-top) 0)
      (= (fref thr -> task-free) tx)
      (for ((= j 0) (< j 511) (inc j))
	(= (fref (aref tx j) prev) (ptr (aref tx (+ j 1))))
	(= (fref (aref tx (+ j 1)) next) (ptr (aref tx j))))
      (= (fref (aref tx 0) next) 0)
      (= (fref (aref tx 511) prev) 0)

      ;; task-homeのリスト
      (= hx (cast (ptr (struct task-home))
	      (csym::malloc (* (sizeof (struct task-home)) 512))))
      (= (fref thr -> treq-top) 0)
      (= (fref thr -> treq-free) hx)
      (= (fref thr -> sub) 0)
      (for ((= j 0) (< j 511) (inc j))
	(= (fref (aref hx j) next) (ptr (aref hx (+ j 1)))))
      (= (fref (aref hx 511) next) 0)
      ;; ワーカスレッド生成
      ;; 同一ノード内（共有メモリ）でもやりくりできるか？
      (csym::systhr-create worker thr)))
  
  ;; masterスレッドはOUTSIDEからのメッセージ処理
  (while (exps (= cmd (recv-command))
	       (and (> (fref cmd c) 0)
		    (!= 0 (csym::strcmp
			   (aref (fref cmd v) 0) "exit"))))
    
    (if (== 0 (csym::strcmp (aref (fref cmd v) 0) "task"))
	(csym::recv-task cmd dummy)
      (if (== 0 (csym::strcmp (aref (fref cmd v) 0) "rslt"))
	  (csym::recv-rslt cmd dummy)
	(if (== 0 (csym::strcmp (aref (fref cmd v) 0) "treq"))
	    (csym::recv-treq cmd)
	  (if (== 0 (csym::strcmp (aref (fref cmd v) 0) "none"))
	      (recv-none cmd)
	    (if (== 0 (csym::strcmp (aref (fref cmd v) 0) "rack"))
		(csym::recv-rack cmd)
	      (csym::proto-error "wrong cmd" cmd)))))))
  (csym::exit 0))

