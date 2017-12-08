(require 'lisp-mode)
(require 'font-lock)

(defconst sc-font-lock-keywords-1
  (eval-when-compile
    `((;;; struct/union/enum specifiers
       ,(concat
	 "(\\(struct\\|union\\|enum\\)"	 
	 "\\([ \t]+"        ; whitespace
	 "\\(\\sw+\\)\\)?") ; struct/union/enum ID
       (1 font-lock-keyword-face)
       (2 font-lock-type-face))
      (;;; deftype
       ,(concat
	 "(\\(deftype\\)" 
	 "\\([ \t]+"           ; whitespace
	 "\\(\\sw+\\)\\)?"     ; struct/union/enum ID
	 "\\([ \t]+"           ; whitespace
	 "\\(struct\\|union\\|enum\\)\\)?")
       (1 font-lock-keyword-face)
       (2 font-lock-type-face)
       (4 font-lock-keyword-face))
      (;;; Declarations
       ,(concat
	 "(\\(" (regexp-opt
		 '("def" "decl" "extern" "extern-def"
		   "extern-decl" "static" "static-def"
		   "auto" "auto-def" "register" "register-def"))
	 "\\)\\>"
	 "[ \t']*"        ; any whitespace
	 "\\((\\(\\sw+\\)\\|\\(\\sw+\\)\\)\\>?")    ; defined object
       (1 font-lock-keyword-face)
       (3 font-lock-function-name-face nil t)
       (4 font-lock-variable-name-face nil t))
      (;;; SC-1 declarations
       ,(concat
	 "(" (regexp-opt
	      '("defs" "extern-defs" "static-defs" "auto-defs" "register-defs")
	      t)
	 "\\>")
       (1 font-lock-keyword-face))
      (;;; Type qualifieers
       ,(concat
	 "\\<" (regexp-opt
		'("void" "char" "signed-char" "unsigned-char"
		  "short" "signed-short" "unsigned-short"
		  "int" "signed-int" "unsigned-int"
		  "long" "signed-long" "unsigned-long"
		  "long-long" "signed-long-long" "unsigned-long-long"
		  "float" "double" "long-double")
		t)
	 "\\>")
       (1 font-lock-type-face))
      ;;; SC preprocessor macro directives
      (,(concat
	 "(\\(%defmacro\\)"
	 "\\([ \t]+"              ; whitespace
	 "\\(\\sw+\\)\\>\\)?")    ; macro ID
       (1 font-lock-preprocessor-face)
       (2 font-lock-function-name-face))
      (,(concat
	 "(\\(%defconstant\\|%undef\\|%ifdef\\*?\\|%ifndef\\*?\\)"
	 "\\([ \t]+"              ; whitespace
	 "\\(\\sw+\\)\\>\\)?")    ; macro ID
       (1 font-lock-preprocessor-face)
       (2 font-lock-variable-name-face))
      (,(concat
	 "(" (regexp-opt
	      '("%include" "%cinclude" "%if" "%if*" "%line" "%error"
		"%pragma" "%splice" "%rule" "%sc2c" "%output-file"
		"%string")
	      t)
	 "\\>")
       (1 font-lock-preprocessor-face))
      ("\\<%else\\>"
       (1 font-lock-preprocessor-face))
      ))
  "Subdued level highlighting for SC modes.")

(defconst sc-font-lock-keywords-2
  (append sc-font-lock-keywords-1
   (eval-when-compile
     `(;; Statement tags
       (,(concat
	  "(" (regexp-opt
	       '("label" "case" "default" "begin" "if" "switch"
		 "do-while" "goto" "continue" "break" "return"
		 ;; SC1
		 "let" "while" "cond" "for" "loop") t)
	  "\\>")
	. 1)
       ;; Type operators, type-qualifiers
       (,(concat
	  "(" (regexp-opt
	       '("array" "struct" "ptr" "fn" "csym::fn") t)
	  "\\>")
	. 1)
       (,(concat
	  "\\<" (regexp-opt '("va-arg" ":bit"
			      "const" "restrict" "volatile") t)
	  "\\>")
	. 1)
       ;; Operators, designators
       (,(concat
	  "(" (regexp-opt
	       '("aref-this" "aref" "fref" "fref-this" "->"
		 "inc" "dec" "init" "sizeof" "++" "--" "ptr"
		 "mref" "+" "-" "bit-not" "not" "cast"
		 "*" "/" "%" "<<" ">>" "<" ">" "<=" ">=" "==" "!="
		 "bit-and" "bit-xor" "bit-or" "and" "or" "if-exp"
		 "=" "*=" "/=" "%=" "+=" "-=" "<<=" ">>="
		 "bit-and=" "bit-xor=" "bit-or=" "exps" "c-exp") t)
	  "\\>")
	  . 1)
       ("->" . 0)
       ;; Character constants, #+/#- macros
       ("#[\\\\\\+-]\\sw+\\>" 0 font-lock-builtin-face)
       ;; Constant values.
       ("\\<:\\sw+\\>" 0 font-lock-builtin-face)
      )))
  "Gaudy level highlighting for SC modes.")

(defvar sc-font-lock-keywords sc-font-lock-keywords-1
  "Default expressions to highlight in SC modes.")


(define-derived-mode sc-mode lisp-mode "SC"
  (setq font-lock-defaults
	(cons
	 (list 'sc-font-lock-keywords
	       'sc-font-lock-keywords-1 'sc-font-lock-keywords-2)
	 (cdr font-lock-defaults)))
  (let ((tag 'lisp-indent-function))
    ;; SC constructs
    (put 'def tag 2)
    (put 'decl tag 2)
    (put 'extern tag 2)
    (put 'extern-def tag 2)
    (put 'extern-decl tag 2)
    (put 'static tag 2)
    (put 'static-def tag 2)
    (put 'static-decl tag 2)
    (put 'auto tag 2)
    (put 'auto-def tag 2)
    (put 'register tag 2)
    (put 'register-def tag 2)
    (put 'begin tag 0)
    (put 'if tag 2)
    (put 'switch tag 1)
    (put 'do-while tag 1)
    (put 'let tag 1)   ; SC1
    (put 'for tag 1)   ; SC1
    (put 'while tag 1) ; SC1
    (put 'loop tag 0)  ; SC1
    (put 'cast tag 1)
    (put 'if-exp tag 2)
    ;; SC preprocessor directives
    (put '%cinclude tag 1)
    (put '%defmacro tag 2)
    (put '%if tag 2)
    (put '%ifdef tag 2)
    (put '%ifndef tag 2)
    (put '%if* tag 1)
    (put '%ifdef* tag 1)
    (put '%ifndef* tag 1)
    ))

(provide 'sc-mode)
