;;
;;; sc2c 構文解析部
;;; S式を受け取り、translation-unit構造体を作る
;;;

;;; 2003/11/11 mref->'*' ptr->'&' に意味を変更

(defvar *keyword-list*
  '(fn inline va-arg :attr deftype enum struct union :bit array ptr
    def decl extern extern-def extern-decl static static-def
    auto auto-def register register-def
    defs extern-defs static-defs auto-defs register-defs
    void char signed-char unsigned-char short signed-short unsigned-short
    int signed-int unsigned-int long signed-long unsigned-long
    long-long signed-long-long unsigned-long-long float double long-double
    const restrict volatile
    begin let label case default if switch while do-while for loop
    goto continue break return
    aref fref inc dec sizeof cast if-exp exps
    bit-xor bit-and bit-or and or bit-and= bit-xor= bit-or=
    ptr mref bit-not not
    defmacro private))


(defvar *storage-class-specifier-list*
;; 第３要素:定義->t 宣言のみ->nil
  '((def "" t) (decl "" nil) (extern "extern" t) (extern-def "extern" t)
    (extern-decl "extern" nil) (static "static" t) (static-def "static" t) 
    (auto "auto" t) (auto-def "auto" t) (register "register" t) 
    (register-def "register" t)))

(defvar *compound-storage-class-specifier-list*
  '((defs "") (extern-defs "extern") (static-defs "static") (auto-defs "auto")
    (register-defs "register")))

(defvar *type-specifier-list*
  '((void "void") (char "char") (signed-char "signed char") 
    (unsigned-char "unsigned char") (short "short")
    (signed-short "signed short") (unsigned-short "unsigned short")
    (int "int") (signed-int "signed int") (unsigned-int "unsigned int")
    (long "long") (signed-long "signed long") (unsigned-long "unsigned long")
    (long-long "long long") (signed-long-long "signed long long")
    (unsigned-long-long "unsigned long long") (float "float")
    (double "double") (long-double "long double")))

;; (<sc表現> <c表現> <優先度>)
(defvar *operator-list* 
  '((* "*" 4) (/ "/" 4) (% "%" 4) (+ "+" 5) (- "-" 5) (<< "<<" 6) (>> ">>" 6)
    (bit-xor "^" 10) (bit-and "&" 9) (bit-or "|" 11) 
    (and "&&" 12) (or "||" 13)))
(defvar *comparator-list* 
  '((< "<" 7) (> ">" 7) (<= "<=" 7) (>= ">=" 7) (== "==" 8) (!= "!=" 8)))
(defvar *assignment-operator-list* 
  '((= "=" 15)(*= "*=" 15) (/= "/=" 15) (%= "%=" 15) (+= "+=" 15) (-= "-=" 15)
    (<<= "<<=" 15) (>>= ">>=" 15)
    (bit-and= "&=" 15) (bit-xor= "^=" 15) (bit-or= "|=" 15)))
