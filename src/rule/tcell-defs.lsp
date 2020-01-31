;;; Copyright (c) 2008-2019 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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

;;; Utilities for tcell.rule

(defpackage "TCELL"
  (:shadow cl:declaration)
  (:use "RULE" "CL" "SC-MISC" "TYPE")
  (:export :with-environment-bound :task-maps :with-task
           :with-new-bk :latest-bk
           :task-id :task-cid :current-task :add-task
           :make-task-struct-id :task-struct-id :task-no
           :task-body-function :add-backtrack-sentinel :nonlocal-goto
           :set-task-send :set-task-recv :set-rslt-send :set-rslt-recv
           :task-add-field :task-add-input-var :task-add-output-var
           :task-field-p
           :sender-and-receiver-functions-all
           :entry-worker-data :entry-worker-init :*worker-init-args* :*worker-init-type*
           :with-worker-data :wdata-accessible-p
           :make-dummy-worker-data-if-needed
           :nestfunc-type
           :add-defined-func-name :func-name-exists-p
	   :add-toplevel-pre :additional-toplevel-declarations-pre
	   :add-toplevel-post :additional-toplevel-declarations-post
	   :set-tcell-main-defined :make-dummy-tcell-main-if-needed
     :in-or-out :with-type-info-ruleset :add-pthis :simple-syntax-addexpr
           ))
