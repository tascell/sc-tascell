;;;; T-Cell用 拠点サーバ
(eval-when (:compile-toplevel :load-toplevel :execute)
  (require "SC-MISC" "../../sc-misc.lsp"))
#+sc-system (use-package "SC-MISC")
(use-package :socket)

;;; 強制的に待ち受けポート再利用
(defvar *reuse-address* t)

;;; 親への接続アドレス:ポート
(defvar *parent-host* nil)
(defvar *parent-port* 8888)

;;; 子からの接続ホスト/ポート
(defvar *server-host* "localhost")
(defvar *children-port* 8888)

;;; 子からの接続要求を監視する間隔（秒）
(defvar *accept-connection-interval* 1)

;;; send/recvのログを出力する長さ
(defvar *transfer-log-length* 70)

(defvar *retry* 20)

(defclass host ()
  ((server :accessor host-server :type tcell-server :initarg :server)
   (host :accessor host-hostname :type string :initarg :host)
   (port :accessor host-port :type fixnum :initarg :port)
   (sock :accessor host-socket :type stream :initarg :sock)))

(defclass parent (host)
  ((host :initform *parent-host*)
   (port :initform *parent-port*)
   (n-treq-sent :accessor parent-n-treq-sent :type fixnum :initform 0)
   (last-none-time :accessor parent-last-none-time :type integer :initform -1)
   ))

(defclass terminal-parent (parent)
  ((host :initform "Terminal")
   (port :initform -1)
   (sock :initform (make-two-way-stream *standard-input* *standard-output*))))

(defclass child (host)
  ((id :accessor child-id :type fixnum)
   (divisibility :accessor child-divisibility
		 :type fixnum :initform 0) ; (max 0 <dvblが来た回数-taskが来た回数>)
   (work-size :accessor child-wsize :type fixnum :initform 0)     ; 仕事の大きさの目安．taskがきたら更新
   ;; (in-treq :accessor child-in-treq :type boolean :initform nil)
   ))

(defclass ta-entry ()    ; elemens of treq-any-list
  ((from :accessor tae-from :type host :initarg :from)
   (head :accessor tae-head :type string :initarg :head)
   ))

