;;; Copyright (c) 2008 Tasuku Hiraishi <hiraisi@kuis.kyoto-u.ac.jp>
;;; All rights reserved.

;;; Redistribution and use in source and binary forms, with or without
;;; modification, are permitted provided that the following conditions
;;; are met:
;;; 1. Redistributions of source code must retain the above copyright
;;;    notice, this list of conditions and the following disclaimer.
;;; 2. Redistributions in binary form must reproduce the above copyright
;;;    notice, this list of conditions and the following disclaimer in the
;;;    documentation and/or other materials provided with the distribution.

;;; THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
;;; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;;; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;;; ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
;;; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;;; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
;;; OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
;;; HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
;;; LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
;;; OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
;;; SUCH DAMAGE.

;;;; Parse C code to generate SC-0 code

(provide "C2SC-PARSER")
(eval-when (:compile-toplevel :load-toplevel :execute)
  (require "SC-DECL" "sc-decl.lsp"))

(in-package "C2SC")

(scr:require "C2SC-LEX")
(scr:require "SC-FILE")
(scr:require "SC-MISC")
(scr:require "SCT")
(rule:require-ruleset :sc2c)

(defconstant *c2sc-package* *package*)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
(defvar *cursymtab*)                    ; list (struct symbol-table)
;;(defvar *curlabels*)
;;(defvar *curtypes*)

;; information about a last read identifier
(defvar *cursym* nil)                   ; (struct symbol-dat)

(defvar *level* 0)   ;; block level
(defconstant *level-global* 0)
(defconstant *level-function* 1)
(defconstant *level-statement* 2)

;;; whether c2sc::piece-c2sc is being processed
(defvar *parsing-piece* nil)

;;; c2sc-getline 中に preprocessing directive が現れたらここに溜めておく。
;;; %defmacro, %defconstant, %undef
;;;  --> *scpp-macro-buffer* 
;;;  --> translation-unit の末尾につなげる
;;; それ以外 
;;;  --> *scpp-directive-buffer* 
;;;  --> *directive-emit-functions* に指定されている関数の返り値の頭に
;;;      つなげて一緒に吐き出す
(defvar *scpp-directive-buffer* nil)
(defvar *scpp-macro-buffer* nil)
(defparameter *directive-emit-functions* '(declaration statement))

;; Dynamically bound for each top-level declaration
;; The translation result is ignored if set non-nil while processing
(defvar *ignore-reasons* nil)

;; not handled completely yet
(defparameter *allow-gcc-nested-function* t) 
;; not handled completely yet
(defparameter *allow-statement-expressions* nil)
;; alist to denote whether identifier is type-name
(defparameter *macroarg-typep* '() )
;; C macros/varaibles/aggregates to be translated into SC
;; t --> all macros/varaibles/aggregates
(defparameter *required-macros* t)
(defparameter *required-variables* t)   ; includes functions
(defparameter *required-aggregates* t)  ; struct, union and enum
;;; ignore %line
(defparameter *ignore-line-directive* t)

;;; structures
(defstruct (symbol-dat
            (:constructor create-symbol-dat
                          (name storage-class object-type)))
  (name "")
  (storage-class nil)
  (object-type nil)
  )

(defstruct symbol-table
  (level *level-global*)                ; block-level
  (symbols (list))                      ; list of (struct symbol-dat)
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun display-lnum ()
  (format *error-output* "~A.~D: "
    (aif (lex-env-filename *lex-env*) it "")
    (lex-env-lnum *lex-env*)))

(defmacro c2sc-error (&rest error-args)
  `(if *parsing-piece*
       (throw 'try-parsing :parse-failed)
     (progn
       (display-lnum)
       (error ,@error-args))))
(defmacro c2sc-warn (&rest warn-args)
  `(if *parsing-piece*
       (throw 'try-parsing :parse-failed)
     (progn
       (display-lnum)
       (warn ,@warn-args))))


;;;

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
           (member token '(star lparen identifier) :test #'eq))))

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
    (push (make-symbol-table) *cursymtab*))
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
      (c2sc-warn "Warning: redeclaration of symbol ~a as ~a~%~
                  Warning: previously declared as ~a~%"
                 name (object-name object-type)
                 (symbol-dat-object-type sym)) )
    (push (create-symbol-dat name storage-class object-type)
          (symbol-table-symbols (car *cursymtab*)))))


