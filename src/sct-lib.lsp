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

;;; Utility functions for writing .rule files
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(provide "SCT-LIB")
(eval-when (:compile-toplevel :load-toplevel :execute)
  (require "SC-DECL" "sc-decl.lsp"))

(in-package "SC-TRANSFORMER-USER")

(scr:require "SC-MISC")
(scr:require "SC-FILE")

;;;;;
(defconstant *keyword-package* (find-package "KEYWORD"))

(defvar *type-size-alist*
  ~((char . 8) (signed-char . 8) (unsigned-char . 8)
    (short . 16) (signed-short .16) (unsigned-short . 16)
    (int . 16) (signed-int . 16) (unsigned-int . 16)
    (long . 32) (signed-long . 32) (unsigned-long . 32)
    (long-long . 32) (signed-long-long . 32)
    (unsigned-long-long . 32)
    (float . 32) (double . 64) (long-double . 64)))
(defvar *type-rank-alist*
  ~((char . 0) (signed-char . 0) (unsigned-char . 0)
    (short . 1) (signed-short . 1) (unsigned-short . 1)
    (int . 2) (signed-int . 2) (unsigned-int . 2)
    (long . 3) (signed-long . 3) (unsigned-long . 3)
    (long-long . 4) (signed-long-long . 4)
    (unsigned-long-long . 4)))
(defvar *signed-to-unsigned-alist*
  ~((char . unsigned-char) (signed-char . unsigned-char)
    (short . unsigned-short) (signed-short . unsigned-short)
    (int . unsigned-int) (signed-int . unsigned-int)
    (long . unsigned-long) (signed-long . unsigned-long)
    (long-long . unsigned-long-long)
    (signed-long-long . unsigned-long-long)))
(defvar *float-type-list*
  ~(long-double double float))

;;; C ソースコード中の識別子か？
(defun cid-p (id)
  (and (symbolp id)
       (eq c2sc:*clang-package* (symbol-package id)) ))

