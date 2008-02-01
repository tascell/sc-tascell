(defvar *storage-class-specifier-list*
  '((def "" t) (decl "" nil) (extern "extern" t) (extern-def "extern" t)
    (extern-decl "extern" nil) (static "static" t) (static-def "static" t) 
    (auto "auto" t) (auto-def "auto" t) (register "register" t) 
    (register-def "register" t)))

(defvar *compound-storage-class-specifier-list*
  '((defs "") (extern-defs "extern") (static-defs "static") (auto-defs "auto")
    (register-defs "register")))

;;; x:list
;;; 返値:Cコード文字列のリスト
(defun declaration-list (x &aux (ret1 nil) ret2)
  (if (listp x)
      (dolist (a x ret1)
	(if (setq ret2 (declaration a))
	    (setq ret1 (nconc ret1 (list ret2)))
	    (return nil)))))

;;; x:list bitstr:bit-field指定式文字列
;;; 返値:Cコード文字列
(defun declaration (x &optional bitstr &aux ret1 ret2 ret3 ret4 ret5 astrl) 
  (cond
    ((setq ret1 (inlined-declaration x bitstr)) ret1)
    ((not (listp x)) (return-from declaration))

 ;;; variable
    ((and (setq ret1 (identifier (car x)))
	  (setq ret2 (type-expression (cadr x)))
	  (progn
	    (entry-variable ret1 (cadr x))
	    (or (endp (cddr x))
		(setq ret3 (initializer (cddr x)))))) ;;;             
     (if (endp (cddr x)) (setq ret3 nil))
     (concatenate 'string (make-declstr ret2 ret1 ret3 bitstr) ";" ))

 ;;; function
    ((and (setq ret1 (identifier-list (car x)))
	  (listp (cadr x))
	  (let ((y (cadr x)))
	    (and (eq (car y) 'fn)
		 (setq ret2 (function-type-list (cdr y)))))
	  (let ((y (cddr x)))
	    (and (if (eq (car y) ':attr)
		     (if (setq ret3 (function-attribute (cadr y)))
			 (progn (setq y (cddr y)) t))
		     (progn (setq ret3 "") t))
		 (if (setq ret4 (register-declarator (car y)))
		     (progn (setq y (cdr y)) t)
		     (progn (setq ret4 nil) t))
		 (let ((cfn (entry-function (car ret1) ret2)) flag)  
		   (inc-lev)
		   (entry-argument (cdr ret1) cfn)
		   (if (endp y)
		       (setq ret5 nil flag t)
		       (setq ret5 (block-item-list y) flag ret5))
		   (dec-lev)
		   flag)))) ;;;
     (let* ((ri (cdr ret1)) (li (length ri))
	    (rt (if (eq 'va-arg (car (last (cadr x))))
		    (butlast (cdr ret2))
		    (cdr ret2)))
	    (lt (length rt)))
       (if (< li lt)
	   (error "too many type-expressions"))
       (setq rt (nconc rt (make-list (- li lt) 
				     :initial-element (list "" "" ""))))
       (do ((id ri (cdr id)) (tp rt (cdr tp))) ((endp id))
	 (let ((reg (if (member (car id) ret4 :test 'string=)
			"register "
			"")))
	   (setq astrl 
		 (nconc astrl 
			(list (concatenate 'string reg
					   (make-declstr (car tp) (car id))))))
	   )))
     (if (eq 'va-arg (car (last (cadr x))))
	 (setq astrl (nconc astrl (list "..."))))
     (concatenate 'string
		  ret3 " " (first (first ret2)) " "
		  (second (first ret2)) (first ret1) 
		  "( " (string+ astrl ", ") " )"
		  (third (first ret2)) "{ " (string+ ret5) " }"))

 ;;;struct
    ((and (setq ret1 (struct-or-union-specifier (car x)))
	  (or (endp (cdr x)) 
	      (multiple-value-setq (ret2 ret3) 
		(struct-declaration-list (cdr x))))) ;;;    
     (if (endp (cdr x)) (setq ret2 "" ret3 nil))
     (entry-struct (car x) ret3)
     (concatenate 'string ret1 " { " (string+ ret2) " };"))

 ;;;enum
    ((and (setq ret1 (enum-specifier (car x)))
	  (multiple-value-setq (ret2 ret3) (enumerator-list (cdr x)))) ;;;
     (entry-enum (car x) ret3)
     (concatenate 'string ret1 " { " (string+ ret2 ", ") " };"))
    (t nil)))


;;; x:list
;;; 返値:Cコード文字列のリスト
(defun inlined-declaration-list (x &aux (ret1 nil) ret2)
  (if (listp x)
      (dolist (a x ret1)
	(if (setq ret2 (inlined-declaration a))
	    (setq ret1 (nconc ret1 (list ret2)))
	    (return nil)))))


;;; x:list bitstr:bit-field指定式文字列
;;; 返値:Cコード文字列
(defun inlined-declaration 
    (x &optional bitstr &aux ret1 ret2 ret3 ret4 ret5 ret6 astrl defp)
  (if (not (listp x)) (return-from inlined-declaration))
  (cond

 ;;;variable
    ((and (setq ret1 (storage-class-specifier (car x)))
	  (setq ret2 (identifier (cadr x)))
	  (setq ret3 (type-expression (caddr x)))
	  (progn
	    (entry-variable ret2 (caddr x))	    
	    (or (endp (cdddr x))
		(setq ret4 (initializer (cdddr x)))))) ;;;       
     (if (endp (cdddr x)) (setq ret4 nil))
     (concatenate 'string ret1 " " (make-declstr ret3 ret2 ret4 bitstr) ";"))

 ;;;function
    ((and (multiple-value-setq (ret1 defp) (storage-class-specifier (car x)))
	  (setq ret2 (function-identifier (cadr x)))
	  (let ((y (caddr x)))
	    (and (listp y)
		 (eq (car y) 'fn)
		 (setq ret3 (function-type-list (cdr y)))))
	  (let ((y (cdddr x)))
	    (and (if (eq (car y) ':attr)
		     (if (setq ret4 (function-attribute (cadr y)))
			 (progn (setq y (cddr y)) t))
		     (progn (setq ret4 "") t))
		 (if (setq ret5 (register-declarator (car y)))
		     (progn (setq y (cdr y)) t)
		     (progn (setq ret4 nil) t))
		 (let ((cfn (entry-function (car ret2) ret3)) flag)
		   (inc-lev)
		   (when (or y defp) (entry-argument (cdr ret2) cfn))
		   (if (endp y)
		       (setq ret6 nil flag t)
		       (setq ret6 (block-item-list y) flag ret6))
		   (dec-lev)
		   flag)))) ;;;;
     (let* ((ri (cdr ret2)) (li (length ri))
	    (rt (if (eq 'va-arg (car (last (caddr x))))
		    (butlast (cdr ret3))
		    (cdr ret3)))
	    (lt (length rt)))
       (if (or defp ret6)
	   (progn
	     (if (< li lt)
		 (error "too many type-expressions"))
	     (setq rt (nconc rt (make-list (- li lt) 
					   :initial-element (list "" "" "")))))
	   (progn
	     (if (> li lt)
		 (error "too many identifiers"))
	     (setq ri (nconc ri (make-list (- lt li) :initial-element "")))))
       (do ((id ri (cdr id)) (tp rt (cdr tp))) ((endp id))
	 (let ((reg (if (member (car id) ret5 :test 'string=)
			"register "
			"")))
	   (setq astrl 
		 (nconc astrl 
			(list (concatenate 'string reg
					   (make-declstr (car tp) (car id))))))
	   )))
     (if (eq 'va-arg (car (last (caddr x))))
	 (setq astrl (nconc astrl (list "..."))))
     (concatenate 'string
		  ret4 ret1 " " (first (first ret3)) " "
		  (second (first ret3)) (first ret2) 
		  "( " (string+ astrl ", ") " )"
		  (third (first ret3)) 
		  (if (and (null ret6)
			   (not defp))
		      ";"
		      (concatenate 'string "{ " (string+ ret6) " }"))))

 ;;;struct
    ((and (multiple-value-setq (ret1 defp) (def-or-decl (car x)))
	  (setq ret2 (struct-or-union-specifier (cadr x)))
	  (or (endp (cddr x))
	      (multiple-value-setq (ret3 ret4)
		(struct-declaration-list (cddr x))))) ;;;  
     (if (endp (cddr x)) (setq ret3 nil))
     (entry-struct (cadr x) ret4)
     (concatenate 'string ret1 " " ret2 " "
		  (if (and (null ret3)
			   (not defp))
		      ";"
		      (concatenate 'string "{ " (string+ ret3) " };"))))

 ;;;enum
    ((and (eq (car x) 'def)
	  (setq ret1 (enum-specifier (cadr x)))
	  (multiple-value-setq (ret2 ret3) (enumerator-list (cddr x)))) ;;; 
     (entry-enum (car x) ret3)
     (concatenate 'string ret1 " { " (string+ ret2 ", ") " };"))
    
 ;;;defs
    ((and (setq ret1 (compound-storage-class-specifier (car x)))
	  (setq ret2 (type-expression (cadr x)))
	  (setq ret3 (init-declarator-list (cddr x) (cadr x))))          
     (setq ret4 (mapcar 'car ret3) ret5 (mapcar 'cadr ret3))
     (concatenate 'string ret1 " " (make-declstr ret2 ret4 ret5 bitstr) ";"))

 ;;;typedef
    ((and (eq (car x) 'deftype)
	  (setq ret1 (identifier (cadr x)))
	  (setq ret2 (type-expression (caddr x))))
     (entry-type ret1 ret2)
     (concatenate 'string "typedef " (make-declstr ret2 ret1) ";"))

 ;;;typedef(struct,union)
    ((and (eq (car x) 'deftype)
	  (setq ret1 (identifier (cadr x)))
	  (setq ret2 (struct-or-union (caddr x)))
	  (or (endp (cdddr x))
	      (multiple-value-setq (ret3 ret4) 
		(struct-declaration-list (cdddr x))))) ;;;
     (if (endp (cdddr x)) (setq ret3 "" ret4 nil))
     (entry-struct-type ret1 (caddr x) ret4)
     (concatenate 'string "typedef " ret2 
		  "{ " (string+ ret3) " } " ret1 ";" )) 

 ;;;typedef(enum)
    ((and (eq (car x) 'deftype)
	  (setq ret1 (identifier (cadr x)))
	  (eq (caddr x) 'enum)
	  (multiple-value-setq (ret2 ret3) (enumerator-list (cdddr x)))) ;;;
     (entry-enum-type ret1 ret3)
     (concatenate 'string "typedef enum{ " 
		  (string+ ret2 ", ") " } " ret1 ";"))
    (t nil)))


;;; x:list
;;; 返値:identifierの返値のリスト
(defun function-identifier (x &aux ret)
  (cond
    ((setq ret (identifier x)) (list ret))
    ((setq ret (identifier-list x)) ret)
    (t nil)))

 
;;; x:list
;;; 返値:Cコード文字列 & (定義->t 宣言->nil)
(defun def-or-decl (x)
  (case x
    ('def (values "" t))
    ('decl (values "" nil))
    (otherwise nil)))

;;; x:list texp:type-expression
;;; 返値:init-declaratorの返値のリスト
(defun init-declarator-list (x texp &aux (ret1 nil) ret2)
  (if (listp x)
      (dolist (a x ret1)
	(if (setq ret2 (init-declarator a texp))
	    (setq ret1 (nconc ret1 (list ret2)))
	    (return nil)))))

;;; x:list texp:type-expression
;;; 返値:(str1 str2) str1--identifierの返値 str2--初期値の文字列(or nil)
(defun init-declarator (x texp &aux ret1 ret2)
  (cond
    ((setq ret1 (identifier x)) 
     (entry-variable ret1 texp)
     (setq ret2 nil))
    ((and (listp x)
	  (setq ret1 (identifier (car x)))
	  (progn
	    (entry-variable ret1 texp)
	    (setq ret2 (initializer (cdr x))))))
    (t (return-from init-declarator)))
  (list ret1 ret2))


;;; x:list
;;; 返値:Cコード文字列 & (定義->t 宣言->nil)
(defun storage-class-specifier (x &aux ret)
  (if (setq ret (member x *storage-class-specifier-list* :key 'car))
      (values (cadar ret) (caddar ret))))

;;; x:list
;;; 返値:Cコード文字列 & (定義->t 宣言->nil)
(defun compound-storage-class-specifier (x &aux ret)
  (if (setq ret (member x *compound-storage-class-specifier-list* :key 'car))
      (values (cadar ret) (caddar ret))))

;;; x:list
;;; 返値:Cコード文字列
(defun function-attribute (x)
  (if (eq x 'inline) "inline"))

;;; x:list
;;; 返値:identifier-listの返値
(defun register-declarator (x &aux ret)
  (if (and (listp x)
	   (eq (car x) 'register)
	   (setq ret (identifier-list (cdr x))))
      ret))

;;; x:list
;;; 返値:Cコード文字列のリスト & メンバidデータのリスト *兼struct-declaration*
(defun struct-declaration-list (x &aux ret1 ret2 ret3 sstr m-id)
  (inc-lev)
  (setq sstr
	(unwind-protect
	     (if (listp x)
		 (do ((a x (cdr a))) ((endp a) ret1)
		   (if (eq (cadr a) ':bit)
		       (if (setq ret3 (expression (caddr a) 15 15))
			   (progn
			     (setq ret2 (declaration (car a) ret3))
			     (setq a (cddr a)))
			   (return))
		       (setq ret2 (declaration (car a))))
		   (if (null ret2) (return))
		   (add-element ret1 ret2)))
	  (setq m-id (get-block-index))
	  (dec-lev)))
  (values sstr m-id))
	     
;;; x:list
;;; 返値:enumeratorの返値のリスト & enumeration-constantのリスト
(defun enumerator-list (x &aux e-list ec-list ret1 ret2)
  (if (listp x)
      (dolist (a x (values e-list ec-list))
	(if (multiple-value-setq (ret1 ret2) (enumerator a))
	    (progn
	      (add-element e-list ret1)
	      (entry-econst ret2)
	      (add-element ec-list ret2))
	    (return nil)))))

;;; x:list
;;; 返値:Cコード文字列 & enumeration-constant
(defun enumerator (x &aux ret1 ret2)
  (if (setq ret1 (enumeration-constant x)) (values ret1 ret1)
      (if (and (listp x)
	       (setq ret1 (enumeration-constant (car x)))
	       (setq ret2 (expression (cadr x) 15 15)))
	  (values (concatenate 'string ret1 "=" ret2) ret1))))
  

;;; x:list
;;; 返値:identifierの返値
(defun enumeration-constant (x)
  (identifier x))

;;; x:list
;;; 返値:Cコード文字列のリスト
(defun initializer-list (x &aux ret1 ret2)
  (if (listp x)
      (dolist (a x ret1)
	(cond
	  ((setq ret2 (expression a 16 16)) 
	   (add-element ret1 ret2))
	  ((setq ret2 (compound-initializer a))
	   (add-element ret1 ret2))
	  (t (return-from initializer-list))))))

;;; x:list 
;;; 返値:Cコード文字列
(defun compound-initializer (x &aux ret)
  (if (not (listp x)) (return-from compound-initializer))
  (cond
    ((and (eq (car x) 'array)
	  (setq ret (initializer-list (cdr x))))
     (string+ ret ", " "{ " " }"))
    ((and (eq (car x) 'struct)
	  (setq ret (initializer-list (cdr x))))
     (string+ ret ", " "{ " " }"))
    (t nil)))

;;; x:list
;;; 返値:Cコード文字列
(defun initializer (x &aux ret)
  (if (not (listp x)) (return-from initializer))
  (cond
    ((and (setq ret (expression (car x)))
	  (endp (cdr x)))
     ret)
    ((setq ret (compound-initializer x))
     ret)
    (t nil)))

