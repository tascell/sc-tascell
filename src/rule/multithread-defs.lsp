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

;;; Utilities for multithread.rule

(defpackage "MULTITHREAD"
  (:nicknames "MT")
  (:export :*current-func* 
	   :make-finfo :finfo-id :finfo-rettype :finfo-decl :finfo-label :finfo-nfunc-id
	   :make-nestfunc-id :separate-decl :make-nestfunc)
  (:shadow cl:declaration)
  (:use "RULE" "SC-MISC" "CL"))

(provide "MULTITHREAD-DEFS")
(in-package "MULTITHREAD")

(defvar *current-func* nil)

;(<ラベル 変数><入れ子関数名><ローカル変数><スレッドの判断(生成か継続か)>)
(defstruct finfo
  ;; 関数名? <- 違った。たぶん李さんの勘違い
  ;; 関数名として使うことにする
  id
  ;; 返り値の型
  rettype 
  ;; 関数の先頭に追加する宣言のリスト
  decl
  ;; 計算がどこまで終わったかを示すラベル名のリスト
  ;; list of ( <label-id> <retval-id> )
  ;; 出現順にpush
  label
  ;; 入れ子関数の名前
  nfunc-id
  ;; 次につけるlnの値?
  ;; ln)
  )

; make list of "case <num>: goto L<num>" in initial switch statement
; for rsn_cont of nested-function
(defun switch-cont (label-list)
  ~(switch
    ln
    ,@(let ((i 0))
	(mapcan #'(lambda (x)
		    (list ~(case ,(incf i)) ~(goto ,(car x))))
		label-list))))

; make list of "case <num>: return t<num>" in initial switch statement 
; for rsn_retval of nested-function
(defun switch-retval (label-list)
  ~(switch
    ln
    ,@(let ((i 0))
	(mapcan #'(lambda (x)
		    (incf i)
		    (when (second x)
		      (list
		       ~(case ,i)
		       ~(return (cast (ptr void) (ptr ,(second x)))))))
		label-list))))

; make identifier of nested-function from identifier of parent-func
(defun make-nestfunc-id (f-id)  
  (generate-id (string+ (identifier f-id) "_c")))

; separate declarations from the others in block-item-list
(defun separate-decl (bil &aux dcl-list oth-list)
  (dolist (bi bil (list (reverse dcl-list) (reverse oth-list)))
    (if (and (listp bi)
	     (or (storage-class-specifier (first bi))
		 (eq ~deftype (first bi)))
	     (identifier (second bi)))
	(push bi dcl-list)
      (push bi oth-list))))

;; 入れ子関数の定義を生成
(defun make-nestfunc (nf-body)
  #|
  (setq label-l (get-id-from-string (string+ "@L" (write-to-string ln))))
  (setq returninfos ~((label ,label-l nil)
  (= (fref cp -> c) c_p)
  (= (fref cp -> stat) thr_runnable)
		      (return)))
  |#
  (let ((nfunc-id (finfo-nfunc-id *current-func*))
	(label-list (reverse (finfo-label *current-func*))))
    ~(def (,nfunc-id cp rsn) (NESTFUNC-TAG (ptr void) thst_ptr reason)
	  (switch rsn 
		  (case rsn_cont)
		  ,(switch-cont label-list)
		  (return)
		  (case rsn_retval)
		  ,(switch-retval label-list)
		  (return))
	  (return)
	  
	  ;;<入れ子関数の文>部分
	  ,@nf-body
	  ;;関数の最後にreturnがない場合もちゃんと呼び出し元のスレッドを
	  ;;runnableにしてからスケジューラにreturn
	  (= (fref cp -> c) c_p)
	  (= (fref cp -> stat) thr_runnable)
	  (return))
	  #|
	  ,@(cond 
	     ((eq `thr_c flag)
	      `((return)))
	     ((eq `thr_s flag)
	      returninfos)
	     (t
	      returninfo))
          |#
    ))
