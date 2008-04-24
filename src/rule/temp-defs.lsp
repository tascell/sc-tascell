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

;;; Utilities for temp.rule

(defpackage "TEMP"
  (:export :with-temprule-environment
           :with-new-block :with-new-block-item
           :with-deeper-subexpression :subexpression-p
           :add-add-decl
           :add-precedent :flush-precedents :flush-precedents-decl
           :handle-block-tag)
  (:use "CL" "SC-MISC" "RULE")
  (:shadow cl:declaration))
(in-package "TEMP")

(defvar *current-block* nil)       ; (struct c-block), manages additional decls
(defvar *expression-depth* 0)
(defvar *precedents* nil)          ; list of <statement>

(defstruct c-block
  add-here-p                            ; nil: additonal decls are added to an outer block
  add-decls                             ; list of (<id> . <type>)
  parent
  )

(defmacro with-temprule-environment (&body body)
  `(let ((*current-block* nil)
         (*expression-depth* 0)
         (*precedents* nil))
     ,@body))

(defmacro with-new-block (add-here-p &body body)
  (with-fresh-variables (ret-var)
    `(let* ((*current-block* (make-c-block :add-here-p ,add-here-p
                                           :parent *current-block*))
            (,ret-var (progn ,@body)))
       (nconc (flush-add-decls)         ; tmpの定義列を返す or 外ブロックに回してnilを返す
              ,ret-var))))

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
  (if (c-block-add-here-p *current-block*)
      (prog1
          (loop for (id . type) in (nreverse (c-block-add-decls *current-block*))
              collect ~(def ,id ,type))
        (setf (c-block-add-decls *current-block*) nil))
    (progn
      ;; 
      (setf (c-block-add-decls (c-block-parent *current-block*))
        (nconc (c-block-add-decls (c-block-parent *current-block*))
               (c-block-add-decls *current-block*)))
      nil)))

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

;; (a b c 'temp::mk-block d e f ...) -> (a b c (begin d e f)) : recursively
(defun handle-block-tag (transformed-body)
  (handle-block-tag2 (splice transformed-body)))

(defun handle-block-tag2 (transformed-body)
  (multiple-value-bind (prev rest)
      (list-until transformed-body 'mk-block :key #'car)
    (if (endp rest) prev
      (nconc prev (list ~(begin ,@(handle-block-tag2 (cdr rest))))))))