(defclass tcell-server ()
  ((host :accessor ts-hostname :initform *server-host* :initarg :local-host)
   (parent :accessor ts-parent :type parent)
   (children-port :accessor ts-chport :initform *children-port* :initarg :children-port)
   (children-sock0 :accessor ts-chsock0 :type stream)   ; 待ち受け
   (children :accessor ts-children :type list :initform '())
   (eldest-child :accessor ts-eldest-child :type child :initform nil)
   (n-children :accessor ts-n-children :type fixnum :initform 0)
   (child-next-id :accessor ts-child-next-id :type fixnum :initform 0)
   (treq-any-list :accessor ts-talist :type list :initform '()) ;; treq-anyを出せていないリスト
   (accept-connection-interval
    :accessor ts-accept-connection-interval
    :type number :initform *accept-connection-interval*)
   (retry :accessor ts-retry :type fixnum :initform *retry* :initarg :retry)
   ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmethod start-server ((sv tcell-server) (prnt parent))
  ;; 親へ接続
  (setf (ts-parent sv) (connect-to prnt))
  ;; 子からの待ち受けポートを開く
  (unwind-protect 
       (progn
	 (setf (ts-chsock0 sv)
	       (make-socket :connect :passive
			    :reuse-address *reuse-address*
			    :local-host (ts-hostname sv)
			    :local-port (ts-chport sv)))
	 ;; 最初の子が接続してくるのを待つ
	 (format *error-output* "~&Waiting for connection to ~A:~D...~%"
		 (ts-hostname sv) (ts-chport sv))
	 (add-child 
	  sv (connect-from (make-instance 'child :server sv)
			   (ts-chsock0 sv)
			   :wait t))
	 (let ((next-child (make-instance 'child :server sv)))
	   (loop
	     (mp:wait-for-input-available
	      (mapcar #'host-socket (cons prnt (ts-children sv)))
	      :timeout (ts-accept-connection-interval sv))
	     ;; 親からのメッセージ処理
	     (awhen (recv-cmd sv prnt)
	       (proc-cmd sv prnt it))
	     ;; 子からのメッセージ処理
	     (loop for chld in (ts-children sv)
		 do (awhen (recv-cmd sv chld)
		      (proc-cmd sv chld it)))
	     ;; 新たな子からの接続要求を受け付ける
	     (awhen (connect-from next-child (ts-chsock0 sv) :wait nil)
	       (add-child sv it)
	       (setq next-child (make-instance 'child :server sv))))))
    (close (ts-chsock0 sv))))

(defmethod print-server-status ((sv tcell-server))
  (format *error-output* "~&~S~%"
   (list `(parent ,(hostinfo (ts-parent sv)))
	 `(children ,@(mapcar #'hostinfo (ts-children sv)))
	 `(eldest-child ,(awhen (ts-eldest-child sv) (hostinfo it)))
	 `(n-children ,(ts-n-children sv))
	 `(child-next-id ,(ts-child-next-id sv))
	 `(children-port ,(ts-chport sv))
	 `(treq-any-list ,@(mapcar #'ta-entry-info (ts-talist sv)))
	 `(retry ,(ts-retry sv)))))

(defun make-and-start-server (&key 
			      (local-host *server-host*)
			      (children-port *children-port*)
			      (retry *retry*)
			      (terminal-parent nil)
			      (parent-host *parent-host* ph-given)
			      (parent-port *parent-port*))
  (unless ph-given (setq terminal-parent t))
  (let* ((sv (make-instance 'tcell-server
			    :local-host local-host
			    :children-port children-port
			    :retry retry))
	 (prnt (if terminal-parent
		   (make-instance 'terminal-parent :server sv)
		   (make-instance 'parent :server sv
				  :host parent-host
				  :port parent-port))))
    (start-server sv prnt)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmethod host-name-port ((hst host))
  (string+ (host-hostname hst) ":" (host-port hst)))
(defmethod host-name-port ((hst terminal-parent))
  "Terminal")

(defmethod hostinfo ((chld child))
  (string+ (host-name-port chld) " (child " (hostid chld) ")"))
(defmethod hostinfo ((prnt parent))
  (string+ (host-name-port prnt) " (parent)"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmethod connect-to ((hst host))
  (setf (host-socket hst)
	(make-socket :remote-host (host-hostname hst)
		     :remote-port (host-port hst)))
  hst)

(defmethod connect-to ((hst terminal-parent))
  (assert (host-socket hst))
  hst)

(defmethod connect-from ((hst host) sock0 &key (wait t))
  (let ((sock
	 (setf (host-socket hst)
	       (accept-connection sock0 :wait wait))))
    (if sock
	(progn
	  (setf (host-hostname hst)
		(ipaddr-to-hostname (remote-host sock)))
	  (setf (host-port hst)
		(remote-port sock))
	  hst)
	nil)))

(defmethod connect-from :after ((hst host) sock0 &key (wait t))
  (when (host-socket hst)
    (format *error-output*
	    "~&Accept connection from ~A.~%" (host-name-port hst))))
  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 要素へのaccessor

;;; 子の追加・削除
(defmethod add-child ((sv tcell-server) (chld child))
  (setf (child-id chld)	(ts-child-next-id sv))
  (incf (ts-child-next-id sv))
  (incf (ts-n-children sv))
  (push chld (ts-children sv))
  (unless (ts-eldest-child sv)
    (setf (ts-eldest-child sv) chld)))

(defmethod remove-child ((sv tcell-server) (chld child))
  (setf (ts-children sv) (delete chld (ts-children sv) :count 1))
  (delete-treq-any sv chld "*")
  (when (eq (ts-eldest-child sv) chld)
    (setf (ts-eldest-child sv) (car (last (ts-children sv)))))
  (decf (ts-n-children sv))
  )

;;; (= id n) の子へのアクセス
(defmethod nth-child ((sv tcell-server) n)
  (find n (ts-children sv)
	:key #'(lambda (chld) (child-id chld))
	:test #'=))

;;; 一番仕事が残ってそうな子
(defmethod most-divisible-child ((sv tcell-server))
  (find-max (ts-children sv)
	    :key #'(lambda (chld)
		     (child-divisibility chld))))

(defmethod increment-divisibility ((chld child))
  (incf (child-divisibility chld)))
(defmethod decrement-divisibility ((chld child))
  (when (> (child-divisibility chld) 0)
    (decf (child-divisibility chld))))
(defmethod reset-divisibility ((chld child))
  (setf (child-divisibility chld) 0))

;;; 子のwork-size を更新
(defmethod renew-work-size ((chld child) wsize)
  (setf (child-wsize chld) wsize))

;;; treq-any-list への要素追加
;;; p-task-head は，"<treqのfromのid>" ":" "<treqのtask-head>" 
(defmethod push-treq-any0 ((sv tcell-server) (tae ta-entry))
  (push tae (ts-talist sv)))
(defmethod push-treq-any ((sv tcell-server) (from host) (p-task-head string))
  (push-treq-any0 sv (make-instance 'ta-entry :from from :head p-task-head)))

;;; treq-any-list のpop
(defmethod pop-treq-any ((sv tcell-server))
  (aand (pop (ts-talist sv))
	(list (tae-from it) (tae-head it))))


;;; treq-any-list から指定要素削除
(defmethod delete-treq-any ((sv tcell-server) (from host) (p-task-head string))
  (setf (ts-talist sv)
    (delete (make-instance 'ta-entry :from from :head p-task-head)
	    (ts-talist sv)
	    :test #'ta-entry-match)))

;;; treq-any-list のmemberか
(defmethod member-treq-any ((sv tcell-server) (from host) (p-task-head string))
  (member (make-instance 'ta-entry :from from :head p-task-head)
	  (ts-talist sv)
	  :test #'ta-entry-match))

;;; ta-entry の同一性
(defmethod ta-entry-match ((x ta-entry) (y ta-entry))
  (and (eq (tae-from x) (tae-from y))
       (or (string= "*" (tae-head x))
	   (string= "*" (tae-head y))
	   (string= (tae-head x) (tae-head y)))))

;; ta-entry の情報
(defmethod ta-entry-info ((tae ta-entry))
  `((from ,(hostinfo (tae-from tae)))
    (head ,(tae-head tae))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 相対アドレス文字列の操作
(defmethod hostid ((chld child))
  (format nil "~D" (child-id chld)))

(defmethod hostid ((prnt parent))
  "p")

(defmethod hostid-to-host ((sv tcell-server) hostid)
  (cond
    ((string= "p" hostid)
     (ts-parent sv))
    (t
     (nth-child sv (parse-integer hostid)))))

;;; アドレスの先頭を切り取って，残りと切り取った先頭アドレス
;;; に相当するホストを返す
(defmethod head-shift ((sv tcell-server) head-string)
  (let* ((sp-head (split-string-1 head-string #\:))
	 (host (hostid-to-host sv (first sp-head))))
    (unless host
      (warn "Connection from/to ~S does not exist." (first sp-head)))
    (list host (second sp-head))))

;;; アドレスhead-stringの先頭にhstのidを追加したものを返す
(defmethod head-push ((hst host) head-string)
  (string+ (hostid hst) ":" head-string))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; メッセージ送信
(defmethod send ((to host) fmt-string &rest args)
  (let ((sock (host-socket to)))
    (apply #'format sock fmt-string args)
    (force-output sock)))

(defmethod send ((to (eql nil)) fmt-string &rest args)
  (format *error-output* "Failed to send \"~A\"~%"
	  (string-firstn
	   (string-right-trim '(#\Newline)
			      (apply #'format nil fmt-string args))
	   *transfer-log-length*)))

(defmethod send :before ((to terminal-parent) fmt-string &rest args)
  (write-string ">> " (host-socket to)))

(defmethod send :after ((to host) fmt-string &rest args)
  (format *error-output* "~&Sent \"~A\" to ~A~%"
	  (string-firstn
	   (string-right-trim '(#\Newline)
			      (apply #'format nil fmt-string args))
	   *transfer-log-length*)
	  (hostinfo to)))

(defmethod send-treq (to task-head treq-head)
  (send to "treq ~A ~A~%" task-head treq-head))

(defmethod send-task (to wsize-str rslt-head task-head task-no task-body)
  (send to "task ~A ~A ~A ~A~%" wsize-str rslt-head task-head task-no)
  (send-task-body to task-body))
(defmethod send-task :before ((to child) wsize-str rslt-head task-head task-no task-body)
  (decrement-divisibility to))

(defmethod send-task-body (to task-body)
  (send to "~A~%" task-body))

(defmethod send-rslt (to rslt-head rslt-body)
  (send to "rslt ~A~%" rslt-head)
  (send-rslt-body to rslt-body))

(defmethod send-rslt-body (to rslt-body)
  (send to "~A~%" rslt-body))

(defmethod send-none (to task-head)
  (send to "none ~A~%" task-head))

(defmethod send-rack (to task-head)
  (send to "rack ~A~%" task-head))

(defmethod send-dvbl (to)
  (send to "dvbl~%"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; メッセージ受信->処理

(defmethod recv-cmd ((sv tcell-server) (from host))
  (let ((sock (host-socket from)))
    (awhen (aand (listen sock)
		 (read-line sock nil "eof")
		 (split-string it))
      (format *error-output* "~&Received ~S from ~A~%"
	      it (hostinfo from))
      it)))

;;; Dispatch
(defmethod proc-cmd ((sv tcell-server) (from host) cmd)
  (string-case (car cmd)
    ("treq" (proc-treq sv from cmd))
    ("task" (proc-task sv from cmd))
    ("none" (proc-none sv from cmd))
    ("rslt" (proc-rslt sv from cmd))
    ("rack" (proc-rack sv from cmd))
    ("dvbl" (proc-dvbl sv from cmd))
    ("eof"  (remove-child sv from))
    ("stat" (print-server-status sv))
    (otherwise (warn "Unknown Command:~S" cmd))))

;;; treq
(defmethod proc-treq ((sv tcell-server) (from host) cmd)
  (let ((p-task-head (head-push from (second cmd)))  ; タスク要求者
	(treq-head (third cmd)))  ; 要求先
    (unless
	(if (string= "any" treq-head)
	    (try-send-treq-any sv from p-task-head)
	    (destructuring-bind (hst0 s-treq-head)
		(head-shift sv treq-head)
	      (try-send-treq sv hst0 p-task-head s-treq-head)))
      ;; treqを送れなかった場合
      (refuse-treq sv from p-task-head))))
  
(defmethod proc-treq :before ((sv tcell-server) (from child) cmd)
  ;; (setf (child-in-treq from) t)
  (setf (child-divisibility from) 0)
  )

(defmethod try-send-treq-any ((sv tcell-server) (from host) p-task-head)
  (or (try-send-treq sv (most-divisible-child sv)
		     p-task-head "any")
      (and (eq (ts-eldest-child sv) from)
	   (try-send-treq sv (ts-parent sv)
			  p-task-head "any"))))

(defmethod try-send-treq ((sv tcell-server) (to host) p-task-head s-treq-head)
  (send-treq to p-task-head s-treq-head)
  t)

(defmethod try-send-treq :around ((sv tcell-server) (to child) p-task-head s-treq-head)
  (if (> (child-divisibility to) 0)
      (call-next-method)
      nil))

(defmethod refuse-treq ((sv tcell-server) (from host) p-task-head)
  (if (member-treq-any sv from p-task-head)
      (send-none from (second (head-shift sv p-task-head)))
      (push-treq-any sv from p-task-head)))

;; treq-any-listにある要素を tryしなおす
(defmethod retry-treq ((sv tcell-server))
  (loop
     for n-sent upfrom 0
     do (aif (pop-treq-any sv)
	     (destructuring-bind (from head) it
	       (unless (try-send-treq-any sv from head)
		 (push-treq-any sv from head)
		 (loop-finish)))
	     (loop-finish))
     finally (return n-sent)))

;;; task
(defmethod proc-task ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (fourth cmd))  ; タスク送信先
    (let ((wsize-str (second cmd))   ; 仕事の大きさ
	  (p-rslt-head (head-push from (third cmd))) ; 結果返信先
	  (task-no (fifth cmd)) ; タスク番号 （fib, lu, ...）
	  (task-body (recv-task-body from)))
      (send-task to wsize-str p-rslt-head s-task-head task-no
		 task-body)
      )))

(defmethod proc-task :before ((sv tcell-server) (from child) cmd)
  (decrement-divisibility from)
  (let ((wsize (parse-integer (second cmd))))
    (renew-work-size from wsize)))

(defmethod recv-task-body ((from host))
  (read-line (host-socket from)))

;;; none
(defmethod proc-none ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (second cmd)) ; none送信先
    (send-none to s-task-head)))

(defmethod proc-none :before ((sv tcell-server) (from child) cmd)
  (reset-divisibility from))

;;; rslt
(defmethod proc-rslt ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-rslt-head)
      (head-shift sv (second cmd)) ; rslt送信先
    (let ((rslt-body (recv-rslt-body from)))
      (send-rslt to s-rslt-head rslt-body))))

(defmethod recv-rslt-body ((from host))
  (read-line (host-socket from)))

;;; rack
(defmethod proc-rack ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (second cmd)) ; rack送信先
    (send-rack to s-task-head)))

;;; dvbl
(defmethod proc-dvbl ((sv tcell-server) (from host) cmd)
  (if (<= (retry-treq sv) 0)
      (send-dvbl (ts-parent sv))))

(defmethod proc-dvbl :before ((sv tcell-server) (from child) cmd)
  (increment-divisibility from))
