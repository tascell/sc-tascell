(defpackage "TCELL"
  (:shadow cl:declaration)
  (:use "RULE" "CL" "SC-MISC")
  (:export :with-environment-bound :task-maps :with-task
           :with-new-bk :latest-bk
           :task-cid :current-task :add-task
           :make-task-struct-id :task-struct-id :task-no
           :task-body-function
           :set-task-send :set-task-recv :set-rslt-send :set-rslt-recv
           :task-add-field :task-add-input-var :task-add-output-var
           :task-field-p
           :sender-and-receiver-functions-all
           :nestfunc-type))
(in-package "TCELL")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; task の管理
(defvar *tasks* (list))
(defvar *current-task* nil)
(defstruct (task-info
            (:constructor create-task-info
                          (cid no 
                               &aux
                               (struct-id (make-task-struct-id cid))
                               (do-task (make-do-task-id cid))
                               (task-send (make-task-send-id cid))
                               (task-recv (make-task-recv-id cid))
                               (rslt-send (make-rslt-send-id cid))
                               (rslt-recv (make-rslt-recv-id cid)) )))
  (cid "" :type string)
  (no -1 :type fixnum)                     ; task の通し番号
  (struct-id nil :type symbol)
  (do-task   nil :type symbol)
  (task-send nil :type symbol)  (task-send-body nil)
  (task-recv nil :type symbol)  (task-recv-body nil)
  (rslt-send nil :type symbol)  (rslt-send-body nil)
  (rslt-recv nil :type symbol)  (rslt-recv-body nil)
  (fields (list))                       ; list of (<id> <texp>)
  ;; fields のうち :in, :out指定されたもの
  ;; <tsize>: 転送する配列の要素数．整数 or taskメンバ変数．
  (input-vars (list))                   ; list of (<id> <texp> <size>)
  (output-vars (list))                  ; list of (<id> <texp> <size>)
  )

;; create new-task
(defun add-task (cid)
  (push (create-task-info cid (length *tasks*)) *tasks*))
