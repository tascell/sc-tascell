;;;
;;; 与えられた構造体以下の識別子名(Cの構造体メンバ以外)を変換
;;;

(defvar *old-id* nil)
(defvar *new-id* nil)

(defun exec-rename (sl *old-id* *new-id*)
  (rename-id (car sl) t)
  (mapc 'rename-id (cdr sl)))

(defun rename-id (s &rest r)
  (typecase s
    (translation-unit (rn-translation-unit s))
    (external-declaration (rn-external-declaration s))
    (decl-variable (rn-decl-variable s))
    (decl-function (rn-decl-function s))
    (decl-struct (rn-decl-struct s))
    (decl-enum (rn-decl-enum s))
    (decl-defs (apply 'rn-decl-defs s r))
    (typedef-texp (rn-typedef-texp s))
    (typedef-struct (rn-typedef-struct s))
    (typedef-enum (rn-typedef-enum s))
    (enumerator (rn-enumerator s))
    (initializer (rn-initializer s))
    (type-exp (rn-type-exp s))
    (texp-array (rn-texp-array s))
    (tspec-function (rn-tspec-function s))
    (tspec-name (rn-tspec-name s))
    (tspec-enum (rn-tapec-enum s))
    (tspec-struct (rn-tspec-struct s))
    (compound-stat (rn-compound-stat s))
    (expression-stat (rn-expression-stat s))
    (label-name (rn-label-name s))
    (label-case (rn-label-case s))
    (if-stat (rn-if-stat s))
    (switch-stat (rn-switch-stat s))
    (while-stat (rn-while-stat s))
    (do-while-stat (rn-do-while-stat s))
    (for1-stat (rn-for1-stat s))
    (for2-stat (rn-for2-stat s))
    (return-stat (rn-return-stat s))
    (exp-identifier (rn-exp-identifier s))
    (exp-funcall (rn-exp-funcall s))
    (exp-aref (rn-exp-aref s))
    (exp-fref (rn-exp-fref s))
    (exp-incdec (rn-exp-incdec s))
    (exp-preincdec (rn-exp-preincdec s))
    (exp-unary-op (rn-exp-unary-op s))
    (exp-sizeof (rn-exp-sizeof s))
    (exp-sizeof-t (rn-exp-sizeof-t s))
    (exp-cast (rn-exp-cast s))
    (exp-operator (rn-exp-operator s))
    (exp-comparator (rn-exp-comparator s))
    (exp-ifexp (rn-exp-ifexp s))
    (exp-assignment (rn-exp-assignment s))
    (exp-exps (rn-exp-exps s))
    (otherwise nil)))

;; 置き換え対象の識別子なら置き換え
(defmacro replace-id (loc-id)
  `(when (string= ,loc-id *old-id*)
    (setf ,loc-id *new-id*)))
     
;; 識別子リストに対して置き換えを実行
(defun replace-id-list (idlist)
  (do ((idl idlist (cdr idl)))
      ((endp idl))
    (replace-id (car idl))))

(defun rn-translation-unit (s)
  (mapc 'rename-id (translation-unit-edecl-list s)))

(defun rn-external-declaration (s)
  (rename-id (external-declaration-decl s)))

(defun rn-decl-variable (s)
  (replace-id (decl-variable-id s))
  (rename-id (decl-variable-texp s))
  (rename-id (decl-variable-init s))
  (rename-id (decl-variable-bitfield s)))

(defun rn-decl-function (s)
  (replace-id (decl-function-id s))
  (replace-id-list (decl-function-arg-id-list s))
  (rename-id (decl-function-texp s))
  (mapc 'rename-id (decl-function-arg-texp-list s))
  (replace-id-list (decl-function-reg-arg-list s))
  (mapc 'rename-id (decl-function-item-list s)))

(defun rn-decl-struct (s)
  (replace-id (decl-struct-id s)))

(defun rn-decl-enum (s)
  (replace-id (decl-enum-id s))
  (mapc 'rename-id (decl-enum-elist s)))

;; tailがtのときは、*old-name*の宣言部以降のみをrename
(defun rn-decl-defs (s &optional (tail nil))
  (rename-id (decl-defs-texp s))
  (rename-id (decl-defs-bitfield s))
  (if tail
      (progn
	(replace-id-list (decl-defs-id-list s))
	(mapc 'rename-id (decl-defs-init-list s)))
      (let* ((id-list (decl-defs-id-list s))
	     (n (- (lentgh id-list) (length (memeber *old-id* id-list)))))
	(replace-id-list (nthcdr n id-list))
	(mapc 'rename-id (nthcdr n decl-defs-init-list s)))))

(defun rn-typedef-texp (s)
  (replace-id (typedef-texp-id s))
  (rename-id (typedef-texp-texp s)))

(defun rn-typedef-struct (s)
  (replace-id (typedef-struct-id s)))

(defun rn-typedef-enum (s)
  (replace-id (typedef-enum-id s))
  (mapc 'rename-id (typedef-enum-elist s)))

(defun rn-enumerator (s)
  (replace-id (enumerator-id s))
  (rename-id (enumerator-value s)))

(defun rn-initializer (s)
  (mapc 'rename-id (initializer-init-list s)))

(defun rn-type-exp (s)
  (rename-id (type-exp-spec s))
  (rename-id (type-exp-array s)))

(defun rn-texp-array (s)
  (mapc 'rename-id (texp-array-sub-list s))
  (rename-id (texp-array-array s)))

(defun rn-tspec-function (s)
  (rename-id (tspec-function-texp s))
  (mapc 'rename-id (tspec-function-arg-texp-list s)))

(defun rn-tspec-name (s)
  (replace-id (tspec-name-id s)))

(defun rn-tspec-enum (s)
  (replace-id (tspec-enum-id s)))

(defun rn-tspec-struct (s)
  (replace-id (tspec-struct-id s)))

(defun rn-compound-stat (s)
  (mapc 'rename-id (compound-stat-item-list s)))

(defun rn-expression-stat (s)
  (rename-id (expression-stat-exp s)))

(defun rn-label-name (s)
  (rename-id (label-name-stat s)))

(defun rn-label-case (s)
  (rename-id (label-case-exp s)))

(defun rn-if-stat (s)
  (rename-id (if-stat-exp s))
  (rename-id (if-stat-then-stat s))
  (rename-id (if-stat-else-stat s)))

(defun rn-switch-stat (s)
  (rename-id (switch-stat-exp s))
  (mapc 'rename-id (switch-stat-item-list s)))
 
(defun rn-while-stat (s)
  (rename-id (while-stat-exp s))
  (mapc 'rename-id (while-stat-item-list s)))
 
(defun rn-do-while-stat (s)
  (rename-id (do-while-stat-exp s))
  (mapc 'rename-id (do-while-stat-item-list s)))

(defun rn-for1-stat (s)
  (mapc 'rename-id (for1-stat-exp1-list s))
  (rename-id (for1-stat-exp2 s))
  (rename-id (for1-stat-exp3 s))
  (mapc 'rename-id (for1-stat-item-list s)))

(defun rn-for2-stat (s)
  (mapc 'rename-id (for2-stat-decl-list s))
  (rename-id (for2-stat-exp2 s))
  (rename-id (for2-stat-exp3 s))
  (mapc 'rename-id (for2-stat-item-list s)))

(defun rn-return-stat (s)
  (rename-id (return-stat-exp s)))

(defun rn-exp-identifier (s)
  (replace-id (exp-identifier-id s)))

(defun rn-exp-funcall (s)
  (rename-id (exp-funcall-exp s))
  (mapc 'rename-id (exp-funcall-arg-exp-list s)))

(defun rn-exp-aref (s)
  (rename-id (exp-aref-exp s))
  (mapc 'rename-id (exp-aref-aref-exp-list s)))

(defun rn-exp-fref (s)
  (rename-id (exp-fref-exp s)))

(defun rn-exp-incdec (s)
  (rename-id (exp-incdec-exp s)))

(defun rn-exp-preincdec (s)
  (rename-id (exp-preincdec-exp s)))

(defun rn-exp-unary-op (s)
  (rename-id (exp-unary-op-exp s)))

(defun rn-exp-sizeof (s)
  (rename-id (exp-sizeof-exp s)))

(defun rn-exp-sizeof-t (s)
  (rename-id (exp-sizeof-t-texp s)))

(defun rn-exp-cast (s)
  (rename-id (exp-cast-texp s))
  (rename-id (exp-cast-exp s)))

(defun rn-exp-operator (s)
  (mapc 'rename-id (exp-operator-exp-list s)))

(defun rn-exp-comparator (s)
  (rename-id (exp-comparator-exp1 s))
  (rename-id (exp-comparator-exp2 s)))

(defun rn-exp-ifexp (s)
  (rename-id (exp-ifexp-exp1 s))
  (rename-id (exp-ifexp-exp2 s))
  (rename-id (exp-ifexp-exp3 s)))

(defun rn-exp-assignment (s)
  (rename-id (exp-assignment-exp1 s))
  (rename-id (exp-assignment-exp2 s)))

(defun rn-exp-exps (s)
  (mapc 'rename-id (exp-exps-exp-list s)))

  