(defun match (expected-tok &key (resume-when-unexpected nil))
  (if (not (eq expected-tok *tok*))
      (if (and *parsing-piece* resume-when-unexpected)
          *tok*
        (c2sc-warn
         "Parse failed: Expected ~a,~%got ~a~%"
         (tokname expected-tok) (tokname *tok*)))
    (prog1
        (setq *tok* (lex-get-token))
      (setq *cursym*
        (if (eq 'identifier *tok*)
            (c2sc-find-symbol 
             *cursymtab* 
             (lex-env-lastval *lex-env*)
             t)
          nil)))))

(defun check-not-typedef ()
  (or (null *cursym*)
      (not (eq 'obj-typedef-name (symbol-dat-object-type *cursym*)))))

;;;

;; convert C identifier to SC-0 identifier
(defun identifier-c2sc (cid)
  (rule:make-id cid *clang-package*))
#+obsolete
(defun identifier-c2sc (cid)
  (let ((cid-list (nsubstitute #\- #\_ (concatenate 'list cid))))
    (labels ((cidscid (remain acc capital)
               (if (endp remain)
                   acc
                 (let ((ch (car remain)))
                   (if (or (and capital       (char<= #\a ch #\z))
                           (and (not capital) (char<= #\A ch #\Z)))
                       (cidscid (cdr remain)
                                (cons ch (cons #\@ acc))
                                (not capital))
                     (cidscid (cdr remain)
                              (cons ch acc)
                              capital))))))
      (intern
       (map 'string
         #'char-invertcase
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
                    ((char= #\~ ch)
                     (cstr-scstr
                      (cdr remain)
                      (cons "~~" acc)))
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

;; get-gensymid で mark をつけたところの identifier を決定する
(defun decide-unidentified (sc-program)
  (let ((dec-id (memoize
                 #'(lambda (usym)
                     (declare (ignorable usym))
                     (gentemp "UNNAMED" *clang-package*)))))
    (rmapcar #'(lambda (x)
                 (if (is-unidentified x)
                     (funcall dec-id x)
                   x))
             sc-program)))

(defun is-unidentified (sym)
  (and (symbolp sym)
       (string-begin-with "$UNIDENTIFIED" (symbol-name sym))))

(defun remove-not-required (sc-program
                            &optional (rvars *required-variables*)
                                      (ragrs *required-aggregates*))
  (if (and (eq t rvars) (eq t ragrs)) sc-program
    (loop for df in sc-program
        when (or (atom df) (not (defsymbol-p (car df))) ; 宣言・定義以外
                 (if (consp (second df))
                     (if (member (car (second df)) ~(struct union enum))
                         (aggregate-required-p (second (second df)))
                       (variable-required-p (car (second df))))
                   (variable-required-p (second df))))
        collect df)))

(defun defsymbol-p (sym)
  (rule:declaration-tag? sym :sc2c))

;; 無名のstructなどは，SCでは無名にできないので，
;; mark をつけておいて,後で identifier を決定する
(defvar *gensym-num*)
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
;;;             |-(<id> <type>-><type>)
;;; Returns "declaration" of function definition
(defun make-function-definition (dspec idclr)
  (let ((st-class (first dspec))
        (base-type (second dspec))
        (id (first (first idclr)))
        (gettype (second (first idclr)))
        (cp-stat (first (second idclr))))
    (unless (and (consp cp-stat)
                 (eq 'sc::begin (car cp-stat)))
      (error "~s is not compound-statement." cp-stat))
    (let ((defsymbol (make-defsymbol st-class cp-stat)))
      `(,defsymbol ,id ,(funcall gettype base-type)
         ,@(cdr cp-stat)))))

;;; dspec : (<storage-class> <base-type>)
;;; idclr-list : (<dclr> <initializer> [<bitfield>]) list
;;;                  |-(<id> <type>-><type>)
;;; Returns declaration list
(defun make-variable-definition (dspec idclr-list &aux def-aggr)
  (destructuring-bind (st-class base-type) dspec
    (when (and (consp base-type)
               (member (car base-type)
                       '(sc::struct sc::union sc::enum))
               (not (endp (cddr base-type))))
      (let ((mems (cddr base-type)))
        (setq base-type (firstn base-type 2))
        (setq def-aggr (list `(sc::def ,base-type ,@mems)))))
    (nconc
     def-aggr
     (mapcan
      #'(lambda (idclr)
          (destructuring-bind ((id gettype) init &optional bitfield) idclr
            (let ((defsymbol (make-defsymbol st-class init))
                  (type (funcall gettype base-type)))
              (when (consp id)
                (if (and (consp type)
                         (eq 'sc::fn (car type)))
                    (setq id (list-until id nil :key #'car))
                  (setq id (car id))))
              ;; (format *error-output* "~&~S; ~S~&" base-type (funcall gettype base-type))
              (list*
               ;; declaration
               `(,defsymbol ,id ,type ,@(if init (list init) '()))
               ;; bitfield
               (when bitfield `(:bit ,bitfield))))))
      idclr-list))))

;;
(defun make-defsymbol (st-class init)
  (if (eq 'typedef st-class)
      'sc::deftype
    (let ((def-or-decl (if (or init (eq 'static st-class))
                           'sc::def
                         'sc::decl)))
      (if st-class
          (immigrate-package 
           (symbol+ st-class '- def-or-decl) sc-file:*code-package*)
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
    (c2sc-error "Unexpected token in expression"))
  
  (let ((rev-as-exp-list (list (assignment-expression))))
    (vwhile (eq 'comma *tok*)
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
  ;;   ( expression )         --> handled in unary-expression
  ;;   ( compound-statement ) --> handled in unary-expression (gcc extension)
  (cond
   ((eq 'identifier *tok*)
    (prog1
        (identifier-c2sc (lex-env-lastval *lex-env*))
      (match 'identifier)))
   ((eq 'string-constant *tok*)
    (prog1
        (string-c2sc (lex-env-lastval *lex-env*))
      (match 'string-constant)))
   ((is-constant *tok*)
    (prog1
        (lex-env-lastval *lex-env*)
      (match *tok*)))
   (t
    (c2sc-error "unexpected token in primary-expression."))))

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
             (vwhile (eq 'comma *tok*)
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
         (let ((id (identifier-c2sc (lex-env-lastval *lex-env*))))
           #'(lambda (cur-exp)
               `(sc::fref ,cur-exp ,id)))
       (match 'identifier)))
    ;; fref ->
    ((arrow)
     (prog2 
         (match 'arrow)
         (let ((id (identifier-c2sc (lex-env-lastval *lex-env*))))
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
     (c2sc-error "unexpected token in postfix-operator"))))

;;;
(defun postfix-operators ()
  (let ((ret-func #'identity))
    (vwhile (member *tok* 
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
      (let ((exp (if (eq 'lbrace *tok*)
		     ;; statement-expression (gcc extension)
		     (let ((ret `(sc::stat-exp ,@(cdar (compound-statement)))))
		       (unless *allow-statement-expressions*
			 (push `("statement-expression ~S" ,ret)
			       *ignore-reasons*))
		       ret)		   
		   ;; parenthesized exp
		   (expression))))
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
    (vwhile (or (and (eq 'star    *tok*) (setq current-op 'sc::*))
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
    (vwhile (or (and (eq 'plus  *tok*) (setq current-op 'sc::+))
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
    (vwhile (or (and (eq 'lshift *tok*) (setq current-op 'sc::<<))
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
    (vwhile (or (and (eq 'greaterthan *tok*) (setq current-op 'sc::>))
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
      (vwhile (or (and (eq 'eqeq *tok*)  (setq current-op 'sc::==))
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
    (vwhile (eq 'and *tok*)
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
    (vwhile (eq 'xor *tok*)
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
    (vwhile (eq 'or *tok*)
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
    (vwhile (eq 'andand *tok*)
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
    (vwhile (eq 'oror *tok*)
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
  (let ((id (identifier-c2sc (lex-env-lastval *lex-env*)))
        stat)
    (match 'identifier)
    (match 'colon)
    (setq stat (statement))
    (list
     `(sc::label ,id ,stat))))

;;;
(defun case-statement ()
  (let (cs-exp stat)
    (match 'case)
    (setq cs-exp (constant-expression))
    (match 'colon)
    (setq stat (statement))
    (list*
     `(sc::case ,cs-exp)
     stat)))

;;;
(defun default-statement ()
  (let (stat)
    (match 'default)
    (match 'colon)
    (setq stat (statement))
    (list*
     `(sc::default)
     stat)))

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
          (list `(sc::if ,exp ,(mk-compound stat1) ,(mk-compound stat2)))
        (list `(sc::if ,exp ,(mk-compound stat1))) ))))

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
            stat)))))


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
            stat)))))


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
    (match 'semi :resume-when-unexpected t)        
    (list
     `(sc::do-while ,exp
        ,@(if (and (endp (cdr stat))
                   (consp (car stat))
                   (eq 'sc::begin (caar stat)))
              (cdar stat)
            stat)))))

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
           (setq decl-or-exp-list (declaration))
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
    (list
     `(sc::for (,@decl-or-exp-list ,exp2 ,exp3)
        ,@(if (and (endp (cdr stat))
                   (consp (car stat))
                   (eq 'sc::begin (caar stat)))
              (cdar stat)
            stat)))))

;;;
(defun break-statement ()
  ;; continue-statement:
  ;;   continue ;
  (match 'break)
  (match 'semi)
  (list
   '(sc::break)))

;;;
(defun continue-statement ()
  ;; goto-statement:
  ;;   goto identifier ;
  (match 'continue)
  (match 'semi)
  (list
   '(sc::continue)))

;;;
(defun goto-statement ()
  ;; goto-statement:
  ;;   goto identifier ;
  (prog2
      (match 'goto)
      (list
       `(sc::goto ,(identifier-c2sc (lex-env-lastval *lex-env*))))
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
        (list
         '(sc::return)))
    (prog1
        (list
         `(sc::return ,(expression)))
      (match 'semi))))


;;;
(defun empty-statement ()
  (match 'semi)
  (list ()))

;;;
(defun expression-statement ()
  ;; expression-statement:
  ;;   expression ;
  (if (and (eq 'identifier *tok*)
           (lex-colon-follows))
      (labeled-statement)
    (prog1
        (list (expression))
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
       (c2sc-error "unexpected error in statement")))))

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
     (vwhile (not (eq 'rbrace *tok*))
             (if (is-declaration *tok*)
                 (if (and (eq 'identifier *tok*)
                          (lex-colon-follows))
                     (push (statement) rev-bi-list)
                   (push (declaration) rev-bi-list))
               (push (statement) rev-bi-list))))
    (match 'rbrace)
    (list
     `(sc::begin ,@(mapcan #'copy-list (reverse rev-bi-list))))))


;;;
(defun enumerator ()
  ;; enumerator:
  ;;   enumeration-constant
  ;;   enumeration-constant = constant-expression
  ;; enumeration-constant:
  ;;   identifier
  (unless (eq 'identifier *tok*)
    (c2sc-error "Unexpected token in enumerator."))
  (let ((id nil)
        (val nil))
    (let ((id-tmp (lex-env-lastval *lex-env*)))
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
  (match 'enum)
  (let ((id (get-gensymid))
        (rev-enm-list (list)))
    (when (eq 'identifier *tok*)
      (setq id (identifier-c2sc (lex-env-lastval *lex-env*)))
      (match 'identifier))
    (when (eq 'lbrace *tok*)
      (match 'lbrace)
      (push (enumerator) rev-enm-list)
      (vwhile (eq 'comma *tok*)
              (match 'comma)
              (push (enumerator) rev-enm-list))
      (match 'rbrace))
    `(sc::enum ,id ,@(reverse rev-enm-list))))  

;;;
(defun struct-member ()
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
(defun struct-members ()
  ;; struct-declaration-list:
  ;;   struct-declaration
  ;;   struct-declaration-list struct-declaration
  ;; struct-declaration:
  ;;   specifier-qualifier-list struct-declarator-list;
  ;; struct-declarator-list:
  ;;   struct-declarator
  ;;   struct-declarator-list , struct-declarator
  (let ((rev-fdecl-list (list)))
    (vwhile (not (eq 'rbrace *tok*))
            (let* ((*storage-class* nil)
                   (dspec (declaration-specifiers t))
                   (rev-memb-list (list (struct-member))))
              (vwhile (eq 'comma *tok*)
                      (match 'comma)
                      (push (struct-member) rev-memb-list))
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
      (setq id (identifier-c2sc (lex-env-lastval *lex-env*)))
      (match *tok*))
    (when (eq 'lbrace *tok*)
      (match 'lbrace)
      (setq def-p t)
      (setq fdecl-list (struct-members))
      (match 'rbrace))
    (values
     `(,(case str-or-uni 
          ((struct) 'sc::struct)
          ((union)  'sc::union)
          (otherwise
           (c2sc-error "Invalid struct-or-union specifier.")))
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
     (car (last 
           (make-variable-definition
            dspec (list (list dclr nil)))) ))))


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
    (unless (is-declaration *tok*)
      (c2sc-error "Unexpected token in declaration-spceiciers."))
    (vwhile (is-declaration *tok*)
            (when (and no-storage-class
                       (is-storage-class *tok*))
              (c2sc-error "Parse failed: unexpected storage class ~a"
                          (tokname *tok*)))
            (when (and (eq 'identifier *tok*) type-found)
              (return))                 ; from while
            (when (or (is-type-specifier *tok*)
                      (eq 'identifier *tok*))
              (setq type-found t))
            (let ((savedtok *tok*))
              (cond
               ;; struct, union
               ((is-struct-or-union *tok*)
                (setq type-exp (struct-or-union-specifier))
                (return))               ; from while
               ;; enum
               ((eq 'enum *tok*)
                (setq type-exp (enum-specifier))
                (return))               ; from while
               ;; typedef-name
               ((eq 'identifier *tok*)
                (setq type-exp 
                  (identifier-c2sc (lex-env-lastval *lex-env*))))
               ;; (storage-class)
               ((is-storage-class *tok*)
                (setq st-class *tok*)
                (setq *storage-class* *tok*))
               ;; element type
               ((member *tok* '(void char int float double) :test #'eq)
                (setq type-exp (immigrate-package 
                                *tok* sc-file:*code-package*)))
               ;; gcc implementation dependent types
               ((and *allow-gcc-types*
                     (eq 'gcc-type *tok*))
                (setq type-exp (identifier-c2sc (lex-env-lexeme *lex-env*))))
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
                (push (immigrate-package *tok* sc-file:*code-package*)
                      rev-qf-list))
               (t
                (c2sc-error
                 "unexpected error in declaration-specifiers.")))
              (match *tok*)
              (when (eq 'identifier savedtok)
                (return))))             ; from while
    
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
    (vwhile (eq 'star *tok*)
            (push 'sc::ptr rev-pt-qf-list)
            (match 'star)
            (vwhile (is-type-qualifier *tok*)
                    (push (immigrate-package *tok* sc-file:*code-package*)
                          rev-pt-qf-list)
                    (match *tok*)))
    (let ((pt-qf-list (reverse rev-pt-qf-list)))
      #'(lambda (remtype)
          (let ((ret-type remtype))
            (dolist (pt-qf pt-qf-list ret-type)
              (setq ret-type `(,pt-qf ,ret-type))))))))

;;; returns (id type->type)
(defun direct-declarator (abstract)
  ;; ;; concerned with arrays or functions, see suffix-declarator
  ;; direct-declarator:
  ;;   identifier
  ;;   ( declarator )
  ;;   direct-declarator[ type-qualifier-list(opt) assignment-expression(opt) ]
  ;;   direct-declarator[static type-qualifier-list(opt) assignment-expression]
  ;;   direct-declarator[type-qualifier-list static assignment-expression ]
  ;;   direct-declarator[type-qualifier-list(opt) *]
  ;;   direct-declarator( parameter-type-list )
  ;;   direct-declarator[ identifier-list(opt) ]
  ;; direct-abstract-declarator:   ; when the argument abstract is non-nil
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
            (let ((id (lex-env-lastval *lex-env*)))
              (setq *saw-ident* t)
              (cond
               ((eq 'typedef *storage-class*)
                (install-symbol id 'typedef 'obj-typedef-name))
               ((and (not *parsing-struct*)
                     (not *parsing-oldstyle-parmdecl*))
                (install-symbol id *storage-class* 'obj-identifier))
               (t
                (values)))              ; do nothing
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
    (let* ((id-tmp (lex-env-lastval *lex-env*))
           (rev-id-list (list (identifier-c2sc id-tmp))))
      (match 'identifier)
      (vwhile (eq 'comma *tok*)
              (match 'comma)
              (check-not-typedef)
              (setq id-tmp (lex-env-lastval *lex-env*))
              (push (identifier-c2sc id-tmp) rev-id-list)
              (install-symbol id-tmp 'auto 'obj-parameter)
              (match 'identifier))
      (values (list (reverse rev-id-list) (list))
              nil)))                    ; indicates old-style
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
      (vwhile (eq 'comma *tok*)
              (match 'comma)
              (when (eq 'ellipsis *tok*)
                (match 'ellipsis)
                (push 'sc::va-arg rev-type-list)
                (return))               ; from while
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
              t)))))                    ; indicates new-type



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
    (c2sc-error "unexpected error in suffix-declarator"))))

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
    (vwhile (member *tok* '(lbrac lparen) :test #'eq)
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
  (c2sc-error "SC-0 has not handle designators of C99 yet.")
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
      (vwhile (eq 'comma *tok*)
              (match 'comma)
              (push (initializer (1+ recurse)) rev-init-list))
      (match 'rbrace)
      `(array ,@(reverse rev-init-list))))
   ((and (< 0 recurse)
         (member *tok* '(lbrac dot) :test #'eq))
    (c2sc-error "SC does not handle designator of C99 yet.")
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
            (vwhile (is-declaration *tok*)
                    (let ((decl (declaration)))
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
                 (cp-stat (second fdef))) ; function-body
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
(defun declaration ()
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
          (init-declarator (or *allow-gcc-nested-function*
                               (= *level-global* *level*)))
        (if fn-def-p
            (list (make-function-definition dspec tmp-idclr))
          (let ((rev-idecl-list (list tmp-idclr)))
            (vwhile (eq 'comma *tok*)
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
  (let* ((rev-decl-list (list)))
    (vwhile *tok*
            (cond
             ((is-external-declaration *tok*)
              (let* ((*ignore-reasons* (list))
                     (decl (declaration)))
                ;; <- debug-print
                ;;    (dolist (dcl decl)
                ;;      (prin1 dcl *error-output*)
                ;;      (fresh-line *error-output*))
                ;; ->
                (if (not *ignore-reasons*)
                    (push decl rev-decl-list)
                  (progn
                    (warn "~&~S is ignored because of:" decl)
                    (loop for rsn in (nreverse *ignore-reasons*)
                        do (write-string "  " *error-output*) 
                           (apply #'format *error-output* rsn)
                           (fresh-line *error-output*))))
                ))
             ((eq 'semi *tok*)
              (match *tok*))
             (t
              (c2sc-error "Parse failed: unexpected input ~a"
                          (tokname *tok*))))
            ;;(assert (= *level* *level-global*))
            )
    (nconc
     (reduce #'nconc (nreverse rev-decl-list) :from-end t)
     (prog1 (nreverse *scpp-macro-buffer*) 
       (setq *scpp-macro-buffer* nil)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun c2sc-getline (&optional (stream *standard-input*))
  (when *lex-env*
    (incf (lex-env-lnum *lex-env*)))
  (aif (read-line stream nil)
      (if (eql #\$ (string-ref it 0))
          (progn
            ;; S-expression としてreadして、、、
            (handle-c2sc-directive (string-left-ntrim it))
            ;; あらためて次の行を読む
            (c2sc-getline stream))
        (funcall (if *want-debugging-output* #'print #'identity)
                 (string+ it #.(format nil "~%"))))
    nil))

(defun handle-c2sc-directive (string)
  (with-input-from-string (strm string)
    (let ((directive 
           (with-readtable sc-file:*code-readtable* (read strm))))
      (case (car directive)
        ;; (%defconstant-cexp <macro-name> <c-code-string>)
        (%defconstant-cexp
         (awhen (defconstant-cexp-c2sc directive)
           (push it *scpp-macro-buffer*)))
        ;; (%defmacro-cexp <macro-name> <c-code-string> <arg-list>)
        (%defmacro-cexp
         (awhen (defmacro-cexp-c2sc directive)
           (push it *scpp-macro-buffer*)))
        ;; (%pragma <string> {<string>})
        ((%pragma sc::%pragma)
         (let ((pragma-arg (apply #'string+ (cdr directive))))
           ;; indicate <id> is/isn't type-name
           (acond
            ((string-begin-with "c2sc_typename" pragma-arg)
             (set-macroarg-typep (get-all-identifier it) t))
            ((string-begin-with "c2sc_not_typename" pragma-arg)
             (set-macroarg-typep (get-all-identifier it) nil))
            ((string-begin-with "c2sc_query_typename" pragma-arg)
             (set-macroarg-typep (get-all-identifier it) :remove))
            (t
             (push directive *scpp-directive-buffer*)))))
        ;;
        ((sc::%defmacro sc::%defconstant)
         (push directive *scpp-macro-buffer*))
        ;;
        (sc::%undef
         (push directive *scpp-macro-buffer*))
        ;;
        (sc::%line
         (unless *ignore-line-directive*
           (push directive *scpp-directive-buffer*)))
        ;;
        (otherwise
         (push directive *scpp-directive-buffer*))))))

;; *directive-emit-functions* の関数（構文単位）処理後，
;; *scpp-direcive-buffer*にたまったプリプロセッサ指令を吐き出す
(dolist (de-func *directive-emit-functions*)
  (let ((orig-func (symbol-function de-func)))
    (setf (symbol-function de-func)
      #'(lambda (&rest args)
          (let ((emit-directive 
                 (nreverse *scpp-directive-buffer*)))
            (setq *scpp-directive-buffer* nil)
            (nconc emit-directive
                   (apply orig-func args)))))))

;;; %defconstant-cexp 中のCコードの断片を可能ならSC化
(defun defconstant-cexp-c2sc (defconstant-exp-directive)
  (assert (eq '%defconstant-cexp (car defconstant-exp-directive)))
  (let* ((const-name (second defconstant-exp-directive))
         (piece-c-code (third defconstant-exp-directive))
         ;; C の "~" は "~~" になっているので
         (formatted-c-code #|(format nil|# piece-c-code #|)|#))
    (if (not (macro-required-p const-name)) nil
      (progn
        (let ((*package* sc-file:*code-package*))
          (format *error-output*
              "~&#define ~A ~A~%"
            (rule:identifier const-name :sc2c) formatted-c-code))
        `(sc::%defconstant
          ,const-name
          ,(acond
            ((piece-c2sc formatted-c-code)
             (format *error-output* "==> ~S~%" it)
             it)
            (t
             (unless (string= "" formatted-c-code)
               (format *error-output* "==> <give up...>~%"))
             `(sc::c-exp ,piece-c-code)) ))))))

;;; defmacro における c-code の "$<arg_num>$" の部分を引数名に置き換える
;;; 返り値は
;;; (values `(c-exp ...)
;;;         <formatted-c-code>)
(defun replace-arguments (piece-c-code arg-list)
  (let ((argname-vect 
         (concatenate 'vector (mapcar (suffixed-func #'rule:identifier :sc2c) arg-list)))
        (rev-cexpstr) (rev-formatted-c-code))
    (dolist (token-cons (get-token-list piece-c-code))
      (let ((token (car token-cons))
            (lexeme (cdr token-cons)))
        (if (and (eq 'identifier token)
                 (char= (aref lexeme 0) #\$))
            (let* ((argno-str (string-trim "$" lexeme))
                   (argno (parse-integer argno-str)))
              (push (string+ "~" argno-str "@*~a") rev-cexpstr)
              (push (aref argname-vect argno) rev-formatted-c-code))
          (progn
            (push (if (string= lexeme "~")
                      "~~"
                    lexeme)
                  rev-cexpstr)
            (push lexeme rev-formatted-c-code)))))
    (let ((cexpstr (strcat (reverse rev-cexpstr) " "))
          (formatted-c-code (strcat (reverse rev-formatted-c-code) " ")))
      (values `(cl:list 'sc::c-exp ,cexpstr ,@arg-list)
              formatted-c-code) )))

;; マクロ本体を backquote-macro に変換
(defun to-eval-macroarg (scized-defmacro macro-arg-list)
  (if (atom scized-defmacro)
      (if (member scized-defmacro macro-arg-list)
          scized-defmacro
        `',scized-defmacro)
    `(cl:list ,@(mapcar (suffixed-func #'to-eval-macroarg macro-arg-list)
                        scized-defmacro))))

;;; %defmacro-cexp 中のCコードの断片を可能ならSC化
(defun defmacro-cexp-c2sc (defmacro-exp-directive)
  (assert (eq '%defmacro-cexp (car defmacro-exp-directive)))
  (destructuring-bind (macro-name piece-c-code arg-list)
      (cdr defmacro-exp-directive)
    (if (not (macro-required-p macro-name)) nil
      (multiple-value-bind (cexp-exp formatted-c-code)
          (replace-arguments piece-c-code arg-list)
        (format *error-output*
            "~&#define ~A(~{~A~^,~}) ~A~%"
          (rule:identifier macro-name :sc2c)
          (mapcar (suffixed-func #'rule:identifier :sc2c) arg-list)
          formatted-c-code)
        `(sc::%defmacro
             ,macro-name
             ,arg-list
           ,(acond
             ((string= "" formatted-c-code)
              cexp-exp)
             ((piece-c2sc formatted-c-code)
              (format *error-output* "==> ~S~%" it)
              (to-eval-macroarg it arg-list))
             (t
              (format *error-output* "==> <give up...>~%")
              cexp-exp) ))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; 環境を整えてinitiatorから構文解析開始
;; input : input-stream or string
(defun parser-main (input
                    &key
                    (filename "input-stream")
                    (initiator #'translation-unit)
                    (symtab-list (list (make-symbol-table))))
  (cond
   ((or (stringp input)
        (pathnamep input))
    (with-open-file (istream input :direction :input)
      (parser-main istream :filename (namestring input)
                   :initiator initiator :symtab-list symtab-list)))
   ((input-stream-p input)
    (let* ((*lex-env* (make-lex-env :filename filename :stream input))
           (*level* *level-global*)
           (*cursymtab* symtab-list)
           (*gensym-num* 0)
           (*macroarg-typep* *macroarg-typep*)
           (*tok* (lex-get-token)))
      (values
       (decide-unidentified (funcall initiator))
       *tok*)))
   (t
    (error "Unexpected type of 'input'"))))

(defparameter *tried-function*
    (list #'translation-unit #'type-name #'struct-members
          #'expression #'statement))

(defun set-macroarg-typep (id-list mode)
  (setq id-list (mklist id-list))
  (dolist (id id-list)
    (setq *macroarg-typep*
      (remove id *macroarg-typep* :key #'car :test #'string=)))
  (unless (eq :remove mode)
    (dolist (id id-list)
      (setq *macroarg-typep* (acons id mode *macroarg-typep*))))
  *macroarg-typep*)

(defun macro-required-p (macroname &optional (required-macros *required-macros*))
  (or (eq t required-macros)
      (member (rule:identifier0 macroname :sc2c)
              required-macros :test #'string=)))
(defun variable-required-p (variablename &optional (required-variables *required-variables*))
  (or (eq t required-variables)
      (member (rule:identifier0 variablename :sc2c)
              required-variables :test #'string=)))
(defun aggregate-required-p (agrname &optional (required-aggregates *required-aggregates*))
  (or (eq t required-aggregates)
      (member (rule:identifier0 agrname :sc2c)
              required-aggregates :test #'string=)))

(defun get-all-typename-pattern (id-list)
  (setq id-list (remove-if #'(lambda (x) 
                               (member x *macroarg-typep*
                                       :key #'car :test #'string=))
                           id-list))
  (power-set id-list) )

(defun make-symtab (typename-list)
  (flet ((make-typename-entry (typename)
           (create-symbol-dat typename 'typedef 'obj-typedef-name) ))
    (let ((fixed-typename-entries
           (mapcar (compose #'make-typename-entry #'car)
                   (remove-if-not #'cdr *macroarg-typep*) )))
      (make-symbol-table
       :symbols
       (append 
        fixed-typename-entries
        (mapcar #'make-typename-entry typename-list) )))))

;; 同じ変換結果を削除する．
;; 複数の候補の原因が型変数(or not)の場合，その変数名を特定する．
(defun remove-duplicate-candidates (all-candidates)
  (let ((ret (list)))   ;; (<candidate> <typename-list> <not-typename-list>)
    (dolist (cand all-candidates)
      (aif (member (car cand) ret :key #'car :test #'equal)
          (rplaca it (list (car cand)
                           (intersection (second (car it)) (second cand))
                           (intersection (third  (car it)) (third  cand)) ))
        (push cand ret)))
    ;; 候補の違いに影響してない変数名を除く
    (mapc
     #'(lambda (get-place &aux (get-placea (compose #'car get-place)))
         (dolist (tn (funcall get-placea (first ret)))
           (when (every #'(lambda (x) (member tn x :test #'string=))
                        (mapcar get-placea (cdr ret)))
             (dolist (cand ret)
               (rplaca (funcall get-place cand)
                       (remove tn (funcall get-placea cand) 
                               :test #'string=) )))))
     (list #'cdr #'cddr))
    ret))

;; tried-functionで指定された全ての場所からparsingを試みる
(defun try-all-parsing (istring &key (tried-function *tried-function*))
  (check-type istring string)
  (let* ((all-candidates)               ; list of (<result> <tn-list> <not-tn-list>)
         (all-identifier (get-all-identifier istring))
         (all-typename-pattern (get-all-typename-pattern all-identifier)))
    (dolist (typename-list all-typename-pattern)
      (let ((symtab (make-symtab typename-list)))
        (dolist (parsing-function tried-function)
          (let ((*parsing-piece* t)
                (*scpp-directive-buffer* nil)
                (*scpp-macro-buffer* nil))
            (with-input-from-string (istream (string+ istring #|";"|#))     
              (multiple-value-bind (parse-result remained-tok)
                  (catch 'try-parsing
                    (parser-main istream
                                 :initiator parsing-function
                                 :symtab-list (list symtab)))
                (when (and (not (eq :parse-failed parse-result))
                           parse-result
                           (or (not remained-tok)
                               #|(eq 'semi remained-tok)|#) )
                  (push (list parse-result
                              typename-list
                              (set-difference 
                               all-identifier typename-list :test #'string=) )
                        all-candidates) )))))))
    (remove-duplicate-candidates all-candidates) ))

(defun piece-c2sc (istring &key (tried-function *tried-function*))
  (let* ((candidates (try-all-parsing
                      istring
                      :tried-function (mklist tried-function)))
         (n-candidates (length candidates)))
    (case n-candidates
      (0 nil)
      (1 (first (car candidates)))
      (otherwise
       (let ((*package* sc-file:*code-package*))
         (car
          (query-select-list 
           candidates
           :print-elm 
           #'(lambda (elm)
               (string+ 
                (write-to-string (first elm))
                (format nil "~@[~%[typename --- ~{~a~^, ~}]~]" (second elm))
                (format nil "~@[~%[not typename --- ~{~a~^, ~}]~]" (third elm))
                )))))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;; MCPP
(defparameter *cpp-command* (namestring (merge-pathnames "c2scpp/src/c2sc_cpp" 
                                                         (truename scr:*sc-system-path*))))
#+(and allegro mswindows)
(setq *cpp-command* (car (excl.osi:command-output (string+ "\\cygwin\\bin\\cygpath " *cpp-command*))))

(defparameter *cpp-option*
    (append
     '("-D__complex__"
       "-D__const=const" "-D__extension__=" "-D__cdecl="
       "-D__attribute__(x)=" "-D__inline=" "-D__inline__="
       "-D__asm(x)=" "-D__asm__(x)="
       "-D__declspec(x)="
       )
     #+(or cygwin mingw mswindows) '("-I" "/lib/gcc/i686-pc-cygwin/3.4.4/include/"
                                     "-I" "/usr/lib/gcc/i686-pc-cygwin/3.4.4/include/")
     ))

;;(defparameter *cpp-command* "gcc -E -P")
;;(defparameter *cpp-option* 
;;             '("-D__extension__=" "-D__attribute__(x)=" "-D__inline="))

(defun unix-namestring (path)
  (substitute #\/ #\\ (namestring path)))

(defun call-c2sc-preprocessor 
    (infile 
     &key (option '())
     &aux (outfile (change-extension infile "se")))
  (let ((inname (unix-namestring infile)))
    (command-line *cpp-command*
                  :args`(,inname ,@*cpp-option* ,@option)
                  :verbose *error-output*)
    )
  outfile)

;;;
(defun c2sc (infile &key 
                    (outfile :default) (delete-efile nil) 
                    (call-preprocessor :default)
                    (cpp-option '())
                    ((:required-macros *required-macros*) *required-macros*)
                    ((:required-variables *required-variables*) *required-variables*)
                    ((:required-aggregates *required-aggregates*) *required-aggregates*)
                    )
  (setq outfile
    (cond 
     ((eq :default outfile)
      (change-extension infile "sc"))
     (outfile
      (make-pathname
       :name (pathname-name outfile)
       :type (pathname-type outfile)
       :directory (aif (pathname-directory outfile) it
                    (pathname-directory infile))))
     (t nil)))
  (setq call-preprocessor
    (if (eq :default call-preprocessor)
        (string/= "se" (pathname-type infile))
      call-preprocessor))
  (let* ((efile (if call-preprocessor
                    (call-c2sc-preprocessor infile :option cpp-option)
                  infile))
         (sc-program (remove-not-required (parser-main efile))))
    (when outfile
      (sc-file:write-sc-file outfile sc-program))
    (when delete-efile
      (delete-file efile)
      (loop for i from 1 to 7
          do (awhen (probe-file (change-extension efile (string+ "se" i)))
               (delete-file it))))
    (values sc-program outfile)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; (c2sc "csample/c_parser.c")

;;; 対話環境での利用向けpiece-c2sc 
;;; sc:: などの情報を除いてすっきり出力
(defun pcsc (x &rest try)
  (unless try (setq try *tried-function*))
  (immigrate-package (piece-c2sc x :tried-function try) *package*))