;;; Cの識別子を表す文字列->scの識別子のsymbol-name
#-readtable-case
(defun cid-to-scidname (cid)
  (let ((symstr "") (mode *print-case*))
    (dotimes (n (length cid) symstr)
      (let ((idn (aref cid n)))
	(cond
	  ((char= #\_ idn)
	   (add-string symstr "-"))
	  ((and (eq ':downcase mode)
		(upper-case-p idn))
	   (setq mode :upcase)
	   (add-string symstr (string+ "@" idn)))
	  ((and (eq ':upcase mode)
		(lower-case-p idn))
	   (setq mode :downcase)
	   (add-string symstr (string+ "@" idn)))
	  (t (add-string symstr (string idn))))))))
#+readtable-case
(defun cid-to-scidname (cid)
  (substitute
   #\- #\_
   (funcall (if (case-mixed-p cid)
		#'identity
		#'string-invertcase)
	    cid)))


;;;; 識別子生成
;: with-setup-generate-id は最初に sc2c (in "SC-MAIN") でやる．
;; 最初の既使用識別子登録はscppでやる．

(defvar *used-identifier* :undefined)

;; generate-idで使ってはいけない名前を登録する．
(defmacro with-setup-generate-id (&body body)
  `(let ((*used-identifier* (make-hash-table :test #'equal)))
     ,@body))

;; 文字列（使用したCの識別子）を *used-identifier* に登録する．
(defun entry-used-identifier (cid)
  (assert (stringp cid))
  (assert (hash-table-p *used-identifier*))
  (setf (gethash cid *used-identifier*) cid)
  *used-identifier*)

;; 重複を気にせずscのidentifierを生成．登録はする．
(defun make-id (&optional (basename "SCID") (package sc-file:*code-package*))
  (when (hash-table-p *used-identifier*)
    (entry-used-identifier basename))
  (intern (cid-to-scidname basename) package))

;; 重複しないscのidentifierを生成
(defun generate-id (&optional (basename "SCID") (package sc-file:*code-package*))
  (if (hash-table-p *used-identifier*)
      ;; *used-identifier* とかぶらないように
      (let ((gen-cid
	     (if (gethash basename *used-identifier*)
		 (loop for i from 2
		    as cand = (string+ basename (write-to-string i))
		    while (gethash cand *used-identifier*)
		    finally (return cand))
		 basename)))
	(make-id gen-cid package))
      ;; *used-identifier* 非使用 ＋ 警告
      (progn
	(warn "Generate-id should be used in with-setup-generate-id environment.")
	(gentemp (cid-to-scidname basename) package))))

;;;; 型関連
;;; !!!拡張性のためにはdefruleに書きかえるべき

;;; remove type-qualifier from type-expression
(defun remove-type-qualifier (x)
  (when (not (listp x)) (return-from remove-type-qualifier x))
  (setq x (remove-if #'type-qualifier? x))
  (when (endp (cdr x))
    (return-from remove-type-qualifier 
      (remove-type-qualifier (car x))))
  (mapcar #'remove-type-qualifier x))

;;; whether unsigned-type
(defun unsigned-p (texp)
  (case texp
    ((sc::unsigned-char sc::unsigned-short sc::unsigned-int sc::unsigned-long
			sc::unsigned-long-long)
     t)
    (otherwise nil)))


;; return signed, unsigned ,enum ,float or other
(defun classify-type (texp)
  (cond 
    ((member texp (mapcar #'car *signed-to-unsigned-alist*) :test #'eq)
     'signed)
    ((member texp (mapcar #'cdr *signed-to-unsigned-alist*) :test #'eq)
     'unsigned)
    ((and (consp texp) (eq ~enum (car texp)))
     'enum)
    ((member texp *float-type-list*)
     'float)
    ((eq ~void texp)
     'void)
    (t 'other)))

;;; 一番外側の array を ptr に変換
(defun array2ptr (texp)
  (if (and (consp texp)
           (eq ~array (car texp)))
      `(sc::ptr ,(second texp))
      texp))

;;; implicit type conversion
;;; lightweight入れ子関数に対応(2003/12/28)
(defun type-conversion (texp1 texp2)
  (let* ((class1 (classify-type texp1))
	 (class2 (classify-type texp2)))
    (cond
      ;; 同じ型のとき
      ((equal texp1 texp2)
       texp1)
      ;; どちらかが voidのとき
      ((eq 'void  class1) texp1)
      ((eq 'void  class2) texp2)
      ;; どちらかが算術型でないとき
      ((eq 'other class1) (array2ptr texp1))
      ((eq 'other class2) (array2ptr texp2))
      ;; どちらかが浮動小数点型のとき
      ((eq 'float class1) texp1)
      ((eq 'float class2) texp2)
      ;; 両方整数型
      (t
       ;; enum=>int
       (when (eq 'enum class1)
	 (setq texp1 ~int class1 'signed))
       (when (eq 'enum class2)
	 (setq texp2 ~int class2 'signed))
       ;; (少なくとも一方がunsignedの場合、texp1がunsignedになるように入れ替え)
       (unless (eq 'unsigned class1)
	 (swap texp1 texp2)
	 (swap texp1 texp2)
	 (swap class1 class2))
       (cond
	 ;; どちらもsigned, どちらもunsigned
	 ((eq (eq 'unsigned class1) (eq 'unsigned class2))
	  (if (> (cdr (assoc texp1 *type-rank-alist*))
		 (cdr (assoc texp2 *type-rank-alist*)))
	      texp1 texp2))
	 ;; unsigned側のランクが低くない場合
	 ((>= (cdr (assoc texp1 *type-rank-alist*))
	      (cdr (assoc texp2 *type-rank-alist*)))
	  texp1)
	 ;; signed側の型がもう一方の型の値を全て表現できる場合
	 ((> (cdr (assoc texp2 *type-size-alist*))
	     (cdr (assoc texp1 *type-size-alist*)))
	  texp2)
	 ;; それ以外(signed側の型のunsignedに変換)
	 (t
	  (cdr (assoc texp2 *signed-to-unsigned-alist*)) ))))))

;;; (a b (rule::splice c d) e f) -> (a b c d e f)
(defun splice (x)
  (car (splice2 x)))

(defun splice2 (x)
  (cond
   ((atom x)
    (list x))
   ((eq 'rule::splice (car x))
    (mapcan #'splice2 (cdr x)))
   (t
    (list (mapcan #'splice2 x)))))
