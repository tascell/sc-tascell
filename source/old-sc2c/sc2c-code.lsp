;;; 
;;; sc2c コード生成部
;;; translation-unit構造体を受け取り、Cコード文字列を返す
;;;

(defun codegen (s &rest r)
  (typecase s
    (translation-unit (cd-translation-unit s))
    (external-declaration (cd-external-declaration s))
    (decl-variable (cd-decl-variable s))
    (decl-function (cd-decl-function s))
    (decl-struct (cd-decl-struct s))
    (decl-enum (cd-decl-enum s))
    (decl-defs (cd-decl-defs s))
    (typedef-texp (cd-typedef-texp s))
    (typedef-struct (cd-typedef-struct s))
    (typedef-enum (cd-typedef-enum s))
    (enumerator (cd-enumerator s))
    (initializer (cd-initializer s))
    (type-exp (cd-type-exp s))
    (texp-array (cd-texp-array s))
    (tspec-function (cd-tspec-function s))
    (tspec-basic (cd-tspec-basic s))
    (tspec-name (cd-tspec-name s))
    (tspec-enum (cd-tspec-enum s))
    (tspec-struct (cd-tspec-struct s))
    (null-stat (cd-null-stat s))
    (compound-stat (cd-compound-stat s))
    (expression-stat (cd-expression-stat s))
    (label-name (cd-label-name s))
    (label-case (cd-label-case s))
    (label-default (cd-label-default s))
    (if-stat (cd-if-stat s))
    (switch-stat (cd-switch-stat s))
    (while-stat (cd-while-stat s))
    (do-while-stat (cd-do-while-stat s))
    (for1-stat (cd-for1-stat s))
    (for2-stat (cd-for2-stat s))
    (goto-stat (cd-goto-stat s))
    (continue-stat (cd-continue-stat s))
    (break-stat (cd-break-stat s))
    (return-stat (cd-return-stat s))
    (exp-identifier (cd-exp-identifier s))
    (exp-constant (cd-exp-constant s))
    (exp-string (cd-exp-string s))
    (exp-funcall (cd-exp-funcall s))
    (exp-aref (apply 'cd-exp-aref s r))
    (exp-fref (apply 'cd-exp-fref s r))
    (exp-incdec (apply 'cd-exp-incdec s r))
    (exp-preincdec (apply 'cd-exp-preincdec s r))
    (exp-unary-op (apply 'cd-exp-unary-op s r))
    (exp-sizeof (apply 'cd-exp-sizeof s r))
    (exp-sizeof-t (apply 'cd-exp-sizeof-t s r))
    (exp-cast (apply 'cd-exp-cast s r))
    (exp-operator (apply 'cd-exp-operator s r))
    (exp-comparator (apply 'cd-exp-comparator s r))
    (exp-ifexp (apply 'cd-exp-ifexp s r))
    (exp-assignment (apply 'cd-exp-assignment s r))
    (exp-exps (apply 'cd-exp-exps s r))
    (otherwise nil)))

(defun cd-translation-unit (s)
  (let ((str1 "") (str2 "") (nn (make-string 2 :initial-element #\Newline)))
    (dolist (incl *auto-include*)
      (add-string str1 (string+ `("#include" ,incl #\Newline))))
    
    (setq str2
	  (string+ (mapcar 'codegen (translation-unit-edecl-list s)) nn nn nn))
	               		    
    (string+ `(,str1 ,str2))))

(defun cd-decl-variable (s)
  (let (str
	(id (decl-variable-id s))
	(st-class (decl-variable-st-class s))
	texp texp1 texp2 texp3
	(init (codegen (decl-variable-init s)))
	(bitfield (codegen (decl-variable-bitfield s) 14)))
    (multiple-value-setq (texp texp1 texp2 texp3)
      (codegen (decl-variable-texp s)))
    (string+ `(,st-class " " 
	       ,(make-declstr texp1 texp2 texp3 id init bitfield) ";"))))

(defun cd-decl-function (s)
  (let (m1str m2str astr bstr)
    (let ((id (decl-function-id s))
	  (st-class (decl-function-st-class s))
	  texp texp1 texp2 texp3
	  (inline (if (decl-function-inline s) "inline " "")))
      (multiple-value-setq (texp texp1 texp2 texp3)
	(codegen (decl-function-texp s)))
      (setq m1str
	    (string+ `(,inline ,st-class " " 
		       ,(make-declstr texp1 texp2 "" id)))
	    m2str texp3))
    
    (let ((arg-id-list (decl-function-arg-id-list s))
	  (arg-texp-list (decl-function-arg-texp-list s))
	  texp texp1 texp2 texp3
	  (va-argp (decl-function-va-argp s))
	  (reg-arg-list (decl-function-reg-arg-list s))
	  regs arg-list)
      (setq arg-list
	    (do ((xi arg-id-list (cdr xi))
		 (xt arg-texp-list (cdr xt)))
		((and (endp xi) (endp xt)) arg-list)
	      (cond
		((endp xi)
		 (push (codegen (car xt)) arg-list))
		(t
		 (if (member (car xi) reg-arg-list)
		     (setq regs "register ")
		     (setq regs ""))
		 (if (endp xt)
		     (push (string+ `(,regs ,(car xi))) arg-list)
		     (progn
		       (multiple-value-setq (texp texp1 texp2 texp3)
			 (codegen (car xt)))
		       (push (string+ `(,regs 
					,(make-declstr texp1 texp2 texp3
						       (car xi))))
			     arg-list)))))))
      (when va-argp
	(push "..." arg-list))
      (setq arg-list (nreverse arg-list))
      (setq astr (string+ arg-list ", " "( " " )")))

    (let* ((defp (decl-function-defp s))
	   (il (decl-function-item-list s))
	   (item-list (mapcar 'codegen il))
	   (singlec (and (eq 'compound-stat (type-of (first il)))
			 (endp (cdr il)))))
      (if defp
	  (if singlec
	      (setq bstr (string+ item-list #\Newline #\Newline))
	      (setq bstr (string+ item-list #\Newline "{ " " }")))
	  (setq bstr ";")))
    
    (string+ `(,m1str ,astr ,m2str ,bstr))))

(defun cd-decl-struct (s)
  (let (mstr dstr)
    (let ((s-or-u (write-to-string (decl-struct-s-or-u s)))
	  (id (decl-struct-id s)))
      (setq mstr
	    (string+ `(,s-or-u " " ,id))))
    
    (let ((defp (decl-struct-defp s))
	  (sdecl-list (mapcar 'codegen (decl-struct-sdecl-list s))))
      (if defp
	  (setq dstr (string+ sdecl-list #\Newline "{ " " }"))
	  (setq dstr ";")))
    
    (string+ `(,mstr ,dstr ";"))))

(defun cd-decl-enum (s)
  (let ((id (decl-enum-id s))
	(elist (mapcar 'codegen (decl-enum-elist s))))
    (string+ `("enum " ,id ,(string+ elist ", " " { " " };")))))
		     
(defun cd-decl-defs (s)
  (let ((st-class (decl-defs-st-class s))
	texp texp1 texp2 texp3
	(id-list (decl-defs-id-list s))
	(init-list (mapcar 'codegen (decl-defs-init-list s)))
	(bitfield (codegen (decl-defs-bitfield s) 14)))
    (multiple-value-setq (texp texp1 texp2 texp3)
      (codegen (decl-defs-texp s)))
    (string+ `(,st-class " "
		   ,(make-declstr texp1 texp2 texp3 
				  id-list init-list bitfield) ";"))))

(defun cd-typedef-texp (s)
  (let ((id (typedef-texp-id s))
	texp texp1 texp2 texp3)
    (multiple-value-setq (texp texp1 texp2 texp3)
      (codegen (typedef-texp-texp s)))
    (string+ `("typedef " ,(make-declstr texp1 texp2 texp3 id) ";"))))

(defun cd-typedef-struct (s)
  (let ((id (typedef-struct-id s))
	(s-or-u (write-to-string (typedef-struct-s-or-u s)))
	(sdecl-list (mapcar 'codegen (typedef-struct-sdecl-list s))))
    (string+ `("typedef " ,s-or-u " { " #\Newline
		   ,(string+ sdecl-list #\Newline) " }" ,id ";"))))

(defun cd-typedef-enum (s)
  (let ((id (typedef-enum-id s))
	(elist (mapcar 'codegen (typedef-enum-elist s))))
    (format nil "typedef enum { ~a } ~a;"
	    (string+ elist ", ") id)))

(defun cd-enumerator (s)
  (let* ((id (enumerator-id s))
	 (value (codegen (enumerator-value s)))
	 (op (if value "=" "")))
    (string+ `(,id ,op ,value))))

(defun cd-initializer (s)
  (let ((compoundp (initializer-compoundp s))
	(init-list (mapcar #'(lambda (x) (codegen x 16))
			   (initializer-init-list s))))
    (if compoundp
	(string+ init-list ", " "{ " " }")
	(string+ init-list))))

(defun cd-type-exp (s)
  (let ((qlist (type-exp-qlist s))
	(plev (type-exp-plev s))
	(ar (type-exp-array s))
	a a2 a3 st st1 st2 st3 texp1 texp2 texp3)
    (if ar
	(multiple-value-setq (a a2 a3) (codegen ar))
	(setq a "" a2 "" a3 ""))
    (multiple-value-setq (st st1 st2 st3)
      (codegen (type-exp-spec s)))
    (setq texp1 (string+ `(,(string+ qlist " " "" " ") ,st1 #\Tab)))
    (if (> plev 0)
	(setq texp2 (string+ `("(" ,(make-string plev :initial-element #\*)))
	      texp3 ")")
	(setq texp2 "" texp3 ""))
    (setq texp2 (string+ `(,a2 ,st2 ,texp2))
	  texp3 (string+ `(,texp3 ,st3 ,a3)))
    (values (string+ `(,texp1 ,texp2 ,texp3)) texp1 texp2 texp3)))

(defun cd-texp-array (s)
  (let ((plev (texp-array-plev s))
	(sub-list (mapcar 'codegen (texp-array-sub-list s)))
	(ar (texp-array-array s))
	a a2 a3)
    (if ar
	(multiple-value-setq (a a2 a3) (codegen ar))
	(setq a "" a2 "" a3 ""))
    (setq a2 (string+ `(,a2 "(" ,(make-string plev :initial-element #\*)))
	  a3 (string+ `(,(string+ sub-list "][" "[" "]") ")" ,a3)))
    (values (string+ `(,a2 ,a3)) a2 a3)))

(defun cd-tspec-function (s)
  (let ((arg-texp-list (mapcar 'codegen (tspec-function-arg-texp-list s)))
	(va-argp (tspec-function-va-argp s))
	texp texp1 texp2 texp3)
    (multiple-value-setq (texp texp1 texp2 texp3)
      (codegen (tspec-function-texp s)))
    (when va-argp
      (add-element arg-texp-list "..."))
    (setq texp3 (string+ `(,(string+ arg-texp-list ", " "( " " )") ,texp3)))
    (values (string+ `(,texp1 ,texp2 ,texp3)) texp1 texp2 texp3)))

(defun cd-tspec-basic (s)
  (let ((type (tspec-basic-type s)))
    (values type type "" "")))

(defun cd-tspec-name (s)
  (let ((id (tspec-name-id s)))
    (values id id "" "")))

(defun cd-tspec-enum (s)
  (let ((eid (string+ `("enum " ,(tspec-enum-id s)))))
    (values eid eid "" "")))

(defun cd-tspec-struct (s)
  (let ((s-or-u (write-to-string (tspec-struct-s-or-u s)))
	(id (tspec-struct-id s)))
    (let ((sid (string+ `(,s-or-u " " ,id))))
      (values sid sid "" ""))))

(defun cd-null-stat (s) ";")

(defun cd-compound-stat (s)
  (let ((item-list (mapcar 'codegen (compound-stat-item-list s))))
    (string+ item-list #\Newline "{ " " }")))


(defun cd-expression-stat (s)
  (string+ `(,(codegen (expression-stat-exp s)) ";")))

(defun cd-label-name (s)
  (string+ `(,(label-name-id s) ":" #\Newline ,(codegen (label-name-stat s)))))

(defun cd-label-case (s)
  (string+ `("case " ,(codegen (label-case-exp s)) ":" #\Newline)))

(defun cd-labe-default (s) 
  (string+ `("default:" #\Newline)))

(defun cd-if-stat (s)
  (let ((exp (codegen (if-stat-exp s)))
	(then-stat (codegen (if-stat-then-stat s)))
	(else-stat (codegen (if-stat-else-stat s)))
	estr)
    (if else-stat
	(setq estr (string+ `(#\Newline "else " ,else-stat)))
	(setq estr ""))
    (string+ `("if( " ,exp " )" #\Newline ,then-stat #\Newline ,estr))))

(defun cd-switch-stat (s)
  (let ((exp (codegen (switch-stat-exp s)))
	(item-list (mapcar 'codegen (switch-stat-item-list s))))
    (string+ `("switch( " ,exp " )"
	       "{ " ,(string+ item-list #\Newline) " }"))))

(defun cd-while-stat (s)
  (let ((exp (codegen (while-stat-exp s)))
	(item-list (mapcar 'codegen (while-stat-item-list s))))
    (string+ `("while( " ,exp " )"
                   "{ " ,(string+ item-list #\Newline) " }"))))

(defun cd-do-while-stat (s)
  (let ((exp (codegen (do-while-stat-exp s)))
        (item-list (mapcar 'codegen (do-while-stat-item-list s))))
    (string+ `("do" "{ " ,(string+ item-list #\Newline) " }"
		   "while( " ,exp " );"))))

(defun cd-for1-stat (s)
  (let* ((e1l (for1-stat-exp1-list s))
	 (exp1-list (mapcar 'codegen e1l
			    (make-list (length e1l) :initial-element 16)))
	 (exp2 (codegen (for1-stat-exp2 s)))
	 (exp3 (codegen (for1-stat-exp3 s)))
	 (item-list (mapcar 'codegen (for1-stat-item-list s))))
    (string+ `("for( " ,(string+ exp1-list ", ") " ; "
		   ,exp2 " ; " ,exp3 " ){ " #\Newline
		   ,(string+ item-list #\Newline) "}"))))

(defun cd-for2-stat (s)
  (when (for2-stat-for1p s)
    (return-from cd-for2-stat 
      (cd-for1-stat (make-for1-stat
		       :exp1-list (for2-stat-decl-list s)
		       :exp2      (for2-stat-exp2 s)
		       :exp3      (for2-stat-exp3 s)
		       :item-list (for2-stat-item-list s)))))

  (let* ((dl (for2-stat-decl-list s))
	 (decl-list (mapcar 'codegen dl))
	 (nd (length dl))
	 (exp2 (codegen (for2-stat-exp2 s)))
	 (exp3 (codegen (for2-stat-exp3 s)))
	 (item-list (mapcar 'codegen (for2-stat-item-list s)))
	 st1 st2 st3)    
    (if (< nd 2)
	(let ((semicolon (if (= nd 0) ";" "")))
	  (setq st1 (string+ `("for( "  ,(car decl-list) ,semicolon))
		st3 ""))
	(setq st1 (string+ `("{" #\Newline
				 ,(string+ decl-list #\Newline)
				 "for(; "))
	      st3 "}"))
    (setq st2 (string+ `(,exp2 " ; " ,exp3 " ){ " #\Newline
			     ,(string+ item-list #\Newline) "}")))
    (string+ `(,st1 ,st2 ,st3))))

(defun cd-goto-stat (s)
  (string+ `("goto " ,(goto-stat-id s) ";")))

(defun cd-continue-stat (s) "continue;")

(defun cd-break-stat (s) "break;")

(defun cd-return-stat (s)
  (if (return-stat-exp s)
      (string+ `("return( " ,(codegen (return-stat-exp s)) " );"))
      "return;"))

(defun cd-exp-identifier (s) (exp-identifier-id s))

(defun cd-exp-constant (s) (exp-constant-const s))

(defun cd-exp-string (s) (exp-string-str s))

(defun cd-exp-funcall (s)
  (let* ((exp (codegen (exp-funcall-exp s)))
	 (ael (exp-funcall-arg-exp-list s))
	 (arg-exp-list (mapcar 'codegen ael 
			       (make-list (length ael) :initial-element 16))))
    (string+ `(,exp "( " ,(string+ arg-exp-list ",") " )"))))

(defun cd-exp-aref (s &optional as)
  (let ((exp (codegen (exp-aref-exp s) 1))
	(aref-exp-list (mapcar 'codegen (exp-aref-aref-exp-list s)))
	(st2 "") (p1 "") (p2 ""))
    (when (and as (<= as 1)) (setq p1 "(" p2 ")"))
    (when aref-exp-list
      (setq st2 (string+ aref-exp-list "][" "[" "]")))
    (string+ `(,p1 ,exp ,st2 ,p2))))

(defun cd-exp-fref (s &optional as)
  (let* ((exp (codegen (exp-fref-exp s) 1))
	 (id-list (exp-fref-id-list s))
	 (op-list (exp-fref-op-list s))
	 (slist (make-list (length id-list) :initial-element 'string))
	 (fr-list (mapcar 'concatenate slist op-list id-list))
	 (p1 "") (p2 ""))
    (when (and as (<= as 1)) (setq p1 "(" p2 ")"))
    (string+ `(,p1 ,exp ,(string+ fr-list) ,p2))))

(defun cd-exp-incdec (s &optional as)
  (let ((exp (codegen (exp-incdec-exp s) 1))
	(i-or-d (exp-incdec-i-or-d s))
	(p1 "") (p2 ""))
    (when (and as (<= as 1)) (setq p1 "(" p2 ")"))
    (string+ `(,p1 ,exp ,i-or-d ,p2))))

(defun cd-exp-preincdec (s &optional as)
  (let ((exp (codegen (exp-preincdec-exp s) 2))
        (i-or-d (exp-preincdec-i-or-d s))
        (p1 "") (p2 ""))
    (when (and as (<= as 2)) (setq p1 "(" p2 ")"))
    (string+ `(,p1 ,i-or-d ,exp ,p2))))

(defun cd-exp-unary-op (s &optional as)
  (let* ((asoc-lev (exp-unary-op-asoc-lev s))
	 (op (exp-unary-op-op s))
	 (exp (codegen (exp-unary-op-exp s) asoc-lev))
	 (p1 "") (p2 ""))
    (when (and as (<= as asoc-lev)) (setq p1 "(" p2 ")"))
    (string+ `(,p1 ,op ,exp ,p2))))

(defun cd-exp-sizeof (s &optional as)
  (let ((exp (codegen (exp-sizeof-exp s) 2))
	(p1 "") (p2 ""))
    (when (and as (<= as 2)) (setq p1 "(" p2 ")"))
    (string+ `(,p1 "sizeof(" ,exp ")" ,p2))))

(defun cd-exp-sizeof-t (s &optional as)
  (let ((texp (codegen (exp-sizeof-t-texp s)))
	(p1 "") (p2 ""))
    (when (and as (<= as 2)) (setq p1 "(" p2 ")"))
    (string+ `(,p1 "sizeof(" ,texp ")" ,p2))))

(defun cd-exp-cast (s &optional as)
  (let ((texp (codegen (exp-cast-texp s)))
	(exp (codegen (exp-cast-exp s) 3))
	(p1 "") (p2 ""))
    (when (and as (<= as 3)) (setq p1 "(" p2 ")"))
    (string+ `(,p1 "(" ,texp ")" ,exp ,p2))))

(defun cd-exp-operator (s &optional as)
  (let* ((el (exp-operator-exp-list s))
	 (nel (length el))
	 (asoc-lev (exp-operator-asoc-lev s))
	 (exp-list (mapcar 'codegen el
			   (make-list nel :initial-element asoc-lev)))
	 (op (exp-operator-op s))
	 (p1 "") (p2 ""))
    (when (and as (<= as asoc-lev)) (setq p1 "(" p2 ")"))
    (string+ `(,p1 ,(string+ exp-list op) ,p2))))

(defun cd-exp-comparator (s &optional as)
  (let* ((op (exp-comparator-op s))
	 (asoc-lev (exp-comparator-asoc-lev s))
	 (exp1 (codegen (exp-comparator-exp1 s) asoc-lev))
	 (exp2 (codegen (exp-comparator-exp2 s) asoc-lev))
	 (p1 "") (p2 ""))
    (when (and as (<= as asoc-lev)) (setq p1 "(" p2 ")"))
    (string+ `(,p1 ,exp1 ,op ,exp2 ,p2))))

(defun cd-exp-ifexp (s &optional as)
  (let ((exp1 (codegen (exp-ifexp-exp1 s) 14))
	(exp2 (codegen (exp-ifexp-exp2 s) 14))
	(exp3 (codegen (exp-ifexp-exp3 s) 14))
	(p1 "") (p2 ""))
    (when (and as (<= as 14)) (setq p1 "(" p2 ")"))
    (string+ `(,p1 ,exp1 "?" ,exp2 ":" ,exp3 ,p2))))

(defun cd-exp-assignment (s &optional as)
  (let* ((op (exp-assignment-op s))
         (asoc-lev (exp-assignment-asoc-lev s))
         (exp1 (codegen (exp-assignment-exp1 s) asoc-lev))
         (exp2 (codegen (exp-assignment-exp2 s) asoc-lev))
         (p1 "") (p2 ""))
    (when (and as (<= as asoc-lev)) (setq p1 "(" p2 ")"))
    (string+ `(,p1 ,exp1 ,op ,exp2 ,p2))))

(defun cd-exp-exps (s &optional as)
  (let* ((el (exp-exps-exp-list s))
	 (nel (length el))
	 (exp-list (mapcar 'codegen el 
			   (make-list nel :initial-element 16)))
	 (p1 "") (p2 ""))
    (when (and as (<= as 16)) (setq p1 "(" p2 ")"))
    (string+ `(,p1 ,(string+ exp-list ",") ,p2))))
