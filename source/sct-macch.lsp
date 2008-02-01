;;; 規則ファイル読み込み用のreadtable
(provide "SCT-MACCH")
(eval-when (:compile-toplevel :load-toplevel :execute)
  (require "SC-DECL" "sc-decl.lsp"))

(in-package "SC-TRANSFORMER")
(scr:require "SC-MISC")

;; 
(defconstant comma 'BQ-COMMA)          
(defconstant comma-at 'BQ-COMMA-ATSIGN)
(defconstant comma-dot 'BQ-COMMA-DOT)
(defconstant optional 'OPTIONAL)

;; patternをreadするときのパッケージ，リードテーブル
;; nil なら変更しない.
(defvar *pattern-package* sc-file:*code-package*)   
;; パターン変数を読むときのパッケージ
;; patternの外のパッケージをread開始時に覚えておく
(defvar *outside-pattern-package*)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; read環境を変えるマクロ
(defmacro with-pattern-package (&body body)
  (with-fresh-variables (package-var)
    `(let ((,package-var (aif *pattern-package* it *package*))
       (*outside-pattern-package* *package*))
       (with-package ,package-var ,@body))))

(defmacro with-pattern-read-environment (&body body)
  `(with-pattern-package
     (with-readtable *pattern-readtable*
       ,@body)))

(defmacro with-pv-read-environment (&body body)
  `(with-package *outside-pattern-package*
     (with-readtable *pattern-readtable*
       ,@body)))

(defmacro with-[]-read-environment (&body body)
  `(with-package *outside-pattern-package*
     (with-readtable in-[]-readtable
       ,@body)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; 通常のreadtable環境の変更

;;; #?<pattern> で ,や@ 等を含むパターンを書ける
(set-dispatch-macro-character 
 #\# #\?
 #'(lambda (stream char1 char2)
     (declare (ignore char1 char2))
     (read-pattern stream t nil t)))

;;; `~'：続くS式が SC-readtable で読まれるback-quote
(set-macro-character 
 #\~
 #'(lambda (stream char)
     (declare (ignore char))
     (quasiquote-expand (pattern-body (read-pattern stream))))
 t)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; pattern-readtable
(defun make-pattern-readtable
    (&aux
     (in-[]-readtable (copy-readtable))
     (ret-readtable (copy-readtable)))
  ;; #+readtable-case (setf (readtable-case in-[]-readtable) :invert)
  #+readtable-case (setf (readtable-case ret-readtable) :invert)
  ;; <,symbol>  => (comma <symbol>)
  ;; ,@<symbol> => (comma-at <symbol>)
  ;; <,symbol>[x y ...]  => (comma <symbol> x y ...)
  ;; ,@<symbol> =>[x y ...] (comma-at <symbol> x y ...)
  (set-macro-character 
   #\,
   #'(lambda (stream char &aux fst scd rest)
       (declare (ignore char))
       (acase (peek-char nil stream)
     ((#\@ #\.)
      (read-char stream)
      (setq fst (ecase it ((#\@) comma-at) ((#\.) comma-dot))) )
     (otherwise
      (setq fst comma)))
       (with-pv-read-environment
     (setq scd (read stream t nil t)))
       (when (eql #\[ (peek-char nil stream nil))
     (read-char stream)
     (with-[]-read-environment
       (setq rest
         (read-delimited-list #\] stream t))))
       (cons fst (cons scd rest)))
   nil ret-readtable)
  ;; [ <list> ] => occurs error 
  (set-macro-character #\] (get-macro-character #\)) nil in-[]-readtable)
  (set-macro-character 
   #\[
   #'(lambda (stream char)
       (declare (ignore stream char))
       (error "[ must follows either ,<symbol> or ,@<symbol>"))
   nil ret-readtable)
  ;;   ;; `<symbol> => (bquote <symbol>)
  ;;   (set-macro-character
  ;;    #\`
  ;;    #'(lambda (stream char)
  ;;        (declare (ignore char))
  ;;        (list bquote (read stream)))
  ;;    nil ret-readtable)
  ;; { <list> } => (optional <list>)
  (set-macro-character #\} (get-macro-character #\)) nil ret-readtable)
  (set-macro-character 
   #\{
   #'(lambda (stream char)
       (declare (ignore char))
       (let ((optional-list (read-delimited-list #\} stream t)))
     `(,optional ,@optional-list)))
   nil ret-readtable)
  ;;
  ret-readtable)

(defvar *pattern-readtable* (make-pattern-readtable))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; #\~ 読み込み用，自作のbackquoteマクロ展開関数
(defun quasiquote-expand (exp)
  (assert (not (pat-commaat-p exp)))
  (assert (not (pat-commadot-p exp)))
  (assert (not (pat-optional-p exp)))
  (cond
    ((pat-comma-p exp)
     `,(second exp))
    ((consp exp)
     (quasiquote-expand-list exp))
    ((stringp exp)
     `(quote ,exp))
    ((vectorp exp)
     `(cl:apply #'cl:vector 
        ,(quasiquote-expand (coerce exp 'list))) )
    (t
     `(quote ,exp))))
    
(defun quasiquote-expand-list (exp)
  (acond
   ((or (and (pat-commaat-p (car exp)) 'cl:append)
	(and (pat-commadot-p (car exp)) 'cl:nconc))
    (let ((car-ret (second (car exp)))
	  (cdr-ret (quasiquote-expand-cdr (cdr exp))))
      (cond
       ;; <== optimization
       ((and (quoted-p cdr-ret)
	     (null (second cdr-ret)))
	car-ret)
       ;; optimization ==>
       (t
	`(,it ,car-ret ,cdr-ret) ))))
   (t ; pat-comma and otherwise
    (let ((car-ret (if (pat-comma-p (car exp))
		       (second (car exp))
		     (quasiquote-expand (car exp))))
	  (cdr-ret (quasiquote-expand-cdr (cdr exp))))
      (cond
       ;; <== optimization
       ((and (quoted-p car-ret) (quoted-p cdr-ret))
	`'(,(second car-ret) . ,(second cdr-ret)) )
       ((or (tagged-p 'cl:list* cdr-ret)
	    (tagged-p 'cl:list cdr-ret))
	`(,(car cdr-ret) ,car-ret ,@(cdr cdr-ret)) )
       ((and (quoted-p cdr-ret)
	     (null (second cdr-ret)))
	`(cl:list ,car-ret) )
       ;; optimization ==>
       (t
	`(cl:list* ,car-ret ,cdr-ret)))))
   ))

(defun quasiquote-expand-cdr (exp)
  (assert (not (pat-commaat-p exp)))
  (assert (not (pat-commadot-p exp)))
  (cond
    ((pat-comma-p exp)
     (second exp))
    ((atom exp)
     `(quote ,exp))
    (t
     (quasiquote-expand-list exp))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; パターンを扱う関数・マクロ

(defstruct (:pattern (:print-function pattern-print))
  body)

;; パターン判定述語
#+obsolete
(defun pattern-p (x)
  (and (consp x)
       (eq 'pattern (car x))))

;; パターンの種類
(defun pat-comma-p (pbody)
  (and (consp pbody)
       (eq comma (car pbody))))
(defun pat-commaat-p (pbody)
  (and (consp pbody)
       (eq comma-at (car pbody))))
(defun pat-commadot-p (pbody)
  (and (consp pbody)
       (eq comma-dot (car pbody))))
(defun pat-optional-p (pbody)
  (and (consp pbody)
       (eq optional (car pbody))))

;;; patternの内部表現を、, や ,@ を使った形の文字列で出力
;;; string2patの逆
(defun pattern-print (pat ostream depth)
  (assert (pattern-p pat))
  (with-pattern-package
      (format ostream "#?~A"
        (pattern-print2 (pattern-body pat) depth))))
(defun pattern-print2 (pbody depth)
  (if (listp pbody)
      (acond
       ((or (and (pat-comma-p pbody) ",")
            (and (pat-commaat-p pbody) ",@"))
        (format nil "~A~S~A"
          it
          (second pbody)
          (aif (cddr pbody)
               (format nil "[~{~S~^ ~}]" it)
               "")))
       ((eq optional (car pbody))
        (format nil "{~A}"
          (strcat (loop for x in (cdr pbody)
                      collect (pattern-print2 x (1+ depth)))
                  " ")))
       (t
        (format nil "(~A)"
          (strcat (loop for x in pbody
                      collect (pattern-print2 x (1+ depth)))
                  " "))))
    (format nil "~S" pbody)))

;; パターンをLispで評価したとき
#+obsolete
(defmacro pattern (&whole x y)
  (declare (ignore y))
  `',x)


;; パターンを読む環境でread
(defun read-pattern (&rest read-args)
  (with-pattern-read-environment
      #+obsolete `(pattern ,(apply #'read read-args))
      (make-pattern :body (apply #'read read-args))))

;; パターンを読む環境でread-from-string
(defun string2pat (str)
  (with-input-from-string (istr str)
    (read-pattern istr)))
