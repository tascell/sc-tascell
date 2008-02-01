;;;
;;; hoisting Sexpr->Sexpr version
;;; renamehで取り除いてあるのでletとdeclつきforは考慮しない
;;;

(defconstant *iniv-postfix* "_init")

(defun remove-const (x)
  (when (not (listp x)) (return-from remove-const x))
  (setq x (remove 'const x))
  (when (= 1 (length x)) (return-from remove-const (remove-const (car x))))
  (mapcar #'remove-const x))

(defun get-init-id (x &aux id)
  (when (setq id (par-identifier x))
    (id-to-scid (string+ `(,id ,*iniv-postfix*)))))

;;仮
(defun make-init-exp (id texp init)
  `(= ,id ,init))

(defun hoisting2 (x)
  (setq x (renameh x))
  (when *step-exec* (princ x) (break))
  (mapcar #'hoist-ext x))

;;; top-level
(defun hoist-ext (x)  
  (let ((xf (first x)))
    (cond
      ;;; inlined-declaration      
      ((par-storage-class-specifier xf)
       (cons xf (hoist-ext (cdr x))))
      ;;; declaration
      ((and (par-function-identifier xf)
	    (listp (second x))
	    (eq 'fn (caadr x)))
       (hoist-function x))
      (t x))))

;;; inside of function
(defun hoist-function (x &aux pbil bil spbil)    
  ;; block-item-list の獲得
  (setq pbil (cdr x))
  (when (eq ':attr (cadr pbil)) (setq pbil (cddr pbil)))
  (when (par-register-declarator (cadr pbil)) (setq pbil (cdr pbil)))
  (setq bil (cdr pbil))
 
  (setq spbil (split-block-item-list bil))
  (rplacd pbil `(,@(first spbil) ,@(second spbil)))
  x)

;;;
;;; 以下、宣言文とそれ以外の文の分割処理
;;; 返り値は(decl other)のリスト形式
;;;

(defun split-block-item-list (bil &aux spbil)
  (setq spbil (mapcar #'split-block-item bil))
  `(,(apply 'nconc (mapcar #'first spbil))
    ,(apply 'nconc (mapcar #'second spbil))))

(defun split-block-item (x)
  (when (not (listp x)) (return-from split-block-item `(nil (,x))))
  
  (let ((xf (first x)) (xs (second x)) (xt (third x)))
    (cond 
      ;;variable-decl
      ((and (par-storage-class-specifier xf)
	    (par-identifier xs)
	    (par-type-expression xt))
       (setf (third x) (remove-const xt))
       (if (fourth x)
	   (progn
	     `((,(let ((y (copy-list x)))
		      (setf (cdddr y) nil)
		      y))
	       (,(apply #'make-init-exp (cdr x)))))
	   `((,x) nil)))
      ;;function-decl
      ((and (par-storage-class-specifier xf)
	    (par-function-identifier xs)
	    (and (listp xt))
	    (eq 'fn (first xt)))
       `(,(cons (car x) (hoist-function (cdr x))) 
	 nil))
      ;;struct-decl
      ((and (par-def-or-decl xf)
	    (listp xs)
	    (let ((xfs (first xs)))
	      (or (eq 'struct xfs)
		  (eq 'union xfs)))
	    (par-identifier (second xs)))
       `((,x) nil))
      ;;enum-decl
      ((and (eq 'def xf)
	    (listp xs)
	    (eq 'enum (first xs))
	    (par-identifier (second xs)))
       `((,x) nil))
      ;;defs
      ((par-compound-storage-class-specifier xf)
       (setq xs (remove-const xs))
       (let ((init-list (remove-if-not #'listp (cddr x))))
	 (mapl #'(lambda (init) 
		   (rplaca init
			   `(make-init-exp ',(caar init) ',xs ',(cadar init))))
	       init-list)
	 `(((defs ,xs 
		,@(mapcar #'(lambda (in) (if (listp in) (car in) in)) (cddr x))))
	   ,(mapcar #'eval init-list))))
      ;;typedef
      ((eq 'deftype xf)
       `((,x) nil))
      (t (split-statement x)))))

(defun split-statement (x)
  (if (listp x)
    (let ((xf (first x)))
      (case xf
	;; begin, loop
	((begin loop) 
	 (let ((bil (split-block-item-list (cdr x))))
	  `(,(first bil)
	    ((,xf ,@(second bil))))))
	;; if
	((if)
	 (let ((stat1 (split-statement (third x)))
	       (stat2 (when (fourth x) split-statement (fourth x))))
	   (if (fourth x)
	       `((,@(first stat1) ,@(first stat2))
		 ((if ,(second x)
		       (begin ,@(second stat1))
		       (begin ,@(second stat2)))))
		`(,(first stat1)
		  ((if ,(second x) (begin ,@(second stat1))))))))
	;; switch, while, for
	((switch while for)
	 (let ((bil (split-block-item-list (cddr x))))
	   `(,(first bil) ((,xf ,(second x) ,@(second bil))))))
	(t `(nil (,x)))))
    `(nil (,x))))
