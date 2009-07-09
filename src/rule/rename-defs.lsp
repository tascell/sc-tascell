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

;;; Utilities for rename.rule

(defpackage "RENAME"
  (:nicknames "REN")
  (:export :with-rename-environment :repl-id :bind-id
	   :inc-block-level :begin-function)
  (:shadow cl:declaration)
  (:use "RULE" "CL" "SC-MISC"))
(provide "RENAME-DEFS")

(in-package "RENAME")
  
(defvar *id-alist* nil)      ; list of (<id> . <block-level>)
(defvar *replace-alist* nil) ; list of (<oldid> . <newid>)
(defvar *block-id* 0)        ; 現在のブロックID
(defvar *next-block-id* 0)   ; 次に使用するブロックID-1

(defmacro with-rename-environment (&body body)
  `(let ((*id-alist* *id-alist*)
         (*replace-alist* *replace-alist*)
         (*block-id* 0)
         (*next-block-id* 0))	      
     ,@body))
       
; get *block-id* of id
(defun assoc-id-level (id)
  (cdr (assoc id *id-alist* :test #'eq)))

; get replacement-id of old-id
(defun assoc-replacement-id (id)
  (or (cdr (assoc id *replace-alist* :test #'eq))
      id))

; entry identifier
; 返り値は更新先の (values *id-alist* *replace-alist*) 
(defun entry-identifier (id)
  (let ((lev (assoc-id-level id)))
    (if lev 
	(if (= *block-id* lev)     
	    ;同一スコープで宣言済 ->そのまま
	    (values *id-alist* *replace-alist*)
	    ;別のスコープで宣言済
	    (let* ((new-id (generate-id (identifier0 id :sc2c))))
	      (values
	       (cons (cons id *block-id*) *id-alist*)
	       (cons (cons id new-id) *replace-alist*))))
      ; 未宣言
      (values 
       (cons (cons id *block-id*) *id-alist*)
       (cons (cons id id) *replace-alist*)))))

; replace all identifier in s-expression
(defun repl-id (x)
  (cond
   ((atom x)
    (assoc-replacement-id x))
   ((tagged-p ~fref x)
    ~(fref ,(repl-id (second x)) ,@(cddr x)))
   (t
    (cons (repl-id (car x)) (repl-id (cdr x))))))

; entry id (list) and bind *id-alist* *replace-alist* 
; Returns replacement id (list)
(defun bind-id (id)
  (if (listp id)
      (mapcar #'bind-id id)
    (progn
      (multiple-value-setq
	  (*id-alist* *replace-alist*) (entry-identifier id))
      (assoc-replacement-id id))))

; increment block-level
(defmacro inc-block-level (&body form)
  `(let ((*replace-alist* *replace-alist*)
	 (*block-id* (incf *next-block-id*)))    
    ,@form))

; begin function
(defmacro begin-function (&body form)
  `(let ((*id-alist* *id-alist*)
	 (*replace-alist* *replace-alist*)
	 (*block-id* (incf *next-block-id*)))
    ,@form))
