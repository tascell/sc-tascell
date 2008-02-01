;;;
;;; thread-local-storage
;;;

(defun tls-ext (x)
  (let ((plist (remove nil (mapcar #'get-private-def x))))
    `((def (struct thread-local-storage) ,@plist)
      ,@(remove nil (mapcar #'tls-bitem x)))))

(defun get-private-def (x)
  (cond 
    ((and (listp x)
	  (eq 'private (car x)))
     `(def ,@(cdr x)))
    (t nil)))

(defun tls-bitem (x)
  (when (not (listp x)) (return-from tls-bitem x))
  (let ((x1 (first x)) (x2 (second x)) (x3 (third x)) (x4 (fourth x)))
    (cond
      ((eq 'private x1) nil)
      ((par-storage-class-specifier x1)
       (cond
	 ((par-identifier x2)
	  (if x4
	      `(,x1 ,x2 ,(tls-bitem x3) ,(tls-bitem x4))
	      `(,x1 ,x2 ,(tls-bitem x3))))
	 ((and (listp x2)
	       (not (member nil (mapcar #'par-identifier x2)))
	       (listp x3)
	       (eq 'fn (car x3)))
	  `(,x1 (,(car x2) tls ,@(cdr x2))
	    (fn ,(tls-bitem (cadr x3)) (ptr (struct thread-local-storage))
	     ,@(mapcar #'tls-bitem (cddr x3)))
	    ,@(mapcar #'tls-bitem (cdddr x))))
	 ((par-struct-or-union-specifier x2)
	  `(,x1 ,x2 ,@(mapcar #'tls-bitem (cddr x))))
	 (t x)))
      ((eq 'deftype x1)
       (cond
	 ((or (eq 'struct x3)
	      (eq 'union x3))
	  `(,x1 ,x2 ,x3 ,@(mapcar #'tls-bitem (cdddr x))))
	 ((eq 'enum x3)
	  x)	 
	 (t `(,x1 ,x2 ,(tls-bitem x3)))))
      ((par-expression x1)
       `(,(tls-bitem x1) tls ,@(mapcar #'tls-bitem (cdr x))))
      ((eq 'fn x1)
       `(fn ,(tls-bitem x2) (ptr (struct thread-local-storage))
	 ,@(mapcar #'tls-bitem (cddr x))))       
      (t (mapcar #'tls-bitem x)))))
