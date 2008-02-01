;;; SC Preprocessor
(provide 'scpp)

(unless (find-package "SCPP")
  (make-package "SCPP" :use '("LISP")))
(in-package "SCPP")

(export '(scpp-initialize scpp))

(require "SC-MAIN")
(require "SC-MISC")
(require "C2SC")

(use-package "SC-MISC")

;;;;; #include
;;; (%include <filename>)
;;; (%cinclude <filename>)
;;;;; #define
;;; (%defmacro <lambda-list> <form>*)
;;; (%defconstant <symbol> <form>)
;;;;; #undef
;;; (%undef <symbol>)
;;;;; #ifdef, #ifndef ,#if, #elif, #endif
;;; (%ifdef <symbol>)
;;; (%ifndef <symbol>)
;;; (%if <constant>)
;;; (%elif <constant>)
;;; (%endif)
;;;;; #line, #error
;;; (%line <digit> {<string>})
;;; (%error <string> {form}*)
;;;;; #pragma 
;;; (c-exp "#pragma ...") 

(defconstant *scpp-macro-package* (find-package2 "SCPP-MACRO"))
(defconstant *scpp-package* (find-package "SCPP"))

(defvar *clibdir-list* '(("/usr/include/")))

(defvar *macro-list* nil) ; macro-entry list

(defstruct macro-entry
  id
  type  ;; one of {constant macro}
  )

(defmacro in-macro-package (&body forms)
  `(let ((*package* *scpp-macro-package*))
    ,@forms))


(defun scpp-initialize ()  
  (setq *macro-list* nil))

;; x : s-exp or filename
(defun scpp (x)
  (when (stringp x)
    (setq x (read-file x :package sc-main:*code-package*)))  
  (scpp-list x)
  )

(defvar *scpp-abort* nil)
(defvar *scpp-section* nil) ; one of {nil, then, else}

(defun scpp-list (x)
  (reduce #'append (mapcar #'scpp-1exp x) :from-end t))

(defun scpp-1exp (x &aux fst)
  (cond
    ((and (listp x)
	  (symbolp (car x)))
     (case (setq fst (intern (symbol-name (car x)) *scpp-package*))
       ((%include)
	(scpp-list (read-file (scpp-macroexpand (second x))
			      :package sc-main:*code-package*)))
       ((%cinclude)
        (scpp-list (c2sc:c2sc 
                    (path-search
                     (scpp-macroexpand (second x)) 
                     *clibdir-list* nil t)
                    :outfile nil
                    :delete-efile t
                    :cpp-option "")))
       ((%defmacro)
	(entry-macro (cons fst (cdr x)))
	nil)
       ((%defconstant)
	(entry-macro `(,fst ,(second x)
		       ',(scpp-macroexpand (third x))))
	nil)
       ((%undef)
	(delete-macro (second x))
	nil)
       ((%ifdef %ifndef %if)
	(if (let ((macsymbol (immigrate-package (second x) 
						*scpp-macro-package*))) 
	      (case fst
		((%ifdef)  (macro-find macsymbol))
		((%ifndef) (not (macro-find macsymbol)))
		((%if) (scpp-macroexpand macsymbol))))
	    (scpp-list (third x))
	    (scpp-list (fourth x))))
       ((%line)
	(list
	 `(sc::c-exp ,(format nil "#line ~D ~a"
                              (scpp-macroexpand (second x))
                              (if (third x)
                                  (scpp-macroexpand (third x))
                                  "")))))
       ((%error)
	(apply #'format *error-output* (cdr x))
	(format *error-output* "~%") 
	nil)
       (otherwise
	(list (scpp-macroexpand x)))))
    (t 
     (list (scpp-macroexpand x)))))

;;; マクロ処理
(defun macro-find (macsymbol)
  (find macsymbol *macro-list* :test #'eq :key #'macro-entry-id))

(defun entry-macro (dm &aux macsymbol entry-type)
  (in-macro-package
    (let ((macname (symbol-name (second dm))))
      (shadow (intern macname))
      (setq macsymbol (intern macname))))
  (case (car dm)
    ((%defmacro)
     (eval `(defmacro ,macsymbol ,@(cddr dm)))
     (setq entry-type 'macro))
    ((%defconstant)
     (eval `(defconstant ,macsymbol ,(third dm)))
     (setq entry-type 'constant))
    (otherwise
     (error "~S is not a defmacro/defconstant directive." dm)))
  (push
   (make-macro-entry :id macsymbol :type entry-type)
   *macro-list*))

(defun delete-macro (macsymbol)
  (setq *macro-list*
	(remove macsymbol *macro-list* :test #'eq :key #'macro-entry-id)))

(defun scpp-macroexpand (x &aux macsymbol)
  (cond
    (;; 定数
     (symbolp x)
     (setq macsymbol (intern (symbol-name x) *scpp-macro-package*))
     (if (aand (macro-find macsymbol)
	       (eq 'constant (macro-entry-type it)))
	 (scpp-macroexpand (symbol-value macsymbol))
	 x))
    ((listp x)
     (if (aand (symbolp (car x))
	       (setq macsymbol 
		     (intern (symbol-name (car x)) *scpp-macro-package*))
	       (macro-find macsymbol)
	       (eq 'macro (macro-entry-type it)))
	 ;; マクロ
	 (scpp-macroexpand (macroexpand-1 (cons macsymbol (cdr x))))
	 ;; その他リスト
	 (if x
	     (mapcar #'scpp-macroexpand x)
	     '() )))
    (t x)))
