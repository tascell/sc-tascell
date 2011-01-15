;;; Copyright (c) 2009-2010 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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

;;;; Tascell server
;;; 起動は (make-and-start-server) 必要に応じてkeyword parameterを参照
;;; 性能評価の際は，  (push :tcell-no-transfer-log *features*)
;;; をして，ログ関係のコードを無視してコンパイルする．

(defpackage "TCELL-SERVER"
  (:nicknames "TSV")
  (:use "CL" "QUEUE" "MISC" "EXCL")
  (:export "MAKE-AND-START-SERVER" "*TRANSFER-LOG*" "*TRANSFER-LOG-OUTPUT*")
  (:shadowing-import-from "QUEUE" #:empty-queue-p #:delete-queue #:find-delete-queue))

(in-package "TCELL-SERVER")

(deftype tsv-stream () 
  #+sbcl '(or stream socket::server-socket)
  #-sbcl 'stream)

;;; logging, debug print
(defvar *log-lock* (mp:make-process-lock))

(defun tcell-print-log (format-string &rest args)
  (mp:with-process-lock (*log-lock*)
    (apply #'format *error-output* format-string args)
    (force-output *error-output*)))



;;; 強制的に待ち受けポート再利用
(defparameter *reuse-address* t)

;;; 親への接続アドレス:ポート
(defparameter *parent-host* nil)
(defparameter *parent-port* 9865)

;;; 子からの接続ホスト/ポート
(defparameter *server-host* "localhost")
(defparameter *children-port* 9865)

;;; ソケットのフォーマット（バイナリが不要なら:textにすると速度向上）
(defparameter *socket-format* :bivalent)

;;; anyでないtreqを常に転送
(defparameter *transfer-treq-always-if-notany* t)

;;; log出力の有無，出力先
(defparameter *transfer-log* t)         ; (featurep :tcell-no-transfer-log) の場合は常に無効
(defparameter *transfer-log-output* *error-output*) ; (featurep :tcell-no-transfer-log) の場合は常に無効
(defparameter *transfer-log-format* :normal) ; :normal or :gnuplot
(defparameter *connection-log* t)

;;; send/recvのログを出力する長さ
(defparameter *transfer-log-length* 70)

;;; read-lineで読み込める長さ（許されるコマンド行の長さ）の最大
(defconstant +max-line-length+ 128)
;;; task, rslt, dataのバッファを転送するときのバッファのサイズ
;;; 十分大きくしないとデッドロックの原因
(defconstant +body-buffer-size+ 4096)

;;; コマンドに続いてデータをともなうコマンド
;;; These constants are referred to in compile time with #. reader macros.
(eval-when (:execute :load-toplevel :compile-toplevel)
  (defparameter *commands* '("treq" "task" "none" "back" "rslt" "rack" "bcst" "bcak" "dreq" "data" "leav"
                             "log"  "stat" "verb" "eval" "exit"))
  (defparameter *commands-with-data* '("task" "rslt" "bcst" "data"))
  (defparameter *commands-without-data* (set-difference *commands* *commands-with-data* :test #'string=)))

(defparameter *retry* 20)


(defclass host ()
  ((server :accessor host-server :type tcell-server :initarg :server)
   (host :accessor host-hostname :type string :initarg :host)
   (port :accessor host-port :type fixnum :initarg :port)
   (sock :accessor host-socket :type tsv-stream :initarg :sock)
   (sender :accessor host-sender :type (or null sender) :initform nil)
   (receiver :accessor host-receiver :type (or null receiver) :initform nil)
   (last-none-time :accessor host-none-time :type fixnum :initform -1)
                                        ; 最後にnoneを受け取った時刻（get-internal-real-timeで獲得）
                                        ; taskを送った相手に対してはリセットする
   (unreplied-treqs :accessor host-unreplied-treqs :type fixnum :initform 0)
					; <treqを送った回数>-<taskまたはnoneをもらった回数>
   ))

(defclass parent (host)
  ((host :initform *parent-host*)
   (port :initform *parent-port*)
   (n-treq-sent :accessor parent-n-treq-sent :type fixnum :initform 0) ; 未使用
   (diff-task-rslt :accessor parent-diff-task-rslt :type fixnum :initform 0)
                                        ; <taskをもらった回数>-<rsltを送った回数> (childとは違うので注意)
   ))

;; terminal-parentのauto-rack, auto-resend-taskを実現するために
;; 覚えておくtaskコマンドの情報
(defstruct task-home
  task-cmd                              ; taskコマンドそのもの
  rack-to                               ; rack送信先
  rack-task-head                        ; rackのtask-head
  start-time)                           ; 開始時刻 (get-internal-real-time)

(defclass terminal-parent (parent)
  ((host :initform "Terminal")
   (port :initform -1)
   (sock :initform (make-two-way-stream *standard-input* *standard-output*))
   (auto-rack :accessor parent-auto-rack :type boolean :initarg :auto-rack :initform nil)
                                        ; 自動的にrackを返す
   (auto-resend-task :accessor parent-auto-resend-task :type fixnum :initarg :auto-resend-task :initform 0)
                                        ; auto-rack後，自動的に同じtaskを再送信する回数
   (auto-exit :accessor parent-auto-exit :type boolean :initarg :auto-exit :initform nil)
                                        ; auto-rack後，かつauto-resend-taskが尽きていたら自動でexitを送る
   (auto-treq-response-func :accessor parent-auto-treq-response-func :type (or null function) :initform nil)
                                        ; treqを送ったら自動的に実行される関数
   (task-home :accessor parent-task-home :type list :initform ())
                                        ; ↑を実現するために親から送られたtaskを覚えておくリスト
   ))

(defclass child (host)
  ((id :accessor child-id :type fixnum)
   (diff-task-rslt :accessor child-diff-task-rslt :type fixnum :initform 0)
                                        ; <taskを送った回数>-<rsltが返ってきた回数>
   (work-size :accessor child-wsize :type fixnum :initform 0)
                                        ; 仕事の大きさの目安．
                                        ; taskがきたら/送ったら(- <ndiv>)に更新
   ;; (in-treq :accessor child-in-treq :type boolean :initform nil)
   ))

(defclass ta-entry ()                   ; elemens of treq-any-list
  ((from :accessor tae-from :type host :initarg :from)
   (head :accessor tae-head :type string :initarg :head)
   ))

(defclass sender ()
  ((queue :accessor send-queue :type shared-queue
          :initform (make-instance 'shared-queue) :initarg :queue)
   (writer :accessor writer :type function ; obj stream -> (write to stream)
           :initform #'write-string :initarg :writer)
   (send-process :accessor send-process :type (or null mp:process) :initform nil)
   (destination :accessor sender-dest :type tsv-stream :initform nil :initarg :dest)))
(defclass receiver ()
  ((queue :accessor receive-queue :type shared-queue
          :initform (make-instance 'shared-queue) :initarg :queue)
   (reader :accessor reader :type function ; stream -> <obj,eof-p>
           :initform #'(lambda (strm) (aif (read-line strm nil nil) (values it nil)
                                        (values nil t)))
           :initarg :reader)
   (receive-process :accessor receive-process :type (or null mp:process) :initform nil)
   (source :accessor receiver-src :type tsv-stream :initform nil :initarg :src)))

(defclass tcell-server ()
  ((host :accessor ts-hostname :initform *server-host* :initarg :local-host)
   (message-queue :accessor ts-queue :type shared-queue
                  :initform (make-instance 'shared-queue))
   (proc-cmd-process :accessor ts-proc-cmd-process :type (or null mp:process) :initform nil)
   (read-cmd-process :accessor ts-read-cmd-process :type (or null mp:process) :initform nil)
   (parent :accessor ts-parent :type parent)
   (children-port :accessor ts-chport :type fixnum :initform *children-port* :initarg :children-port)
   (children-sock0 :accessor ts-chsock0 :type tsv-stream) ; 待ち受け
   (children :accessor ts-children :type list :initform '())
   (eldest-child :accessor ts-eldest-child :type (or null child) :initform nil)
   (n-children :accessor ts-n-children :type fixnum :initform 0)
   (n-wait-children :accessor ts-n-wait-children :type fixnum :initform 0 :initarg :n-wait-children)
                                        ; この数のchildが接続するまでメッセージ処理を行わない
   (child-next-id :accessor ts-child-next-id :type fixnum :initform 0)
   (socket-format :accessor ts-socket-format :type symbol :initform *socket-format* :initarg :socket-format)
   (bcst-receipants :accessor ts-bcst-receipants :type list :initform ())
					; (<bcak返信先> . <bcstをforwardしたhostのリスト>)のリスト
   (exit-gate :accessor ts-exit-gate :initform (mp:make-gate nil))
   (treq-any-list :accessor ts-talist :type list :initform '()) ;; treq-anyを出せていないリスト
   (accept-connection-process :accessor ts-accept-connection-process
                              :type (or null mp:process) :initform nil)
   (retry :accessor ts-retry :type fixnum :initform *retry* :initarg :retry)
   ))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Logger
#-tcell-no-transfer-log
(defmacro tcell-server-dprint (format-string &rest args)
  `(when *transfer-log*
     (mp:with-process-lock (*log-lock*)
       (format *transfer-log-output* ,format-string ,@args)
       ;; (force-output *taransfer-log-output*)
       )))
#+tcell-no-transfer-log
(defmacro tcell-server-dprint (&rest args)
  (declare (ignore args))
  '(progn))

#-tcell-no-transfer-log
(defgeneric get-gnuplot-error-bar (from to msg))
#-tcell-no-transfer-log
(defmethod get-gnuplot-error-bar ((from string) (to host) (msg string))
  (let ((x (get-internal-real-time))
        (y-from (if (string-begin-with "p" from)
                    -1
                   (parse-integer from :junk-allowed t)))
        (y-to (if (typep to 'parent)
                  -1
                (parse-integer (hostid to)))))
    (format nil "~&~D ~D ~D ~D # ~A~%" x y-from y-from y-to msg)))

#-tcell-no-transfer-log
(defmethod get-gnuplot-error-bar ((from host) (to string) (msg string))
  (let ((x (get-internal-real-time))
        (y-from (if (typep from 'parent)
                    -1
                  (parse-integer (hostid from))))
        (y-to (if (string-begin-with "p" to)
                  -1
                (parse-integer to :junk-allowed t))))
    (format nil "~&~D ~D ~D ~D # ~A~%" x y-from y-from y-to msg)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Initializers
(defmethod initialize-instance :after ((sdr sender) &rest initargs)
  (declare (ignore initargs))
  (setf (send-process sdr)
    (mp:process-run-function "SEND-PROCESS"
      #'monitor-and-send-queue
      (send-queue sdr) (sender-dest sdr) (writer sdr))))

(defmethod initialize-instance :after ((rcvr receiver) &rest initargs)
  (declare (ignore initargs))
  (setf (receive-process rcvr)
    (mp:process-run-function "RECEIVE-PROCESS"
      #'receive-and-add-to-queue
      (receive-queue rcvr) (receiver-src rcvr) (reader rcvr))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Finalizers
(defgeneric cleanup (obj))
(defmethod cleanup ((sdr sender))
  (when (send-process sdr)
    (mp:process-wait-with-timeout "Wait until send queue becomes empty"
                                  5
                                  #'empty-queue-p (send-queue sdr))
    (mp:process-kill (send-process sdr))
    (setf (send-process sdr) nil)))

(defmethod cleanup ((rcvr receiver))
  (when (receive-process rcvr)
    ;; (mp:process-wait-with-timeout "Wait until receive queue becomes empty"
    ;;                               5
    ;;                               #'empty-queue-p (receive-queue rcvr))
    (mp:process-kill (receive-process rcvr))
    (setf (receive-process rcvr) nil)))

(defmethod cleanup ((sv tcell-server))
  (when (ts-accept-connection-process sv)
    (mp:process-kill (ts-accept-connection-process sv))
    (setf (ts-accept-connection-process sv) nil))
  (when (ts-proc-cmd-process sv)
    (mp:process-kill (ts-proc-cmd-process sv))
    (setf (ts-proc-cmd-process sv) nil))
  (when (ts-read-cmd-process sv)
    (mp:process-kill (ts-read-cmd-process sv))
    (setf (ts-read-cmd-process sv) nil))
  (cleanup (ts-parent sv))
  (loop for chld in (ts-children sv)
      do (cleanup chld))
  (close (ts-chsock0 sv)))

(defmethod cleanup ((hst host))
  (cleanup (host-sender hst))
  (cleanup (host-receiver hst)))

(defmethod cleanup :before ((hst child))
  (send-exit hst))

(defmethod cleanup ((hst (eql nil))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defgeneric start-server (sv prnt))
(defmethod start-server ((sv tcell-server) (prnt parent))
  (unwind-protect
      (progn
        ;; 親へ接続
        ;; sv.parent = connect_to (prnt):
        (setf (ts-parent sv) (connect-to prnt (ts-socket-format sv)))
        ;; 子からの待ち受けポートを開く
        (setf (ts-chsock0 sv)
          (socket:make-socket :connect :passive
                              :format (ts-socket-format sv)
                              :reuse-address *reuse-address*
                              :local-host (ts-hostname sv)
                              :local-port (ts-chport sv)))
        ;; 最初の子が接続してくるのを待つ
        (format *error-output* "~&Waiting for connection to ~A:~D...~%"
          (ts-hostname sv) (ts-chport sv))
        (wait-and-add-child sv)
        ;; 子からの接続受け付け用プロセス起動
        (activate-accept-connection-process sv)
        ;; 必要数の子が接続してくるのを待つ（n-wait-childrenオプション）
        (wait-children-connections sv)
        ;; メッセージ処理用プロセス起動
        (activate-proc-cmd-process sv)
        ;; 親の入力ストリームからの入力待ちプロセス起動
        (activate-read-cmd-process sv prnt)
        ;; exitコマンドによってexit-gateがopenになるまでここで停止
        (with1 gate (ts-exit-gate sv)
          (mp:process-wait "Wait for exit command" #'mp:gate-open-p gate))
        )
    (cleanup sv)))

(defgeneric print-server-status (sv))
(defmethod print-server-status ((sv tcell-server))
  (fresh-line *error-output*)
  (pprint (list `(:parent ,(hostinfo (ts-parent sv)))
                `(:children ,@(mapcar #'hostinfo (ts-children sv)))
                `(:eldest-child ,(awhen (ts-eldest-child sv) (hostinfo it)))
                `(:n-children ,(ts-n-children sv))
                `(:child-next-id ,(ts-child-next-id sv))
                `(:children-port ,(ts-chport sv))
                `(:treq-any-list ,@(mapcar #'ta-entry-info (ts-talist sv)))
                `(:retry ,(ts-retry sv))
                `(:diff-task-rslt
                  ,@(mapcar #'(lambda (host)
                                (list (hostid host) (child-diff-task-rslt host)))
                            (ts-children sv)))
                `(:unreplied-treqs
                  ,@(mapcar #'(lambda (host)
                                (list (hostid host) (host-unreplied-treqs host)))
                            (cons (ts-parent sv) (ts-children sv))))
                `(:bcst-receipants
                  ,@(mapcar #'(lambda (rcp-entry)
                                (cons (car rcp-entry)
                                      (mapcar #'hostid (cdr rcp-entry))))
                            (ts-bcst-receipants sv)))
                )
          *error-output*)
  (terpri *error-output*)
  (force-output *error-output*))

(defgeneric wait-and-add-child (sv))
(defmethod wait-and-add-child ((sv tcell-server))
  (let ((next-child (make-instance 'child :server sv)))
    (awhen (connect-from next-child (ts-chsock0 sv) :wait t)
      (add-child sv it))))

(defgeneric wait-children-connections (sv))
(defmethod wait-children-connections ((sv tcell-server))
  (mp:process-wait "Wait for connections of children."
                   #'(lambda () (<= (ts-n-wait-children sv) (ts-n-children sv)))))

(defgeneric activate-accept-connection-process (sv))
(defmethod activate-accept-connection-process ((sv tcell-server))
  (setf (ts-accept-connection-process sv)
    (mp:process-run-function "ACCEPT-CHILD-CONNECTION"
      #'(lambda () (loop (wait-and-add-child sv))))))

;;; サーバのメッセージ処理プロセスを起動
(defgeneric activate-proc-cmd-process (sv))
(defmethod activate-proc-cmd-process ((sv tcell-server))
  (setf (ts-proc-cmd-process sv)
    (mp:process-run-function "PROC-CMD"
      #'(lambda (msg-q &aux (msg-gate (sq-gate msg-q)))
          (loop
            ;; 親または子からの入力待ち
            (mp:process-wait "Waiting for a message to Tascell server."
                             #'mp:gate-open-p msg-gate)
            ;; メッセージ処理
            (while (mp:gate-open-p msg-gate)
              (destructuring-bind (host . message) (delete-queue msg-q)
                (proc-cmd sv host message))
              (retry-treq sv))))
      (ts-queue sv))))

;;; サーバのメッセージ読み込みプロセス起動
(defgeneric activate-read-cmd-process (sv prnt))
(defmethod activate-read-cmd-process ((sv tcell-server) (prnt parent))
  (setf (ts-read-cmd-process sv)
    (mp:process-run-function "READ-FROM-PARENT"
      #'(lambda ()
          (let* ((msg-q (ts-queue sv))
                 (prnt (ts-parent sv))
                 (reader (make-receiver-reader prnt))
                 (src (host-socket prnt)))
            (loop
              (multiple-value-bind (msg eof-p) (funcall reader src)
                (add-queue msg msg-q)
                (when eof-p (return))))
            )))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defgeneric host-name-port (hst))
(defmethod host-name-port ((hst host))
  (string+ (host-hostname hst) ":" (write-to-string (host-port hst))))
(defmethod host-name-port ((hst terminal-parent))
  "Terminal")

(defgeneric hostinfo (hst))
(defmethod hostinfo ((chld child))
  (string+ (host-name-port chld)
           " (child " (hostid chld) ")"))
(defmethod hostinfo ((prnt parent))
  (string+ (host-name-port prnt)
           " (parent)"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defgeneric connect-to (hst &optional format))
(defmethod connect-to ((hst host) &optional (format *socket-format*))
  (setf (host-socket hst)
    (socket:make-socket :format format
                        :remote-host (host-hostname hst)
                        :remote-port (host-port hst)))
  (initialize-connection hst)
  hst)

(defmethod connect-to ((hst terminal-parent) &optional format)
  (declare (ignore format))
  (assert (host-socket hst))
  (initialize-connection hst)
  hst)

(defgeneric connect-from (hst sock0 &key wait))
(defmethod connect-from ((hst host) sock0 &key (wait t))
  (let ((sock (setf (host-socket hst)
                (socket:accept-connection sock0 :wait wait))))
    (if sock
        (progn
          (initialize-connection hst)
          hst)
      nil)))

(defmethod connect-from :after ((hst host) sock0 &key (wait t))
  (declare (ignorable sock0 wait))
  (when (host-socket hst)
    (when *connection-log*
      (tcell-print-log "~&Accept connection from ~A.~%" (host-name-port hst)))))

(defgeneric initialize-connection (hst))
(defmethod initialize-connection ((hst host) &aux (sock (host-socket hst)))
  (setf (host-hostname hst) (with1 ipaddr (socket:remote-host sock)
                                   (if (= ipaddr 0)
                                       "Unknown"
                                     (or (socket:ipaddr-to-hostname ipaddr)
                                         (socket:ipaddr-to-dotted ipaddr)))))
  (setf (host-port hst) (or #-sbcl (socket:remote-port sock)
                            #+sbcl 0))
  hst)

(defmethod initialize-connection :around ((hst parent))
  (call-next-method)
  (initialize-sender hst)
  hst)

(defmethod initialize-connection ((hst terminal-parent))
  (initialize-sender hst)
  hst)

;; make-instance -> initialize-connection -> initialize-sender/receiverなので
;; initialize-instanceでできない
(defgeneric initialize-sender (hst))
(defmethod initialize-sender ((hst host) &aux (sock (host-socket hst)))
  (setf (host-sender hst)
    (make-instance 'sender :dest sock :writer #'sender-writer))
  hst)

(defgeneric initialize-receiver (hst))
(defmethod initialize-receiver ((hst host) &aux (sock (host-socket hst)))
  (setf (host-receiver hst)
    (make-instance 'receiver
      :src sock :reader (make-receiver-reader hst)
      :queue (ts-queue (host-server hst)))) ; readしたものは共有のバッファに入れる
  hst)

;;;
(defun sender-writer (obj dest)
  (etypecase obj
    (list (mapc #'(lambda (ob) (sender-writer ob dest)) obj))
    (character (write-char obj dest))
    (string (write-string obj dest))
    (function (funcall obj dest))
    (array (write-sequence obj dest))))

(defun msg-log-string (obj &optional (separator #\Space))
  (with-output-to-string (s)
    (write-msg-log obj s separator)))

(defun write-msg-log (obj dest &optional (separator #\Space))
  (typecase obj
    (list (write-msg-log (car obj) dest separator)
          (mapc #'(lambda (ob)
                    (when separator (write-char separator dest))
                    (write-msg-log ob dest separator))
                (cdr obj)))
    ;; (symbol (write-string (symbol-name obj) dest))
    (character (if (char= #\Newline obj)
                   (write-string "_" dest)
                 (write-char obj dest)))
    (string (write-string obj dest))
    (function (write-string "#<data body>" dest))
    (array (format dest "#<Binary data: SIZE=~D>" (length obj)))))

(defgeneric make-receiver-reader (hst))
(defmethod make-receiver-reader ((hst host))
  (let ((line-buffer
         #+allegro
         (make-array +max-line-length+
                     :element-type 'standard-char :fill-pointer +max-line-length+))
        (body-buffer (make-array +body-buffer-size+ :element-type '(unsigned-byte 8) :adjustable t))
        (gate (mp:make-gate t)))        ; body-buffer の使用許可
    #'(lambda (stream)
        #+allegro (setf (fill-pointer line-buffer) +max-line-length+)
        (let* ((n-char #+allegro (setf (fill-pointer line-buffer)
                                   (excl:read-line-into line-buffer stream nil 0))
                       #-allegro (progn
                                   (setq line-buffer (read-line stream nil ""))
                                   (length line-buffer)))
               (eof-p (= 0 n-char))
               (msg (if eof-p '("leav") (split-string line-buffer))))
          (string-case-eager (car msg)
            (#.*commands-with-data*
             (mp:process-wait "Waiting for finishing reading body from the buffer"
                              #'mp:gate-open-p gate)
             (mp:close-gate gate)
             (setq msg (nconc msg (read-body-into-buffer stream body-buffer) ; read-body-into-buffer用
                              (list #'(lambda (dummy) (declare (ignore dummy))
                                              (mp:open-gate gate)))))
             #+comment                  ;forward-body用
             (setq msg (nconc msg (list #'(lambda (ostream)
                                            (forward-body stream ostream line-buffer body-buffer
                                                          +max-line-length+ +body-buffer-size+)
                                            (mp:open-gate gate))))))
            (#.*commands-without-data* nil)
            (otherwise (error "Unknown command ~S from ~S." msg (hostinfo hst))))
          (tcell-server-dprint "~&~A(~6D): ~A~15T>>> ~A~%"
                               (if (eq :gnuplot *transfer-log-format*) "# " "") 
                               (get-internal-real-time)
                               (hostid hst) (msg-log-string msg))
          (values (cons hst msg) eof-p))
        )))

;;; "task", "rslt", "data" のbody部を読み込み，リストにして返す
;;; バイナリ部分はbufferに書き込み，読み出すための関数を用意する
(defun read-body-into-buffer (stream buffer)
  (let ((ret '())
        (buf-used 0))
    (declare (fixnum buf-used))
    (loop
      (let* ((pre (read-line stream t))
             (len (length pre)))
        ;; 空行で終了
        (when (= len 0) (return))
        (pushs pre #\Newline ret)
        ;; (tcell-server-dprint "~A~%" pre)
        ;; #\(でおわっていたら次の行はbyte-header，次いでbyte-data
        (when (char= #\( (aref pre (- len 1)))
          ;; ヘッダ: <whole-size> <elm-size> <endian(0|1)>
          (let* ((byte-header (read-line stream t))
                 (whole-size (parse-integer byte-header :junk-allowed t)))
            (declare (fixnum whole-size))
            (pushs byte-header #\Newline ret)
            (tcell-server-dprint "Binary data header: ~A~%" byte-header)
            (let ((start buf-used) (end (+ buf-used whole-size)))
              (when (> end (length buffer))
                (with1 newsize
                    (do ((sz (* 2 (length buffer)) (+ sz sz)))
                        ((>= sz end) sz))
                  (adjust-array buffer newsize)
                  (tcell-server-dprint "Extended buffer size to ~D~%" newsize)))
              (read-sequence buffer stream :start start :end end)
              (push #'(lambda (ostream)
                        (write-sequence buffer ostream :start start :end end))
                    ret))
            (tcell-server-dprint "#<byte-data size=~D>~%" whole-size)
            ;; この後，terminator ")\n" がくるが，
            ;; 次のiterationで，単に他の文字列データと同様に処理
            ))))
    (nreverse ret)))

;;; "task", "rslt", "data" のbody部をistreamから読み込んでostreamに送る
#+comment                               ; write-* の実行がブロックして，デッドロックの可能性
(defun forward-body (istream ostream
                     &optional (line-buffer
                                #+allegro
                                (make-array +max-line-length+
                                            :element-type 'standard-char :fill-pointer +max-line-length+))
                               (buffer (make-array +body-buffer-size+ :element-type 'unsigned-byte))
                               (line-bufsize (length line-buffer))
                               (bufsize (length buffer)))
  (loop
    (setf (fill-pointer line-buffer) line-bufsize)
    (let* ((len #+allegro (setf (fill-pointer line-buffer)
                            (excl:read-line-into line-buffer istream nil 0))
                #-allegro (progn
                            (setq line-buffer (read-line stream nil ""))
                            (length line-buffer)))
           )
      ;; 空行で終了
      (when (= len 0) (return))
      (write-string line-buffer ostream)
      (terpri ostream)
      ;; (tcell-server-dprint "~A~%" line-buffer)
      ;; #\(でおわっていたら次の行はbyte-header，次いでbyte-data
      (when (char= #\( (aref line-buffer (- len 1)))
        #+allegro (setf (fill-pointer line-buffer) line-bufsize)
        ;; ヘッダread: <whole-size> <elm-size> <endian(0|1)>
        #+allegro (setf (fill-pointer line-buffer)
                    (excl:read-line-into line-buffer istream nil 0))
        #-allegro (setq line-buffer (read-line istream nil ""))
        (let* ((whole-size (parse-integer line-buffer :junk-allowed t)))
          (write-string line-buffer ostream)
          (terpri ostream)
          (tcell-server-dprint "Binary data header: ~A~%" line-buffer)
          ;; バイナリ本体
          (loop for rem-size from whole-size downto 1 by bufsize
              as rd-size = (if (> rem-size bufsize) bufsize rem-size)
              do (read-sequence buffer istream :end rd-size)
                 (write-sequence buffer ostream :end rd-size))
          (tcell-server-dprint "#<byte-data size=~D>~%" whole-size)
          ;; この後，terminator ")\n" がくるが，
          ;; 次のiterationで，単に他の文字列データと同様に処理
          )))))

;;; "task", "rslt", "data" のbody部を読み込み，リストにして返す
#+comment
(defun read-body (stream)
  (let ((ret '()))
    (loop
      (let* ((pre (read-line stream t))
             (len (length pre)))
        ;; 空行で終了
        (when (= len 0) (return))
        (pushs pre #\Newline ret)
        ;; (tcell-server-dprint "~A~%" pre)
        ;; #\(でおわっていたら次の行はbyte-header，次いでbyte-data
        (when (char= #\( (aref pre (- len 1)))
          ;; ヘッダ: <whole-size> <elm-size> <endian(0|1)>
          (let* ((byte-header (read-line stream t))
                 (whole-size (parse-integer byte-header :junk-allowed t)))
            (pushs byte-header #\Newline ret)
            (tcell-server-dprint "Binary data header: ~A~%" byte-header)
            (let ((byte-data (make-array whole-size)))
              (read-sequence byte-data stream :end whole-size)
              (push byte-data ret)
              #-tcell-no-transfer-log   ; debug print
              (when *transfer-log* (write-msg-log byte-data *transfer-log-output*))
              )))))
    (nreverse ret)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; バッファ監視->送信
(defgeneric monitor-and-send-queue (sq dest &optional writer))
(defmethod monitor-and-send-queue ((sq shared-queue) dest
                                   &optional (writer #'write-string))
  (let ((gate (sq-gate sq)))
    (loop
      (mp:process-wait "Waiting for something is added to the queue"
                       #'mp:gate-open-p gate)
      (while (mp:gate-open-p gate)
        (funcall writer (delete-queue sq) dest))
      (force-output dest))))

;; 受信->バッファに追加
(defgeneric receive-and-add-to-queue (sq src &optional reader))
(defmethod receive-and-add-to-queue ((sq shared-queue) src
                                     &optional (reader #'read-line))
  (loop
    (multiple-value-bind (obj eof-p) (funcall reader src)
      (add-queue obj sq)
      (when eof-p (return)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 要素へのaccessor

;;; 子の追加・削除
(defgeneric add-child (sv chld))
(defmethod add-child ((sv tcell-server) (chld child))
  ;; 子の情報の初期化．senderとreceiverの起動．
  (setf (child-id chld)	(ts-child-next-id sv))
  (initialize-sender chld)
  (initialize-receiver chld)
  ;; server状態の更新
  (incf (ts-child-next-id sv))
  (incf (ts-n-children sv))
  (push chld (ts-children sv))
  (tcell-print-log "~&Added a new child (~D children in total).~%" (ts-n-children sv))
  (unless (ts-eldest-child sv)
    (setf (ts-eldest-child sv) chld)))

(defgeneric remove-child (sv chld))
(defmethod remove-child ((sv tcell-server) (chld child))
  (cleanup chld)
  (setf (ts-children sv) (delete chld (ts-children sv) :count 1))
  (delete-treq-any sv chld "*")
  (when (eq (ts-eldest-child sv) chld)
    (setf (ts-eldest-child sv) (car (last (ts-children sv)))))
  (decf (ts-n-children sv))
  )

;;; (= id n) の子へのアクセス
(defgeneric nth-child (sv n))
(defmethod nth-child ((sv tcell-server) n)
  (find n (ts-children sv)
        :key #'(lambda (chld) (child-id chld))
        :test #'=))

;;; 一番仕事が残ってそうな子
(defgeneric most-divisible-child (sv from))
(defmethod most-divisible-child ((sv tcell-server) (from host))
  (with* (cur-time (get-internal-real-time)
          limit (* 0 internal-time-units-per-second)
          candidates (remove-if-not #'(lambda (x)
                                        (and (> (child-diff-task-rslt x) 0)
                                             (not (eq x from))
                                             (>= (- cur-time (host-none-time x)) limit)))
                                    (ts-children sv)))
    #+debug     
    (when (typep (ts-parent sv) 'terminal-parent)
      (dolist (c (ts-children sv))
        (print `((from ,(hostid from)) (c ,(hostid c)) (diff-task-rslt ,(child-diff-task-rslt c))
                                       ,(>= (- cur-time (host-none-time c)) limit)))))
    ;; Strategy1: （送ったtaskの数-受け取ったrsltの数）>0 かつ 最後に送ったtaskのndivが最小
    #+comment
    (let ((max nil) (maxchld nil))
      (loop for chld in candidates
          do (when (or (null max)
                       (> (child-wsize chld) max))
               (setq max (child-wsize chld))
               (setq maxchld chld)))
      #-tcell-no-transfer-log
      (when maxchld
        (tcell-server-dprint "~&Most-divisible-child selected ~S: (child-diff-task-rslt ~D), (child-wsize ~D)~%"
                             (hostinfo maxchld) (child-diff-task-rslt maxchld) (child-wsize maxchld)))
      maxchld)
    ;; Strategy2: （送ったtaskの数-受け取ったrsltの数）>0 のものからランダム
    #+swopp10
    (if candidates (list-random-select candidates) nil)
    ;; Strategy3: SWoPP10 random
    #-swopp10
    (when candidates
      (if (not (typep (ts-parent sv) 'terminal-parent))
          (list-random-select candidates)
        (let ((cand (if (< 0.75 (random 1.0))
                        (remove-if-not #'(lambda (c) (<= 2 (child-id c)))
                                       candidates)
                      (remove-if #'(lambda (c) (<= 2 (child-id c)))
                                 candidates))))
          (if cand
              (list-random-select cand)
            (list-random-select candidates)))))
    ))

;;; 子のwork-size を更新
(defgeneric renew-work-size (chld wsize))
(defmethod renew-work-size ((chld child) wsize)
  (setf (child-wsize chld) wsize))

;;; treq-any-list への要素追加
;;; p-task-head は，"<treqのfromのid>" ":" "<treqのtask-head>" 
(defgeneric push-treq-any0 (sv tae))
(defmethod push-treq-any0 ((sv tcell-server) (tae ta-entry))
  (push tae (ts-talist sv)))
(defgeneric push-treq-any1 (sv tae))
(defmethod push-treq-any1 ((sv tcell-server) (tae ta-entry))
  (push tae (cdr (ts-talist sv))))
(defgeneric push-treq-any (sv from p-task-head))
(defmethod push-treq-any ((sv tcell-server) (from host) (p-task-head string))
  (let ((entry (make-instance 'ta-entry :from from :head p-task-head)))
    (if (or (eq from (ts-eldest-child sv)) ; eldest-childからのtreqは優先的に先頭
            (null (ts-talist sv)))      ;  ; （retry時に先頭がだめだったら以降をあきら
        (push-treq-any0 sv entry)       ;  ;   めるようにしているが，その時に親も試す
      (push-treq-any1 sv entry))))      ;  ;   ようにするため）

;;; treq-any-list のpop
(defgeneric pop-treq-any (sv))
(defmethod pop-treq-any ((sv tcell-server))
  (aand (pop (ts-talist sv))
        (list (tae-from it) (tae-head it))))


;;; treq-any-list から指定要素削除
(defgeneric delete-treq-any (sv from p-task-head))
(defmethod delete-treq-any ((sv tcell-server) (from host) (p-task-head string))
  (setf (ts-talist sv)
    (delete (make-instance 'ta-entry :from from :head p-task-head)
            (ts-talist sv)
            :test #'ta-entry-match)))

;;; treq-any-list のmemberか
(defgeneric member-treq-any (sv from p-task-head))
(defmethod member-treq-any ((sv tcell-server) (from host) (p-task-head string))
  (member (make-instance 'ta-entry :from from :head p-task-head)
          (ts-talist sv)
          :test #'ta-entry-match))

;;; ta-entry の同一性
(defgeneric ta-entry-match (x y))
(defmethod ta-entry-match ((x ta-entry) (y ta-entry))
  (and (eq (tae-from x) (tae-from y))
       (or (string= "*" (tae-head x))
           (string= "*" (tae-head y))
           (string= (tae-head x) (tae-head y)))))

;; ta-entry の情報
(defgeneric ta-entry-info (tae))
(defmethod ta-entry-info ((tae ta-entry))
  `((from ,(hostinfo (tae-from tae)))
    (head ,(tae-head tae))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 相対アドレス文字列の操作
(defgeneric hostid (host))
(defmethod hostid ((chld child))
  (format nil "~D" (child-id chld)))

(defmethod hostid ((prnt parent))
  "p")

(defgeneric hostid-to-host (sv hostid))
(defmethod hostid-to-host ((sv tcell-server) hostid)
  (cond
   ((string= "p" hostid)
    (ts-parent sv))
   (t
    (nth-child sv (parse-integer hostid)))))

;;; アドレスの先頭を切り取って，残りと切り取った先頭アドレス
;;; に相当するホストを返す
(defgeneric head-shift (sv head-string))
(defmethod head-shift ((sv tcell-server) head-string)
  (let* ((sp-head (split-string-1 head-string #\:))
         (host (hostid-to-host sv (first sp-head))))
    (unless host
      (warn "Connection from/to ~S does not exist." (first sp-head)))
    (list host (second sp-head))))

;;; アドレスhead-stringの先頭にhstのidを追加したものを返す
(defgeneric head-push (hst head-string))
(defmethod head-push ((hst host) head-string)
  (let ((sp-head (split-string-1 head-string #\:)))
    (if (string= "f" (first sp-head))   ; forward => 追加せず，f以降のアドレスを返す
        (second sp-head)
      (string+ (hostid hst) ":" head-string))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; メッセージ送信
(defgeneric send (to obj))
(defmethod send ((to host) obj)
  (add-queue obj (send-queue (host-sender to))))

(defmethod send ((to (eql nil)) obj)
  (format *error-output* "Failed to send ~S~%" (msg-log-string obj nil)))

;; debug print
#-tcell-no-transfer-log
(defmethod send :after ((to host) obj)
  (tcell-server-dprint "~&~A(~6D): ~A~15T<<< ~A~%"
                       (if (eq :gnuplot *transfer-log-format*) "# " "")
                       (get-internal-real-time)
                       (hostid to) (msg-log-string obj nil)))

(defgeneric send-treq (to task-head treq-head))
(defmethod send-treq (to task-head treq-head)
  (send to (list "treq " task-head #\Space treq-head #\Newline)))

(defmethod send-treq :after ((to host) task-head treq-head)
  (declare (ignore #+no-transfer-log task-head treq-head))
  #-no-transfer-log
  (tcell-server-dprint (get-gnuplot-error-bar task-head to "treq"))
  (incf (host-unreplied-treqs to)))
  
;; treqへの応答として，task，またはexitを自動再送信（バッチ実行用）
(defmethod send-treq :after ((to terminal-parent) task-head treq-head)
  (awhen (parent-auto-treq-response-func to)
    (funcall it to task-head treq-head)
    (setf (parent-auto-treq-response-func to) nil)))

(defgeneric send-task (to wsize-str rslt-head task-head task-no task-body))
(defmethod send-task (to wsize-str rslt-head task-head task-no task-body)
  (send to (list "task "
                 wsize-str #\Space
                 rslt-head #\Space
                 task-head #\Space
                 task-no #\Newline
                 task-body #\Newline)))

(defmethod send-task :after ((to child) wsize-str rslt-head task-head task-no task-body)
  (declare (ignore rslt-head task-head task-no task-body))
  (incf (child-diff-task-rslt to))
  (let ((wsize (parse-integer wsize-str)))
    (renew-work-size to (- wsize))))

(defmethod send-task :after ((to host) wsize-str rslt-head task-head task-no task-body)
  (declare (ignore wsize-str task-head task-no task-body))
  (setf (host-none-time to) -1)
  #-tcell-no-transfer-log
  (when (eq :gnuplot *transfer-log-format*)
    (tcell-server-dprint (get-gnuplot-error-bar rslt-head to "task"))))

(defgeneric send-rslt (to rslt-head rslt-body))
(defmethod send-rslt (to rslt-head rslt-body)
  (send to (list "rslt " rslt-head #\Newline rslt-body #\Newline)))

(defmethod send-rslt :after ((to parent) rslt-head rslt-body)
  (declare (ignore rslt-head rslt-body))
  (decf (parent-diff-task-rslt to)))

(defmethod send-rslt :after ((to terminal-parent) rslt-head rslt-body)
  (declare (ignore rslt-body))
  ;; log
  (tcell-server-dprint "~&# (~D) rslt sent to terminal parent." (get-internal-real-time))
  ;; rack，task，exit自動送信の設定（task再送信は性能評価用）
  (when (parent-auto-rack to)
    (let ((end-time (get-internal-real-time)))
      (aif (find rslt-head (parent-task-home to) :test #'string=
                 :key #'(lambda (x) (third (task-home-task-cmd x))))
          (let* ((cmd (task-home-task-cmd it))
                 (rslt-head (third cmd))
                 (rslt-no (parse-integer rslt-head :junk-allowed t))
                 (rack-to (task-home-rack-to it))
                 (rack-task-head (task-home-rack-task-head it))
                 (start-time (task-home-start-time it)))
            ;; rack返信
            (format *error-output*
                "~&Time: ~S~%~
                Auto-send \"rack ~A\" to ~S~%"
              (/ (- end-time start-time)
                 internal-time-units-per-second 1.0)
              rack-task-head (hostinfo rack-to))
            (send-rack rack-to rack-task-head)
            ;; n-resendチェック→task自動再送信関数をセット
            ;; rslt-no（rslt-headの最後の数字）が送った回数のカウントの役割
            (if (< rslt-no (parent-auto-resend-task to))
                (let ((new-rslt-head (format nil "~D" (1+ rslt-no))))
                  (setf (caddr cmd) new-rslt-head)
                  ;; treqがきたら自動的にtaskを再送信するように関数をセット
                  (setf (parent-auto-treq-response-func to)
                    #'(lambda (&rest args)
                        (declare (ignore args))
                        (sleep 1)
                        (excl:gc t)
                        (format *error-output* "~&auto-resend-task~%")
                        (setf (task-home-start-time it) (get-internal-real-time))
                        (incf (parent-diff-task-rslt to))
                        (send-task rack-to ; rackの送り先と同じでよいことを仮定
                                   (second cmd) (head-push to new-rslt-head)
                                   rack-task-head (fifth cmd) (nthcdr 5 cmd)))))
              (progn
                ;; task-homeのエントリ削除
                (delete it (parent-task-home to) :count 1)
                ;; exitを自動送信するように関数をセット
                (when (parent-auto-exit to)
                  (setf (parent-auto-treq-response-func to)
                    #'(lambda (&rest args)
                        (declare (ignore args))
                        (format *error-output* "~&auto-send-exit~%")
                        (proc-cmd (host-server to) to '("exit"))))))
              ))
        (warn "No task-home corresponding to rslt ~S" rslt-head)))))

(defgeneric send-none (to task-head))
(defmethod send-none (to task-head)
  (send to (list "none " task-head #\Newline)))

(defgeneric send-back (to task-head))
(defmethod send-back (to task-head)
  (send to (list "back " task-head #\Newline)))

(defmethod send-back ((to parent) task-head)
  (declare (ignore task-head))
  (decf (parent-diff-task-rslt to)))

(defgeneric send-rack (to task-head))
(defmethod send-rack (to task-head)
  (send to (list "rack " task-head #\Newline)))

(defgeneric send-bcst (to bcak-head task-no body))
(defmethod send-bcst (to bcak-head task-no body)
  (send to (list "bcst " bcak-head #\Space task-no #\Newline body #\Newline)))

(defgeneric send-bcak (to bcak-head))
(defmethod send-bcak (to bcak-head)
  (send to (list "bcak " bcak-head #\Newline)))

(defgeneric send-dreq (to data-head dreq-head range))
(defmethod send-dreq (to data-head dreq-head range)
  (send to (list "dreq " data-head #\Space dreq-head #\Space range #\Newline)))

(defgeneric send-data (to data-head range data-body))
(defmethod send-data (to data-head range data-body)
  (send to (list "data " data-head #\Space range #\Newline data-body #\Newline)))

(defgeneric send-leav (to))
(defmethod send-leav (to)
  (send to (list "leav" #\Newline)))

(defgeneric send-stat (to task-head))
(defmethod send-stat (to task-head)
  (send to (list "stat " task-head #\Newline)))

(defgeneric send-verb (to task-head))
(defmethod send-verb (to task-head)
  (send to (list "verb " task-head #\Newline)))

(defgeneric send-exit (to))
(defmethod send-exit (to)
  (send to (list "exit" #\Newline)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Dispatch
(defgeneric proc-cmd (sv from cmd))
(defmethod proc-cmd ((sv tcell-server) (from host) cmd)
  (string-case-eager (car cmd)
    ("treq" (proc-treq sv from cmd))
    ("task" (proc-task sv from cmd))
    ("none" (proc-none sv from cmd))
    ("back" (proc-back sv from cmd))
    ("rslt" (proc-rslt sv from cmd))
    ("rack" (proc-rack sv from cmd))
    ("bcst" (proc-bcst sv from cmd))
    ("bcak" (proc-bcak sv from cmd))
    ("dreq" (proc-dreq sv from cmd))
    ("data" (proc-data sv from cmd))
    ("leav" (proc-leav sv from cmd))
    ("log"  (proc-log sv from cmd))
    ("stat" (proc-stat sv from cmd))
    ("verb" (proc-verb sv from cmd))
    ("eval" (print (eval (read-from-string (strcat (cdr cmd) #\Space)))))
    ("exit" (proc-exit sv from cmd))
    (otherwise (warn "Unknown Command:~S" cmd))))

;;; treq
(defgeneric proc-treq (sv from cmd))
(defmethod proc-treq ((sv tcell-server) (from host) cmd)
  (let ((p-task-head (head-push from (second cmd))) ; タスク要求者
        (treq-head (third cmd)))        ; 要求先
    (unless
        (if (string= "any" treq-head)
            (try-send-treq-any sv from p-task-head)
          (destructuring-bind (hst0 s-treq-head)
              (head-shift sv treq-head)
            (if *transfer-treq-always-if-notany*
                (send-treq hst0 p-task-head s-treq-head)
              (try-send-treq sv hst0 p-task-head s-treq-head))))
      ;; treqを送れなかった場合
      (refuse-treq sv from p-task-head))))

(defgeneric try-send-treq-any (sv from p-task-head))
(defmethod try-send-treq-any ((sv tcell-server) (from host) p-task-head)
  (or
   ;; 時々優先して親にも聞きにいく（terminal-parentを除く）
   (and (not (typep (ts-parent sv) 'terminal-parent))
        (not (eq (ts-parent sv) from))
        #-swopp10 (= 0 (random (ts-n-children sv)))
        #+swopp10 (< 0.75 (random 1.0))
        (try-send-treq sv (ts-parent sv) p-task-head "any"))
   ;; 子供に聞きにいく
   (awhen (most-divisible-child sv from)
     (try-send-treq sv it p-task-head "any"))
   ;; 自分のところに仕事がなければ，eldestな子が代表して親に聞きにいく
   (and (eq (ts-eldest-child sv) from)
        (try-send-treq sv (ts-parent sv)
                       p-task-head "any"))))

(defgeneric try-send-treq (sv to p-task-head s-treq-head))
(defmethod try-send-treq ((sv tcell-server) (to host) p-task-head s-treq-head)
  (send-treq to p-task-head s-treq-head)
  t)

;; terminal-parentについては，
;; もらった仕事を消化している，かつ，一度に1つまでしかtreqしない
(defmethod try-send-treq :around ((sv tcell-server) (to terminal-parent) p-task-head s-treq-head)
  (declare (ignore p-task-head s-treq-head))
  ;; (print `((host-unreplied-treqs to) ,(host-unreplied-treqs to)))
  (if (and (>= 0 (parent-diff-task-rslt to))
           (>= 0 (host-unreplied-treqs to)))
      (call-next-method)
    nil))

;; childについては，渡したtaskと返ってきたrsltが同数ならtreqしない
(defmethod try-send-treq :around ((sv tcell-server) (to child) p-task-head s-treq-head)
  (declare (ignore p-task-head s-treq-head))
  (if (> (child-diff-task-rslt to) 0)
      (call-next-method)
    nil))

(defgeneric refuse-treq (sv from p-task-head))
(defmethod refuse-treq ((sv tcell-server) (from host) p-task-head)
  (if (member-treq-any sv from p-task-head)
      (send-none from (second (head-shift sv p-task-head)))
    (push-treq-any sv from p-task-head)))

;; treq-any-listにある要素を tryしなおす
(defgeneric retry-treq (sv))
(defmethod retry-treq ((sv tcell-server))
  #+obsoleted ; 最初のentryが失敗したからといって他のentryも失敗するとは限らない
  (loop
      for n-sent upfrom 0
      do (aif (pop-treq-any sv)
             (destructuring-bind (from head) it
               (unless (try-send-treq-any sv from head)
                 (push-treq-any sv from head)
                 (loop-finish)))
           (loop-finish))
      finally (return n-sent))
  (let ((failed-list))
    (awhile (pop-treq-any sv)
      (destructuring-bind (from head) it
        (unless (try-send-treq-any sv from head)
          (push it failed-list))))
    (dolist (f failed-list)
      (destructuring-bind (from head) f
        (push-treq-any sv from head))))
  )

;;; task
(defgeneric proc-task (sv from cmd))
(defmethod proc-task ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (fourth cmd))      ; タスク送信先
    (let ((wsize-str (second cmd))      ; 仕事の大きさ
          (p-rslt-head (head-push from (third cmd))) ; 結果返信先
          (task-no (fifth cmd))         ; タスク番号 （fib, lu, ...）
          (task-body (nthcdr 5 cmd)))
      (send-task to wsize-str p-rslt-head s-task-head task-no
                 task-body)
      )))

(defmethod proc-task :before ((sv tcell-server) (from host) cmd)
  (declare (ignorable sv cmd))
  (decf (host-unreplied-treqs from)))

(defmethod proc-task :before ((sv tcell-server) (from child) cmd)
  (let ((wsize (parse-integer (second cmd))))
    (renew-work-size from (- wsize))))

(defmethod proc-task :before ((sv tcell-server) (from parent) cmd)
  (declare (ignorable sv cmd))
  (incf (parent-diff-task-rslt from))
  )

;; rack自動送信のために受け取ったtaskを覚えておく
(defmethod proc-task :after ((sv tcell-server) (from terminal-parent) cmd)
  (when (parent-auto-rack from)
    (destructuring-bind (to s-task-head)
        (head-shift sv (fourth cmd))
      (let ((th-entry (make-task-home
                       :task-cmd cmd
                       :rack-to   to    ; rack送信先
                       :rack-task-head s-task-head ; rackのtask-head
                       :start-time 0))) ; 開始時刻（あとで）
        (push th-entry (parent-task-home from))
        (setf (task-home-start-time th-entry) (get-internal-real-time))))))

;;; none
(defgeneric proc-none (sv from cmd))
(defmethod proc-none ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (second cmd))      ; none送信先
    (send-none to s-task-head)))

(defmethod proc-none :before ((sv tcell-server) (from host) cmd)
  (declare (ignorable sv #+no-transfer-log cmd))
  #-no-transfer-log
  (tcell-server-dprint (get-gnuplot-error-bar from (second cmd) "none"))
  (decf (host-unreplied-treqs from)))


(defmethod proc-none :after ((sv tcell-server) (from host) cmd)
  ;; noneを受け取った時刻を記憶
  (declare (ignorable sv cmd))
  (setf (host-none-time from) (get-internal-real-time)))

;;; back
(defgeneric proc-back (sv from cmd))
(defmethod proc-back ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (second cmd))      ; back送信先
    (send-back to s-task-head)))

(defmethod proc-back :before ((sv tcell-server) (from child) cmd)
  (declare (ignore cmd))
  (when (< (decf (child-diff-task-rslt from)) 0)
    (warn "~S: diff-task-rslt less than 0!" (hostinfo from))))

;;; rslt
(defgeneric proc-rslt (sv from cmd))
(defmethod proc-rslt ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-rslt-head)
      (head-shift sv (second cmd))      ; rslt送信先
    (let ((rslt-body (cddr cmd)))
      (send-rslt to s-rslt-head rslt-body))))

(defmethod proc-rslt :before ((sv tcell-server) (from child) cmd)
  (declare (ignore cmd))
  (when (< (decf (child-diff-task-rslt from)) 0)
    (warn "~S: diff-task-rslt less than 0!" (hostinfo from))))

;;; rack
(defgeneric proc-rack (sv from cmd))
(defmethod proc-rack ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (second cmd))      ; rack送信先
    (send-rack to s-task-head)))

;; bcst
(defgeneric proc-bcst (sv from cmd))
(defmethod proc-bcst ((sv tcell-server) (from host) cmd)
  (let ((p-bcak-head (head-push from (second cmd))) ; bcak返信先
	(task-no (third cmd))		; タスク番号 （btsk_a, btsk_b, ...)
	(bcst-body (nthcdr 3 cmd)))	; data部
    (let ((recipients ()))
      ;; bcstの送り元とterminal-parent以外にforwardする
      (dolist (to (cons (ts-parent sv) (ts-children sv)))
	(unless (or (eq from to)
		    (typep to 'terminal-parent))
	  (send-bcst to p-bcak-head task-no bcst-body)
	  (push to recipients)))
      ;; (<bcak返信先> . <forward先のリスト>)を記憶する
      (when (member p-bcak-head (ts-bcst-receipants sv)
		    :key #'car :test #'string=)
	(warn "Server received the same broadcast twice: ~S"
	      p-bcak-head))
      (if recipients
          (push (cons p-bcak-head recipients) (ts-bcst-receipants sv))
        ;; broadcast先がいなければ即座にbcakを返す
        (send-bcak from (second cmd)))
      )))

;; bcak
(defgeneric proc-bcak (sv from cmd))
(defmethod proc-bcak ((sv tcell-server) (from host) cmd)
  (let ((bcak-head (second cmd)))
    (destructuring-bind (to s-bcak-head) ; bcak送信先
	(head-shift sv bcak-head)
      (let ((receipants-entry		
	     (car (member bcak-head (ts-bcst-receipants sv)
			  :key #'car :test #'string=))))
	(if (null receipants-entry)	; bcak-headからのbcstがあったかチェック
	    (warn "No bcst from ~S is remenbered." bcak-head)
	  (if (not (member from (cdr receipants-entry) :test #'eq))
					; fromがback待ちリストにあるかチェック
	      (warn "No bcst from ~S to ~S is remembered."
		    bcak-head (hostinfo from))
	    (progn
	      ;; back待ちからfromを削除
	      (rplacd receipants-entry
		      (delete from (cdr receipants-entry) :test #'eq))
	      ;; 待ちリストが空になっていたらbackを返す
	      (when (null (cdr receipants-entry))
		(send-bcak to s-bcak-head)
                (setf (ts-bcst-receipants sv)
                  (delete receipants-entry (ts-bcst-receipants sv) :test #'eq)))
	      )))))))

;;; dreq
(defgeneric proc-dreq (sv from cmd))
(defmethod proc-dreq ((sv tcell-server) (from host) cmd)
  (let ((p-data-head (head-push from (second cmd))) ; データ要求者
        (range (fourth cmd)))           ; データ要求範囲
    (destructuring-bind (hst0 s-dreq-head) ; データ要求先
        (head-shift sv (third cmd))
      (send-dreq hst0 p-data-head s-dreq-head range))))

;;; data
(defgeneric proc-data (sv from cmd))
(defmethod proc-data ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-data-head)  ; data送信先
      (head-shift sv (second cmd))
    (let ((range (third cmd))           ; データ要求範囲
          (data-body (cdddr cmd)))      ; データ本体
      (send-data to s-data-head range data-body))))

;;; leav
;; 子から→登録を外す．親から→無視
(defgeneric proc-leav (sv from cmd))
(defmethod proc-leav ((sv tcell-server) (from child) cmd)
  (declare (ignore cmd))
  (remove-child sv from))

(defmethod proc-leav ((sv tcell-server) (from parent) cmd)
  (declare (ignore cmd))
  )

;;; stat: サーバ／ワーカの状態を出力
(defgeneric proc-stat (sv from cmd))
(defmethod proc-stat ((sv tcell-server) (from host) cmd)
  (if (cdr cmd)
      (cond
       ;; anyを与えた場合は自分の状態を表示後，全ての子供にstat anyを転送
       ((string= "any" (second cmd))
        (mp:process-run-function "Send Status Any"
          #'(lambda ()
              (print-server-status sv)
              (dolist (chld (ts-children sv))
                (format *error-output* "~&Host: ~A~%" (hostid chld))
                (send-stat chld "any")
                (sleep 0.3)))))
       ;; アドレスを与えた場合はそこにstatコマンドを転送
       (t
        (destructuring-bind (to s-task-head)
            (head-shift sv (second cmd)) ; stat送信先
          (send-stat to s-task-head))))
    ;; 無引数の場合はサーバの状態を表示
    (print-server-status sv)))

;;; verb: ワーカのverbose-levelを変更
;;; "verb <送信先>:<level>" で．
(defgeneric proc-verb (sv from cmd))
(defmethod proc-verb ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (second cmd))      ; verb送信先
    (send-verb to s-task-head)))

;;; log: サーバのverbose-levelを設定
(defgeneric proc-log (sv from cmd))
(defmethod proc-log ((sv tcell-server) (from host) cmd)
  (loop
      for str in (cdr cmd)
      as mode = (read-from-string str)
      as setter in (list #'toggle-transfer-log #'toggle-connection-log)
      do (funcall setter mode))
  (show-log-mode))

;;; exit: サーバを終了
(defgeneric proc-exit (sv from cmd))
(defmethod proc-exit ((sv tcell-server) (from host) cmd)
  (declare (ignore cmd))
  (mp:open-gate (ts-exit-gate sv)))

;;; logging related functions
(defun toggle-transfer-log (mode)
  (setq *transfer-log* (not (not mode))))

(defun toggle-connection-log (mode)
  (setq *connection-log* (not (not mode))))

(defun show-log-mode ()
  (pprint `((*transfer-log* #-tcell-no-transfer-log ,*transfer-log*
                            #+tcell-no-transfer-log :invalidated-by-features)
            (*connection-log* ,*connection-log*))
          *error-output*)
  (terpri *error-output*)
  (force-output *error-output*))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun make-and-start-server (&rest args
                              &key 
                              (local-host *server-host*)
                              (children-port *children-port*)
                              (n-wait-children 0)
                              (retry *retry*)
                              (terminal-parent t)
                              (auto-rack t) ; for terminal parent
                              (auto-initial-task nil)
                                        ; 最初に自動的に送信するtask
                                        ; "<task-no> <param1> <param2> ..." の文字列で指定
                                        ; （param間の改行や最後の改行および空行不要）
                              (auto-resend-task 0) ; for terminal-parent
                              (auto-exit nil) ; for terminal parent
                              (parent-host *parent-host*)
                              (parent-port *parent-port*)
                              (socket-format *socket-format*)
                              )
  (when parent-host (setq terminal-parent nil))
  (let* ((sv (make-instance 'tcell-server
               :local-host local-host
               :children-port children-port
               :n-wait-children n-wait-children
               :socket-format socket-format
               :retry retry))
         (prnt (if terminal-parent
                   (make-instance 'terminal-parent
                     :server sv :auto-rack auto-rack
                     :auto-resend-task auto-resend-task
                     :auto-exit auto-exit)
                 (make-instance 'parent :server sv
                                :host parent-host
                                :port parent-port))))
    ;; auto-initial-task のセット
    (when (and terminal-parent auto-initial-task)
      (when (stringp auto-initial-task)
        (setq auto-initial-task (split-string auto-initial-task)))
      (with (task-no (car auto-initial-task)
             task-body (strcat (cdr auto-initial-task) #\Newline "" #\Newline))
        (setf (parent-auto-treq-response-func prnt)
          #'(lambda (to task-head treq-head &rest args)
              (declare (ignore treq-head args))
              (sleep 1)
              (excl:gc t)
              (format *error-output* "~&auto-send-initial-task~%")
              (with1 msg (list* "task" "0" "0" task-head task-no task-body)
                (add-queue (cons to msg) (ts-queue sv)))))))
    #+debug
    (when terminal-parent
      (trace try-send-treq most-divisible-child))
    #+debug
    (when terminal-parent
      (mp:process-run-function "Print Status"
        #'(lambda (sv)
            (loop
              (sleep 10)
              (print-server-status sv)
              (dolist (chld (ts-children sv))
                (format *error-output* "~&Host: ~A~%" (hostid chld))
                (send-stat chld "any")
                (sleep 0.3))
              ))
        sv))
    ;; サーバ起動
    (start-server sv prnt)))