(defvar *unary-operator-list* 
  '((ptr "&" 2) (mref "*" 2) (bit-not "~" 2) (not "!" 2)
    (+ "+" 2) (- "-" 2)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun par-translation-unit (x)
  (make-translation-unit
   :edecl-list (mapcar 'par-declaration x)))

;;(defun par-external-declaration (x)
;;  (par-declaration x))

(defun par-declaration (x)
 ;;; inlined-declaration
  (let ((ind (par-inlined-declaration x)))
    (if ind (return-from par-declaration ind)))

  (if (not (listp x)) (return-from par-declaration))
 
  (let (ret1 ret2 (fst (first x)) (scd (second x)))
    (cond

 ;;;variable
      ((and (setq ret1 (par-identifier fst))
	    (setq ret2 (par-type-expression scd))
	    (not (and (listp scd) (eq (car scd) 'fn))))
       (let ((dv (make-decl-variable :id ret1 :texp ret2))
	     (rem (cddr x)))
	 (when rem
	   (unless (setf (decl-variable-init dv) (par-initializer (car rem)))
	     (error "syntax error at initializer")))
	 (return-from par-declaration dv)))

 ;;;function
      ((and (setq ret1 (par-function-identifier fst))
	    (listp scd)
	    (eq (first scd) 'fn))
       (let ((df (make-decl-function :defp t 
				     :id (car ret1) :arg-id-list (cdr ret1)))
	     tlist va-argp)
	 (multiple-value-setq (tlist va-argp) 
	   (par-function-type-list (cdr scd)))
	 (setf (decl-function-texp df)          (car tlist)
	       (decl-function-arg-texp-list df) (cdr tlist)
	       (decl-function-va-argp df)       va-argp)
	 
	 (let ((y (cddr x)))
	   (when (eq (car y) ':attr)
	     (setf (decl-function-inline df) (par-function-attribute (cadr y)))
	     (setq y (cddr y)))
	   (when (setf (decl-function-reg-arg-list df) 
		       (par-register-declarator (car y)))
	     (setq y (cdr y)))
	   (setf (decl-function-item-list df) (mapcar 'par-block-item y))
	   (return-from par-declaration df))))
      
 ;;;struct-or-union
      ((setq ret1 (par-struct-or-union-specifier fst))
       (let ((ds (make-decl-struct :defp t
				   :s-or-u (car ret1)
				   :id     (cadr ret1))))
	 (setf (decl-struct-sdecl-list ds) 
	       (par-struct-declaration-list (cdr x)))
	 (return-from par-declaration ds)))
      
 ;;;enum
      ((setq ret1 (par-enum-specifier fst))
       (let ((de (make-decl-enum :id ret1)))
	 (setf (decl-enum-elist de) 
	       (mapcar 'par-enumerator (cdr x)))
	 (return-from par-declaration de)))

      (t nil))))

(defun par-inlined-declaration (x)
  (if (not (listp x)) (return-from par-inlined-declaration))
 
  (let (stor ret1 ret2 (fst (first x)) (scd (second x)) (thd (third x)))
    (cond

 ;;;variable
      ((and (setq stor (par-storage-class-specifier fst))
	    (setq ret1 (par-identifier scd))
	    (setq ret2 (par-type-expression thd))
	    (not (and (listp thd) (eq (car thd) 'fn))))
       (let ((dv (make-decl-variable :id ret1
				     :st-class (car stor) 
				     :texp ret2 ))
	     (rem (cdddr x)))
	 (when rem
	   (unless (setf (decl-variable-init dv) (par-initializer (car rem)))
	     (error "syntax error at initializer")))
	 (return-from par-inlined-declaration dv)))

 ;;;function
      ((and (setq stor (par-storage-class-specifier fst))
	    (setq ret1 (par-function-identifier scd))
	    (listp thd)
	    (eq (first thd) 'fn))
       (let ((df (make-decl-function :defp (cadr stor) 
				     :id (car ret1) 
				     :st-class (car stor)
				     :arg-id-list (cdr ret1)))
	     tlist va-argp)
	 (multiple-value-setq (tlist va-argp) 
	   (par-function-type-list (cdr thd)))
	 (setf (decl-function-texp df)          (car tlist)
	       (decl-function-arg-texp-list df) (cdr tlist)
	       (decl-function-va-argp df)       va-argp)
	 
	 (let ((y (cdddr x)))
	   (when (eq (car y) ':attr)
	     (setf (decl-function-inline df) (par-function-attribute (cadr y)))
	     (setq y (cddr y)))
	   (when (setf (decl-function-reg-arg-list df) 
		       (par-register-declarator (car y)))
	     (setq y (cdr y)))
	   (setf (decl-function-item-list df) (mapcar 'par-block-item y))
	   (when y (setf (decl-function-defp df) t))
	   (return-from par-inlined-declaration df))))
      
 ;;;struct-or-union
      ((and (setq stor (par-def-or-decl fst))
	    (setq ret1 (par-struct-or-union-specifier scd)))
       (let ((ds (make-decl-struct :defp (cadr stor)
				   :s-or-u (car ret1)
				   :id     (cadr ret1))))
	 (setf (decl-struct-sdecl-list ds) 
	       (par-struct-declaration-list (cddr x)))
	 (when (cddr x) (setf (decl-struct-defp ds) t))
	 (return-from par-inlined-declaration ds)))
      
 ;;;enum
      ((and (eq fst 'def)
	    (setq ret1 (par-enum-specifier scd)))
       (let ((de (make-decl-enum :id ret1)))
	 (setf (decl-enum-elist de) 
	       (mapcar 'par-enumerator (cddr x)))
	 (return-from par-inlined-declaration de)))

 ;;;defs
      ((setq stor (par-compound-storage-class-specifier fst))
       (let ((ds (make-decl-defs :st-class (car stor))))
	 (setf (decl-defs-texp ds) (par-type-expression scd))
	 (let ((ind-list (mapcar 'par-init-declarator (cddr x))))
	   (setf (decl-defs-id-list ds)   (mapcar 'car  ind-list)
		 (decl-defs-init-list ds) (mapcar 'cadr ind-list)))
	 (return-from par-inlined-declaration ds)))

 ;;;typedef
      ((eq fst 'deftype)
       (setq ret1 (par-identifier scd))
       (cond
	 ;;; texp
	 ((setq ret2 (par-type-expression thd))
	  (return-from par-inlined-declaration 
	    (make-typedef-texp :id ret1 :texp ret2)))
	 ;;; struct
	 ((setq ret2 (par-struct-or-union thd))
	  (return-from par-inlined-declaration
	    (make-typedef-struct :id ret1 :s-or-u ret2
				 :sdecl-list
				 (par-struct-declaration-list (cdddr x)))))
	 ;;; enum
	 ((eq thd 'enum)
	  (return-from par-inlined-declaration
	    (make-typedef-enum :id ret1
			       :elist (mapcar 'par-enumerator (cdddr x)))))))

      (t nil))))


(defun par-function-identifier (x)
  (if (not (listp x)) (setq x (list x)))
  (let ((il (mapcar 'par-identifier x)))
    (unless (member nil il) il)))
	

(defun par-def-or-decl (x)
  (case x
    ((def)  '("" t))
    ((decl) '("" nil))
    (otherwise nil)))

(defun par-init-declarator (x)
  (let ((ind
	 (if (listp x)
	     (list (par-identifier (car x))
		   (par-initializer (cadr x)))
	     (list (par-identifier x)
		   nil))))
    (unless (null (car ind)) ind)))

(defun par-storage-class-specifier (x)
  (let ((ret (car (member x *storage-class-specifier-list* :key 'car))))
    (cdr ret)))

(defun par-compound-storage-class-specifier (x)
  (let ((ret 
	 (car (member x *compound-storage-class-specifier-list* :key 'car))))
    (cdr ret)))

(defun par-function-attribute (x)
  (if (eq x 'inline) t))
     
(defun par-register-declarator (x)
  (when (and (listp x)
	     (eq (car x) 'register))
    (mapcar 'par-identifier    (cdr x))))

(defun par-struct-declaration-list (x)
  (when (listp x)
    (let (sdlist)
      (do ((y x (cdr y)))
	  ((endp y) (nreverse sdlist))
	(let* ((dcl (par-declaration (car y))))
	  (when (eq (cadr y) ':bit)
	    (let ((bf  (par-expression (caddr y))))
	      (typecase dcl
		(decl-variable (setf (decl-variable-bitfield dcl) bf))
		(decl-defs     (setf (decl-defs-bitfield dcl) bf))
		(otherwise (error "syntax-error at bitfield"))))
	    (setq y (cddr y)))
	  (push dcl sdlist))))))

(defun par-enumerator (x)
  (let ((en (make-enumerator)))
    (if (listp x)
	(setf (enumerator-id en)    (par-identifier (car x))
	      (enumerator-value en) (par-expression (cadr x)))
	(setf (enumerator-id en) (par-identifier x)))
    en))

(defun par-initializer (x &aux ret) 
    (if (setq ret (par-expression x))
	(make-initializer :init-list `(,ret))
	(if (listp x)
	    (par-compound-initializer x)
	    (error "syntax-error at initializer"))))

(defun par-compound-initializer (x)
  (let ((in (make-initializer)))
    (case (first x)
      ((array)  (setf (initializer-compoundp in) 'array))
      ((struct) (setf (initializer-compoundp in) 'struct))
      (otherwise (error "syntax-error at initializer")))
    (setf (initializer-init-list in)
	  (mapcar 'par-initializer (cdr x)))
    in))

;;;;;;;
(defvar *qlist* nil)
(defvar *plev* 0)
;;;

(defun par-type-expression (x)
  (let (ret1 (*qlist* *qlist*) (*plev* *plev*))
    (when (setq ret1 (par-type-specifier x))
      (return-from par-type-expression
	(make-type-exp
	 :spec ret1
	 :qlist *qlist*
	 :plev  *plev*
	 :array nil)))
    
    (unless (listp x) (return-from par-type-expression))
        
    (case (car x) 
      ('array
       (setq ret1
	     (let ((*plev* 0))
	       (par-type-expression (second x))))
       (let ((sbl (mapcar 'par-expression (cddr x))))
	 (make-type-exp
	  :spec  (type-exp-spec ret1)
	  :qlist *qlist*
	  :plev  *plev*
	  :array (make-texp-array
		  :plev     (type-exp-plev ret1)
		  :sub-list sbl
		  :array    (type-exp-array ret1)))))
      
      ('ptr
       (setq *plev* (1+ *plev*))
       (par-type-expression (second x)))
      
      ('fn
       (let (tlist va-argp)
	 (multiple-value-setq (tlist va-argp)
	   (par-function-type-list (cdr x)))
	 (make-type-exp
	  :spec  (make-tspec-function
		  :texp           (car tlist)
		  :arg-texp-list  (cdr tlist)
		  :va-argp        va-argp)
	  :qlist *qlist*
	  :plev  *plev*
	  :array nil)))
       
      (otherwise
       (unless (par-type-qualifier (car x))
	 (return-from par-type-expression))
       (let ((ql (mapcar 'par-type-qualifier (butlast x))))
	 (setq *qlist* (append ql *qlist*))
	 (par-type-expression (car (last x)))))))))

(defun par-function-type-list (x)
  (let (va-argp (y x))
    (when (eq (car (last x)) 'va-arg)
      (setq va-argp t
	    y (butlast x)))
    (values (let ((*qlist* nil) (*plev* 0))
	      (mapcar 'par-type-expression y))
	    va-argp)))

(defun par-type-specifier (x)
  (let (ret)
    (cond
      ((setq ret (cadar (member x *type-specifier-list* :key 'car)))
       (make-tspec-basic :type ret))
      ((setq ret (par-struct-or-union-specifier x))
       (make-tspec-struct :s-or-u (car ret)
			  :id     (cadr ret)))
      ((setq ret (par-enum-specifier x))
       (make-tspec-enum :id ret))
      ((setq ret (par-identifier x))
       (make-tspec-name :id ret))
      (t nil))))

(defun par-struct-or-union-specifier (x)
  (unless (listp x) (return-from par-struct-or-union-specifier))
  (let ((id (par-identifier (cadr x))))
    (case (car x)
      ((struct union) (list (car x) id))
      (otherwise nil))))

(defun par-struct-or-union (x)
  (case x
    ((struct union) x)
    (otherwise nil)))

(defun par-enum-specifier (x)
  (when (and (listp x)
	     (eq (car x)' enum))
    (par-identifier (cadr x))))

(defun par-type-qualifier (x)
  (case x
    ('const "const")
    ('restrict "restrict")
    ('volatile "volatile")
    (otherwise nil)))

(defun par-statement (x)
  (let (ret)
    (cond
      ((null x) (make-null-stat))
      ((setq ret (par-compound-statement x)) ret)
      ((setq ret (par-expression-statement x)) ret)
      ((setq ret (par-selection-statement x)) ret)
      ((setq ret (par-iteration-statement x)) ret)
      ((setq ret (par-jump-statement x)) ret)
      ((setq ret (par-labeled-statement x)) ret)
      (t nil))))
   
(defun par-compound-statement (x)
  (unless (listp x) (return-from par-compound-statement))
  (case (car x)
    ((begin)
     (if (and *remove-redundancy*
	      (= 2 (length x))
	      (listp (second x))
	      (eq 'begin (car (second x))))
	 (par-compound-statement (second x))
       (make-compound-stat 
	:item-list (mapcar 'par-block-item (cdr x)))))
    ((let)
     (make-compound-stat
      :item-list (append (mapcar 'par-declaration (second x))
			 (mapcar 'par-block-item  (cddr x)))))
    (otherwise nil)))

(defun par-block-item (x)
  (let (ret)
    (cond
      ((setq ret (par-inlined-declaration x)) ret)
      ((setq ret (par-statement x)) ret)
      (t nil))))  

(defun par-labeled-statement (x)
  (unless (listp x) (return-from par-labeled-statement))
  (case (car x)
    ((label)
     (make-label-name
      :id   (par-identifier (second x))
      :stat (par-statement  (third x))))
    ((case)
     (make-label-case
      :exp  (par-expression (second x))))
    ((default)
     (make-label-default))
    (otherwise nil)))

(defun par-expression-statement (x)
  (let ((exp (par-expression x)))
    (when exp
      (make-expression-stat :exp exp))))

(defun par-selection-statement (x)
  (unless (listp x) (return-from par-selection-statement))
  (case (car x)
    ((if)
     (make-if-stat
      :exp       (par-expression (second x))
      :then-stat (par-statement  (third x))
      :else-stat (when (and (cdddr x)
			    (not (and *remove-redundancy*
				      (null (cadddr x)))))
		   (par-statement (fourth x)))))
    ((switch)
     (make-switch-stat
      :exp       (par-expression (second x))
      :item-list (mapcar 'par-block-item (cddr x))))
    (otherwise nil)))

(defun par-iteration-statement (x)
  (unless (listp x) (return-from par-iteration-statement))
  (case (car x)
    ((while)
     (make-while-stat
      :exp       (par-expression (second x))
      :item-list (mapcar 'par-block-item (cddr x))))
    ((do-while)
     (make-do-while-stat
      :exp       (par-expression (second x))
      :item-list (mapcar 'par-block-item (cddr x))))
    ((loop)
     (par-iteration-statement (append '(while 1) (cdr x))))
    ((for)
     (let* ((x2 (second x)) (bil (cddr x))
	    (y (butlast x2 2)) (z (lastn x2 2)))
       (if (or (null y)
	       (par-expression (car y)))
	   (make-for1-stat
	    :exp1-list (mapcar 'par-expression y)
	    :exp2      (par-expression (first z))
	    :exp3      (par-expression (second z))
	    :item-list (mapcar 'par-block-item bil))
	   (make-for2-stat
	    :decl-list (mapcar 'par-inlined-declaration y)
	    :exp2      (par-expression (first z))
	    :exp3      (par-expression (second z))
	    :item-list (mapcar 'par-block-item bil)))))
    (otherwise nil)))

(defun par-jump-statement (x)
  (unless (listp x) (return-from par-jump-statement))
  (case (car x)
    ((goto)
     (make-goto-stat
      :id (par-identifier (second x))))
    ((continue)
     (make-continue-stat))
    ((break)
     (make-break-stat))
    ((return)
     (make-return-stat
      :exp (when (second x) (par-expression (second x)))))
    (otherwise nil)))

(defun par-expression (x)
  (let (ret)
    (cond
      ((null x) (return-from par-expression))
      ((setq ret (par-identifier x))
       (return-from par-expression (make-exp-identifier :id ret)))
      ((setq ret (par-constant x))
       (return-from par-expression (make-exp-constant :const ret)))
      ((setq ret (par-string-literal x))
       (return-from par-expression (make-exp-string :str ret)))
      (t nil)))

  (unless (listp x) (return-from par-expression))
  
  (case (car x)
    ((aref)
     (make-exp-aref
      :exp           (par-expression (second x))
      :aref-exp-list (mapcar 'par-expression (cddr x))))
    ((fref)
     (let (idl opl)
       (multiple-value-setq (idl opl) (par-field-identifier-list (cddr x)))
       (make-exp-fref
	:exp     (par-expression (second x))
	:id-list idl
	:op-list opl)))
    ((inc dec)
     (make-exp-incdec
      :exp    (par-expression (second x))
      :i-or-d (case (car x) ((inc) "++") ((dec) "--"))))
    ((++ --)
     (make-exp-preincdec
      :exp    (par-expression (second x))
      :i-or-d (case (car x) ((++) "++") ((--) "--"))))
    ((sizeof)
     (let (ret (x2 (second x)))
       (cond
	 ((setq ret (par-expression x2))
	  (make-exp-sizeof :exp ret))
	 ((setq ret (par-type-expression x2))
	  (make-exp-sizeof-t :texp ret))
	 (t (error "syntax-error at sizeof-expression")))))
    ((cast)
     (make-exp-cast
      :texp (par-type-expression (second x))
      :exp  (par-expression (third x))))
    ((if-exp)
     (make-exp-ifexp
      :exp1 (par-expression (second x))
      :exp2 (par-expression (third x))
      :exp3 (par-expression (fourth x))))
    ((exps)
     (make-exp-exps
      :exp-list (mapcar 'par-expression (cdr x))))
    ;; いちおう認識するが、正しいコードは返せない
    ((c-exp) t)
    (otherwise
     (let (ret scd thd)
       (cond
	 ((setq ret (par-expression (first x)))
	  (let ((ael (mapcar 'par-expression (cdr x))))
	    (unless (member nil ael)
	      (make-exp-funcall
	       :exp          ret
	       :arg-exp-list (mapcar #'par-expression (cdr x))))))
	 ((and (setq ret (par-unary-operator (first x)))
	       (setq scd (par-expression (second x)))
	       (endp (cddr x)))
	  (make-exp-unary-op
	   :op       (first ret)
	   :exp      scd
	   :asoc-lev (second ret)))
	 ((and (eq (first x) '/)
	       (endp (cddr x)))
	  (par-expression `(/ 1 ,(second x))))
	 ((and (setq ret (par-operator (first x)))
	       (not (member nil (setq scd (mapcar #'par-expression (cdr x))))))
	  (make-exp-operator
	   :op       (first ret)
	   :exp-list scd
	   :asoc-lev (second ret)))
	 ((and (setq ret (par-comparator (first x)))
	       (setq scd (par-expression (second x)))
	       (setq thd (par-expression (third x))))
	  (make-exp-comparator
	   :op       (first ret)
	   :exp1     scd
	   :exp2     thd
	   :asoc-lev (second ret)))
	 ((and (setq ret (par-assignment-operator (first x)))
	       (setq scd (par-expression (second x)))
	       (setq thd (par-expression (third x))))	       
	  (make-exp-assignment
	   :op       (first ret)
	   :exp1     scd
	   :exp2     thd
	   :asoc-lev (second ret)))
	 (t nil))))))

(defun par-field-identifier-list (x)
  (let (idl opl)
    (do ((y x (cdr y)))
	((endp y) (values (nreverse idl) (nreverse opl)))
      (if (eq (car y) '->)
	  (progn
	    (push "->" opl)
	    (setq y (cdr y)))
	  (push "." opl))
       (push (par-identifier (car y)) idl))))
     
(defun par-operator (x)
  (cdar (member x *operator-list* :key 'car)))

(defun par-comparator (x)
  (cdar (member x *comparator-list* :key 'car)))

(defun par-assignment-operator (x)
  (cdar (member x *assignment-operator-list* :key 'car)))

(defun par-unary-operator (x)
  (cdar (member x *unary-operator-list* :key 'car)))

(defun par-identifier (x &aux sym)
  (when (and (symbolp x) (not (keywd x)) (not (null x)))    
    (setq sym (symbol-name x))
    (when (let ((s1 (aref sym 0)))
	    (or (alpha-char-p s1) 
		(char= s1 #\_) 
		(char= s1 #\-) 
		(char= s1 #\@)))
      (dotimes (n (1- (length sym)) 
		(let ((ret (get-idstring sym)))
		  (unless (string= "" (remove #\_ ret :test #'char=))
		    ret)))
	(unless (let ((sn (aref sym (1+ n))))
		  (or (alphanumericp sn) 
		      (char= sn #\_) 
		      (char= sn #\-) 
		      (char= sn #\@)))
	  (return nil))))))

(defun get-idstring (symstr)
  (let ((idstr (make-string (length symstr))) 
	(getcase (if (eq *print-case* :downcase)
		     'char-downcase
		     'char-upcase))
	(m 0))
    (setq idstr
	  (dotimes (n (length symstr) (remove #\Space idstr))
	    (let ((s (aref symstr n)))
	      (if (eq s #\@)
		  (if (eq getcase 'char-upcase)
		      (setq getcase 'char-downcase)
		      (setq getcase 'char-upcase))
		  (progn
		    (when (eq s #\-) (setq s #\_))
		    (setf (aref idstr m) (funcall getcase s))
		    (incf m))))))
    (if (or (string= idstr "") 
	    (string= idstr "_"))
	nil
	idstr)))


(defun par-constant (x)
  (cond
    ( (integerp x) (write-to-string x) )
    ( (floatp x) (write-to-string x) )
    ( (characterp x) (string+ `("'" ,(char-sc2c x) "'")))
    ( t (par-identifier x) ))) ;;;==enumeration-constant

(defun par-string-literal (x)
  (if (stringp x)
      (concatenate 'string "\"" (string-sc2c x) "\"")
      nil))