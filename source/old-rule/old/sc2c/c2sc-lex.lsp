;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; C Lexer
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(provide "C2SC-LEX")
(in-package "C2SC")
(require "SC-MISC")
(use-package "SC-MISC")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defvar *want-debugging-point* nil)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; to allow gcc "__builtin_va_list" type
(unless (member 'builtin-va-list *features*)
  (setq *features* (cons 'builtin-va-list *features*)))

(defvar *c89* t)


(defstruct lex-env
  lptr             ;; string
  filename         ;; string
  lnum             ;; int
  had-error        ;; bool
  line             ;; string        
  getline          ;; stream->string
  stream           ;; stream
  abort-parse      ;; bool
  lexeme           ;; lexeme
)

(defstruct lexeme
  identifier       ;; string
  constant         ;; datum expressing constant
)

(defvar *lex-env* (make-lex-env))
(defvar *lexeme* (make-lexeme))

(defvar *constant* "")

(defconstant *keytab*
  '(("_Bool"    	BOOL 		nil)
    ("_Complex" 	COMPLEX 	nil)
    ("_Imaginary" 	IMAGINARY 	nil)
    ("auto"     	AUTO 		nil)
    ("break"    	BREAK 		t)
    ("case"     	CASE 		nil)
    ("char"     	CHAR 		nil)
    ("const"    	CONST 		nil)
    ("continue" 	CONTINUE 	t)
    ("default"  	DEFAULT 	nil)
    ("do" 		DO 		nil)
    ("double"   	DOUBLE 		nil)
    ("else"     	ELSE 		nil)
    ("enum"     	ENUM 		nil)
    ("extern"   	EXTERN 		nil)
    ("float"    	FLOAT 		nil)
    ("for" 		FOR 		t)
    ("goto"     	GOTO 		nil)
    ("if" 		IF 		nil)
    ("inline"   	INLINE 		nil)
    ("int" 		INT 		nil)
    ("long"     	LONG 		nil)
    #+builtin-va-list
    ("__builtin_va_list" BUILTIN-VA-LIST nil)
    ("register" 	REGISTER 	nil)
    ("restrict" 	RESTRICT 	nil)
    ("return"   	RETURN 		t)
    ("short"    	SHORT 		nil)
    ("signed"   	SIGNED 		nil)
    ("sizeof"   	SIZEOF 		nil)
    ("static"   	STATIC 		nil)
    ("struct"   	STRUCT 		nil)
    ("switch"   	SWITCH 		nil)
    ("typedef"  	TYPEDEF 	nil)
    ("union"    	UNION 		nil)
    ("unsigned" 	UNSIGNED 	nil)
    ("void"     	VOID 		nil)
    ("volatile" 	VOLATILE 	nil)
    ("while" 	        WHILE 		nil)))

(defconstant *nkeys* (length *keytab*))
(defun lex-error (str)
  (format *error-output* "Error: ~a" str))

;;; behave like isspace in C library
(defun is-space (char)
  (member char '(#\Tab #\Newline #\Page #\Return #\Space)
	   :test #'char=))


;;; #pragma(ignore), #line
(defun parse-hash-directive (line lexenv)
  (let* ((line (string-left-trim-space line))
	 (line-length (length line)))
    (cond 
      ((string= "" line)
       line)
      ((and (<= 6 line-length)
	    (string= "pragma" line :end2 6))
       (warn "#pragma '~a' ignored" 
	     (string-left-trim-space (string-left-ntrim line 6)))
       "")
      ((and (<= 4 line-length)
	    (string= "line" line :end2 4))
       (with-input-from-string (stream (string-left-ntrim line 4))
	 (awhen (read stream)
		(setf (lex-env-lnum lexenv) (- it 2)))
	 ;; Subtract 1 because we number internally from 0,
	 ;;  and 1 because we are just about to bump the
	 ;;  line number.
	 (awhen (read stream)
		(setf (lex-env-filename lexenv) it)))
       "")
      (t ""))))


;;
(defun ci-translate-escape (str)
  (let ((val
	 (case (string-ref str 0)
	   ((#\n) 10)
	   ((#\t) 9)
	   ((#\v) 11)
	   ((#\b) 8)
	   ((#\r) 13)
	   ((#\f) 12)
	   ((#\a) 7)
	   ;; Octal
	   ((#\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7)
	    (do ((i 0 (1+ i)) 
		 (val 0 val))
		((or (>= i 3)
		     (>= i (length str))
		     (not (char<= #\0 (string-ref str i) #\7)))
		 (setq str (string-left-ntrim str (1- i)))
		 val)
	      (setq val (+ (* val 8) 
			   (- (char-code (string-ref str i))
			      (char-code #\0))))))
	   ;; Hexadecimal
	   ((#\x)
	    (do* ((i 1 (1+ i))
		  (val 0 val)
		  (c (string-ref str i) (string-ref str i))) 
		 ((null c)
		  (setq str (string-left-ntrim str (1- i)))
		  val)
	      (aif (digit-char-p c 16)
		   (setq val (+ (* 16 val) it))
		   (progn
		     (setq str (string-left-ntrim str (1- i)))
		     (return val)))))
	   (t (char-code (string-ref str 0))))))
    (values str val)))

;;      
(defun getline (le)
  (if (lex-env-abort-parse le)
      nil
      (progn
	(incf (lex-env-lnum le))
	(setf (lex-env-line le)
	      (funcall (lex-env-getline le) (lex-env-stream le))))))

;; skip whitespace and comments
(defun skip-whitespace (le line)
  (if (and (null line)
	   (null (setq line (getline le))))
      (return-from skip-whitespace nil))
  (let ((read-another-line nil)
	(incomment nil))
    (loop
     (loop
      (setq line (string-left-trim-space line))
      (when (or (not (string= "" line))
		(null (setq line (getline le))))
	(return))
      (setq read-another-line t)
      (when (char= #\# (string-ref line 0))
	(setq line (parse-hash-directive (string-left-ntrim str 1) le))))
     (if incomment
	 (cond
	   ((null line)
	    (warn "Hit EOF while in a comment")
	    (return))
	   ((and (char= #\* (string-ref line 0))
		 (char= #\/ (string-ref line 1)))
	    (setq line (string-left-ntrim line 2))
	    (setq incomment nil))
	   (t
	    (setq line (string-left-ntrim line 1))))
	 (if (and line
		  (char= #\/ (string-ref line 0))
		  (char= #\* (string-ref line 1)))
	     (progn
	       (string-left-ntrim line 2)
	       (setq incomment t))
	     (return))))
    
    (if (and *want-debugging-point*
	     read-another-line)
	(format *error-output*
		"\"~a\", ~d: ~a~%"
		(lex-env-filename le)
		(lex-env-lnum le)
		line))
    line))

;;	     
(defun is-type-specifier (token)
  (or (is-basic-type-specifier token)
      (eq 'typedef-name token)))

(defun is-type-qualifier (token)
  (member token '(const restrict volatile) :test #'eq))

(defun is-storage-class (token)
  (member token '(static extern auto register typedef) :test #'eq))

(defun is-struct-or-union (token)
  (member token '(struct union) :test #'eq))

(defun is-constant (token)
  (member token '(floating-constant integer-constant string-constant
		  character-constant)))

(defun is-aggr-type-specifier (token)
  (or (is-struct-or-union token)
      (eq 'enum token)))

(defun is-basic-type-specifier (token)
  (member token '(#+builtin-va-list builtin-va-list
                  int unsigned signed long short
		  void char float double)
	  :test #'eq))

(defun is-storage-class-or-qualifier (token)
  (or (is-storage-class token)
      (is-type-qualifier token)))

(defun is-decl-specifier (token)
  (or (is-storage-class-or-qualifier token)
      (is-basic-type-specifier token)
      (is-aggr-type-specifier token)
      (eq 'typedef-name token)))

(defun is-assign-operator (token)
  (member token '(equals plus-equals minus-equals star-equals
                  slash-equals percent-equals lshift-equals
                  rshift-equals and-equals xor-equals or-equals)
          :test #'eq))

;;
(defvar *prev-token* nil)
(defvar *colon-follows* nil)

(defun lex-prev-token ()  *prev-token*)
(defun lex-colon-follows () *colon-follows*)

;;  
(defun lex-get-token (&aux (le *lex-env*))
  (unless le
    (when *want-debugging-output*
      (format *error-output* "~%"))
    (return-from lex-get-token nil))
  
  (unless (setq line (skip-whitespace le (lex-env-lptr le)))
    (setf (lex-env-lptr le) line)
    (return-from lex-get-token nil))  ;; EOF
  
  (let ((ch (string-ref line 0))
	(getstr "")
	(token nil))
    (cond
      ;; keyword or identifier
      ((or (char= #\_ ch)
	   (char= #\$ ch)
	   (char<= #\a ch #\z)
	   (char<= #\A ch #\Z))
       (multiple-value-setq (line getstr)
	 (string-left-trim-if
	  line
	  #'(lambda (x)
	      (or (char= #\_ x) (char= #\$ x)
		  (char<= #\a x #\z) (char<= #\A x #\Z)
		  (char<= #\0 x #\9)))))
       (aif (member getstr *keytab*
		    :key #'first :test #'string=)
	    (setq token (second (car it)))
	    (progn
	      (setf (lexeme-identifier *lexeme*) getstr)
	      (setq line (string-left-trim-space line))
	      (setq token 'identifier)
	      (setq *colon-follows*
		    (aand (string-ref line 0)
			  (char= #\: it))))))
      ;; number
      ((char<= #\0 ch #\9)
       (let ((prefix nil) (intstr nil) (suffix nil)
	     (radix nil))
	 (if (char= #\0 ch)
	     (cond
	       ;; hexadecimal
	       ((char= #\x (string-ref line 1))
		(setq line (string-left-ntrim line 2)
		      prefix "0x")
		(setq radix 16)
		(multiple-value-setq (line intstr)
		  (string-left-trim-if
		   line
		   #'(lambda (x) (digit-char-p x 16)))))
	       ;; octal
	       ((digit-char-p (string-ref line 1) 8)
		(setq line (string-left-ntrim line 1)
		      prefix "0")
		(setq radix 8)
		(multiple-value-setq (line intstr)
		  (string-left-trim-if
		   line
		   #'(lambda (x) (digit-char-p x 8)))))
	       ;; zero
	       (t
		(setq line (string-left-ntrim line 1))
		(setq prefix "" 
		      intstr "0")
		(setq radix 10)))
	     ;; decimal
	     (progn
	       (setq prefix "")
	       (setq radix 10)
	       (multiple-value-setq (line intstr)
		 (string-left-trim-if 
		  line
		  #'(lambda (x) (digit-char-p x 10))))))
	 
	 (case (string-ref line 0)
	   ;; floating-constant
	   ((#\e #\E #\.)
	    (multiple-value-setq (token line *constant*)
	      (get-float-constant
	       le (string+ prefix intstr line)))
	    (setf (lexeme-constant *lexeme*) *constant*))
	   ;; integer-costant
	   (t
	    (multiple-value-setq (line suffix)
	      (string-left-trim-if
	       line
	       #'(lambda (x &aux (ux (char-upcase x)))
		   (or (char= #\L ux) (char= #\U ux)))))
	    (setq suffix (string-upcase suffix))
	    (let ((num (parse-integer intstr
				      :radix radix))
		  (type (case (count #\L suffix)
			  ((1) 'long)
			  ((2) 'long-long)
			  (otherwise nil))))
	      (when (find #\U suffix)
		(setq type
		      (if type
			  (symbol+ 'unsigned- type)
			'unsigned-int)))
	      (when type
		(setq num `(cast ,type ,num)))
	      (setq *constant* num)
	      (setf (lexeme-constant *lexeme*) num))
	    (setq token 'integer-constant)))))
     
      ;; noteq, not
      ((char= #\! ch)
       (if (char= #\= (string-ref line 1))
	   (setq line (string-left-ntrim line 2)
		 token 'noteq)
	   (setq line (string-left-ntrim line)
		 token 'not)))
      ;; eqeq, equals
      ((char= #\= ch)
       (if (char= #\= (string-ref line 1))
	   (setq line (string-left-ntrim line 2)
		 token 'eqeq)
	   (setq line (string-left-ntrim line)
		 token 'equals)))
      ;; percent-equals, percent
      ((char= #\% ch)
       (if (char= #\= (string-ref line 1))
	   (setq line (string-left-ntrim line 2)
		 token 'percent-equals)
	   (setq line (string-left-ntrim line)
		 token 'percent)))
      ;; slash-equals, slash
      ((char= #\/ ch)
       (if (char= #\= (string-ref line 1))
	   (setq line (string-left-ntrim line 2)
		 token 'slash-equals)
	   (setq line (string-left-ntrim line)
		 token 'slash)))
      ;; xor-equals, xor
      ((char= #\^ ch)
       (if (char= #\= (string-ref line 1))
	   (setq line (string-left-ntrim line 2)
		 token 'xor-equals)
	   (setq line (string-left-ntrim line)
		 token 'xor)))
      ;; star-equals, star
      ((char= #\* ch)
       (if (char= #\= (string-ref line 1))
	   (setq line (string-left-ntrim line 2)
		 token 'star-equals)
	   (setq line (string-left-ntrim line)
		 token 'star)))
      ;; lbrac, rbrac
      ((char= #\[ ch)
       (setq line (string-left-ntrim line))
       (setq token 'lbrac))
      ((char= #\] ch)
       (setq line (string-left-ntrim line))
       (setq token 'rbrac))
      ;; lbrace, rbrace
      ((char= #\{ ch)
       (setq line (string-left-ntrim line))
       (setq token 'lbrace))
      ((char= #\} ch)
       (setq line (string-left-ntrim line))
       (setq token 'rbrace))
      ;; lparen, rparen
      ((char= #\( ch)
       (setq line (string-left-ntrim line))
       (setq token 'lparen))
      ((char= #\) ch)
       (setq line (string-left-ntrim line))
       (setq token 'rparen))
      ;; comma
      ((char= #\, ch)
       (setq line (string-left-ntrim line))
       (setq token 'comma))
      ;; semi
      ((char= #\; ch)
       (setq line (string-left-ntrim line))
       (setq token 'semi))
      ;; query
      ((char= #\? ch)
       (setq line (string-left-ntrim line))
       (setq token 'query))
      ;; colon
      ((char= #\: ch)
       (setq line (string-left-ntrim line))
       (setq token 'colon))
      ;; comma
      ((char= #\, ch)
       (setq line (string-left-ntrim line))
       (setq token 'comma))
      ;; charcter constant
      ((char= #\' ch)
       (let ((getval nil))
	 (if (char= #\\ (string-ref line 1))
	     (multiple-value-setq (line getval)
	       (ci-translate-escape (string-left-ntrim line 2)))
	     (setq getval (char-code (string-ref line 1))
		   line (string-left-ntrim line)))
	 (setq line (string-left-ntrim line))
         (if (not (eql #\' (string-ref line 0)))
	     (progn
	       (format *error-output* "Unterminated char constant~%")
	       (setq token 'badtok))
	     (progn
	       (setq line (string-left-ntrim line))
	       (setq *constant* (code-char getval))
	       (setf (lexeme-constant *lexeme*) *constant*)
	       (setq token 'character-constant)))))
      ;; string literal
      ((char= #\" ch)
       (multiple-value-setq (token *constant*)
	 (get-string le (string-left-ntrim line)))
       (setf (lexeme-constant *lexeme*) *constant*)
       (setq line (lex-env-lptr le)))
      ;; 
      ((char= #\. ch)
       (cond
	 ;; ellipses
	 ((and (char= #\. (string-ref line 1) (string-ref line 2)))
	  (setq line (string-left-ntrim line 3))
	  (setq token 'ellipsis))
	 ;; float
	 ((digit-char-p (string-ref line 1))
	  (multiple-value-setq (token line *constant*)
	    (get-float-constant (le line)))
	  (setf (lexeme-constant *lexeme*) *constant*))
	 ;; dot
	 (t 
	  (setq line (string-left-ntrim line))
	  (setq token 'dot))))
      ;; tilde
      ((char= #\~ ch)
       (setq line (string-left-ntrim line))
       (setq token 'tilde))
      ;; plusplus, plus-equals, plus
      ((char= #\+ ch)
       (cond
	 ((char= #\+ (string-ref line 1))
	  (setq line (string-left-ntrim line 2)
		token 'plusplus))
	 ((char= #\= (string-ref line 1))
	  (setq line (string-left-ntrim line 2)
		token 'plus-equals))
	 (t
	  (setq line (string-left-ntrim line)
		token 'plus))))
      ;; arrow, minusminus, minus-equals, minus
      ((char= #\- ch)
       (cond
	 ((char= #\> (string-ref line 1))
	  (setq line (string-left-ntrim line 2)
		token 'arrow))
	 ((char= #\- (string-ref line 1))
	  (setq line (string-left-ntrim line 2)
		token 'minusminus))
	 ((char= #\= (string-ref line 1))
	  (setq line (string-left-ntrim line 2)
		token 'minus-equals))
	 (t
	  (setq line (string-left-ntrim line)
		token 'minus))))
      ;; oror, or-equals, or
      ((char= #\| ch)
       (cond
	 ((char= #\| (string-ref line 1))
	  (setq line (string-left-ntrim line 2)
		token 'oror))
	 ((char= #\= (string-ref line 1))
	  (setq line (string-left-ntrim line 2)
		token 'or-equals))
	 (t
	  (setq line (string-left-ntrim line)
		token 'or))))
      ;; andand, and-equals, and
      ((char= #\& ch)
       (cond
	 ((char= #\& (string-ref line 1))
	  (setq line (string-left-ntrim line 2)
		token 'andand))
	 ((char= #\= (string-ref line 1))
	  (setq line (string-left-ntrim line 2)
		token 'and-equals))
	 (t
	  (setq line (string-left-ntrim line)
		token 'and))))
      ;; rshift-equals, rshift, gteq, greaterthan
      ((char= #\> ch)
       (cond
	 ((char= #\> (string-ref line 1))
	  (if (char= #\= (string-ref line 2))
	      (setq line (string-left-ntrim line 3)
		    token 'rshift-equals)
	      (setq line (string-left-ntrim line 2)
		    token 'rshift)))
	 ((char= #\= (string-ref line 1))
	  (setq line (string-left-ntrim line 2)
		token 'gteq))
	 (t
	  (setq line (string-left-ntrim line)
		token 'greaterthan))))
      ;; lshift-equals, lshift, lesseq, lessthan
      ((char= #\< ch)
       (cond
	 ((char= #\< (string-ref line 1))
	  (if (char= #\= (string-ref line 2))
	      (setq line (string-left-ntrim line 3)
		    token 'lshift-equals)
	      (setq line (string-left-ntrim line 2)
		    token 'lshift)))
	 ((char= #\= (string-ref line 1))
	  (setq line (string-left-ntrim line 2)
		token 'lesseq))
	 (t
	  (setq line (string-left-ntrim line)
		token 'lessthan))))
      ;; illegal character
      (t
       (setq line (string-left-ntrim line))
       (setf (lex-env-lptr le) line)
       (format *error-output* "Illegal character '~a' (0x~xx)"
	       ch (char-code ch))
       (setq token 'badtok)))
    
    (setf (lex-env-lptr le) line)
    (setq *prev-token* token)
    token))

;; read string-literal
(defun get-string (le line)
  (let ((getstr nil)
	(getstr-list (list)))
    (do* ((i 0 (1+ i))
	  (c (string-ref line i) (string-ref line i)))
	 ((>= i (length line))
	  (setq line (string-left-ntrim line i))
	  nil)
      (if (char= #\" c)
	  ;; '\"' 
	  (let ((new-line (skip-whitespace le 
					   (string-left-ntrim line (1+ i)))))
	    (when (or (null new-line)
		      (char/= #\" (string-ref new-line 0)))
	      (setf (lex-env-lptr le) new-line)
	      (setq getstr (concatenate 'string (reverse getstr-list)))
	      (return))  ;; from do-loop
	    (setq i 0                ;; "..." "..."
		  line new-line))    ;;        ^ cont reading from here
	  ;;
	  (progn 
	    (cond
	      ((char/= #\\ c)
	       (push c getstr-list))
	      ;; string spanned multiple lines
	      ((char= #\Newline (string-ref line (1+ i)))
	       (setq i 0
		     line (getline le))
	       (push (if (null line) (string-ref line 0) (code-char 0))
		     getstr-list))
	      ;; escape character
	      (t
	       (multiple-value-bind (line0 c0)
		   (ci-translate-escape (string-left-ntrim line (1+ i)))
		 (setq i 0
		       line line0)
		 (push (code-char c0) getstr-list))))
	    (when (or (null line)
		      (char= #\Newline (string-ref line i))
		      (string= "" line))
	      (format *error-output* "Unterminated string constant")
	      (return)))))   ;; from do-loop
    
    (if (stringp getstr)
	(values 'string-constant getstr)
	(values 'badtok *constant*))))


;; returns (values <token> <line-remain> <constant-val>)
(defun get-float-constant (le line)
  (let ((radix nil) (intstr "") (fltstr "")
	(expchar nil) (expsign nil) (exponentstr "")
	(suffix nil))
    ;; hexadecimal-prefix
    (if (and (eql #\0 (string-ref line 0))
	     (find (string-ref line 1) "xX"))
	(setq line (string-left-ntrim line 2)
	      radix 16)
        (setq radix 10))
    ;; (hexadecimal-)digit-sequence
    (multiple-value-setq (line intstr)
      (string-left-trim-if
       line
       #'(lambda (x) (digit-char-p x radix))))
    (when (string= "" intstr) (setq intstr "0"))
    ;; . (hexadecimal-)digit-sequence
    (when (eql #\. (string-ref line 0))
      (multiple-value-setq (line fltstr)
	(string-left-trim-if
	 (string-left-ntrim line)
	 #'(lambda (x) (digit-char-p x radix)))))
    (when (string= "" fltstr) (setq fltstr "0"))
    ;; (binary-)exponent-part
    (when (aand (string-ref line 0)
		(or (and (= radix 10) (find it "eE"))
		    (and (= radix 16) (find it "pP"))))
      (setq expchar (aref line 0))
      (setq line (string-left-ntrim line))
      (awhen (find (string-ref line 0) "-+")
	(setq expsign it)
	(setq line (string-left-ntrim line)))
      (multiple-value-setq (line exponentstr)
	(string-left-trim-if line #'digit-char-p)))
    (when (string= "" exponentstr) (setq exponentstr "0"))
    ;; floating-suffix
    (awhen (find (string-ref line 0) "fFlL")
      (setq suffix it)
      (setq line (string-left-ntrim line)))

    (let ((intnum (parse-integer intstr :radix radix))
	  (fltnum (parse-integer fltstr :radix radix))
	  (exprad (if (= 16 radix) 2 10))
	  (exponent (parse-integer exponentstr)))
      (till (> 1 fltnum)
	    (setq fltnum (/ fltnum radix)))
      (when (eql #\- expsign)
	(setq exponent (- exponent)))
      (let ((flt-const
	     (float (* (+ intnum fltnum)
		       (expt exprad exponent)))))
	(setq flt-const
	      (case suffix
		((#\f #\F) `(cast float ,flt-const))
		((#\l #\L) `(cast long-double ,flt-const))
		(otherwise flt-const)))
	(values 'floating-constant line flt-const)))))

;;
(defconstant *token-tab*
  '(if else while for do goto break continue return switch case
    default sizeof auto register static extern typedef inline
    bool complex imaginary 
    void char short int long float
    double signed unsigned const volatile restrict struct union
    enum and tilde not lessthan greaterthan xor or plus minus
    slash percent star dot colon query semi comma lparen rparen
    lbrace rbrace lbrac rbrac equals star-equals slash-equals
    percent-equals plus-equals minus-equals lshift-equals
    rshift-equals and-equals xor-equals or-equals andand
    oror eqeq noteq gteq lesseq lshift rshift plusplus minusminus
    arrow ellipsis string-constant integer-constant character-constant
    floating-constant identifier typedef-name badtok eof
    #+builtin-va-list builtin-va-list
    ))

(defun tokname (token)
  (if (member token *token-tab* :test #'eq) 
      (string token)
      (format nil "<unknown token ~a>" (string token))))
 