;;; x:list
;;; 返値:Cコード文字列
(defun statement (x &aux ret)
  (cond
    ((null x) ";")
    ((setq ret (compound-statement x)) ret)
    ((setq ret (expression-statement x)) ret)
    ((setq ret (selection-statement x)) ret)
    ((setq ret (iteration-statement x)) ret)
    ((setq ret (jump-statement x)) ret)
    ((setq ret (labeled-statement x)) ret)
    (t nil)))

;;; x:list
;;; 返値:Cコード文字列
(defun compound-statement (x &aux (ret1 "") (ret2 "") (ret3 ""))
  (if (not (listp x)) (return nil))
  (cond
    ((and (eq (car x) 'begin)
	  (or (endp (cdr x))
	      (setq ret1 (block-item-list (cdr x)))))   ;;; "begin"
     (string+ ret1 "" "{" "}"))
    ((and (eq (car x) 'let)
	  (or (null (cadr x))
	      (setq ret1 (declaration-list (cadr x))))  ;;; "let"
	  (or (endp (cddr x))
	      (setq ret2 (block-item-list (cddr x)))))
     (setq ret3 (concatenate 'string "{" (string+ ret1)))
     (setq ret3 (concatenate 'string ret3 (string+ ret2) "}")))
    (t nil)))

;;; x:list
;;; 返値:Cコード文字列のリスト
(defun block-item-list (x &aux (ret1 nil) ret2)
  (if (listp x)
      (dolist (a x ret1)
	(if (setq ret2 (block-item a))
	    (setq ret1 (nconc ret1 (list ret2)))
	    (return nil)))))
      
;;; x:list
;;; 返値:Cコード文字列
(defun block-item (x &aux ret)
  (cond
    ((setq ret (inlined-declaration x)) ret)
    ((setq ret (statement x)) ret)
    (t nil)))

;;; x:list
;;; 返値:Cコード文字列
(defun labeled-statement (x &aux ret1 ret2)
  (if (not (listp x)) (return nil))
  (cond
    ((and (eq (car x) 'label)
	  (setq ret1 (identifier (cadr x)))
	  (setq ret2 (statement (caddr x)))
          (endp (cdddr x)))                      ;;; "label"      
     (concatenate 'string ret1 ": " ret2))
    ((and (eq (car x) 'case)
	  (setq ret1 (expression (cadr x) 15 15))
	  (endp (cddr x)))                       ;;; "case"
     (concatenate 'string "case " ret1 ":"))
    ((and (eq (car x) 'default)
	  (endp (cdr x)))                        ;;; "nil"
     "default:")
    (t nil)))

;;; x:list
;;; 返値:Cコード文字列
(defun expression-statement (x &aux ret)
  (if (setq ret (expression x))
      (concatenate 'string ret ";")))

;;; x:list
;;; 返値:Cコード文字列
(defun selection-statement (x &aux ret1 ret2 ret3)
  (if (not (listp x)) (return nil))
  (cond
    ((and (eq (car x) 'if)
	  (setq ret1 (expression (cadr x)))
	  (setq ret2 (statement (caddr x)))
          (or (endp (cdddr x))
	      (and (setq ret3 (statement (cadddr x)))
		   (endp (cddddr x)))))                         ;;; if
     (if (endp (cdddr x))
	 (setq ret3 "")
	 (setq ret3 (concatenate 'string " else "  ret3)))
     (concatenate 'string "if( " ret1 " )" ret2 ret3))
    ((and (eq (car x) 'switch)
	  (setq ret1 (expression (cadr x)))
	  (or (endp (cddr x))
	      (setq ret2 (block-item-list (cddr x)))))          ;;; switch
     (if (endp (cddr x)) (setq ret2 ""))
     (concatenate 'string "switch( " ret1 " ) " "{ " (string+ ret2) " }"))
    (t nil)))

;;; x:list
;;; 返値:Cコード文字列
(defun iteration-statement (x &aux ret1 ret2 ret3 ret4 ret5)
  (if (not (listp x)) (return nil))
  (cond
    ((and (eq (car x) 'while)
	  (setq ret1 (expression (cadr x)))
	  (or (endp (cddr x))
	      (setq ret2 (block-item-list (cddr x)))))       ;;; while
     (if (endp (cddr x)) (setq ret2 ""))
     (concatenate 'string "while( " ret1 " ){ " (string+ ret2) " }"))
    ((and (eq (car x) 'do-while)
	  (setq ret1 (expression (cadr x)))
	  (or (endp (cddr x))
	      (setq ret2 (block-item-list (cddr x)))))        ;;; do-while
     (if (endp (cddr x)) (setq ret2 ""))
     (concatenate 'string "do{ " (string+ ret2) " }"
		  "while( " ret1 " );"))
    ((and (eq (car x) 'for)
	  (listp (cadr x))
	  (let* ((a (cadr x)) (b (butlast a 2)) (c (lastn a 2)))
	    (prog1
		(and (or (null b)
			 (setq ret1 (expression-list b 16)))
		     (setq ret2 (expression (car c)))
		     (setq ret3 (expression (cadr c))))
	      (if (null b) (setq ret1 nil))))
	  (or (endp (cddr x))
	      (setq ret4 (block-item-list (cddr x)))))        ;;; for(1)
     (if (endp (cddr x)) (setq ret4 ""))
     (concatenate 'string
		  "for( "
		  (string+ ret1 ", " "" " ; " )
		  ret2 " ; " ret3 " ){ "
		  (string+ ret4) " }" ))
    ((and (eq (car x) 'for)
	  (listp (cadr x))
	  (let* ((a (cadr x)) (b (butlast a 2)) (c (lastn a 2)))
	    (and (setq ret1 (inlined-declaration-list b))
		 (setq ret2 (expression (car c)))
		 (setq ret3 (expression (cadr c)))))
	  (or (endp (cddr x))
	      (setq ret4 (block-item-list (cddr x)))))        ;;; for(2)暫定
     (if (endp (cddr x)) (setq ret4 ""))
     (if (endp (cdr ret1))
	 (concatenate 'string "for( " (car ret1) ret2 " ; " ret3
		      " ){ " (string+ ret4) " }")
	 (progn
	   (let ((*standard-output* *error-output*))
	     (princ x) (princ #\Linefeed)
	     (princ "warning: ")
	     (princ "Declarations moved out of 'for' statement")
	     (princ #\Linefeed))
	   (concatenate 'string "{ " (string+ ret1)
			"for(;" ret2 " ; " ret3 " ){ "
			(string+ ret4) " } }" ))))
    ((and (eq (car x) 'loop)
	  (or (endp (cdr x))
	      (setq ret1 (block-item-list (cdr x)))))         ;;; loop
     (concatenate 'string "while(1){ " (string+ ret1) " }"))
    (t nil)))

;;; x:list
;;; 返値:Cコード文字列
(defun jump-statement (x &aux ret)
  (if (not (listp x)) (return nil))
  (cond
    ((and (eq (car x) 'goto)
	  (setq ret (identifier (cadr x)))
          (endp (cddr x)))                          ;;; goto
     (concatenate 'string "goto " ret ";"))
    ((and (eq (car x) 'continue)
	  (endp (cdr x)))                           ;;; continue
     "continue;")
    ((and (eq (car x) 'break)
	  (endp (cdr x)))                           ;;; break
     "break;")
    ((and (eq (car x) 'return)
	  (or (endp (cdr x))
	      (and (setq ret (expression (cadr x)))
		   (endp (cddr x)))))               ;;; return
     (if (endp (cdr x))
	 "return;"
	 (concatenate 'string "return( " ret " );" )))
    (t nil)))



    
		  
		 
		 
	   
