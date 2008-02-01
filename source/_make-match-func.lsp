;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; おまけ
;;;;;;;;;;;;;
;; pattern0 と *arg-name* が match するかどうかを判定する関数を作る
;; pattern -> ( x -> {nil | <bind-list>} )
(defun make-match-func (&optional (pattern0 (read-pattern)))
  (labels 
      ;; 入れ子関数の返り値は (<check-s> <check-c>)
      ;; check-s : マッチするために必要なラムダ式のリスト
      ;;           (リストの構造に関するもの)
      ;; check-c : マッチするために必要なラムダ式のリスト
      ;;           ([...]で指定された関数)
      ;; get-focus : x の現在の注目部分を得るための関数
      ;; get-rest  : x の残りの部分を得るための関数
      ((make-match-func-1exp (pattern check-s check-c get-focus)
	 (if (listp pattern)
	     (cond
	       ((eq comma (car pattern))
		(list
		 (cons (compose
			(aif (third pattern)
			     #'(lambda (x)
				 (let ((retval-list
					(multiple-value-list
					 (funcall it (second x)))))
				   (when (car retval-list)
				     (list (append x retval-list)))))
			     #'list)
			#'(lambda (x)
			    (list (second pattern) 
				  (funcall get-focus x))))
		       check-s)
		 check-c)
		)
	       ;;
	       ((eq comma-at (car pattern))
		(error "~a: pattern consists of only ,@<symbol>" 
		       (pattern-print pattern0))
		)
	       ;;
	       (t
		(make-match-func-list
		 pattern
		 (cons (compose #'listp get-focus) check-s)
		 check-c
		 get-focus
		 )))
	     ;;
	     (list (cons (compose #'(lambda (x) (symbol= pattern x))
				  get-focus)
			 check-s)
		   check-c)
	     ))
       ;;;;;;
       (make-match-func-list (pattern check-s check-c get-rest)
	 (cond 
	   ((endp pattern)
	    (list (cons (compose #'endp get-rest) check-s)
		  check-c))
	   ;;
	   ((and (listp (car pattern))
		 (eq comma-at (caar pattern)))
	    (let ((matchrest (make-match-func (cdr pattern))))
	      (list check-s
		    (cons #'(lambda (x 
				     &aux 
				     (rest0 (funcall get-rest x))
				     (rest rest0)
				     (sname (second (car pattern)))
				     (testfunc (third (car pattern))))
			      (loop
			       (awhen (funcall matchrest rest)
				 (let ((prev (ldiff rest0 rest))
				       (retval-list nil))
				   (when (or (not testfunc)
					     (car 
					      (setq retval-list
						    (multiple-value-list
						     (check-mapcar testfunc
								   prev)))))
				     (return 
				       (cons `(,sname ,prev ,@retval-list)
					     (remove 'x it :key #'car))))))
			       (progn
				 (if (endp rest)
				     (return nil)
				     (setq rest (cdr rest))))))  
			  check-c))))
	   ;;
	   (t
	    (let* ((make1 (make-match-func-1exp
			   (car pattern)
			   nil nil
			   (compose #'car get-rest)))
		   (check-s1 (first make1))
		   (check-c1 (second make1)))
	      (make-match-func-list
	       (cdr pattern)
	       (cons (compose #'not #'null get-rest)
		     (append check-s1 check-s))
	       (append check-c1 check-c)
	       (compose #'cdr get-rest)))))))
    ;;;;;	   
    (let ((ret (make-match-func-1exp pattern0 nil nil #'identity)))
      #'(lambda (x)
	  (awhen
	   (check-mapcar
	    #'(lambda (f) (funcall f x))
	    (append (reverse (first ret)) (reverse (second ret))))
	   (cons `(x ,x) 
		 (reduce #'append (remove-if-not #'listp it)
			 :from-end t)))))))
