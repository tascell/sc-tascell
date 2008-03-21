;;;temporary variableを使って、関数呼び出しが部分式にならないようにする。
(defpackage "TEMP"
  (:export :with-temprule-environment
           :with-new-block :with-new-block-item
           :with-deeper-subexpression :subexpression-p
           :add-add-decl :flush-add-decls
           :add-precedent :flush-precedents :flush-precedents-decl
           :handle-block-tag)
  (:use "CL" "SC-MISC" "RULE")
  (:shadow cl:declaration))
(in-package "TEMP")

(defvar *current-block* nil)       ; (struct c-block), manages additional decls
(defvar *expression-depth* 0)
(defvar *precedents* nil)          ; list of <statement>

(defstruct c-block
  add-decls                             ; list of (<id> . <type>)
  )

(defmacro with-temprule-environment (&body body)
  `(let ((*current-block* nil)
         (*expression-depth* 0)
         (*precedents* nil))
     ,@body))

(defmacro with-new-block (&body body)
  `(let ((*current-block* (make-c-block)))
     ,@body))

(defmacro with-new-block-item (initial-subexpression-depth &body body)
  `(let ((*expression-depth* ,initial-subexpression-depth)
         (*precedents* '()))
     ,@body))

(defmacro with-deeper-subexpression (&body body)
  `(let ((*expression-depth* (1+ *expression-depth*)))
     ,@body))

(defun subexpression-p ()
  (> *expression-depth* 1))

(defun add-add-decl (id type)
  (push (cons id type) (c-block-add-decls *current-block*)))

(defun flush-add-decls ()
  (prog1 
      (loop for (id . type) in (nreverse (c-block-add-decls *current-block*))
	  collect ~(def ,id ,type))
    (setf (c-block-add-decls *current-block*) nil)))

(defun add-precedent (stat)
  (push stat *precedents*))

(defun flush-precedents ()
  (prog1 (nreverse *precedents*)
    (setq *precedents* nil)))

(defun flush-precedents-decl (decl-exp orig-exp)
  (let ((precds (flush-precedents)))
    (if precds
        (progn
          (unless *current-block*
            (iwarn "Non-constant value is used as initializer of top-level declaration!~%~S"
                   orig-exp))
          (nconc (list 'rule::splice)
                 precds
                 (list 'mk-block        ; see function handle-block-tag
                       decl-exp)) )
      decl-exp)))

;; (a b c 'temp::block d e f ...) -> (a b c (begin d e f)) : recursively
(defun handle-block-tag (transformed-body)
  (handle-block-tag2 (splice transformed-body)))

(defun handle-block-tag2 (transformed-body)
  (multiple-value-bind (prev rest)
      (list-until transformed-body 'mk-block :key #'car)
    (if (endp rest) prev
      (nconc prev (list ~(begin ,@(handle-block-tag2 (cdr rest))))))))
