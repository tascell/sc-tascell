;;;
;;; スレッドローカルストレージの適用
;;;
(defvar *thread-local-storage* nil)
(defvar *tls-struct-id* 'thread_local_storage)
(defvar *tls-arg-id* "__ptls")

;;;
(defun get-tls (x)
  (setq *thread-local-storage* nil)
  (push nil x)
  (do ((y x)) ((endp (cdr y)) (setq x (cdr x)))
    (let ((ye (cadr y)))
      (if (and (listp ye)
	       (eq (car ye) 'private))
	  (progn
	    (entry-tls ye)
	    (rplacd y (cddr y)))
	  (setq y (cdr y)))))
  
  (setq *thread-local-storage*
	(sort *thread-local-storage* 'string< 
	      :key '(lambda (x) (par-identifier (second x)))))
  
  (when (and *thread-local-storage* *exec-tls*)
    (push `(def ,`(struct ,*tls-struct-id*) ,@*thread-local-storage*) x))
  x)

(defun entry-tls (x)
  (rplaca x 'def)
  (push x *thread-local-storage*))  
      
;;;     
(defun add-tls-arg (s)
  (typecase s
    (translation-unit (tls-translation-unit s) s)
    (decl-variable (tls-decl-variable s) s)
    (decl-function (tls-decl-function s) s)
    (decl-struct (tls-decl-struct s) s)
    (decl-enum (tls-decl-enum s) s)
    (decl-defs (tls-decl-defs s) s)
    (typedef-texp (tls-typedef-texp s) s)
    (typedef-struct (tls-typedef-struct s) s)
    (typedef-enum (tls-typedef-enum s) s)
    (enumeartor (tls-enumerator s) s)
    (initializer (tls-initializer s) s)
    (type-exp (tls-type-exp s) s)
    (texp-array (tls-texp-array s) s)
    (tspec-function (tls-tspec-function s) s)
    (compound-stat (tls-compound-stat s) s)
    (expression-stat (tls-expression-stat s) s)
    (label-name (tls-label-name s) s)
    (label-case (tls-label-case s) s)
    (if-stat (tls-if-stat s) s)
    (switch-stat (tls-switch-stat s) s)
    (while-stat (tls-while-stat s) s)
    (for1-stat (tls-for1-stat s) s)
    (for2-stat (tls-for2-stat s) s)
    (return-stat (tls-return-stat s) s)
    (exp-funcall (tls-exp-funcall s) s)
    (exp-aref (tls-exp-aref s) s)
    (exp-fref (tls-exp-fref s) s)
    (exp-incdec (tls-exp-incdec s) s)
    (exp-preincdec (tls-exp-preincdec s) s)
    (exp-unary-op (tls-unary-op s) s)
    (exp-sizeof (tls-exp-sizeof s) s)
    (exp-sizeof-t (tls-exp-sizeof-t s) s)
    (exp-cast (tls-exp-cast s) s)
    (exp-operator (tls-exp-operator s) s)
    (exp-comparator (tls-exp-comparator s) s)
    (exp-ifexp (tls-exp-ifexp s) s)
    (exp-assignment (tls-exp-assignment s) s)
    (exp-exp-exps (tls-exp-exps s) s)
    (othereise s)))

(defun tls-translation-unit (s)
  (mapc 'add-tls-arg (translation-unit-edecl-list s)))

(defun tls-decl-variable (s)
  (add-tls-arg (decl-variable-texp s))
  (add-tls-arg (decl-variable-init s))
  (add-tls-arg (decl-variable-bitfield s)))

(defun tls-decl-function (s)
  (push *tls-arg-id* (decl-function-arg-id-list s))
  (add-tls-arg (decl-function-texp s))
  (push (make-type-exp :plev 1
		       :spec (make-tspec-struct 
			      :s-or-u 'struct
			      :id (par-identifier *tls-struct-id*)))
	(decl-function-arg-texp-list s))
  (mapc 'add-tls-arg (decl-function-item-list s)))

(defun tls-decl-struct (s)
  (mapc 'add-tls-arg (decl-struct-sdecl-list s)))

(defun tls-decl-enum (s)
  (mapc 'add-tls-arg (decl-enum-elist s)))

(defun tls-decl-defs (s)
  (add-tls-arg (decl-defs-texp s))
  (mapc 'add-tls-arg (decl-defs-init-list s))
  (add-tls-arg (decl-defs-bitfield s)))

(defun tls-typedef-texp (s)
  (add-tls-arg (typedef-texp-texp s)))

(defun tls-typedef-struct (s)
  (mapc 'add-tls-arg (typedef-struct-sdecl-list s)))

(defun tls-typedef-enum (s)
  (mapc 'add-tls-arg (typedef-enum-elist s)))

(defun tls-enumerator (s)
  (add-tls-arg (enumerator-value s)))

(defun tls-initializer (s)
  (mapc 'add-tls-arg (initializer-init-list s)))

(defun tls-type-exp (s)
  (add-tls-arg (type-exp-spec s))
  (add-tls-arg (type-exp-array s)))

(defun tls-texp-array (s)
  (mapc 'add-tls-arg (texp-array-sub-list s))
  (add-tls-arg (texp-array-array s)))

(defun tls-tspec-function (s)
  (add-tls-arg (tspec-function-texp s))
  (push (make-type-exp :plev 1
		       :spec (make-tspec-struct 
			      :s-or-u 'struct
			      :id (par-identifier *tls-struct-id*)))
	(tspec-function-arg-texp-list s)))

(defun tls-compound-stat (s)
  (mapc 'add-tls-arg (compound-stat-item-list s)))

(defun tls-expression-stat (s)
  (add-tls-arg (expression-stat-exp s)))

(defun tls-label-name (s)
  (add-tls-arg (label-name-stat s)))

(defun tls-label-case (s)
  (add-tls-arg (label-case-exp s)))

(defun tls-if-stat (s)
  (add-tls-arg (if-stat-exp s))
  (add-tls-arg (if-stat-then-stat s))
  (add-tls-arg (if-stat-else-stat s)))

(defun tls-switch-stat (s)
  (add-tls-arg (switch-stat-exp s))
  (mapc 'add-tls-arg (switch-stat-item-list s)))

(defun tls-while-stat (s)
  (add-tls-arg (while-stat-exp s))
  (mapc 'add-tls-arg (while-stat-item-list s)))

(defun tls-do-while-stat (s)
  (add-tls-arg (do-while-stat-exp s))
  (mapc 'add-tls-arg (do-while-stat-item-list s)))

(defun tls-for1-stat (s)
  (mapc 'add-tls-arg (for1-stat-exp1-list s))
  (add-tls-arg (for1-stat-exp2 s))
  (add-tls-arg (for1-stat-exp3 s))
  (mapc 'add-tls-arg (for1-stat-item-list s)))

(defun tls-for2-stat (s)
  (mapc 'add-tls-arg (for2-stat-decl-list s))
  (add-tls-arg (for2-stat-exp2 s))
  (add-tls-arg (for2-stat-exp3 s))
  (mapc 'add-tls-arg (for2-stat-item-list s)))

(defun tls-return-stat (s)
  (add-tls-arg (return-stat-exp s)))

(defun tls-exp-funcall (s)
  (add-tls-arg (exp-funcall-exp s))
  (push (make-exp-identifier :id *tls-arg-id*) (exp-funcall-arg-exp-list s)))

(defun tls-exp-aref (s)
  (add-tls-arg (exp-aref-exp s))
  (mapc 'add-tls-arg (exp-aref-aref-exp-list s)))

(defun tls-exp-fref (s)
  (add-tls-arg (exp-fref-exp s)))

(defun tls-exp-incdec (s)
  (add-tls-arg (exp-incdec-exp s)))

(defun tls-exp-preincdec (s)
  (add-tls-arg (exp-preincdec-exp s)))

(defun tls-exp-unary-op (s)
  (add-tls-arg (exp-unary-op-exp s)))

(defun tls-exp-sizeof (s)
  (add-tls-arg (exp-sizeof-exp s)))

(defun tls-exp-sizeof-t (s)
  (add-tls-arg (exp-sizeof-t-texp s)))

(defun tls-exp-cast (s)
  (add-tls-arg (exp-cast-texp s))
  (add-tls-arg (exp-cast-exp s)))

(defun tls-exp-operator (s)
  (mapc 'add-tls-arg (exp-operator-exp-list s)))

(defun tls-exp-comparator (s)
  (add-tls-arg (exp-comparator-exp1 s))
  (add-tls-arg (exp-comparator-exp2 s)))

(defun tls-exp-ifexp (s)
  (add-tls-arg (exp-ifexp-exp1 s))
  (add-tls-arg (exp-ifexp-exp2 s))
  (add-tls-srg (exp-ifexp-exp3 s)))

(defun tls-exp-assignment (s)
  (add-tls-arg (exp-assignment-exp1 s))
  (add-tls-arg (exp-assignment-exp2 s)))

(defun tls-exp-exps (s)
  (mapc 'add-tls-arg (exp-exps-exp-list s)))



