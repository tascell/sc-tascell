;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; Parse C code to generate SC-0 code
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(provide "C2SC-PARSER")

(unless (find-package "C2SC")
  (make-package "C2SC" :use '("LISP")))
(in-package "C2SC")

(export 'c2sc)

(require "C2SC-LEX")
(require "SC-MISC")
(require "SC-MAIN")
(require "SCT-LIB")

(use-package "SC-MISC")

(defconstant *c2sc-package* *package*)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; not handled completely
(defvar *allow-gcc-nested-function* t) 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defstruct symbol-dat
  name          
  storage-class
  object-type
)

(defstruct symbol-table
  level
  symbols ;; list of symbol-dat
)

;; current token
(defvar *tok*)

;; to control storage-class
(defvar *storage-class*)

;;
(defvar *parsing-oldstyle-parmdecl* nil)

;;
(defvar *is-func*)
(defvar *saw-ident*)

;;
(defvar *parsing-struct* nil)

;; list of symbol-table
(defvar *cursymtab*)
(defvar *curlabels*)
(defvar *curtypes*)

;; information about the last identifier
(defvar *cursym* (make-symbol-dat))

(defvar *level* 0)   ;; block level
(defconstant *level-global* 0)
(defconstant *level-function* 1)
(defconstant *level-statement* 2)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun object-name (obj-type)
  (if (member obj-type
              '(obj-typedef-name obj-function-decl obj-function-defn
                obj-enumerator obj-variable obj-parameter 
		obj-identifier)
              :test #'eq)
      (string-left-ntrim (symbol-name obj-type) 4)
      "invalid"))

;;;
(defun is-type-name (token)
  (or (is-decl-specifier token) 
      (and (eq 'identifier token)
	   *cursym*
	   (eq 'obj-typedef-name (symbol-dat-object-type *cursym*)))))

(defun is-declaration (token)
  (is-type-name token))

(defun is-external-declaration (token)
  (or (is-declaration token)
      (and *c89*
           (member token '(star lparen identifier) :test #'eq)))))

(defun is-expression (token)
  (or (is-constant token)
      (member token 
              '(identifier sizeof and plusplus
                minusminus star plus minus tilde lparen not)
              :test #'eq)))

(defun is-statement (token)
  (or (is-expression token)
      (member token 
              '(if break case continue default do else for goto
                return switch while lbrace semi)
              :test #'eq)))

(defun is-function-body (token)
  (or (eq 'lbrace token)
      (and (is-declaration token)
	   (not (eq 'typedef token)))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun c2sc-find-symbol (symtab-list name all-scope?)
  (if (null symtab-list)
      nil
      (progn
	(check-type (car symtab-list) symbol-table)
	(aif (member name (symbol-table-symbols (car symtab-list))
		     :key #'symbol-dat-name :test #'string=)
	     (car it)
	     (if all-scope?
		 (c2sc-find-symbol (cdr symtab-list) name all-scope?)
		 nil)))))


(defun enter-scope ()
  (incf *level*)
  ;; Although we increment the level when we parse 
  ;; function parameters, and again when we enter the
  ;; function body, ANSI C requires these to be in the same scope.
  (unless (= *level* *level-statement*)
    (push (make-symbol-table :level *level* :symbols (list))
	  *cursymtab*))
  (values))

(defun exit-scope ()
  (decf *level*)
  ;; Although we increment the level when we parse 
  ;; function parameters, and again when we enter the
  ;; function body, ANSI C requires these to be in the same scope.
  (unless (= *level* *level-function*)
    (pop *cursymtab*)
    (check-type (car *cursymtab*) symbol-table))
  (values))

(defmacro in-new-scope (&body args)
  `(unwind-protect (progn (enter-scope) ,@args) (exit-scope)))

(defun install-symbol (name storage-class object-type)
  (let ((sym (c2sc-find-symbol *cursymtab* name nil)))
    (unless (null sym)
      (format *error-output*
	      "Warning: redeclaration of symbol ~a as ~a~%"
	      name (object-name object-type))
      (format *error-output*
	      "Warning: previously declared as ~a~%"
	      (symbol-dat-object-type sym)))
    (push (make-symbol-dat
	   :name name :storage-class storage-class
	   :object-type object-type)
	  (symbol-table-symbols (car *cursymtab*)))))


(defun match (expected-tok)
  (if (not (eq expected-tok *tok*))
      (progn
	(format *error-output*
		"Parse failed: Expected ~a, " (tokname expected-tok))
	(format *error-output* "got ~a~%" (tokname *tok*))
        nil)
      (prog1
          (setq *tok* (lex-get-token))
        (setq *cursym*
              (if (eq 'identifier *tok*)
                  (c2sc-find-symbol 
                   *cursymtab* 
                   (lexeme-identifier *lexeme*)
                   t)
                  nil)))))

(defun check-not-typedef ()
  (or (null *cursym*)
      (not (eq 'obj-typedef-name (symbol-dat-object-type *cursym*)))))

;;;

(defvar *clang-package* (make-package "C" :use nil))

;; convert C identifier to SC-0 identifier
(defun identifier-c2sc (cid)
  (let ((cid-list (concatenate 'list cid)))
    (labels ((cidscid (remain acc capital)
	       (if (endp remain)
		   acc
		   (let ((ch (car remain)))
		     (if (or (and capital       (char<= #\a ch #\z))
			     (and (not capital) (char<= #\A ch #\Z)))
			 (cidscid (cdr remain) (cons ch (cons #\@ acc))
				  (not capital))
			 (cidscid (cdr remain) (cons ch acc) capital))))))
      (intern
       (string-upcase
	(concatenate 'string (reverse (cidscid cid-list '() nil))))
       *clang-package*))))

;; convert C string to SC-0 string
(defun string-c2sc (cstring)
  (let ((str-list (concatenate 'list cstring)))
    (labels ((cstr-scstr (remain acc)
	       (if (endp remain)
		   acc
		   (let ((ch (car remain)))
		     (cond
		       ((some #'(lambda (x) (char= ch x))
                              '(#\~ #\"))
                        (cstr-scstr
                         (cdr remain)
                         (cons (format nil "~~~a" ch) acc)))
		       ((graphic-char-p ch)
			(cstr-scstr (cdr remain) 
				   (cons (string ch) acc)))
		       (t 
			(cstr-scstr 
			(cdr remain)
			(cons (format nil "~~|~o|" (char-code ch))
			      acc))))))))
      (reduce #'string+ 
	      (reverse (cstr-scstr str-list nil)) :from-end t))))

;;;; 
     
(defvar *gensym-num*)

;; get-gensymid で mark をつけたところの identifier を決定する
(defun decide-unidentified (sc-program)
  (let ((used-id (sc-transformer:get-all-identifier sc-program))
        (count 0))
    (let ((dec-id 
           (memoize 
            #'(lambda (usym 
                       &aux
                       newid 
                       (csym (make-symbol (write-to-string count))))
                (while (and (setq newid (symbol+ 'unnamed csym))
                            (member newid used-id))
                  (incf count)
                  (setq csym (make-symbol (write-to-string count))))
                (incf count)
                newid))))
      (rmapcar #'(lambda (x)
                   (if (is-unidentified x)
                       (funcall dec-id x)
                       x))
               sc-program))))

(defun is-unidentified (sym)
  (and (symbolp sym)
       (let ((symstr (symbol-name sym)))
         (and (<= 13 (length symstr))
              (string= "$UNIDENTIFIED" symstr :end2 13)))))

;; mark をつけておいて,後で identifier を決定する
(defun get-gensymid ()
  (prog1
      (symbol+ '$unidentified 
               (make-symbol (write-to-string *gensym-num*)))
    (incf *gensym-num*)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; dclr : (<id> <type>-><type>)
;;; decl-list : sc declaration list
;;; Returns new dclr
(defun import-oldstyle-decl (dclr decl-list)
  (unless decl-list
    (return-from import-oldstyle-decl dclr))
  (let* ((id (first dclr))
         (gettype (second dclr))
         (arg-type-list (copy-list (cdr id))))
    (dolist (decl decl-list)
      (awhen (member (second decl) arg-type-list)
        (rplaca it (list (third decl)))))
    (setq arg-type-list
          (mapcar #'(lambda (x) (if (atom x) 'int (car x)))
                  arg-type-list))
    (let ((newgettype
           #'(lambda (basetype)
               (let ((incomplete-type (funcall gettype basetype)))
                 (assert (and (consp incomplete-type)
                              (eq 'fn (car incomplete-type))))
                 `(sc::fn
                   ,(second incomplete-type) ,@arg-type-list)))))
      (list id newgettype))))

;;; dspec : (<storage-class> <base-type>)
;;; idclr : (<dclr> <compound-statement>)
;;; dclr : (<id> <type>-><type>)
;;; Returns "declaration" of function definition
(defun make-function-definition (dspec idclr)
  (let ((st-class (first dspec))
	(base-type (second dspec))
        (id (first (first idclr)))
        (gettype (second (first idclr)))
        (cp-stat (second idclr)))
    (unless (and (consp cp-stat)
                 (eq 'sc::begin (car cp-stat)))
      (error "~s is not compound-statement." cp-stat))
    (let ((defsymbol (make-defsymbol st-class cp-stat)))
      `(,defsymbol ,id ,(funcall gettype base-type)
        ,@(cdr cp-stat)))))

;;; dspec : (<storage-class> <base-type>)
;;; idclr-list : (<dclr> <initializer> [<bitfield>]) list
;;; dclr : (<id> <type>-><type>)
;;; Returns declaration list
(defun make-variable-definition (dspec idclr-list)
  (let ((st-class (first dspec))
	(base-type (second dspec))
	(def-aggr nil))
    (when (and (consp base-type)
	       (is-aggr-type-specifier (first base-type))
	       (not (endp (cddr base-type))))
      (let ((mems (cddr base-type)))
	(setq base-type `(,(first base-type) ,(second base-type)))
	(setq def-aggr (list `(sc::def ,base-type ,@mems)))))
    (append
     def-aggr
     (mappend
      #'(lambda (idclr)
	  (let ((id (first (first idclr)))
		(gettype (second (first idclr)))
		(init (second idclr))
		(bitfield (third idclr)))
	    (let ((defsymbol (make-defsymbol st-class init))
		  (type (funcall gettype base-type)))
	      `(;; declaration
		(,defsymbol ,id ,type ,@(if init (list init) '()))
		;; bitfield
		,@(if bitfield `(:bit ,bitfield) '())))))
      idclr-list))))

;;
(defun make-defsymbol (st-class init)
  (if (eq 'typedef st-class)
      'sc::deftype
      (let ((def-or-decl (if init 'sc::def 'sc::decl)))
        (if st-class
            (immigrate-package 
             (symbol+ st-class '- def-or-decl) sc-main:*code-package*)
            def-or-decl))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;
(defun constant-expression ()
  (conditional-expression))


;;;
(defun expression ()
  ;; expression:
  ;;   assignment-expression
  ;;   expression , assignment-expression
  (unless (is-expression *tok*)
    (return-from expression))
  
  (let ((rev-as-exp-list (list (assignment-expression))))
    (while (eq 'comma *tok*)
      (match 'comma)
      (push (assignment-expression) rev-as-exp-list))
    (if (cdr rev-as-exp-list)
	`(exps ,@(reverse rev-as-exp-list))
	(car rev-as-exp-list))))

;;;
(defun primary-expression ()
  ;; primary-expression:
  ;;   identifier
  ;;   constant
  ;;   string-literal
  ;;   ( expression )    --> handled in unary-expression
  (cond
   ((eq 'identifier *tok*)
    (prog1
	(identifier-c2sc (lexeme-identifier *lexeme*))
      (match 'identifier)))
   ((eq 'string-constant *tok*)
    (prog1
	(string-c2sc (lexeme-constant *lexeme*))
      (match 'string-constant)))
   ((is-constant *tok*)
    (prog1
	(lexeme-constant *lexeme*)
      (match *tok*)))
   (t
    (error "unexpected token in primary-expression."))))

;;;
(defun postfix-operator ()
  (case *tok*
    ;; aref
    ((lbrac)
     (prog2
	 (match 'lbrac)
	  (let ((exp (expression)))
	    #'(lambda (cur-exp)
		`(sc::aref ,cur-exp ,exp)))
       (match 'rbrac)))
    ;; call
    ((lparen)
     (prog2
	 (match 'lparen)
	 (let ((rev-arg-list (list)))
	   (unless (eq 'rparen *tok*)
	     (push (assignment-expression) rev-arg-list)
	     (while (eq 'comma *tok*)
	       (match 'comma)
	       (push (assignment-expression) rev-arg-list)))
	   (let ((arg-list (reverse rev-arg-list)))
	     #'(lambda (cur-exp)
		 `(,cur-exp ,@arg-list))))
       (match 'rparen)))
    ;; fref .
    ((dot)
     (prog2 
	 (match 'dot)
	 (let ((id (identifier-c2sc (lexeme-identifier *lexeme*))))
	   #'(lambda (cur-exp)
	       `(sc::fref ,cur-exp ,id)))
       (match 'identifier)))
    ;; fref ->
    ((arrow)
     (prog2 
	 (match 'arrow)
	 (let ((id (identifier-c2sc (lexeme-identifier *lexeme*))))
	   #'(lambda (cur-exp)
	       `(sc::fref (sc::mref ,cur-exp) ,id)))
       (match 'identifier)))
    ;; suffix ++
    ((plusplus)
     (match 'plusplus)
     #'(lambda (cur-exp) `(sc::inc ,cur-exp)))
    ;; suffix --
    ((minusminus)
     (match 'minusminus)
     #'(lambda (cur-exp) `(sc::dec ,cur-exp)))
    (otherwise
     (error "unexpected token in postfix-operator"))))

;;;
(defun postfix-operators ()
  (let ((ret-func #'identity))
    (while (member *tok* 
		   '(lbrac lparen dot arrow plusplus minusminus))
      (setq ret-func (compose (postfix-operator) ret-func)))
    ret-func))

;;;
(defun sizeof-expression ()
  ;; sizeof_expression:
  ;;   sizeof unary-expression
  ;;   sizeof ( type-name )
  (match 'sizeof)
  (if (and (eq 'lparen *tok*)
           (match 'lparen))
      (if (is-type-name *tok*)
          ;; sizeof <type>
	  (prog1
	      `(sc::sizeof ,(type-name))
	    (match 'rparen))
          ;; sizeof <exp>
	  (let ((exp (expression)))
	    (match 'rparen)
	    (let ((pf-ops (postfix-operators)))
	      `(sc::sizeof ,(funcall pf-ops exp)))))
      `(sizeof ,(unary-expression))))

      
;;;
(defun unary-expression ()
  ;; unary-expression:
  ;;   postfix-expression
  ;;   ++ unary-expression
  ;;   -- unary-expression
  ;;   unary-operator cast-expression
  ;;   sizeof-expression
  ;; postfix-expression:
  ;;   primary-expression
  ;;   primary-expression postfix-operators
  ;; cast-expression:
  ;;   unary-expression
  ;;   ( type-name ) cast-expression
  (acond
    ;; sizeof
    ((eq 'sizeof *tok*)
     (sizeof-expression))
    ((eq 'lparen *tok*)
     (match 'lparen)
     (if (is-type-name *tok*)
         ;; cast
         (let ((tn (type-name)))
           (match 'rparen)
           (let ((un-exp (unary-expression)))
             `(sc::cast ,tn ,un-exp)))
         ;; parenthesized exp
         (let* ((exp (expression)))
           (match 'rparen)
           (let ((pf-ops (postfix-operators)))
             (funcall pf-ops exp)))))
    ;; unary-operator
    ((assoc *tok*
	    '((plusplus . sc::++) (minusminus . sc::--) (and . sc::ptr)
	      (star . sc::mref) (plus . sc::+) (minus . sc::-)
	      (tilde . sc::bit-not) (not . sc::not)))
     (match *tok*)
     `(,(cdr it) ,(unary-expression)))
    ;;
    (t 
     (let ((prim-exp (primary-expression))
	   (pf-ops (postfix-operators)))
       (funcall pf-ops prim-exp)))))

;;;
(defun multiplicative-expression ()
  ;; multiplicative-expression:
  ;;   cast-expression
  ;;   multiplicative-expression * cast-expression
  ;;   multiplicative-expression / cast-expression
  ;;   multiplicative-expression % cast-expression
  (let ((ret-exp (unary-expression))
	current-op)
    (while (or (and (eq 'star    *tok*) (setq current-op 'sc::*))
	       (and (eq 'slash   *tok*) (setq current-op 'sc::/))
	       (and (eq 'percent *tok*) (setq current-op 'sc::%)))
      (match *tok*)
      (setq ret-exp 
	    `(,current-op ,ret-exp ,(unary-expression))))
    ret-exp))

;;;
(defun additive-expression ()
  ;; additive-expression:
  ;;   multiplicative-expression
  ;;   additive-expression + multiplicative-expression
  ;;   additive-expression - multiplicative-expression
  (let ((ret-exp (multiplicative-expression))
	current-op)
    (while (or (and (eq 'plus  *tok*) (setq current-op 'sc::+))
	       (and (eq 'minus *tok*) (setq current-op 'sc::-)))
      (match *tok*)
      (setq ret-exp 
	    `(,current-op ,ret-exp ,(multiplicative-expression))))
    ret-exp))


;;;
(defun shift-expression ()
  ;; shift-expression:
  ;;   additive-expression
  ;;   shift-expression << additive-expression
  ;;   shift-expression >> additive-expression
  (let ((ret-exp (additive-expression))
	current-op)
    (while (or (and (eq 'lshift *tok*) (setq current-op 'sc::<<))
	       (and (eq 'rshift *tok*) (setq current-op 'sc::>>)))
      (match *tok*)
      (setq ret-exp `(,current-op ,ret-exp ,(additive-expression))))
    ret-exp))
    
	       
;;;
(defun relational-expression ()
  ;; relational-expression:
  ;;   shift-expression
  ;;   relational-expression <  shift-expression
  ;;   relational-expression >  shift-expression
  ;;   relational-expression <= shift-expression
  ;;   relational-expression <= shift-expression
  (let ((ret-exp (shift-expression))
	current-op)
    (while (or (and (eq 'greaterthan *tok*) (setq current-op 'sc::>))
	       (and (eq 'lessthan *tok*)    (setq current-op 'sc::<))
	       (and (eq 'gteq *tok*)        (setq current-op 'sc::>=))
	       (and (eq 'lesseq *tok*)      (setq current-op 'sc::<=)))
      (match *tok*)
      (setq ret-exp 
	    `(,current-op ,ret-exp ,(shift-expression))))
    ret-exp))

;;;
(defun equality-expression ()
  ;; equality-expression:
  ;;   relational-expression
  ;;   equality-expression == relational-expression
  ;;   equality-expression != relational-expression
  (let ((rev-op-rlexp-list (list)))
    (push (relational-expression) rev-op-rlexp-list)
    (let (current-op)
      (while (or (and (eq 'eqeq *tok*)  (setq current-op 'sc::==))
		 (and (eq 'noteq *tok*) (setq current-op 'sc::!=)))
	(match *tok*)
	(push current-op rev-op-rlexp-list)
        (push (relational-expression) rev-op-rlexp-list)))
    (let ((ret-exp (car rev-op-rlexp-list)))
      (do ((rem-list rev-op-rlexp-list (cddr rem-list)))
          ((endp (cdr rem-list)) ret-exp)
        (setq ret-exp
              `(,(second rem-list) ,(third rem-list) ,ret-exp))))))

;;;
(defun and-expression ()
  ;; AND-expression:
  ;;   equality-expression
  ;;   AND-expression & equlity-expression
  (let (eq-exp
	(rev-eq-exp-list (list)))
    (setq eq-exp (equality-expression))
    (while (eq 'and *tok*)
      (match 'and)
      (push (equality-expression) rev-eq-exp-list))
    (if rev-eq-exp-list
	`(sc::bit-and ,eq-exp ,@(reverse rev-eq-exp-list))
	eq-exp)))

;;;
(defun exclusive-or-expression ()
  ;; exclusive-OR-expression:
  ;;   AND-expression
  ;;   exclusive-OR-expression ^ AND-expression
  (let (an-exp
	(rev-an-exp-list (list)))
    (setq an-exp (and-expression))
    (while (eq 'xor *tok*)
      (match 'xor)
      (push (and-expression) rev-an-exp-list))
    (if rev-an-exp-list
	`(sc::bit-xor ,an-exp ,@(reverse rev-an-exp-list))
	an-exp)))
   
;;;
(defun inclusive-or-expression ()
  ;; inclusive-OR-expression:
  ;;   exclusive-OR-expression
  ;;   inclusive-OR-expression | exclusive-OR-expression 
  (let (xo-exp 
	(rev-xo-exp-list (list)))
    (setq xo-exp (exclusive-or-expression))
    (while (eq 'or *tok*)
      (match 'or)
      (push (exclusive-or-expression) rev-xo-exp-list))
    (if rev-xo-exp-list
	`(sc::bit-or ,xo-exp ,@(reverse rev-xo-exp-list))
	xo-exp)))

;;;
(defun logical-and-expression ()
  ;; logical-AND-expression:
  ;;   inclusive-OR-expression
  ;;   logical-AND-expression && inclusive-OR-expression
  (let (or-exp 
	(rev-or-exp-list (list)))
    (setq or-exp (inclusive-or-expression))
    (while (eq 'andand *tok*)
      (match 'andand)
      (push (inclusive-or-expression) rev-or-exp-list))
    (if rev-or-exp-list
	`(sc::and ,or-exp ,@(reverse rev-or-exp-list))
	or-exp)))

;;;
(defun logical-or-expression ()
  ;; logical-OR-expression:
  ;;   logical-AND-expression
  ;;   logical-OR-expression || logical-AND-expression
  (let (la-exp 
	(rev-la-exp-list (list)))
    (setq la-exp (logical-and-expression))
    (while (eq 'oror *tok*)
      (match 'oror)
      (push (logical-and-expression) rev-la-exp-list))
    (if rev-la-exp-list
	`(sc::or ,la-exp ,@(reverse rev-la-exp-list))
	la-exp)))

;;;
(defun conditional-expression ()
  ;; conditional-expression:
  ;;   logical-OR-expression
  ;;   logical-OR-expression ? expression : conditional-expression
  (let (lo-exp exp cd-exp)
    (setq lo-exp (logical-or-expression))
    (if (eq 'query *tok*)
	(progn
	  (match 'query)
	  (setq exp (expression))
	  (match 'colon)
	  (setq cd-exp (conditional-expression))
	  `(sc::if-exp ,lo-exp ,exp ,cd-exp))
	lo-exp)))

;;;
(defun assignment-expression ()
  ;; assignment-expression:
  ;;   conditional-expression
  ;;   unary-expression assignment-operator assignment-expression
  (let ((eqtok-alist
	 '((equals . sc::=) (plus-equals . sc::+=) 
           (minus-equals . sc::-=) (star-equals . sc::*=) 
           (slash-equals . sc::/=) (percent-equals . sc::%=)
	   (lshift-equals . sc::<<=) (rshift-equals . sc::>>=)
	   (and-equals . sc::bit-and=) (xor-equals . sc::bit-xor=)
	   (or-equals . sc::bit-or=))))
    (let (cd-exp as-exp)
      (setq cd-exp (conditional-expression))
      (aif (assoc *tok* eqtok-alist :test #'eq)
	   (progn
	     (match *tok*)
	     (setq as-exp (assignment-expression))
	     `(,(cdr it) ,cd-exp ,as-exp))
	   cd-exp))))

;;;
(defun labeled-statement ()
  ;; labeled-statement:
  ;;   identifier : statement
  ;;   case-statement
  ;;   default-statement
  (let ((id (identifier-c2sc (lexeme-identifier *lexeme*)))
	stat)
    (match 'identifier)
    (match 'colon)
    (setq stat (statement))
    (list
     `(sc::label ,id ,stat)) ))

;;;
(defun case-statement ()
  (let (cs-exp stat)
    (match 'case)
    (setq cs-exp (constant-expression))
    (match 'colon)
    (setq stat (statement))
    (list
     `(sc::case ,cs-exp)
     stat) ))
    
;;;
(defun default-statement ()
  (let (stat)
    (match 'default)
    (match 'colon)
    (setq stat (statement))
    (list
     `(sc::default ,stat)) ))

;;;
(defun if-statement ()
  ;; if-statement:
  ;;   if ( expression ) statement
  ;;   if ( expression ) statement else statement
  (let (exp stat1 (stat2 nil))
    (in-new-scope
     (match 'if)
     (match 'lparen)
     (setq exp (expression))
     (match 'rparen)
     (in-new-scope
      (setq stat1 (statement)))
     (when (eq 'else *tok*)
       (in-new-scope
        (match 'else)
        (setq stat2 (statement)))))
    (flet ((mk-compound (stat)
             (if (cdr stat)
                 `(sc::begin ,@stat)
                 (car stat))))
      (if stat2
          (list
           `(sc::if ,exp ,(mk-compound stat1) ,(mk-compound stat2)))
          (list
           `(sc::if ,exp ,(mk-compound stat1))) ))))

;;;
(defun switch-statement ()
  ;; switch-statement:
  ;;   switch ( expression ) statement
  (let (exp stat)
    (in-new-scope
      (match 'switch)
      (match 'lparen)
      (setq exp (expression))
      (match 'rparen)
      (in-new-scope
        (setq stat (statement))))
    (list
     `(sc::switch ,exp
       ,@(if (and (endp (cdr stat))
                  (consp (car stat))
                  (eq 'sc::begin (caar stat)))
             (cdar stat)
             stat))) ))

;;;
(defun while-statement ()
  ;; while-statement:
  ;;   while ( expression ) statement
  (let (exp stat)
    (in-new-scope
      (match 'while)
      (match 'lparen)
      (setq exp (expression))
      (match 'rparen)
      (in-new-scope
        (setq stat (statement))))
    (list
     `(sc::while ,exp
       ,@(if (and (endp (cdr stat))
                  (consp (car stat))
                  (eq 'sc::begin (caar stat)))
             (cdar stat)
             stat))) ))

;;;
(defun do-while-statement ()
  ;; do-while-statement:
  ;;   do statement while ( expression ) ;
  (let (exp stat)
    (in-new-scope
      (match 'do)
      (in-new-scope
        (setq stat (statement)))
      (match 'while)
      (match 'lparen)
      (setq exp (expression))
      (match 'rparen))
    (match 'semi)
    (list
     `(sc::do-while ,exp
       ,@(if (and (endp (cdr stat))
                  (consp (car stat))
                  (eq 'sc::begin (caar stat)))
             (cdar stat)
             stat))) ))

;;;
(defun for-statement ()
  ;; for-statement:
  ;;   for ( expression(opt) ; expression(opt) ; expression(opt) ) statement
  ;;   for ( declaration expression(opt) ; expression(opt) ) statement 
  (let ((decl-or-exp-list (list))
	(exp2 nil)
	(exp3 nil)
	(stat))
    (in-new-scope
      (match 'for)
      (match 'lparen)
      (if (eq 'semi *tok*)
          (match 'semi)
          (if (is-declaration *tok*)
              (setq decl-or-exp-list (c2sc-declaration))
              (prog1
                  (setq decl-or-exp-list (list (expression)))
                (match 'semi))))
      (unless (eq 'semi *tok*)
        (setq exp2 (expression)))
      (match 'semi)
      (unless (eq 'rparen *tok*)
        (setq exp3 (expression)))
      (match 'rparen)
      (in-new-scope 
        (setq stat (statement)))) 
    `(sc::for (,@decl-or-exp-list ,exp2 ,exp3)
      ,@(if (and (consp stat) (eq 'begin (car stat)))
	    (cdr stat)
	    (list stat)))))

;;;
(defun break-statement ()
  ;; continue-statement:
  ;;   continue ;
  (match 'break)
  (match 'semi)
  '(sc::break))

;;;
(defun continue-statement ()
  ;; goto-statement:
  ;;   goto identifier ;
  (match 'continue)
  (match 'semi)
  '(sc::continue))

;;;
(defun goto-statement ()
  ;; goto-statement:
  ;;   goto identifier ;
  (prog2
      (match 'goto)
      `(sc::goto ,(identifier-c2sc (lexeme-identifier *lexeme*)))
    (match 'identifier)
    (match 'semi)))

;;;
(defun return-statement ()
  ;; return-statement:
  ;;   return expression(opt) ;
  (match 'return)
  (if (eq 'semi *tok*)
      (progn
	(match 'semi)
	'(sc::return))
      (prog1
	  `(sc::return ,(expression))
	(match 'semi))))
      

;;;
(defun empty-statement ()
  (match 'semi)
  '())

;;;
(defun expression-statement ()
  ;; expression-statement:
  ;;   expression ;
  (if (and (eq 'identifier *tok*)
	   (lex-colon-follows))
      (labeled-statement)
      (prog1
	  (expression)
	(match 'semi))))

;;;
(defun statement ()
  ;; statement:
  ;;   labeled-statement
  ;;   compound-statement
  ;;   expression-statement
  ;;   selection-statement
  ;;   iteration-statement
  ;;   jump-statement
  (case *tok*
    ((identifier)   (expression-statement))
    ((case)         (case-statement))
    ((default)      (default-statement))
    ((if)           (if-statement))
    ((switch)       (switch-statement))
    ((while)        (while-statement))
    ((do)           (do-while-statement))
    ((for)          (for-statement))
    ((break)        (break-statement))
    ((continue)     (continue-statement))
    ((goto)         (goto-statement))
    ((return)       (return-statement))
    ((lbrace)       (compound-statement))
    ((semi)         (empty-statement))
    (otherwise
     (if (is-expression *tok*)
	 (expression-statement)
	 (error "unexpected error in statement")))))

;;;
(defun compound-statement ()
  ;; compound-statement:
  ;;   { block-item-list(opt) }
  ;; block-item-list:
  ;;   block-item
  ;;   block-item-list block-item
  ;; block-item:
  ;;   statement
  ;;   declaration
  (match 'lbrace)
  (let ((rev-bi-list (list)))
    (in-new-scope
      (while (not (eq 'rbrace *tok*))
        (if (is-declaration *tok*)
            (if (and (eq 'identifier *tok*)
                     (lex-colon-follows))
                (push (statement) rev-bi-list)
                (dolist (decl (c2sc-declaration))
                  (push decl rev-bi-list)))
            (push (statement) rev-bi-list))))
    (match 'rbrace)
    `(sc::begin ,@(reverse rev-bi-list))))
	      

;;;
(defun enumerator ()
  ;; enumerator:
  ;;   enumeration-constant
  ;;   enumeration-constant = constant-expression
  ;; enumeration-constant:
  ;;   identifier
  (unless (eq 'identifier *tok*)
    (return-from enumerator))
  (let ((id nil)
	(val nil))
    (let ((id-tmp (lexeme-identifier *lexeme*)))
      (setq id (identifier-c2sc id-tmp))
      (install-symbol id-tmp *storage-class* 'obj-enumerator))
    (match 'identifier)
    (when (eq 'equals *tok*)
      (match 'equals)
      (setq val (constant-expression)))
    (if val
	`(,id ,val)
	id)))

;;;
(defun enum-specifier ()
  ;; enum-specifier:
  ;;   enum identifier(opt) { enumerator-list }
  ;;   enum identifier(opt) { enumerator-list , }
  ;;   enum identifier
  ;; enumerator-list:
  ;;   enumerator
  ;;   enumerator-list , enumerator
  (unless (eq 'enum *tok*)
    (return-from enum_specifier))
  (match 'enum)
  (let ((id (get-gensymid))
	(rev-enm-list (list)))
    (when (eq 'identifier *tok*)
      (setq id (identifier-c2sc (lexeme-identifier *lexeme*)))
      (match 'identifier))
    (when (eq 'lbrace *tok*)
      (match 'lbrace)
      (push (enumerator) rev-enm-list)
      (while (eq 'comma *tok*)
	(match 'comma)
	(push (enumerator) rev-enm-list))
      (match 'rbrace))
    `(sc::enum ,id ,(reverse rev-enm-list))))  

;;;
(defun c2sc-member ()
  ;; struct-declarator:
  ;;   declarator
  ;;   declarator(opt) : constant-expression
  (let ((dclr nil)
	(bitfield nil))
    (when (not (eq 'colon *tok*))
      (setq dclr (declarator nil)))
    (when (eq 'colon *tok*)
      (match 'colon)
      (setq bitfield (constant-expression)))
    (list dclr nil bitfield)))

;;;
(defun members ()
  ;; struct-declaration-list:
  ;;   struct-declaration
  ;;   struct-declaration-list struct-declaration
  ;; struct-declaration:
  ;;   specifier-qualifier-list struct-declarator-list;
  ;; struct-declarator-list:
  ;;   struct-declarator
  ;;   struct-declarator-list , struct-declarator
  (let ((rev-fdecl-list (list)))
    (while (not (eq 'rbrace *tok*))
      (let* ((*storage-class* nil)
	     (dspec (declaration-specifiers t))
	     (rev-memb-list (list (c2sc-member))))
	(while (eq 'comma *tok*)
          (match 'comma)
	  (push (c2sc-member) rev-memb-list))
	(match 'semi)
        (push (make-variable-definition
               dspec (reverse rev-memb-list))
              rev-fdecl-list)))
    (mapcan #'copy-list (reverse rev-fdecl-list))))
  
;;;
(defun struct-or-union-specifier ()
  ;; struct-or-union-specifier:
  ;;   struct-or-union identifier(opt) { struct-declaration-list }
  ;;   struct-or-union identifier
  (let ((*parsing-struct* t)
	(str-or-uni *tok*)
	(id (get-gensymid))
	(fdecl-list (list))
	(def-p nil))
    (match *tok*)
    (when (eq 'identifier *tok*)
      (setq id (identifier-c2sc (lexeme-identifier *lexeme*)))
      (match *tok*))
    (when (eq 'lbrace *tok*)
      (match 'lbrace)
      (setq def-p t)
      (setq fdecl-list (members))
      (match 'rbrace))
    (values
     `(,(case str-or-uni 
              ((struct) 'sc::struct)
              ((union)  'sc::union)
              (otherwise (error "Invalid struct-or-union specifier.")))
       ,id ,@fdecl-list)
     def-p)))

;;;
(defun type-name ()
  ;; type-name:
  ;;   specifier-qualifier-list abstract-declarator(opt)
  (let* ((*storage-class* nil)
	 (dspec (declaration-specifiers t))
	 (dclr (declarator t)))
    (third
     (first 
      (make-variable-definition
       dspec (list (list dclr nil)))) )))
  

;;; returns ( <storage-class> <type-exp> )
(defun declaration-specifiers (no-storage-class)
  ;; declaration-specifiers:
  ;;   storage-class-specifier declaration-specifiers(opt)
  ;;   type-specifier declaration-specifiers(opt)
  ;;   type-qualifier declaration-specifiers(opt)
  ;;   function-specifier declaration-specifiers(opt)
  ;; specifier-qualifier-list:   <-- from struct-declaration, type-name
  ;;   type-specifier specifier-qualifier-list(opt)
  ;;   type-qualifier specifier-qualifier-list(opt)
  (let ((type-found nil)
	st-class type-exp 
	found-short found-long found-long-long sign
	(rev-qf-list (list)))
    (while (is-declaration *tok*)
      (when (and no-storage-class
		 (is-storage-class *tok*))
	(error "Parse failed: unexpected storage class ~a"
	       (tokname *tok*)))
      (when (and (eq 'identifier *tok*) type-found)
	(return))   ; from while
      (when (or (is-type-specifier *tok*)
		(eq 'identifier *tok*))
	(setq type-found t))
      (let ((savedtok *tok*))
        (cond
          ;; struct, union
          ((is-struct-or-union *tok*)
	   (setq type-exp (struct-or-union-specifier))
	   (return)) ; from while
          ;; enum
          ((eq 'enum *tok*)
           (setq type-exp (enum-specifier))
	   (return)) ; from while
          ;; typedef-name
          ((eq 'identifier *tok*)
           (setq type-exp 
                 (identifier-c2sc (lexeme-identifier *lexeme*))))
          ;; (storage-class)
          ((is-storage-class *tok*)
           (setq st-class *tok*)
           (setq *storage-class* *tok*))
          ;; element type
          ((member *tok* '(void char int float double) :test #'eq)
           (setq type-exp (immigrate-package 
                           *tok* sc-main:*code-package*)))
          ;; builtin-va-list (for gcc)
          #+builtin-va-list
          ((eq 'builtin-va-list *tok*)
           (setq type-exp (identifier-c2sc "__builtin_va_list")))
          ;; short
          ((eq 'short *tok*)
           (setq found-short t))
          ;; long
          ((eq 'long *tok*)
           (if found-long-long
               (format *error-output* "Too many long tokens.~%")
               (if found-long
                   (setq found-long-long t)
                   (setq found-long t))))
          ;; signed, unsigned
          ((member *tok* '(signed unsigned) :test #'eq)
           (if sign
               (format *error-output*
                       "Excess ~a.~%" (tokname *tok*))
               (setq sign (case *tok* 
                            ((signed)   'sc::signed)
                            ((unsigned) 'sc::unsigned)))))
          ;; (type-qualifier)
          ((is-type-qualifier *tok*)
           (push (immigrate-package *tok* sc-main:*code-package*)
                 rev-qf-list))
          (t
           (error "unexpected error in declaration-specifiers.")))
        (match *tok*)
        (when (eq 'identifier savedtok)
          (return))))   ; from while
    
    (unless type-exp
      (setq type-exp 'sc::int))
    (when (and found-short (eq 'sc::int type-exp))
      (setq type-exp 'sc::short))
    (when (and found-long-long (eq 'sc::int type-exp))
      (setq type-exp 'sc::long-long))
    (when found-long 
      (cond
	((eq 'sc::int type-exp)
	 (setq type-exp 'sc::long))
	((eq 'sc::double type-exp)
	 (setq type-exp 'sc::long-double))
	(t
	 (values))))
    (when (and sign
	       (member type-exp '(sc::char sc::short sc::int sc::long
                                  sc::long-long) 
                       :test #'eq))
      (setq type-exp (symbol+ sign '- type-exp)))
    (when rev-qf-list
      (setq type-exp `(,@(reverse rev-qf-list) ,type-exp)))
    (list st-class type-exp)))
	       

;;; returns type -> type
(defun pointer ()
  ;; pointer:
  ;;   * type-qualifier-list(opt)
  ;;   * type-qualifier-list(opt) pointer
  (let ((rev-pt-qf-list (list)))
    (while (eq 'star *tok*)
      (push 'sc::ptr rev-pt-qf-list)
      (match 'star)
      (while (is-type-qualifier *tok*)
	(push (immigrate-package *tok* sc-main:*code-package*)
              rev-pt-qf-list)
	(match *tok*)))
    (let ((pt-qf-list (reverse rev-pt-qf-list)))
      #'(lambda (remtype)
          (let ((ret-type remtype))
            (dolist (pt-qf pt-qf-list ret-type)
              (setq ret-type `(,pt-qf ,ret-type))))))))

;;; returns (id type->type)
(defun direct-declarator (abstract)
  ;; ;; See also suffix_declarator 
  ;; direct-declarator:
  ;;   identifier
  ;;   ( declarator )
  ;;   direct-declarator[ type-qualifier-list(opt) assignment-expression(opt) ]
  ;;   direct-declarator[static type-qualifier-list(opt) assignment-expression]
  ;;   direct-declarator[type-qualifier-list static assignment-expression ]
  ;;   direct-declarator[type-qualifier-list(opt) *]
  ;;   direct-declarator( parameter-type-list )
  ;;   direct-declarator[ identifier-list(opt) ]
  ;; direct-abstract-declarator:
  ;;   ( abstract-declarator )
  ;;   direct-abstract-declarator(opt) [ assignment-expression(opt) ]
  ;;   direct-abstract-declarator(opt) [ * ]
  ;;   direct-abstract-declarator(opt) ( parameter-type-list(opt) )
  (cond
   ((eq 'lparen *tok*)
    (prog2
	(match 'lparen)
	(declarator abstract)
      (match 'rparen)))
   (t
    (if (not abstract)
	(if (eq 'identifier *tok*)
	    (let ((id (lexeme-identifier *lexeme*)))
	      (setq *saw-ident* t)
	      (cond
		((eq 'typedef *storage-class*)
		 (install-symbol id 'typedef 'obj-typedef-name))
		((and (not *parsing-struct*)
		      (not *parsing-oldstyle-parmdecl*))
		 (install-symbol id *storage-class* 'obj-identifier))
	       (t
		 (values)))  ; do nothing
	      (match 'identifier)
	      (let ((scid (identifier-c2sc id)))
		(list scid #'identity)))
            (list '$abstract #'identity))
        (list '$abstract
              #'identity)))))

;;; returns (arg-id-list arg-type-list)
(defun parameter-list ()
  (cond
    ;; no parameters
    ((eq 'rparen *tok*)
     (values (list '() '()) t))
    ;; oldstyle
    ((and (eq 'identifier *tok*)
          (or (null *cursym*)
              (not (eq 'obj-typedef-name 
                       (symbol-dat-object-type *cursym*)))))
     (let* ((id-tmp (lexeme-identifier *lexeme*))
            (rev-id-list (list (identifier-c2sc id-tmp))))
       (match 'identifier)
       (while (eq 'comma *tok*)
         (match 'comma)
         (check-not-typedef)
         (setq id-tmp (lexeme-identifier *lexeme*))
         (push (identifier-c2sc id-tmp) rev-id-list)
         (install-symbol id-tmp 'auto 'obj-parameter)
         (match 'identifier))
       (values (list (reverse rev-id-list) (list))
               nil)))  ; indicates old-style
    ;; newstyle
    (t
     (let ((rev-id-list (list))
           (rev-type-list (list))
           dspec dclr var-defn)
       (let ((*storage-class* nil))
         (setq dspec (declaration-specifiers nil))
         (setq dclr (declarator nil))
         (setq var-defn (car (make-variable-definition
                              dspec (list (list dclr nil)))))
         (push (second var-defn) rev-id-list)
         (push (third var-defn) rev-type-list))
       (while (eq 'comma *tok*)
         (match 'comma)
         (when (eq 'ellipsis *tok*)
           (match 'ellipsis)
           (push 'sc::va-arg rev-type-list)
           (return))  ; from while
         (let ((*storage-class* nil))
           (setq dspec (declaration-specifiers nil))
           (setq dclr (declarator nil))
           (setq var-defn (car (make-variable-definition
                                dspec (list (list dclr nil)))))
           (if (consp (second var-defn))
               (push (car (second var-defn)) rev-id-list)
               (push (second var-defn) rev-id-list))
           (push (third var-defn) rev-type-list)))
       (values (list (reverse rev-id-list)
                     (reverse rev-type-list))
               t)))))    ; indicates new-type



;;; Returns (<id>-><id> <type>-><type>)
(defun suffix-declarator ()
  ;; suffix of (abstract-)direct-declarator
  (cond
    ;; array
    ((eq 'lbrac *tok*)
     (let ((cs-exp nil))
       (match 'lbrac)
       (unless (eq 'rbrac *tok*)
         (setq cs-exp (constant-expression)))
       (match 'rbrac)
       (list
        #'(lambda (id) id)
        (if cs-exp
            #'(lambda (remtype) `(sc::array ,remtype ,cs-exp))
            #'(lambda (remtype) `(sc::array ,remtype))))))
    ;; function
    ((eq 'lparen *tok*)
     (enter-scope)
     (match 'lparen)
     (multiple-value-bind (param-list new-style)
         (parameter-list)
       (match 'rparen)
       (when (and new-style (not (eq 'lbrace *tok*)))
         (exit-scope))
       (setq *is-func* t)
       ;; param-list = ( <arg-id-list> <arg-type-list> )
       (list
        #'(lambda (id)
            `(,id ,@(first param-list))) 
        #'(lambda (remtype)
            `(sc::fn ,remtype ,@(second param-list))))))
    (t
     (error "unexpected error in suffix-declarator"))))

;;; returns (<id> <type>-><type>)
(defun declarator (abstract)
  ;; declarator:
  ;;   pointer(opt) direct-declarator
  ;; abstract-declarator:
  ;;   pointer
  ;;   pointer(opt) direct-abstract-declator
  (let ((ptr #'identity)
	(ddclr nil)
	(rev-suffix-declarator-list (list)))
    (when (eq 'star *tok*)
      (setq ptr (pointer)))
    (setq ddclr (direct-declarator abstract))
    (while (member *tok* '(lbrac lparen) :test #'eq)
      (push (suffix-declarator) rev-suffix-declarator-list))
    ;; ptr     : <type> -> <type>
    ;; ddclr   : ( <id> <type>-><type>)
    ;; suffix-declarator :
    ;;  (<id>-><id> <type>-><type>)->(<id>-><id> <type>)
    (let* ((generalized-id
            (funcall (apply #'compose
                            (mapcar #'first rev-suffix-declarator-list))
                      (first ddclr)))
	   (basetype-to-type
	    (apply #'compose
		   `(,(second ddclr)
		     ,@(mapcar #'second rev-suffix-declarator-list)
                     ,ptr))))
      (awhen (and (consp generalized-id)
                  (member '$abstract generalized-id))
        (setq generalized-id (list-until generalized-id it)))
      (list generalized-id basetype-to-type))))

;;;
(defun designator ()
  ;; designator:
  ;;   [ constant-expression ]
  ;;   . identifier
  (error "SC-0 has not handle designators of C99 yet.")
  (values))

;;;
(defun initializer (recurse)
  ;; initializer-list:
  ;;   desigonation(opt) initializer
  ;;   initializer-list , desigonation(opt) initializer
  ;; desigonation:
  ;;   designator-list =
  ;; designator-list:
  ;;   designator
  ;;   designator-list designator
  ;; initializer:
  ;;   assignment-expression
  ;;   { initializer-list }
  ;;   { initializer-list , }
  (cond
    ((eq 'lbrace *tok*)
     (match 'lbrace)
     (let ((rev-init-list (list (initializer (1+ recurse)))))
       (while (eq 'comma *tok*)
	 (match 'comma)
	 (push (initializer (1+ recurse)) rev-init-list))
       (match 'rbrace)
       `(array ,@(reverse rev-init-list))))
    ((and (< 0 recurse)
	  (member *tok* '(lbrac dot) :test #'eq))
     (format *error-output* 
	     "SC does not handle designator of C99 yet.~%")
     nil)
    (t
     (assignment-expression))))
 
;;; Returns (<declaration-list> <compound-statement>)
;;;          <--- oldstyle --->
(defun function-definition ()
  ;; function-definition:
  ;;  declaration-specifiers declartor declaration-list(opt) compound-statement
  ;;  <---     (declaration)      ---> ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  (prog1
      (if (eq 'lbrace *tok*)
          (list (list) (compound-statement))
          (let ((rev-parmlist (list)))
            (let ((*parsing-oldstyle-parmdecl* t))
              (while (is-declaration *tok*)
                (let ((decl (c2sc-declaration)))
                  (push decl rev-parmlist))))
            (list (mapcan #'copy-list (reverse rev-parmlist)) 
                  (compound-statement))))
    (exit-scope)))

;;; returns (declarator initlalizer)
(defun init-declarator (check-if-function)
  ;; init-declarator-list:
  ;;   init-declarator
  ;;   init-declarator-list , init-declarator
  ;; init-declarator:
  ;;   declarator
  ;;   declarator = initializer
  (let ((*is-func* nil)
        (*saw-ident* nil))
    (let* ((dclr (declarator nil))
           (func-defn
            (and check-if-function 
		 (= *level-function* *level*)
		 *is-func* *saw-ident*
		 (is-function-body *tok*))))
      (if func-defn
          ;; parse function body instead of initializer
          (let* ((fdef (function-definition))
                 (oldstyle-decl-list (first fdef))  
                 (cp-stat (second fdef)))   ; function-body
            (values (list
                     (import-oldstyle-decl dclr oldstyle-decl-list)
                     cp-stat)
                    func-defn))
          ;; parse initializer if exists
          (if (eq 'equals *tok*)
              (progn
                (match 'equals)
                (values (list dclr (initializer 0))
                        func-defn))
              (values (list dclr nil)
                      func-defn))))))

;;; returns SC declaration list
(defun c2sc-declaration ()
  ;; function-definition:
  ;;   declaration-specifiers declarator declaration-list(opt) compound-statement
  ;; declaration:
  ;;   declaration-specifiers init-declarator-list(opt) ;
  (let* ((*storage-class* nil)
	 (dspec (declaration-specifiers nil)))
    (if (eq 'semi *tok*)
	(progn 
	  (match 'semi)
	  (make-variable-definition dspec '()))
	(multiple-value-bind (tmp-idclr fn-def-p)
	    (init-declarator
	     (or
	      *allow-gcc-nested-function*
	      (= *level-global* *level*)))
	  (if fn-def-p
	      (list 
	       (make-function-definition dspec tmp-idclr))
	      (let ((rev-idecl-list (list tmp-idclr)))
		(while (eq 'comma *tok*)
		  (match 'comma)
		  (push (init-declarator nil) rev-idecl-list))
		(match 'semi)
		(make-variable-definition 
		 dspec (reverse rev-idecl-list)))))))) 
	
;;;
(defun translation-unit ()
  ;; translation-unit:
  ;;   external-declaration
  ;;   translation-unit external-declaration
  ;; 
  ;; external-decalration:
  ;;   function-definition
  ;;   declaration
  (let* ((*level* *level-global*)
         (*cursymtab*
          (list
           (make-symbol-table :level *level* :symbols (list))))
         (rev-decl-list (list))
         (*gensym-num* 0)
         (*tok* (lex-get-token)))
   (while *tok*
      (cond
        ((is-external-declaration *tok*)
         (let ((decl (c2sc-declaration)))
           ;; (prin1 decl *error-output*)
           ;; (write-char #\Newline)
           (push decl rev-decl-list)))
        ((eq 'semi *tok*)
	 (match *tok*))
	(t
	 (format *error-output*
		 "Parse failed: unexpected input ~a~%"
		 (tokname *tok*))
	 (return))  ; from while
	(assert (= *level* *level-global*))))
    (reduce #'append (reverse rev-decl-list) :from-end t)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; mygetline 中に macro directive が現れたらここに溜めておく。
;;; *directive-emit-functions* に指定されている関数の返り値に
;;; つなげて一緒に吐き出す
(defvar *scpp-directive-buffer* nil)

(defun mygetline (&optional (stream *standard-input*))
  (aif (read-line stream nil)
       (if (and (eql #\( (string-ref it 0))
                (eql #\% (string-ref it 1)))
           (progn
             (with-input-from-string (strm it)
               (push (read strm) *scpp-directive-buffer*))
             (mygetline stream))
           (string+ it (make-string 1 :initial-element #\Newline)))
       nil))

(defconstant *directive-emit-functions*
  '(c2sc-declaration))

(eval-when (load eval)
  (dolist (de-func *directive-emit-functions*)
    (let ((orig-func (symbol-function de-func)))
      (setf (symbol-function de-func)
            #'(lambda (&rest args)
                (let ((emit-directive 
                       (nreverse *scpp-directive-buffer*)))
                  (setq *scpp-directive-buffer* nil)
                  (nconc 
                   emit-directive
                   (apply orig-func args))))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun parser-main (&optional filename)
  (let (istream)
    (unwind-protect
         (progn
           (setq istream (if filename 
                             (open filename :direction :input)
                             *standard-input*))
           (let* ((*lex-env* (make-lex-env
                              :lnum 0
                              :filename
                              (if filename filename "<stdin>")
                              :getline #'mygetline
                              :stream  istream
                              :lexeme (make-lexeme)))
		  (*lexeme* (lex-env-lexeme *lex-env*)))
	     (decide-unidentified (translation-unit))))
      (when (and istream (not (eq istream *standard-input*)))
        (close istream)))))

;;;
(defconstant *cpp-command* "/home/lab4/hiraisi/mcpp/mcpp-2.4.1/src/cpp_std")
;;(defconstant *cpp-command* "gcc -E -P")
(defconstant *cpp-option* "-D'__extension__=' -D'__attribute__(x)=' -D'__inline='")

(defun call-c-preprocessor (infile &key outfile (option ""))
  (unless outfile
    (setq outfile 
          (file-namestring (change-extension (pathname-name infile) "e"))))
  (system
   (princ
    (format nil "~a ~a ~a ~a >~a"
	    *cpp-command* *cpp-option* option infile outfile)))
  (write-char #\Newline)
  outfile)

;;;
(defun c2sc (infile &key 
             (outfile :default) (delete-efile t) (cpp-option ""))
  (when (eq :default outfile)
    (setq outfile (file-namestring (change-extension infile "sc"))))
  
  (let* ((efile (call-c-preprocessor infile :option cpp-option))
         (sc-program (parser-main efile)))
    (when outfile
      (write-file outfile sc-program 
                  :overwrite t :package sc-main:*code-package*))
    (when delete-efile (delete-file efile))
    (values sc-program outfile)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; (c2sc "csample/c_parser.c")

