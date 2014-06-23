(load-library "sc-mode")

(defface tcell-keyword-face
  '((t :inherit font-lock-keyword-face :underline t))
  "Face for Tascell keywords."
  :group 'tcell-mode-faces)
(defvar tcell-keyword-face 'tcell-keyword-face)
(defface tcell-function-name-face
  '((t :inherit font-lock-function-name-face :underline t))
  "Face for Tascell method names."
  :group 'tcell-mode-faces)
(defvar tcell-function-name-face 'tcell-function-name-face)
(defface tcell-variable-name-face
  '((t :inherit font-lock-variable-name-face :underline t))
  "Face for Tascell pseudo variable names."
  :group 'tcell-mode-faces)
(defvar tcell-variable-name-face 'tcell-variable-name-face)
(defface tcell-type-face
  '((t :inherit font-lock-type-face :underline t))
  "Face for Tascell types."
  :group 'tcell-mode-faces)
(defvar tcell-type-face 'tcell-type-face)


(defconst *tcell-font-lock-keywords-1
  (eval-when-compile
    `((;;; task specifiers
       ,(concat
	 "(\\(task\\>\\)"
	 "\\([ \t]+"        ; whitespace
	 "\\(\\sw+\\)\\>\\)?") ; task ID
       (1 tcell-keyword-face t t)
       (2 font-lock-type-face))
      (;;; definition of task methods
       ,(concat
	 "(\\(def\\)\\>"
	 "[ \t]*"              ; any whitespace
	 "(" (regexp-opt
	      '("task-body" "task-receiver" "task-sender"
		"rslt-receiver" "rslt-sender") t)
	 "\\([ \t]+"           ; whitespace
	 "\\(\\sw+\\)\\)?")    ; task ID
       (1 font-lock-keyword-face)
       (2 tcell-function-name-face t t)
       (6 font-lock-type-face t t))
      (;;; definition of worker local storage
       ,(concat
	 "(\\(def\\)\\>"
	 "[ \t]*"              ; any whitespace
	 "\\(worker-data\\)")
       (1 font-lock-keyword-face)
       (2 tcell-type-face t t))
      (;;; definition of an initializer of worker local storage
       ,(concat
	 "(\\(def\\)\\>"
	 "[ \t]*"              ; any whitespace
	 "\\(worker-init\\)")
       (1 font-lock-keyword-face)
       (2 tcell-function-name-face t t))
      ))
  )

(defconst tcell-font-lock-keywords-1
  (append sc-font-lock-keywords-1 *tcell-font-lock-keywords-1)
  "Subdued level highlighting for Tascell mode.")

(defconst *tcell-font-lock-keywords-2
  (append *tcell-font-lock-keywords-1
	  (eval-when-compile
	    `(;; Worker function tag
	      (,(concat
		 "(" (regexp-opt
		      '("wfn") t)
		 "\\>")
	       (1 tcell-keyword-face))
	      ;; Tascell constructs
	      (,(concat
		 "(" (regexp-opt
		      '("do-two" "do-many*" ":guard" ":spawn-from" ":put" ":get"
			"tcell-broadcast" "dynamic-wind"
			":before" ":body" ":after"
			"throw" "catch") t)
		 "\\>")
	       (1 tcell-keyword-face t t))
	      (,(concat
		 "(\\(" (regexp-opt '("do-many") t)
		 "\\([ \t]+"           ; whitespace
		 "\\(for\\)\\>\\)?\\)")
	       (2 tcell-keyword-face t t)
	       (4 tcell-keyword-face))
	      (,(concat
		 "(\\(handles\\)\\>"
		 "[ \t]*"              ; any whitespace
		 "\\(\\<\\sw+\\>\\)")
	       (1 tcell-keyword-face)
	       (2 font-lock-type-face))
	      (,(concat
		 "\\<" (regexp-opt
			'(":in" ":out") t)
		 "\\>")
	       (1 tcell-keyword-face t t))
	      (,(concat
		 "\\<" (regexp-opt
			'("from" "to") t)
		 "\\>")
	       (1 tcell-keyword-face))
	      ;; pseudo variables
	      (,(concat
		 "\\<" (regexp-opt
			'("PROBABILITY" "WDATA" "WORKER-ID" "this") t)
		 "\\(\\.\\sw+\\)?\\>")
	       (1 tcell-variable-name-face))
	      ))))

(defconst tcell-font-lock-keywords-2
  (append sc-font-lock-keywords-2 *tcell-font-lock-keywords-2)
  "Gaudy level highlighting for Tascell mode.")

(defvar tcell-font-lock-keywords tcell-font-lock-keywords-1
  "Default expressions to highlight in Tascell mode.")

(define-derived-mode tcell-mode sc-mode "Tascell"
  (setf (car font-lock-defaults)
	(list 'tcell-font-lock-keywords
	      'tcell-font-lock-keywords-1 'tcell-font-lock-keywords-2))
  (let ((tag 'lisp-indent-function))
    ;; Tascell constructs
    (put 'do-two tag 2)
    (put 'do-many tag 6)
    (put 'do-many* tag 1)
    (put 'handles tag 1)
    (put 'dynamic-wind tag 0)
    (put 'tcell-broadcast tag 1)
    (put :guard tag 0)
    (put :get tag 0)
    (put :put tag 0)
    (put :before tag 0)
    (put :body tag 0)
    (put :after tag 0)
    (put catch tag 1)
    ))

(provide 'tcell-mode)
