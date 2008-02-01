;;;現在のブロック階層
(defvar *lev* 0)      
(defun inc-lev ()
  (incf *lev*))
(defun dec-lev ()
  (setf (get-block-index) nil)
  (decf *lev*))

;;;identifeir管理
(defstruct id
  name   ;;;識別子名文字列
  object ;;;識別子の種類 
         ;;;  (type, struct-type, union-type, enum-type, struct(=union),
         ;;;   enum, e-const, variable, function)
  type   ;;;type,variable->texp  struct-type,struct->member-id-list 
         ;;;enum-type,enum->enumeration-identifier-list
         ;;;e-const->nil function->texp-list
         ;;;構造体のメンバは*block-index*と同様に管理
  lev    ;;;宣言の行われたブロックレベル
)  

;;; 識別子の探索
;;; name:識別子の名前 (all t):nilにすると現在の階層のみ探索
(defun search-id (name &key (all t))
  (let ((endlv (if all 0 *lev*)))
    (do ((lv *lev* (1- lv)))
	((< lv endlv) nil)
      (let ((*lev* lv) getid)
	(when (setq getid (member name (get-block-index) 
				  :key 'id-name :test 'string=))
	  (return-from search-id (car getid)))))))

;;; identifier登録
(defun add-id-table (id)
  (if (not (id-p id)) (error "id is not struct 'id'"))
  (setf (id-lev  id) *lev*)
  (renew-block-index id)
  id)

;;; variable登録
;;; v-name:変数名 v-type:型
(defun entry-variable (v-name v-type)
  (if (search-id v-name :all nil)
      (let ((*standard-output* *error-output*))
	(princ "warning:previous declaration exists for ")
	(princ "'") (princ v-name) (princ "'") (princ #\Linefeed)))
  (add-id-table (make-id :name v-name :object 'variable :type v-type)))

;;; 関数の引数をvariableとして登録
;;; a-name:引数名リスト fn:関数登録データ
(defun entry-argument (a-name fn &aux (a-type (cdr (id-type fn))))
  (let ((lnm (length a-name)) (ltp (length a-type)))
    (if (> lnm ltp)
	(error "argument-type incompleted"))
    (if (< lnm ltp)
	(error "too few arguments")))
  (mapcar 'entry-variable a-name a-type))

;;; function登録
;;; f-name:関数名 f-type:型リスト
(defun entry-function (f-name f-type &aux (old-fn (search-id f-name)))
  (if (and old-fn (eq (id-object old-fn) 'function))
      (progn 
	(if (not (equal f-type (id-type old-fn)))
	    (let ((*standard-output* *error-output*))
	      (princ "warning:conflicting types for function ")
	      (princ "'") (princ f-name) (princ "'") (princ #\Linefeed)))
	(setf (id-type old-fn) f-type)
	old-fn)
      (add-id-table (make-id :name f-name :object 'function :type f-type))))

;;; struct,union登録
;;; s-spec:struct-or-union-specifier m-id:メンバの識別子データリスト
(defun entry-struct (s-spec m-id &aux (s-or-u (car s-spec)) 
			    (s-name (identifier (cadr s-spec)))
			    (old-st (search-id s-name)))
  (if (and old-st (eq (id-object old-st) s-or-u) (null (id-type old-st)))
      (progn
	(setf (id-type old-st) m-id)
	old-st)
      (add-id-table (make-id :name s-name :object s-or-u :type m-id))))

;;; enum登録
;;; e-spec:enum-specifier ec-list:enumration-constantのリスト
(defun entry-enum (e-spec ec-list &aux (e-name (identifier (cadr e-spec))))
  (add-id-table (make-id :name e-name :object 'enum :type ec-list)))


;;; e-const登録
;;; ec-name:eumeration-constant名
(defun entry-econst (ec-name)
  (add-id-table (make-id :name ec-name :object 'e-const)))

;;; type登録
;;; t-name:typedef名 texp:type-expression
(defun entry-type (t-name texp)
  (add-id-table (make-id :name t-name :object 'type :type texp)))

;;; struct-type, union-type登録
;;; t-name:typedef名 s-or-u:struct-or-union m-id:メンバの識別子データリスト
(defun entry-struct-type (t-name s-or-u m-id)
  (let ((obj (if (eq s-or-u 'struct) 'struct-type 'union-type)))
    (add-id-table (make-id :name t-name :object obj :type m-id))))

;;; enum-type登録
;;; t-name:typedef名 ec-list:enumeration-constantのリスト
(defun entry-enum-type (t-name ec-list)
  (add-id-table (make-id :name t-name :object 'enum-type :type ec-list)))

;;;ブロックレベルiで最後に宣言された識別子の*id-table*要素へのリンク
(defvar *block-index* (make-array 256))
(defmacro get-block-index ()
  `(aref *block-index* *lev*))
(defun renew-block-index (newindex)
  (setf (get-block-index) (cons newindex (get-block-index))))

