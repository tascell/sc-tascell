(provide "SC-UTIL")

(make-package "SC-UTIL" :use '("LISP"))
(in-package "SC-UTIL")

(export '(add-paren keywd char-sc2c string-sc2c operator-sc2c
	  *keyword-list* *storage-class-specifier-list*
	  *compound-storage-class-specifier-list* *type-specifier-list*
	  *operator-list* *comparator-list* *assignment-operator-list*
	  *unary-operator-list*
	  par-storage-class-specifier par-compound-storage-class-specifier
	  par-constant par-string-literal par-expression
	  par-type-qualifier par-operator par-comparator
	  par-assignment-operator par-unary-operator get-idstring
	  par-identifier))

(require 'sc-main)
(require 'sc-misc)
(use-package "SC-MISC")

(defconstant *scutil-package* *package*)

;;;; SC構文解析関連のお役立ち関数

;;; かっこをつける
(defun add-paren (str)
  (format nil "(~A)" str))

;;; キーワードかどうかを判定
(defun keywd (x)
  (member x *keyword-list* :test #'symbol=))

;;; SC文字->C文字
;;; 返値はstring
(defun char-sc2c (ch)
  (if (not (characterp ch)) 
      (error "ch is not character."))
  (case ch
    (#\Space " ")
    (#\Tab "\\t")
    (#\BackSpace "\\b")
    (#\Rubout "\\f")
    (#\Return "\\r")
    (#\Linefeed "\\n")
    (#\Page "\\f")
    (#\NewLine "\\n")
    (#\\ "\\\\")
    (#\' "\\'")
    (#\" "\\\"")
    (otherwise (string ch))))

;;; SC文字列->C文字列
(defun string-sc2c (str &aux (retstr ""))
  (unless (stringp str)
    (error "str is not string"))
  (let ((f-str (format nil str)))
    (dotimes (i (length f-str) retstr)
      (add-string retstr (char-sc2c (aref f-str i))))))

;;; SC-0演算子->C演算子
(defun operator-sc2c (op)
  (case (immigrate-package op *scutil-package*)
    ((inc) "++")
    ((dec) "--")
    ((++ --) (string op))
    ((ptr) "&")
    ((mref) "*")
    ((bit-not) "~")
    ((not) "!")
    ((* / % + - << >>) (string op))
    ((bit-xor) "^")
    ((bit-and) "&")
    ((bit-or) "|")
    ((and) "&&")
    ((or) "||")
    ((< > <= >= == !=) (string op))
    ((= *= /= %= += -= <<= >>=) (string op))
    ((bit-and=) "&=")
    ((bit-xor=) "^=")
    ((bit-or=) "|=")
    ((exps) ",")
    (otherwise (error "unexpected operator ~S" op))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; sc2c 構文解析部の残骸
;;; 将来的には par-identifier 以外は変形規則に置き換えてしまいたい
;;;

;;; 2003/11/11 mref->'*' ptr->'&' に意味を変更

(defvar *keyword-list*
  '(fn inline va-arg :attr deftype enum struct union :bit array ptr
    def decl extern extern-def extern-decl static static-def
    auto auto-def register register-def
    defs extern-defs static-defs auto-defs register-defs
    void char signed-char unsigned-char short signed-short unsigned-short
    int signed-int unsigned-int long signed-long unsigned-long
    long-long signed-long-long unsigned-long-long float double long-double
    const restrict volatile
    begin let label case default if switch while do-while for loop
    goto continue break return
    aref fref inc dec sizeof cast if-exp exps
    bit-xor bit-and bit-or and or bit-and= bit-xor= bit-or=
    ptr mref bit-not not
    defmacro private))


(defvar *storage-class-specifier-list*
;; 第３要素:定義->t 宣言のみ->nil
  '((def "" t) (decl "" nil) (extern "extern" t) (extern-def "extern" t)
    (extern-decl "extern" nil) (static "static" t) (static-def "static" t) 
    (auto "auto" t) (auto-def "auto" t) (register "register" t) 
    (register-def "register" t)))

(defvar *compound-storage-class-specifier-list*
  '((defs "") (extern-defs "extern") (static-defs "static") (auto-defs "auto")
    (register-defs "register")))

(defvar *type-specifier-list*
  '((void "void") (char "char") (signed-char "signed char") 
    (unsigned-char "unsigned char") (short "short")
    (signed-short "signed short") (unsigned-short "unsigned short")
    (int "int") (signed-int "signed int") (unsigned-int "unsigned int")
    (long "long") (signed-long "signed long") (unsigned-long "unsigned long")
    (long-long "long long") (signed-long-long "signed long long")
    (unsigned-long-long "unsigned long long") (float "float")
    (double "double") (long-double "long double")))

;; (<sc表現> <c表現> <優先度>)
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
  '((ptr "&" 2) (mref "*" 2) (bit-not "~" 2) (not "!" 2)
    (+ "+" 2) (- "-" 2)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun par-storage-class-specifier (x)
  (let ((ret (car (member x *storage-class-specifier-list*
			  :key #'car :test #'symbol=))))
    (cdr ret)))

(defun par-compound-storage-class-specifier (x)
  (let ((ret 
	 (car (member x *compound-storage-class-specifier-list* 
		      :key #'car :test #'symbol=))))
    (cdr ret)))

(defun par-constant (x)
  (cond
    ( (integerp x) (write-to-string x) )
    ( (floatp x) (write-to-string x) )
    ( (characterp x) (string+ "'" (char-sc2c x) "'"))
    ( t (par-identifier x) ))) ;;;==enumeration-constant

(defun par-string-literal (x)
  (stringp x))

(defun par-expression (x)
  (or (par-constant x)
      (par-string-literal x)
      (and x
	   (listp x)
	   (or
	    (member (car x)
		    '(aref fref inc dec ++ -- sizeof cast if-exp exps c-exp)
		    :test #'symbol=)
	    (par-unary-operator (car x))
	    (par-operator (car x))
	    (par-comparator (car x))
	    (par-assignment-operator (car x))
	    (par-expression (car x))))))

(defun par-type-qualifier (x)
  (case x
    ((const) "const")
    ((restrict) "restrict")
    ((volatile) "volatile")
    (otherwise nil)))

(defun par-operator (x)
  (cdar (member x *operator-list* :key 'car :test #'symbol=)))

(defun par-comparator (x)
  (cdar (member x *comparator-list* :key 'car :test #'symbol=)))

(defun par-assignment-operator (x)
  (cdar (member x *assignment-operator-list* :key 'car :test #'symbol=)))

(defun par-unary-operator (x)
  (cdar (member x *unary-operator-list* :key 'car :test #'symbol=)))

(defun get-idstring (symstr)
  (let ((idstr (make-string (length symstr))) 
	(getcase (if (eq *print-case* :downcase)
		     'char-downcase
		     'char-upcase))
	(m 0))
    (setq idstr
	  (dotimes (n (length symstr) (remove #\Space idstr))
	    (let ((s (aref symstr n)))
	      (if (char= s #\@)
		  (if (eq getcase 'char-upcase)
		      (setq getcase 'char-downcase)
		      (setq getcase 'char-upcase))
		  (progn
		    (when (char= s #\-) (setq s #\_))
		    (setf (aref idstr m) (funcall getcase s))
		    (incf m))))))
    (if (or (string= idstr "") 
	    (string= idstr "_"))
	nil
	idstr)))

(defun par-identifier (x &aux sym (*print-case* :downcase))
  (when (and (symbolp x) (not (keywd x)) (not (null x)))    
    (setq sym (symbol-name x))
    (when (let ((s1 (aref sym 0)))
	    (or (alpha-char-p s1) 
		(char= s1 #\_) 
		(char= s1 #\-) 
		(char= s1 #\@)))
      (dotimes (n (1- (length sym)) 
		(let ((ret (get-idstring sym)))
		  (unless (string= "" (remove #\_ ret :test #'char=))
		    ret)))
	(unless (let ((sn (aref sym (1+ n))))
		  (or (alphanumericp sn) 
		      (char= sn #\_) 
		      (char= sn #\-) 
		      (char= sn #\@)))
	  (return nil))))))
