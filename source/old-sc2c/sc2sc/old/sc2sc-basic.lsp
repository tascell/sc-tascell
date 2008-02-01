;;
;; C言語相当の記述方法など記述能力の強化
;; def なし宣言/
;; defs/
;; リスト形式でないfunction-identifier/
;; let/
;; for
;; do-while/
;; loop/
;; field-identifier
;;

(defconstant *defs-to-def*
  '((defs . def) (extern-defs . def) (static-defs . static)
    (auto-defs . auto) (register-defs . register)))

(defun btrans (x)
  (apply #'append (mapcar #'btrans-decl-adding-def x)))

;; 判定条件をもっと厳しくすべきか?
(defun btrans-decl-adding-def (x)
  (when (and (listp x)
	     (or (par-identifier (car x))
		 (and (listp (car x))
		      (not (member nil (mapcar #'par-identifier (car x)))))
		 (par-struct-or-union-specifier (car x))
		 (par-enum-specifier (car x))))
    (push 'def x))
  (btrans-decl x))

(defun btrans-decl (x &aux temp)
  (when (not (listp x)) (return-from btrans-decl `(,x)))	  
  (cond
    ((par-storage-class-specifier (car x))
     (cond
       ((and (listp (caddr x))
	     (eq 'fn (caaddr x)))
	(let ((fid (if (listp (cadr x))
		       (cadr x)
		       (list (cadr x)))))
	  `( (,(car x) ,fid ,(caddr x) ,@(btrans-bitem-list (cdddr x))) )))
       ((and (par-identifier (cadr x))
	     (par-type-expression (caddr x))
	     (cadddr x))
	`( (,(car x) ,(cadr x) ,(caddr x) ,(btrans-expression (cadddr x))) )) 
       ((par-struct-or-union-specifier (cadr x))
	`( (,(car x) ,(cadr x) ,@(btrans (cddr x))) ))
       (t `( ,x ))))
    ((setq temp (assoc (car x) *defs-to-def*))
     (let ((texp (cadr x)))
       (mapcar 
	#'(lambda (ind &aux id init)
	    (if (not (listp ind))
		(setq id ind init nil)
		(setq id (car ind) init (cdr ind)))
	    `(,(cdr temp) ,id ,texp ,@init))
	(cddr x))))
    ((and (eq 'deftype (car x))
	  (par-identifier (cadr x))
	  (or (eq 'struct (caddr x))
	      (eq 'union  (caddr x))) )
     `( (,(car x) ,(cadr x) ,(caddr x) ,@(btrans (cdddr x))) ))
    (t `( ,x ))))
       
(defun btrans-bitem-list (x)
  (apply #'append (mapcar #'btrans-bitem x)))
	      
(defun btrans-bitem (x)
  (when (not (listp x)) (return-from btrans-item `( ,x )))
  (case (car x)
    ((begin)
     `( (begin ,@(btrans-bitem-list (cdr x))) ))
    ((label)
     `( (label ,(cadr x) ,@(btrans-bitem (caddr x))) ))
    ((case)
     `( (case ,(btrans-expression (cadr x))) ))
    ((default)
     '( (default) ))
    ((if)
     `( (if ,(btrans-expression (cadr x))
	     ,@(mapcar #'(lambda (st) (car (btrans-bitem st))) (cddr x))) ))
    ((switch)
     `( (switch ,(btrans-expression (cadr x))
	 ,@(btrans-bitem-list (cddr x))) ))
     ((let)
     `( (begin ,@(btrans (cadr x)) ,@(btrans-bitem-list (cddr x))) ))
    ((do-while)
     `( (do-while ,(btrans-expression (cadr x))
	 ,@(btrans-bitem-list (cddr x))) ))
    ((while)
     (let ((cdr (btrans-expression (cadr x))))
       `( (if ,cdt
	   (do-while ,cdt ,@(btrans-bitem-list (cddr x)))) )))
    ((for)
     (let* ((ex (lastn (cadr x) 2))
	    (exp2 (btrans-expression (car ex)))
	    (exp3 (btrans-expression (cadr ex))))
       `( (begin
	   ,@(btrans-bitem-list (butlast (cadr x) 2))
	   (if ,exp2
		(do-while (exps ,exp3 ,exp2)
		  ,@(btrans-bitem-list (cddr x))))) )))
    ((loop)
     `( (do-while 1 ,@(btrans-bitem-list (cdr x))) ))  
    ((goto continue break)
     `( ,x ))
    ((return)
     `( (return ,@(mapcar #'btrans-expression (cdr x))) ))
    (otherwise
     (cond
       ((or (par-storage-class-specifier (car x))
	    (assoc (car x) *defs-to-def*)
	    (eq 'deftype (car x)))
	(btrans-decl x))
       (t `( ,(btrans-expression x)))))))
     
(defun btrans-expression (x)
  (when (not (listp x)) (return-from btrans-expression x))
  (case (car x)
    ((fref)
     (let* ((y (copy-list x))
	    (ar1 (member '-> y)) ar2)
       (setf (cadr y) (btrans-expression (cadr y)))
       (if ar1
	   (progn
	     (setq ar2 (cdr ar1))
	     (rplacd ar1 nil)
	     (setq y (butlast y))
	     (btrans-expression `(fref (ptr ,y) ,@ar2)))
	   y)))
    (otherwise
     (mapcar #'btrans-expression x))))