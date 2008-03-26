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

;;; Loader of .rule files.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(provide "SCT-FILE")
(eval-when (:compile-toplevel :load-toplevel :execute)
  (require "SC-DECL" "sc-decl.lsp"))

(in-package "SC-TRANSFORMER")

;;(scr:require "SCT-MACRO")
;;(scr:require "SCT-MACCH")
(scr:require "SCT-RULE2LSP")
(scr:require "SC-MISC")
(scr:require "SC-FILE")

;;; rule に対応する .rule.lsp （rule.o） のうち最新のものをload
;;; .rule が最新の場合はコンパイルしてからload
;;; force-load が t なら ruleファイルが最新でも読み直す．
;;; compile が t なら, .rule.lsp をコンパイルする
(defvar *force-recompile* nil)
(defvar *compile-rulelsp* t)

;;;;;
(defconstant *default-sct-user-package* (find-package "SC-TRANSFORMER-USER"))
(defconstant *init-function-name* "INITIATE") ; 規則の変換を開始する関数名
(defconstant *arg-name* "X")       ; パターン全体を参照するための変数名
(defconstant *rulepackage-varname* "*RULE-PACKAGE*")
(defconstant *parameters-varname* "*PARAMETERS*")
(defconstant *block-label* 'pattern-matched)
(defvar *initiator-alist* (list)) ; Rule-set で最初に呼び出す関数

;;;;; Rule-set ごとの設定
(defstruct (rule-environment
             (:constructor create-rule-environment (rule-name)))
  rule-name
  (source-package *default-sct-user-package*) ; 普通は in-package で指定
  (initiator :default)    ; rule-set を起動する関数
  (package-sensitive t)   ; symbol の比較を package sensitive に行う
  (otherwise-action '(error "No pattern matched.")))
					; パターンにマッチしなかったときの動作
(defvar *rule-environment* (create-rule-environment "DEFAULT"))

;;;;;


;; 与えられた規則名に対応するruleファイルをlispプログラムにコンパイル
;; 返り値は出力ファイル名
(defun rule2lsp (rule-name
		 &aux input-file output-file rule-list lisp-forms
		 (*rule-environment* (create-rule-environment rule-name)))
  ;; 入力/出力ファイル名
  (unless (setq input-file (get-rule-rulefile rule-name))
    (error "Can't find rule-file for ~S" rule-name))
  (setq output-file 
        (make-pathname 
         :name (pathname-name (get-rule-lspfile-name rule-name))
         :directory (pathname-directory (truename input-file))))
  
  ;; 規則ファイル読み込み
  (format *error-output* "Reading ~S...~%" input-file)
  (finish-output *error-output*)
  (multiple-value-setq (rule-list lisp-forms)
    (read-rule input-file))
  
  ;; 出力生成 => ret
  (format *error-output* "Compiling ~S...~%" input-file)
  (finish-output *error-output*)
  (let*
      ((rule-id-list (mapcar #'car rule-list))
       (ext-rule-id-list (mapcar #'make-ext-rule-id rule-id-list))
       (source-package (rule-environment-source-package *rule-environment*))
       (var-rulepackage (intern *rulepackage-varname* source-package))
       (var-parameters (intern *parameters-varname* source-package))
       (var-initiator (intern *init-function-name* source-package))
       (output
	`(;; provide, lisp-form, package
	  (provide ,rule-name)
	  ,@(mapcar 
	     #'(lambda (form)
		 `(eval-when (:compile-toplevel :load-toplevel :execute)
		   ,form))
	     lisp-forms)
	  (export ',ext-rule-id-list)
          (defconstant ,var-rulepackage *package*)
          (defvar ,var-parameters nil)
	  ;; function "initiate"
	  (defun ,var-initiator
	    (x &key
	       (package *package*)
	       ((:parameters ,var-parameters) ,var-parameters))
	    (,(make-ext-rule-id
               (rule-environment-initiator *rule-environment*))
              x :package package))
	  (pushnew (cons ,rule-name #',var-initiator)
		   sct::*initiator-alist* :test #'eq :key #'car) 
	  ;; lisp-compiled functions
	  ,@(mapcan #'rule2defun rule-list)
	  ;; rule called from external
	  ,@(mapcar
	     #'(lambda (rule-id ext-rule-id)
		 `(defun ,ext-rule-id (x &key ((:package src-package) 
                                               *package*)
				       &aux (*package* ,var-rulepackage))
                   (setq src-package (or src-package
                                      sc-file:*code-package*))
		   (immigrate-package
		    (,rule-id (immigrate-package x ,var-rulepackage
                                                 src-package))
		    src-package ,var-rulepackage)))
	     rule-id-list ext-rule-id-list)		
	  )))
    (let ((*print-case* :downcase))
      (write-file output-file output
		  :overwrite t :package source-package
		  :readtable sc-file:*code-readtable*)))
  output-file
  )

;;; 変形規則ごとの設定 *rule-environment* の設定
;;; rule-set の記述者が使う．
(defmacro set-rule-environment (&rest options)
  (when *rule-environment*
    (let ((opt (gensym))
          (initiator (gensym)))
      `(dolist (,opt ',options *rule-environment*)
      (ecase (car ,opt)
        ((:initiator)
         (let ((,initiator
                (etypecase (second ,opt)
                  (symbol (second ,opt))
                  (string (setq ,opt (intern (second ,opt)))) )))
           (setf (rule-environment-initiator *rule-environment*) 
                 ,initiator)))
        ((:package-sensitive)
	 (setf (rule-environment-package-sensitive *rule-environment*)
	       (second ,opt)))
        ((:otherwise-action)
         (setf (rule-environment-otherwise-action *rule-environment*)
               (if (cddr ,opt) 
                   `(progn ,@(cdr ,opt))
                   (second ,opt)))))))))

;;; *rule-package* の環境を実際のload時と同じにする
#+obsolete
(defun renew-rule-package-environment ()
  (let ((*package* *rule-package*))
    (mapcar #'(lambda (module)
		(apply #'require (mklist module)))
	    *requirement*)
    (mapcar #'use-package *using-package*)
    (funcall #'shadow *rule-shadowing-symbols*))
  (values))



;;; 外部からの呼び出し用の規則symbol
;;; rule -> @rule
(defun make-ext-rule-id (rule-id)
  (intern (string+ "@" (symbol-name rule-id))
          (symbol-package rule-id)))

;;;; 規則ファイル読み込み（コンパイル実行用）
;;; 返り値は (values rule-list lisp-forms)
;;; rule-list := list of (<fsymbol> . list of (<patterns> . <action>))
;;; lisp-forms := <(%lisp ...) で記述されたフォームのリスト
(defun read-rule (rule-file
		  &aux 
		  (rule-list '())  ; return value1
                  (lisp-forms '())) ; return value2
  (let ((*package* *default-sct-user-package*)
	(*readtable* *readtable*)
        (*load-pathname* (pathname rule-file))
        (*load-truename* (truename rule-file)))
    (with-open-file (istream rule-file :direction :input)
      ;; Evaluate NORMAL Lisp forms
      (unless (eq :lisp-begin (read istream))
	(error "Rule-files must begin with :lisp-begin ... :lisp-end"))
      (do ((form (read istream) (read istream)))
          ((eq :lisp-end form)
           (setq lisp-forms (nreverse lisp-forms)))
        (push form lisp-forms)
        (eval form))

      ;; *package* が defaultのままだったら
      ;; defpackage, in-package を自動的に作る
      (when (eq *package* *default-sct-user-package*)
	(let* ((rule-name (rule-environment-rule-name *rule-environment*))
	       (add-forms `((defpackage ,rule-name
			      (:use "SCT-USER"))
			    (in-package (find-package rule-name)))))
	  (dolist (form add-forms) (eval form))
          (dolist (form (reverse add-forms)) (push form lisp-forms))))
      
      (setf (rule-environment-source-package *rule-environment*)
            *package*)

      ;; 変形規則本体の読み込み
      (do ((y (read-pattern istream nil 'eof) 
	      (read-pattern istream nil 'eof))
	   (patlist '()))
	  ((eq y 'eof) 
	   (setq rule-list
		 (nreverse
		  (mapcar #'(lambda (x) (cons (car x) (nreverse (cdr x))))
			  rule-list))))
	(cond 
	  ;; (rule <pattern>)
	  ((listp y)
	   (unless (symbolp (car y))
	     (error "~S may not be a rule-function name." (car y)))
	   (push y patlist))
	  ;; ->
	  ((and (symbolp y)
		(string= "->" (symbol-name y)))
	   (setq y (read istream))	; y <== <action>
	   ;; -> より前のパターンを rule-list に追加
	   ;;(format t "~S~%" y)
	   (let ((fsymbol (caar patlist)))
	     (let ((pos	   ; (<patterns> . <action>) の挿入位置
		    (car 
		     (or (member fsymbol rule-list :key #'car :test #'eq)
			 (push (cons fsymbol nil) rule-list)))))
	       (push (cons (mapcar #'cdr patlist) y) (cdr pos))))
	   (setq patlist '()))
	  (t
	   (error "syntax-error: ~S" y)) ))))
  
  (when (eq :default (rule-environment-initiator *rule-environment*))
    (setf (rule-environment-initiator *rule-environment*) 
      (caar rule-list)))
  
  (values rule-list lisp-forms)
  )


;; rule = (rname . patterns-act-list)
(defun rule2defun (rule
		   &key (source-package
			 (rule-environment-source-package *rule-environment*))
		        (arg-var (intern *arg-name* source-package))
                   &aux (rname (car rule)) (pat-act-list (cdr rule)))
  (multiple-value-bind (defun-list start-form)
      (patacts2defun rname pat-act-list)
    (list*
     `(defun ,rname (,arg-var) ,start-form)
     defun-list)))

;; pat-act-list = list of <pat-act>
;; pat-act = ( ((pattern ,@rest-arg) ...) . action)
;; 1つの rulenameに対応するdefun式のリストを生成する．
;; 返り値：(values <defunのリスト> <最初のパターンマッチを起動するためのform>)
(defun patacts2defun (rulename pat-act-list
		      &key
		      (source-package
		       (rule-environment-source-package *rule-environment*))
		      (arg-var (intern *arg-name* source-package)))
  (let* ((patterns-list (mapcar #'(lambda (x) (mapcar #'car (car x))) pat-act-list))
	 (rest-arg-list (mapcar #'cdaar pat-act-list)) ;先頭のrestargを使う
	 (action-list (mapcar #'cdr pat-act-list))
	 (defun-name-list (mapcar (prefixed-func #'patlist-defun-symbol rulename)
				  patterns-list))
	 (fcall-list (mapcar #'(lambda (fname) `(,fname ,arg-var))
			     defun-name-list)))
    (values
     (mapcar
      #'(lambda (patterns rest-arg defun-name action else-action)
	  `(defun ,defun-name (,arg-var ,@rest-arg)
	     (block ,rulename
	       (sct-user::cond-match ,arg-var
		 ((,@patterns) ,action)
		 (otherwise ,else-action)))))
      patterns-list
      rest-arg-list
      defun-name-list
      action-list
      (append1 (cdr fcall-list)
	       (rule-environment-otherwise-action *rule-environment*)) )
     (car fcall-list))))

(defun patlist-defun-symbol (rulename pattern-list
			     &optional
			     (source-package
			      (rule-environment-source-package *rule-environment*)))
  (intern (string+ (symbol-name rulename) " "
		   (strcat (mapcar (suffixed-func #'pat-defun-name source-package)
				   pattern-list)
			   (format nil "~% OR ")))
	  source-package))

(defun pat-defun-name (pattern
		       &optional
		       (source-package
			(rule-environment-source-package *rule-environment*)))
  (let ((*package* source-package))
    (pattern-print pattern)))


(defun compile-rulelsp (rulename)
  (let ((rule-lspfile (get-rule-lspfile rulename)))
    (compile-file (get-rule-lspfile rulename)
		  :output-file
		  (namestring
		   (make-pathname 
		    :name (namestring (get-rule-ofile-name rulename))
		    :directory (pathname-directory rule-lspfile))))) )

(defun require-rule (rulename &key (force-recompile *force-recompile*) 
		                   (compile *compile-rulelsp*))
  (let ((rule-file (get-rule-rulefile rulename))
	(lisp-file (get-rule-lspfile rulename))
	(fasl-file (get-rule-ofile rulename)))
    (unless (or rule-file lisp-file fasl-file)
      (error "Can't find ~s rule" rulename))
    (let ((file-prio-list 
	   (sort (list rule-file lisp-file fasl-file) 
		 #'>
		 :key #'(lambda (file) (if file (file-write-date file) -1)))))
      (cond
        ((or force-recompile  ; .rule が最新 or force-recompile
	     (eq rule-file (first file-prio-list)))
         (let ((lisp-filename (rule2lsp rulename)))
           (load (if compile
		     (compile-rulelsp rulename) 
		     lisp-filename))))
        ((and compile         ; コンパイル要求 & .rule.lsp が最新
              (eq lisp-file (first file-prio-list)))
	 (load (compile-rulelsp rulename)))
        (t                    ; 最新のものを（未読なら）load
         (let ((load-filename (first file-prio-list)))
	   (require rulename load-filename)))))))

;;;; 規則名に対応する各ファイル[名]
;; 規則ファイル
(defun get-rule-rulefile-name (rulename)
  (change-extension (string-downcase (string rulename)) "rule"))
(defun get-rule-rulefile (rulename)
  (path-search (get-rule-rulefile-name rulename) *rule-path*))
;; lispコンパイル済規則ファイル
(defun get-rule-lspfile-name (rulename)
  (change-extension
   (string+ *cl-implementation* "-"
	    (string-downcase (string rulename)) 
            "-rule" )
   "lsp"))
(defun get-rule-lspfile (rulename)
  (probe-file
   (change-filename (get-rule-rulefile rulename)
		    (get-rule-lspfile-name rulename))))
;; faslコンパイル済規則ファイル
(defun get-rule-ofile-name (rulename)
  (compile-file-pathname 
   (change-extension (get-rule-lspfile-name rulename) nil)))
(defun get-rule-ofile (rulename)
  (probe-file
   (change-filename (get-rule-rulefile rulename)
		    (get-rule-ofile-name rulename))))
;; 規則呼出し用関数
(defun get-initiator (rulename)
  (require-rule rulename)
  (cdr (assoc rulename *initiator-alist* :test #'eq)))