(defun get-task (task-or-cid)
  (or (and (task-info-p task-or-cid) task-or-cid)
      (find task-or-cid *tasks* :key #'task-info-cid :test #'string=)
      (error "Task ~S is undefined." task-or-cid)))

;; task番号と処理関数への対応表（配列）を定義するコード
(defun task-maps ()
  (let ((rev-tasks (reverse *tasks*))
        (len ~TASK-MAX)                 ; worker4.sh で %defconstant
        (doer-type      ~(ptr ,(task-body-type ~void)))
        (tsender-type   ~(ptr ,(task-sender-type ~void)))
        (treceiver-type ~(ptr ,(task-receiver-type ~void)))
        (rsender-type   ~(ptr ,(rslt-sender-type ~void)))
        (rreceiver-type ~(ptr ,(rslt-receiver-type ~void))))
    (list
     ~(def task-doers (array ,doer-type ,len)
           (array ,@(mapcar (compose (tagger ~cast doer-type) #'do-task-id)
                            rev-tasks)))
     ~(def task-senders (array ,tsender-type ,len)
           (array ,@(mapcar (compose (tagger ~cast tsender-type) #'task-send-id)
                            rev-tasks)))
     ~(def task-receivers (array ,treceiver-type ,len)
           (array ,@(mapcar (compose (tagger ~cast treceiver-type) #'task-recv-id)
                            rev-tasks)))
     ~(def rslt-senders (array ,rsender-type ,len)
           (array ,@(mapcar (compose (tagger ~cast rsender-type) #'rslt-send-id)
                            rev-tasks)))
     ~(def rslt-receivers (array ,rreceiver-type ,len)
           (array ,@(mapcar (compose (tagger ~cast rreceiver-type) #'rslt-recv-id)
                            rev-tasks)))  )
    ))

;; make task-no, struct/function identifier
(defun make-task-struct-id (cid)
  ;;(generate-id (string+ "_task_" cid ))
  (make-id cid))
(defun make-do-task-id (cid)
  (generate-id (string+ "do_" cid "_task")))
(defun make-task-send-id (cid)
  (generate-id (string+ "send_" cid "_task")))
(defun make-task-recv-id (cid)
  (generate-id (string+ "recv_" cid "_task")))
(defun make-rslt-send-id (cid)
  (generate-id (string+ "send_" cid "_rslt")))
(defun make-rslt-recv-id (cid)
  (generate-id (string+ "recv_" cid "_rslt")))

;; accessor
(defun task-cid (&optional (task-or-cid *current-task*))
  (task-info-cid (get-task task-or-cid)))
(defun task-no (&optional (task-or-cid *current-task*))
  (task-info-no (get-task task-or-cid)))
(defun task-struct-id (&optional (task-or-cid *current-task*))
  (task-info-struct-id (get-task task-or-cid)))
(defun do-task-id (&optional (task-or-cid *current-task*))
  (task-info-do-task (get-task task-or-cid)))
(defun task-send-id (&optional (task-or-cid *current-task*))
  (task-info-task-send (get-task task-or-cid)))
(defun task-recv-id (&optional (task-or-cid *current-task*))
  (task-info-task-recv (get-task task-or-cid)))
(defun rslt-send-id (&optional (task-or-cid *current-task*))
  (task-info-rslt-send (get-task task-or-cid)))
(defun rslt-recv-id (&optional (task-or-cid *current-task*))
  (task-info-rslt-recv (get-task task-or-cid)))
(defun task-add-field (id texp &optional (task-or-cid *current-task*))
  (push (list id texp)
        (task-info-fields (get-task task-or-cid))))
(defun task-field-p (id &optional (task-or-cid *current-task*))
  (member id (task-info-fields (get-task task-or-cid))
          :test #'eq :key #'car))
(defun task-add-input-var (id texp size &optional (task-or-cid *current-task*))
  (push (list id texp size)
        (task-info-input-vars (get-task task-or-cid))))
(defun task-add-output-var (id texp size &optional (task-or-cid *current-task*))
  (push (list id texp size)
        (task-info-output-vars (get-task task-or-cid))))

;;; do-task を作る
(defun task-body-function (body &optional (task *current-task*))
  (let ((id (do-task-id task))
        (struct-id (task-struct-id task)))
    ~(def (,id -thr pthis) ,(task-body-type ~(struct ,struct-id))
          ,.(unless (ruleset-param 'rule::no-nestfunc)
              (list
               ~(def (-bk) ,(nestfunc-type) (return 0)) ))
          ,@body)
    ))

;;; task/rslt sender/receiver （変形済み）本体のset
(defun set-task-send (body &optional (task *current-task*))
  (setf (task-info-task-send-body task) body))
(defun set-task-recv (body &optional (task *current-task*))
  (setf (task-info-task-recv-body task) body))
(defun set-rslt-send (body &optional (task *current-task*))
  (setf (task-info-rslt-send-body task) body))
(defun set-rslt-recv (body &optional (task *current-task*))
  (setf (task-info-rslt-recv-body task) body))

;;; task/rslt sender/receiver を作る．
(defun sender-and-receiver-functions-all ()
  (mapcan #'sender-and-receiver-functions *tasks*))

(defun sender-and-receiver-functions (task)
  (mapcar #'(lambda (f) (funcall f task))
          (list #'task-send-function
                #'task-recv-function
                #'rslt-send-function
                #'rslt-recv-function)))

(defun task-send-function (task)
  (let ((id (task-send-id task))
        (struct-id (task-struct-id task)))
    ~(def (,id pthis) ,(task-sender-type ~(struct ,struct-id))
          ,@(mapcar #'make-send-var (reverse (task-info-input-vars task)))
          ,@(task-info-task-send-body task))
    ))

(defun task-recv-function (task)
  (let ((id (task-recv-id task))
        (struct-id (task-struct-id task)))
    ~(def (,id) ,(task-receiver-type ~(struct ,struct-id))
          (def pthis (ptr (struct ,struct-id))
            (csym::malloc (sizeof (struct ,struct-id))))
          ,@(mapcar #'make-recv-var (reverse (task-info-input-vars task)))
          ,@(task-info-task-recv-body task)
          (return pthis))
    ))

(defun rslt-send-function (task)
  (let ((id (rslt-send-id task))
        (struct-id (task-struct-id task)))
    ~(def (,id pthis) ,(rslt-sender-type ~(struct ,struct-id))
          ,@(mapcar #'make-send-var (reverse (task-info-output-vars task)))
          ,@(task-info-rslt-send-body task)
          (csym::free pthis))))

(defun rslt-recv-function (task)
  (let ((id (rslt-recv-id task))
        (struct-id (task-struct-id task)))
    ~(def (,id pthis) ,(rslt-receiver-type ~(struct ,struct-id))
          ,@(mapcar #'make-recv-var (reverse (task-info-output-vars task)))
          ,@(task-info-rslt-recv-body task))
    ))


;; !!! 今のところ適当
(defvar *var-sender*
    ~((int . csym::send-int) (long . csym::send-long) (double . csym::send-double)))
(defvar *var-receiver*
    ~((int . csym::recv-int) (long . csym::recv-long) (double . csym::recv-double)))

;; i-t-s == (<id> <type> <size(or nil)>)
(defun make-send-var (i-t-s)
  (destructuring-bind (id texp tsize) i-t-s
    (make-sendrecv-var-array id texp tsize () #'make-send-var-unit)))

(defun make-recv-var (i-t-s)
  (destructuring-bind (id texp tsize) i-t-s
    (make-sendrecv-var-array id texp tsize () #'make-recv-var-unit)))

;; 配列を送受信するforループを作る．sr-unitで送信or受信を指定
(defun make-sendrecv-var-array (id texp tsize subsc sr-unit)
  (if (and (consp texp) (eq ~array (car texp)))
      (let ((tsize-exp (or (car tsize) (third texp)))
            (i-var (generate-id "i"))
            (s-var (generate-id "sz"))
            (next-texp (if (cdddr texp)
                           ~(,@(firstn texp 2) ,@(cdddr texp))
                         (second texp))))
        (assert tsize-exp)
        ~(begin
          (def ,i-var int)
          (def ,s-var int ,tsize-exp)
          (for ((= ,i-var 0) (< ,i-var ,s-var) (inc ,i-var))
            ,(make-sendrecv-var-array
              id next-texp (cdr tsize) (append1 subsc i-var) sr-unit)))
        )
    (let ((make-aref (if subsc
                         #'(lambda (x) ~(aref ,x ,@subsc))
                       #'identity)))
      (funcall sr-unit id make-aref texp))))

(defun make-send-var-unit (id make-aref texp)
  (aif (assoc texp *var-sender*)
      ~(,(cdr it) ,(funcall make-aref ~(fref (mref pthis) ,id)))
    (error "Default sender for ~S is not prepeared." texp)))
(defun make-recv-var-unit (id make-aref texp)
  (aif (assoc texp *var-receiver*)
      ~(= ,(funcall make-aref ~(fref (mref pthis) ,id)) (,(cdr it)))
    (error "Default receiver for ~S is not prepeared." texp)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; 入れ子関数の型
(defun nestfunc-type (&optional (fn-tag (ruleset-param 'rule::nestfunc-tag)))
  ~(,fn-tag int void))

;; タスク授受，実行関数の型
(defun task-body-type (data-type)
  ~(fn void (ptr (struct thread-data)) (ptr ,data-type)) )
(defun task-sender-type (data-type)
  ~(csym::fn void (ptr ,data-type)) )
(defun task-receiver-type (data-type)
  ~(csym::fn (ptr ,data-type) ))
(defun rslt-sender-type (data-type)
  ~(csym::fn void (ptr ,data-type)) )
(defun rslt-receiver-type (data-type)
  ~(csym::fn void (ptr ,data-type)) )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 直近の入れ子関数
(defvar *latest-bk* nil)

(defun latest-bk ()
  (if *latest-bk*
      *latest-bk*
    (progn
      (warn "*latest-bk* not set!")
      nil)))

(defmacro with-new-bk (id &body body)
  `(let ((*latest-bk* ,id))
     ,@body))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmacro with-environment-bound (&body body)
  `(let ((*tasks* ())
         (*current-task* nil)
         (*latest-bk* nil))
     ,@body))

(defmacro with-task (task-or-cid &body body)
  `(let ((*current-task* (get-task ,task-or-cid)))
     ,@body))

(defun current-task ()
  *current-task*)
