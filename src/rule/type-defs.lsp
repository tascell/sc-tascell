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

;;; Utiliteis for type-info.rule

(provide "TYPE-DEFS")

(defpackage "TYPE"
  (:nicknames "TYPE-INFO")
  (:export :with-typerule-environment :with-function
           :with-new-environment :with-field-information
           :add-variable :assume-variable :add-typename :assume-typename
           :unnamed-id :get-struct-member-info :add-struct
           :leave-typename
           :assoc-vartype :assoc-struct :expand-typedef-name
           :$type
           :handle-exp-args :handle-exp-1arg
           :add-flow-printed :already-printed)
  (:use "CL" "SC-MISC"))

(in-package "TYPE")

;;; *str-alist* : list of ( <strname> . ( [(<field-id> . <type>)]* ) )
;;; *var-alist* : list of ( <name> . <type> )
;;; deftype -> ( name . ($type <type>) ) in *var-alist*
;;; enum and struct without name -> $<typedef-name> 
;;; (struct $str) (enum $en) etc.
(defvar *str-alist* (list))
(defvar *var-alist* (list))
(defvar *interim-str-alist* (list (cons '$dummy nil))) ; 未定義時の仮定義（未使用）
(defvar *interim-var-alist* (list (cons '$dummy nil))) ; 未定義時の仮定義

(defvar *flow-printed* '())

;;;;;;;;;;;;;;;;;;;;
;;; 環境設定

(defmacro with-typerule-environment (&body body)
  `(let ((*interim-str-alist* (list (cons '$dummy nil)))
         (*interim-var-alist* (list (cons '$dummy nil))))
     (let ((*str-alist* *interim-str-alist*)
           (*var-alist* *interim-var-alist*))
       ,@body)))

(defmacro with-function (id &body body)
  `(error-indent (format nil "In function ~S:" ,id)
                 (let ((*flow-printed* '())) ; print-flow用
                   ,@body)))

(defmacro with-new-environment (&body body)
  `(let ((*str-alist* *str-alist*)
         (*var-alist* *var-alist*))
     ,@body))

(defmacro with-field-information (struct-id &body body)
  (with-fresh-variables (asc-str)
    `(let ((,asc-str (assoc-struct ,struct-id)))
       (error-indent (format nil "Referencing field of ~S." ,struct-id)
                     (let ((*var-alist* (append ,asc-str (leave-typename *var-alist*)))
                           (*interim-var-alist* ,asc-str))
                       ,@body))) ))

;;;;;;;;;;;;;;;;;;;;
;;; 変数登録

;;; Add variable declaration
(defun add-variable (id type &optional (remove-type-qualifier t))
  (push (cons id (funcall (if remove-type-qualifier
                              #'rule:remove-type-qualifier #'identity)
                          type))
        *var-alist*))

;;; Add variable declaration ad interim
(defun assume-variable (id type &optional (remove-type-qualifier t))
  (push (cons id (funcall (if remove-type-qualifier
                              #'rule:remove-type-qualifier #'identity)
                          type))
        (cdr *interim-var-alist*)))

;;; Add typename definition
(defun add-typename (id type &optional (remove-type-qualifier t))
  (add-variable id (list '$type type) remove-type-qualifier))

;;; Add variable declaration ad interim
(defun assume-typename (id type &optional (remove-type-qualifier t))
  (assume-variable id (list '$type type) remove-type-qualifier))

;;; Get struct member definition
;;; メンバ定義のalistとstruct-declaration による変形結果を返す
(defun get-struct-member-info (sdeclist)
  (let* ((orig-var-alist *var-alist*)
         (*var-alist* *var-alist*)
         (sdecl-with-type (mapcar #'rule:struct-declaration! sdeclist))
                                        ; ここで*var-alist*に登録される
         (member-alist (ldiff *var-alist* orig-var-alist)))
    (values member-alist sdecl-with-type)))

;;; for unnamed struct/union/enum id
(defun unnamed-id (base-id)
  (symbol+ '$ base-id))

;;; Add struct
(defun add-struct (id member-alist)
  (push (cons id member-alist) *str-alist*))

;;; Add struct ad interim
(defun assume-struct (id member-alist)
  (push (cons id member-alist) (cdr *interim-str-alist*)))

;;;;;;;;;;;;;;;;;;;;
;;; 変数リストのフィルタリング

;;; leave only type-entry from *var-alist*
(defun leave-typename (&optional (alist *var-alist*))
  (remove-if-not
   #'(lambda (x) (and (consp (cdr x))
                      (eq '$type (cadr x))))
   alist))

;;;;;;;;;;;;;;;;;;;;
;;; 変数検索

;;; get type of variable
;;; expect: idは変数，型名のどちらであるはずか（期待と一致しなければerror）
(defun assoc-vartype (id &optional (expect nil))
  ;; (format t "(assoc-vartype ~s)~%" id)
  (acond
   ((assoc id *var-alist* :test #'eq)
    (let ((datum (cdr it)))
      (if (and (consp datum) (eq '$type (car datum)))
          (progn                        ; typename
            (when (and expect (not (eq :typename expect)))
              (ierror "~S is declared as typename." id))
            (values (cadr datum) :typename))
        (progn                          ; variable
          (when (and expect (not (eq :variable expect)))
            (ierror "~S is declared as variable." id))
          (values datum :variable)))))
   ;; 見つからなかった場合
   ((eq :typename expect)
    (iwarn "The type ~S is undefined." id)
    (assume-typename id id)
    (values id :typename))
   (t
    (iwarn "The variable ~S is undeclared.~%~
            ~AAssume its type as '~S'."
           id *error-indent-spaces* 'undefined)
    (assume-variable id 'undefined)
    (values 'undefined :variable))
   ))

;;; get struct member list
(defun assoc-struct (id)
  (aif (assoc id *str-alist* :test #'eq)
       (cdr it)
       (let ((interim-member (list (cons '$dummy nil))))
         (iwarn "The struct-or-union ~S is undefined." id)
         (assume-struct id interim-member)
         interim-member)))

;;; Expand typename
;; (defun expand-typedef-name (texp)
;;   ;; (format t "(expand-typedef-name ~s)~%" texp)
;;   (if (listp texp)
;;       (if (or (rule:struct-or-union-specifier? texp)
;;               (rule:enum-specifier? texp))
;;           texp
;;         (mapcar #'expand-typedef-name texp))
;;     (if (or (not (rule:identifier? texp))
;;             (eq 'undefined texp))
;;         texp
;;       (let ((asc (assoc-vartype texp :typename)))
;;         (if (eq texp asc)
;;             asc
;;           (expand-typedef-name asc))))))

;;; * operator
;;; * 引数の型リスト->式全体の型 の関数
;;; * operatorの引数リスト (expressionのみ)
;;; を受けとって，式の変換結果を返す
(defun handle-exp-args (op fun exp-list)
  (let* ((exp-rets (mapcar #'rule:expression! exp-list))
         (exp-types (mapcar #'second exp-rets))
         (ret-type (funcall fun exp-types)))
    ~(the ,ret-type (,op ,@exp-rets))))

;; argが1個のとき用
(defun handle-exp-1arg (op fun exp)
  (let* ((exp-ret (rule:expression! exp))
         (exp-type (second exp-ret))
         (ret-type (funcall fun exp-type)))
    ~(the ,ret-type (,op ,exp-ret))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; print-flow 用
(defun add-flow-printed (fexp)
  (push fexp *flow-printed*))

(defun already-printed (fexp)
  (member fexp *flow-printed* :test #'equal))
