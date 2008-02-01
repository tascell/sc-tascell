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
  '((ptr "*" 2) (mref "&" 2) (bit-not "~" 2) (not "!" 2)))

;;; 結合優先順位
;;;  1 (>) : () [] -> . post++ post--
;;;  2 (<) : ! ~ pre++ pre-- unary+ unary- unary* unary& sizeof
;;;  3 (<) : (type)
;;;  4 (>) : * / %
;;;  5 (>) : + - 
;;;  6 (>) : << >>
;;;  7 (>) : < <= > >=
;;;  8 (>) : == !=
;;;  9 (>) : &
;;; 10 (>) : ^
;;; 11 (>) : |
;;; 12 (>) : &&
;;; 13 (>) : ||
;;; 14 (<) : ?:
;;; 15 (<) : = += -= *= /= %= &= ^= |= <<= >>=
;;; 16 (>) : , 

;;; asoc:結合優先順位
;;; 返値:> or <
(defun asociativity (asoc)
  (case asoc
    ((1 4 5 6 7 8 9 10 11 12 13 16) '>)
    ((2 3 14 15) '<)
    (otherwise nil)))


;;; x:list las ras:結合優先順位
;;; 返値:Cコード文字列
(defun expression 
    (x &optional (las 255) (ras 255) &aux ret1 ret2 ret3 estr (asoc 0))
  (cond
    ((setq ret1 (identifier x))                          ;;; identifier
     (setq asoc 0 estr ret1))    
    ((setq ret1 (constant x))                            ;;; constant
     (setq asoc 0 estr ret1))         
    ((setq ret1 (string-literal x))                      ;;; string-literal
     (setq asoc 0 estr ret1))                           
    ((not (listp x)) (setq estr nil))
    ((setq ret2 (expression-list x 16))                  ;;; function call
     (setq asoc 0)
     (setq ret1 (string+ (car ret2) "" "" "( "))
     (setq estr (string+ (cdr ret2) ", " ret1 " )")))
    ((and (eq (car x) 'aref)
	  (setq ret1 (expression (cadr x) 255 1))
	  (or (endp (cddr x))
	      (setq ret2 (expression-list (cddr x)))))   ;;; aref
     (setq asoc 1)
     (if (endp (cddr x))
	 (setq estr (car ret1))
	 (progn
	   (setq estr 
		 (concatenate 'string ret1 "[" (string+ ret2 "][") "]")))))
    ((and (eq (car x) 'fref)
	  (setq ret1 (expression (cadr x) 255 1))
	  (setq ret2 (field-identifier-list (cddr x))))  ;;; fref
     (setq asoc 1
	   estr (concatenate 'string ret1 ret2)))
    ((and (eq (car x) 'inc)
	  (setq ret1 (expression (cadr x) 255 1))
	  (endp (cddr x)))                               ;;; inc
     (setq asoc 1
	   estr (concatenate 'string ret1 "++ ")))
    ((and (eq (car x) 'dec)
	  (setq ret1 (expression (cadr x) 255 1))
	  (endp (cddr x)))                               ;;; dec
     (setq asoc 1
	   estr (concatenate 'string ret1 "-- ")))
    ((and (eq (car x) '++)
	  (setq ret1 (expression (cadr x) 2 255))             
	  (endp (cddr x)))                               ;;; ++
     (setq asoc 2
	   estr (concatenate 'string " ++" ret1)))
    ((and (eq (car x) '--)
	  (setq ret1 (expression (cadr x) 2 255))             
	  (endp (cddr x)))                               ;;; --
     (setq asoc 2
	   estr (concatenate 'string " --" ret1)))
    ((and (multiple-value-setq (ret1 asoc) (unary-operator (car x)))
	  (setq ret2 (expression (cadr x) asoc 255))
	  (endp (cddr x)))                               ;;; unary-operator
     (setq estr (concatenate 'string ret1 ret2)))
    ((and (eq (car x) '+)
	  (setq ret2 (expression (cadr x) 2 255))
	  (endp (cddr x)))                               ;;; unary+
     (setq asoc 2
	   estr (concatenate 'string " +" ret2)))
    ((and (eq (car x) '-)
	  (setq ret2 (expression (cadr x) 2 255))
	  (endp (cddr x)))                               ;;; unary-
     (setq asoc 2
	   estr (concatenate 'string " -" ret2)))
    ((and (eq (car x) '/)
	  (setq ret2 (expression (cadr x) 4 255))
	  (endp (cddr x)))                               ;;; unary/ ??
     (setq asoc 0
	   estr (concatenate 'string "(1/" ret2 ")")))
    ((and (eq (car x) 'sizeof)
	  (setq ret1 (expression (cadr x) 2 255))
	  (endp (cddr x)))                               ;;; sizeof(exp)
     (setq asoc 2
	   estr (concatenate 'string "sizeof " ret1)))
    ((and (eq (car x) 'sizeof)
	  (setq ret1 (type-expression (cadr x)))
	  (endp (cddr x)))                               ;;; sizeof(t-exp)
     (setq asoc 2
	   estr (concatenate 'string "sizeof(" (make-declstr ret1) ")")))
    ((and (eq (car x) 'cast)
	  (setq ret1 (type-expression (cadr x)))
	  (setq ret2 (expression (caddr x) 3 255)))      ;;; cast
     (setq asoc 3
	   estr (concatenate 'string "(" (make-declstr ret1) ")" ret2)))
    ((and (multiple-value-setq (ret1 asoc) (operator (car x)))
	  (setq ret2 (expression-list (cdr x) asoc)))    ;;; operator
     (setq estr (string+ ret2 ret1)))
    ((and (multiple-value-setq (ret1 asoc) (comparator (car x)))
	  (setq ret2 (expression (cadr x) 255 asoc))   
          (setq ret3 (expression (caddr x) asoc 255))
          (endp (cdddr x)))                              ;;; comparator
          (setq estr (concatenate 'string ret2 ret1 ret3)))
    ((and (eq (car x) 'if-exp)
	  (setq ret1 (expression (cadr x) 14))
	  (setq ret2 (expression (caddr x)))   
          (setq ret3 (expression (cadddr x) 14)))        ;;; if-exp
     (setq asoc 14
	   estr (concatenate 'string ret1 "?" ret2 ":" ret3)))
    ((and (multiple-value-setq (ret1 asoc) (assignment-operator (car x)))
	  (setq ret2 (expression (cadr x) 255 asoc))
	  (setq ret3 (expression (caddr x) asoc 255))
	  (endp (cdddr x)))                              ;;; assignment-op
     (setq estr (concatenate 'string ret2 ret1 ret3)))
    ((and (eq (car x) 'exps)
	  (setq ret1 (expression-list (cdr x) 16))       ;;; exps
     (setq asoc 16
	   estr (string+ ret1 ", "))))
    (t (setq estr nil)))

  (if (null estr) (return-from expression))
  (if (or (< las asoc) (< ras asoc)
	  (and (= las asoc) (eq (asociativity asoc) '>))
	  (and (= ras asoc) (eq (asociativity asoc) '<)))
      (setq estr (concatenate 'string "(" estr ")")))

  estr)

;;; x:list as:結合優先順位
;;; 返値:Cコード文字列のリスト
(defun expression-list (x &optional (as 255) &aux ret1 ret2)
  (if (listp x)
      (do ((a x (cdr a))) ((endp a) ret1)
	(if (setq ret2 (expression (car a) as as))
	    (setq ret1 (add-element ret1 ret2))
	    (return))))) 
  
;;; x:list
;;; 返値:Cコード文字列
(defun field-identifier-list (x &aux (ret1 "") ret2)
  (if (and (listp x) (not (null x)))
      (do ((a x (cdr a)))
	  ((endp a) ret1)
	(cond
	  ((setq ret2 (identifier (car a)))
	   (setq ret1 (concatenate 'string ret1 "." ret2)))
	  ((eq (car a) '->)
	   (setq a (cdr a))
	   (if (setq ret2 (identifier (car a)))
	       (setq ret1 (concatenate 'string ret1 "->" ret2))
	       (return nil)))
	  (t (return nil))))))

;;; x:list
;;; 返値:Cコード文字列 & 結合優先順位 
(defun operator (x &aux ret)
  (setq ret (member x *operator-list* :key 'car))
  (values (cadar ret) (caddar ret)))

;;; x:list
;;; 返値:Cコード文字列 & 結合優先順位 
(defun comparator (x &aux ret)
  (setq ret (member x *comparator-list* :key 'car))
  (values (cadar ret) (caddar ret)))

;;; x:list  
;;; 返値:Cコード文字列 & 結合優先順位
(defun assignment-operator (x &aux ret)
  (setq ret (member x *assignment-operator-list* :key 'car))
  (values (cadar ret) (caddar ret)))


;;; x:list  
;;; 返値:Cコード文字列 & 結合優先順位
(defun unary-operator (x &aux ret)
  (setq ret (member x *unary-operator-list* :key 'car))
  (values (cadar ret) (caddar ret)))
     