(in-package "TCELL")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; task の管理
(defvar *tasks* (list))
(defvar *current-task* nil)
(defstruct (task-info
            (:constructor create-task-info
                          (id no
                              &aux
                              (cid (rule:identifier id :sc2c))
                              (struct-id (make-task-struct-id cid))
                              (do-task (make-do-task-id cid))
                              (task-send (make-task-send-id cid))
                              (task-recv (make-task-recv-id cid))
                              (rslt-send (make-rslt-send-id cid))
                              (rslt-recv (make-rslt-recv-id cid)) )))
  (id nil :type symbol)
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
(defun add-task (scid)
  (push (create-task-info scid (length *tasks*)) *tasks*))
(defun get-task (task-or-scid)
  (or (and (task-info-p task-or-scid) task-or-scid)
      (find task-or-scid *tasks* :key #'task-info-id :test #'eq)
      (error "Task ~S is undefined." task-or-scid)))
 
;; task番号と処理関数への対応表（配列）を定義するコード
(defun task-maps ()
  (let ((rev-tasks (reverse *tasks*))
        (len ~TASK-MAX)                 ; worker.sh で %defconstant
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
(defun task-id (&optional (task-or-scid *current-task*))
  (task-info-id (get-task task-or-scid)))
(defun task-cid (&optional (task-or-scid *current-task*))
  (task-info-cid (get-task task-or-scid)))
(defun task-no (&optional (task-or-scid *current-task*))
  (task-info-no (get-task task-or-scid)))
(defun task-struct-id (&optional (task-or-scid *current-task*))
  (task-info-struct-id (get-task task-or-scid)))
(defun do-task-id (&optional (task-or-scid *current-task*))
  (task-info-do-task (get-task task-or-scid)))
(defun task-send-id (&optional (task-or-scid *current-task*))
  (task-info-task-send (get-task task-or-scid)))
(defun task-recv-id (&optional (task-or-scid *current-task*))
  (task-info-task-recv (get-task task-or-scid)))
(defun rslt-send-id (&optional (task-or-scid *current-task*))
  (task-info-rslt-send (get-task task-or-scid)))
(defun rslt-recv-id (&optional (task-or-scid *current-task*))
  (task-info-rslt-recv (get-task task-or-scid)))
(defun task-add-field (id texp &optional (task-or-scid *current-task*))
  (push (list id texp)
        (task-info-fields (get-task task-or-scid))))
(defun task-field-p (id &optional (task-or-scid *current-task*))
  (member id (task-info-fields (get-task task-or-scid))
          :test #'eq :key #'car))
(defun task-add-input-var (id texp size &optional (task-or-scid *current-task*))
  (push (list id texp size)
        (task-info-input-vars (get-task task-or-scid))))
(defun task-add-output-var (id texp size &optional (task-or-scid *current-task*))
  (push (list id texp size)
        (task-info-output-vars (get-task task-or-scid))))

;;; Generate the body of "task-body" method.
;;; The generated function is set to task-doers[] and called from
;;; recv-exec-send() in "worker.sc".
(defun task-body-function (body &optional (task *current-task*))
  (let ((id (do-task-id task))
	(struct-id (task-struct-id task)))
    ~(def (,id -thr pthis) ,(task-body-type ~(struct ,struct-id))
       ,@(add-backtrack-sentinel body))
    ))

;; Take a function body and add a nested function for terminating temporarily backtrackings
;; and a label definition for aborting a task.
(defun add-backtrack-sentinel (body &optional (return-value ~(%splice)))
  (let ((label-id (generate-id "task_exit")))
    ~(,@(unless (ruleset-param 'rule::no-exception)
	  (list ~(def ,label-id __label__)))
      ,@(unless (ruleset-param 'rule::no-nestfunc)
	  (list
	   ;; Nested function
	   ~(def (-bk) ,(nestfunc-type)
		 ;; * When worker is propagating an exception or cancelling the task,
		 ;;   exit the task with returning an abort flag.
		 (if (or (== -thr->exiting EXITING-EXCEPTION)
			 (== -thr->exiting EXITING-CANCEL))
		     (begin
		      ,(nonlocal-goto label-id)))
		 ;; * The terminal of temporary backtracking
		 (return 0)) ))
      ,@body
      (label ,label-id (return ,return-value)))))

(defun nonlocal-goto (label-id)
  (if (not (ruleset-param 'rule::no-exception))
      ~(goto ,label-id)
      ~(begin
         (csym::fprintf stderr "Error: Exception is thrown but this program is complied invalidating exception support.~%")
         (csym::exit 99) )))

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
    (warn "Default sender for ~S is not prepeared." texp)))
(defun make-recv-var-unit (id make-aref texp)
  (aif (assoc texp *var-receiver*)
      ~(= ,(funcall make-aref ~(fref (mref pthis) ,id)) (,(cdr it)))
    (warn "Default receiver for ~S is not prepeared." texp)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; worker-data管理
(defvar *defined-worker-data* nil)
(defvar *defined-worker-init* nil)
(defvar *wdata-accessible* nil)
(defconstant *worker-init-args* ~(-thr))
(defconstant *worker-init-type* ~(csym::fn void (ptr (struct thread-data))))

(defun entry-worker-data ()
  (setq *defined-worker-data* t))
(defun entry-worker-init ()
  (setq *defined-worker-init* t))

(defun make-dummy-worker-data-if-needed ()
  (append
   (unless *defined-worker-data*
     (list ~(def (struct worker-data))) )
   (unless *defined-worker-init*
     (list ~(def (csym::worker-init ,@*worker-init-args*) ,*worker-init-type*)) ) )
  )

(defmacro with-worker-data (&body body)
  `(let ((*wdata-accessible* t))
     ,@body))

(defun wdata-accessible-p ()
  *wdata-accessible*)

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
;; 定義された関数名リスト
(defvar *defined-func-names* ())

(defun add-defined-func-name (id)
  (pushnew id *defined-func-names*))

(defun func-name-exists-p (id)
  (member id *defined-func-names*))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; トップレベルに追加すべき宣言のリスト
(defvar *additional-toplevel-declarations-pre* ())
(defvar *additional-toplevel-declarations-post* ())

(defun add-toplevel-pre (decl)
  (push decl *additional-toplevel-declarations-pre*))
(defun additional-toplevel-declarations-pre ()
  (reverse *additional-toplevel-declarations-pre*))

(defun add-toplevel-post (decl)
  (push decl *additional-toplevel-declarations-post*))
(defun additional-toplevel-declarations-post ()
  (reverse *additional-toplevel-declarations-post*))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; tcell-main関数の定義の有無および無い場合のダミー生成
(defvar *defined-tcell-main-p* nil)

(defun set-tcell-main-defined ()
  (setq *defined-tcell-main-p* t))

(defun make-dummy-tcell-main-if-needed ()
  (if *defined-tcell-main-p*
      (list 
       ~(%splice))
      (list
       ~(def (tcell-main -thr argc argv)
            (fn int (ptr (struct thread-data)) int (ptr (ptr char)))
          (csym::fprintf stderr "No tcell-main function defined.~%") ))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defmacro with-environment-bound (&body body)
  `(let ((*tasks* ())
         (*current-task* nil)
         (*latest-bk* nil)
         (*defined-func-names* ())
	 (*additional-toplevel-declarations-pre* ())
	 (*additional-toplevel-declarations-post* ())
	 (*defined-tcell-main-p* nil)
         (*worker-init-data* t) (*worker-init-body* t) (*wdata-accessible* nil))
     ,@body))

(defmacro with-task (task-or-scid &body body)
  `(let ((*current-task* (get-task ,task-or-scid)))
     (with-worker-data
         ,@body)))

(defun current-task ()
  *current-task*)




(defun in-or-out (attr)
  (case attr ((:in :copyin) :in) ((:out :copyout) :out)))

(defmacro with-type-info-ruleset (&body body)
  `(sct::with-ruleset (sct::ensure-ruleset-instance :type-info)
     (type:with-new-environment ,@body)))

(defun add-pthis (varlist expr)
  (print "<add-pthis>") (print varlist) (print expr)
  (cond 
    ((member expr varlist)
     (print ~(fref (mref pthis) ,expr))
     ~(fref (mref pthis) ,expr))
    ((atom expr)
     (print expr)
     expr)
    ((eq ~the (car expr)) 
      (print ~(the ,(second expr) ,(add-pthis (third expr))))
      ~(the ,(second expr) ,(add-pthis (third expr))))
    ((eq ~cast (car expr)) 
      (print ~(the ,(second expr) ,(add-pthis (third expr))))
      ~(cast ,(second expr) ,(add-pthis (third expr))))
    (t
      (print (mapcar #'(lambda (x) (add-pthis varlist x)) expr))
      (mapcar #'(lambda (x) (add-pthis varlist x)) expr)))
)

(defun simple-syntax-addexpr (var-attr-params-list taskname)
  (let ((before-stat2 ())
        (after-stat2 ())
        (fun-put ())
        (fun-get ())
        (task-send ()) ;send-int32s
        (task-recv ()) ;malloc -> recv-int32s
        (rslt-send ()) ;free
        (rslt-recv ());none
        (checked-var ()))
      (mapcar #'(lambda (x)
                        (let ((varname (car (car x))) 
                              (vartype (cadr (car x)))
                              (attribute (cadr x))
                              (params (cddr x)))
                          (print varname) (print vartype) (push varname checked-var)
                          (case attribute
                               ((:in)
                                 (push  
                                   ~(def ,varname
                                         ,vartype 
                                         (the ,vartype 
                                              (fref (the (struct ,taskname) this) ,varname)))
                                    before-stat2)                                          
                                 (push 
                                   ~(the ,vartype
                                         (= (the ,vartype
                                                 (fref (the (struct ,taskname) this) ,varname)) 
                                            (the ,vartype ,varname)))
                                    fun-put))
                               ((:copyin) ;;assert:vartype is array or pointer. params contains one integer(length of array)
                                 (push ~(the (ptr ,(cadr vartype))
                                             (= (the ,vartype (fref (the (struct ,taskname) this) ,varname))
                                                (the (ptr ,(cadr vartype)) 
                                                     (cast (ptr ,(cadr vartype)) 
                                                           (the (ptr void) 
                                                                (call (the (csym::fn (ptr void) size-t) csym::malloc)
                                                                      (the int (* (the int ,(first params)) 
                                                                                  (the int (sizeof ,(cadr vartype)))))))))))
                                    fun-put)
                                 (push
                                   ~(the (ptr void) (call (the (ptr (fn (ptr void) (ptr void) (ptr void) size-t)) csym::memcpy)
                                                          (the ,vartype (fref (the (struct ,taskname) this) ,varname))
                                                          (the ,vartype ,varname)
                                                          (the int (* (the int ,(first params)) 
                                                                      (the int (sizeof ,(cadr vartype)))))))
                                    fun-put)
                                 (push ~(the void (call (the (ptr (fun void (ptr void))) csym::free) 
                                                        (the (ptr ,(cadr vartype)) 
                                                             (fref (the (struct ,taskname) this) ,varname)))) 
                                    fun-get)
                                 (push 
                                   ~(def ,varname 
                                         ,vartype
                                         (the ,vartype (fref (the (struct ,taskname) this) ,varname)))
                                    before-stat2)
                                 (if (eq (cadr vartype) ~INT32)
                                     (push ~(the void (call (the (ptr (fun void (ptr ,(cadr vartype)) int)) csym::send-int32s)
                                                            (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                            ,(add-pthis checked-var (first params))))
                                           task-send))
                                 (if (eq (cadr vartype) ~int)
                                     (push ~(the void (call (the (ptr (fun void (ptr ,(cadr vartype)) int)) csym::send-int32s)
                                                            (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                            ,(add-pthis checked-var (first params))))
                                           task-send))
                                 (if (eq (cadr vartype) ~double)
                                     (push ~(the void (call (the (ptr (fun void (ptr double) int)) csym::send-doubles)
                                                            (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                            ,(add-pthis checked-var (first params))))
                                           task-send))
                                 (push ~(the (ptr ,(cadr vartype))
                                             (= (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                (the (ptr ,(cadr vartype)) 
                                                     (cast (ptr ,(cadr vartype)) 
                                                           (the (ptr void) 
                                                                (call (the (csym::fn (ptr void) size-t) csym::malloc)
                                                                      (the int (* ,(add-pthis checked-var (first params))
                                                                                  (the int (sizeof ,(cadr vartype)))))))))))
                                    task-recv)
                                 (if (eq (cadr vartype) ~INT32)
                                     (push ~(the int (call (the (ptr (fun int (ptr ,(cadr vartype)) int)) csym::recv-int32s)
                                                           (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                           ,(add-pthis checked-var (first params))))
                                           task-recv))
                                 (if (eq (cadr vartype) ~int)
                                     (push ~(the int (call (the (ptr (fun int (ptr ,(cadr vartype)) int)) csym::recv-int32s)
                                                           (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                           ,(add-pthis checked-var (first params))))
                                           task-recv))
                                 (if (eq (cadr vartype) ~double)
                                     (push ~(the int (call (the (ptr (fun int (ptr double) int)) csym::recv-doubles)
                                                           (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                           ,(add-pthis checked-var (first params))))
                                           task-recv))
                                 (push ~(the void (call (the (ptr (fun void (ptr void))) csym::free) 
                                                        (the (ptr ,(cadr vartype)) 
                                                             (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))))
                                       rslt-send))
                                    
                               ((:ecopyin) ;;assert:vartype is array or pointer. params contains one integer(length of array)
                                 (push  
                                   ~(def ,varname
                                         ,vartype 
                                         (the ,vartype 
                                              (fref (the (struct ,taskname) this) ,varname)))
                                    before-stat2)                                          
                                 (push 
                                   ~(the ,vartype
                                         (= (the ,vartype
                                                 (fref (the (struct ,taskname) this) ,varname)) 
                                            (the ,vartype ,varname)))
                                    fun-put)
                                 (if (eq (cadr vartype) ~INT32)
                                     (push ~(the void (call (the (ptr (fun void (ptr ,(cadr vartype)) int)) csym::send-int32s)
                                                            (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                            ,(add-pthis checked-var (first params))))
                                           task-send))
                                 (if (eq (cadr vartype) ~int)
                                     (push ~(the void (call (the (ptr (fun void (ptr ,(cadr vartype)) int)) csym::send-int32s)
                                                            (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                            ,(add-pthis checked-var (first params))))
                                           task-send))
                                 (if (eq (cadr vartype) ~double)
                                     (push ~(the void (call (the (ptr (fun void (ptr double) int)) csym::send-doubles)
                                                            (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                            ,(add-pthis checked-var (first params))))
                                           task-send))
                                 (push ~(the (ptr ,(cadr vartype))
                                             (= (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                (the (ptr ,(cadr vartype)) 
                                                     (cast (ptr ,(cadr vartype)) 
                                                           (the (ptr void) 
                                                                (call (the (csym::fn (ptr void) size-t) csym::malloc)
                                                                      (the int (* ,(add-pthis checked-var (first params))
                                                                                  (the int (sizeof ,(cadr vartype)))))))))))
                                    task-recv)
                                 (if (eq (cadr vartype) ~INT32)
                                     (push ~(the int (call (the (ptr (fun int (ptr ,(cadr vartype)) int)) csym::recv-int32s)
                                                           (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                           ,(add-pthis checked-var (first params))))
                                           task-recv))
                                 (if (eq (cadr vartype) ~int)
                                     (push ~(the int (call (the (ptr (fun int (ptr ,(cadr vartype)) int)) csym::recv-int32s)
                                                           (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                           ,(add-pthis checked-var (first params))))
                                           task-recv))
                                 (if (eq (cadr vartype) ~double)
                                     (push ~(the int (call (the (ptr (fun int (ptr double) int)) csym::recv-doubles)
                                                           (the ,vartype (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))
                                                           ,(add-pthis checked-var (first params))))
                                           task-recv))
                                 (push ~(the void (call (the (ptr (fun void (ptr void))) csym::free) 
                                                        (the (ptr ,(cadr vartype)) 
                                                             (fref (the (struct ,taskname) (mref (the (ptr (struct ,taskname)) pthis))) ,varname))))
                                       rslt-send))

                                ((:out)
                                 (push 
                                   ~(def ,varname ,vartype)
                                   before-stat2 )
                                 (push
                                   ~(the ,vartype
                                         (= (the ,vartype 
                                                 (fref (the (struct ,taskname) this) ,varname)) 
                                            (the ,vartype ,varname)))
                                    after-stat2)
                                 (push 
                                   ~(the ,vartype
                                             (= (the ,vartype ,varname)
                                               (the ,vartype
                                                    (fref (the (struct ,taskname) this) ,varname))))
                                    fun-get))

                                ((:copyout)　;;assert:vartype is array or pointer. params contains one integer(length of array)
                                 (push 
                                   ~(def ,varname ,vartype)
                                   before-stat2)
                                 (push
                                   ~(the ,vartype
                                         (= (the ,vartype 
                                                 (fref (the (struct ,taskname) this) ,varname)) 
                                            (the ,vartype ,varname)))
                                    after-stat2)
                                 (push
                                   ~(csym::memcpy (fref (the (struct ,taskname) this) ,varname)
                                                  (the ,vartype ,varname)
                                                  (the int ,(first params)))
                                    fun-put)))))
               var-attr-params-list)

      (print (reverse task-send))
      (print (reverse task-recv))
      (print (reverse rslt-send))
      (print (reverse rslt-recv))
      (list (reverse before-stat2)
            (reverse after-stat2) 
            (reverse fun-put)
            (reverse fun-get) 
            (reverse task-send) 
            (reverse task-recv) 
            (reverse rslt-send) 
            (reverse rslt-recv))))