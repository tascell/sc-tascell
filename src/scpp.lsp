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

;;; SC Preprocessor

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(provide "SCPP")
(eval-when (:compile-toplevel :load-toplevel :execute)
  (require "SC-DECL" "sc-decl.lsp"))

(in-package "SCPP")

(scr:require "SC-FILE")
(scr:require "SC-MISC")
(scr:require "SCT")
(scr:require "C2SC")
(rule:require-ruleset :sc2c)

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
;;;;; #line, #error
;;; (%line <digit> {<string>})
;;; (%error <string> {form}*)
;;;;; #pragma 
;;; (%pragma <string> {<string>}) 

(defconstant *scpp-macro-package* (find-package "SCPP-MACRO"))
(defconstant *scpp-package* (find-package "SCPP"))

(defparameter *cinclude-h-file* "cinclude_tmp") ; Cのヘッダをincludeする一時ヘッダ
(defparameter *delete-c2sc-intermediate* nil) ; cinclude_tmp.h, *.se, *.se? を削除するか
(defparameter *input-file-path* "./")   ; 入力ファイルの場所
                                        ; cinclude_tmp.h の生成場所であり，
                                        ; #include "..." のファイルの探索場所に影響する
(defparameter *include-path-list* '("./")) ; %includeのヘッダファイルを探す場所

(defvar *macro-entries* (make-hash-table :test #'eq)) ; macro-entries
(defvar *rule-modifier* #'identity)     ; sc-main から渡される関数．rule-listを変更する．
(defvar *sc2c-modifier* #'identity)     ; sc-main から渡される関数．sc2cに使うルールを変更する．
(defvar *ofile-modifier* #'identity)    ; sc-main から渡される関数．出力ファイル名を変更する．

(defvar *extracting-macro* (list))      ; List of macro symbols currently being extracted.
					; (Such macros are not extracted more than onece recursively)

(defstruct (macro-entry
            (:constructor create-macro-entry (type value)))
  type                                  ; one of {constant macro}
  value                                 ; replacement
  )

(defmacro in-macro-package (&body forms)
  `(let ((*package* *scpp-macro-package*))
     ,@forms))

;; kill all macros
(defun initialize ()
  (maphash #'(lambda (id val)
               (declare (ignorable val))
               (delete-macro id))
           *macro-entries*)
  (setq *macro-entries* (make-hash-table :test #'eq)))

;; x : s-exp or filename
(defun scpp (x &key
               ((:rule-modifier *rule-modifier*) *rule-modifier*)
               ((:sc2c-modifier *sc2c-modifier*) *sc2c-modifier*)
               ((:ofile-modifier *ofile-modifier*) *ofile-modifier*)
               ((:input-file-path *input-file-path*) *input-file-path*)
               ((:include-path *include-path-list*) *include-path-list*))
  (initialize)
  (when (or (pathnamep x) (stringp x))
    (setq x (sc-file:read-sc-file x)))
  (scpp-list x))

;; * Cに変換したときに同じidentifierで，
;; * 同じpackageに登録されている
;; symbolどうしはeqであることを保証できるようにするため，
;; それと，*used-identifier* に使用した識別子が登録するため，
;; make-id で識別子を作りなおす．
(defun entry-and-normalize-id (sym)
  (aif (rule:identifier0? sym :sc2c)
      (rule:make-id it (symbol-package sym))
    sym))

(defun scpp-list (x)
  (apply #'nconc (mapcar #'scpp-1exp x)))

(defun scpp-1exp (x)
  (cond
   ((and (consp x)
         (symbolp (car x))
         (with1 sname (symbol-name (car x))
           (and (> (length sname) 1)         ; `%' 演算子はだめなので >=2
                (char= #\% (aref sname 0)))))
    (case (car x)
      ((sc::%splice)
       (scpp-list (cdr x)))
      ((sc::%include)
       (scpp-list (sc-file:read-sc-file 
                   (path-search (car (scpp-list (cdr x)))
                                *include-path-list*
                                :current-directory nil
                                :error-when-unfound t))) )
      ((sc::%cinclude)
       (multiple-value-bind (filenames options)
           (list-until-if #'consp (scpp-list (cdr x)) :key #'car)
         (let ((required-macros t)
               (required-variables t)
               (required-aggregates t))
           ;; handle "required-macro" option
           (loop for (tag . args) in options
               do (case tag
                    ((:macro :required-macros)
                     (setq required-macros
                       (loop for arg in args
                           collect (rule:identifier0! arg :sc2c))))
                    ((:var :required-variables)
                     (setq required-variables
                       (loop for arg in args
                           collect (rule:identifier0! arg :sc2c))))
                    ((:aggr :required-aggregates)
                     (setq required-aggregates
                       (loop for arg in args
                           collect (rule:identifier0! arg :sc2c))))
                    (otherwise
                     (warn "~S: Unknown %cinclude option." tag))))
           (let ((tmp-header (make-cinclude-h filenames)))
             (prog1
                 (scpp-list (c2sc:c2sc
                             tmp-header
                             :outfile nil
                             :delete-efile *delete-c2sc-intermediate*
                             :cpp-option '()
                             :required-macros required-macros
                             :required-variables required-variables
                             :required-aggregates required-aggregates))
               (when *delete-c2sc-intermediate*
                 (delete-file tmp-header)))))))
      ((sc::%rule)
       (let ((new-rules
              (funcall *rule-modifier* (apply #'append (scpp-list (cdr x))))))
         (entry-macro ~(%defconstant RULE ,new-rules) :redefine-warn nil))
       '() )
      ((sc::%sc2c)
       (funcall *sc2c-modifier* (car (scpp-list (cdr x))))
       '() )
      ((sc::%output-file)
       (funcall *ofile-modifier* (apply #'string+ (scpp-list (cdr x))))
       '() )
      ((sc::%defmacro sc::%defconstant)
       (entry-macro x)
       '() )
      ((sc::%undef)
       (delete-macro (entry-and-normalize-id (second x)))
       '() )
      ((sc::%ifdef sc::%ifndef sc::%if)
       (destructuring-bind (directive cnd then-list &optional else-list) x
         (if (case directive
               ((sc::%ifdef)  (macro-find (entry-and-normalize-id cnd)))
               ((sc::%ifndef) (not (macro-find (entry-and-normalize-id cnd))))
               ((sc::%if)     (let ((val (eval (car (scpp-1exp cnd)))))
                                (and val (not (eql val 0))))))
             (scpp-list then-list)
           (scpp-list else-list))))
      ((sc::%ifdef* sc::%ifndef* sc::%if*) ; (%ifdef* ... %else ...) = (%ifdef (...) (...))
       (multiple-value-bind (then-forms else-forms)
           (list-until (cddr x) ~%else :key #'car)
         (let ((tag (cdr (assoc (car x) ~((%ifdef* . %ifdef) (%ifndef* . %ifndef)
                                                             (%if* . %if))))))
           (assert tag)
           (scpp-1exp ~(,tag ,(second x)
                             ,then-forms ,(cdr else-forms))))))
      ((sc::%string)
       (let ((ret (scpp-list (cdr x))))
         (list (strcat (mapcar #'princ-to-string ret)))))
      ((sc::%line)
       (list `(sc::c-exp ,(format nil "#line ~D \"~a\""
                            (second x)
                            (aif (third x) it "")))))
      ((sc::%error)
       (apply #'format *error-output* (scpp-list (cdr x)))
       (format *error-output* "~%")
       '() )
      ((sc::%pragma)
       (list
        `(sc::c-exp ,(format nil "#pragma ~A ~A"
                       (second x)
                       (aif (third x) it "")))))
      (otherwise
       (error "Unknown scpp-directive ~S." (car x)))))
   ((and (symbolp x)
         (string-begin-with "%%" (symbol-name x) nil))
    '())
   (t
    ;; Macro expansion and recursive application of scpp-exp1.
    (scpp-macroexpand x))
   ))

;; *cinclude-h-file* を作成
(defun make-cinclude-h (header-list
                        &optional (output-filename *cinclude-h-file*))
  (let ((output-file (make-pathname :name output-filename
                                    :directory (pathname-directory *input-file-path*)
                                    :type "h")))
    (write-file output-file
                (apply #'string+
                       (mapcar #'(lambda (hfile)
                                   (format nil "#include ~A~%"
                                     (if (char= #\< (aref hfile 0))
                                         hfile
                                       (string+ "\"" hfile "\""))))
                               header-list))
                :overwrite t :write-string t)
    output-file))

;;;; マクロ処理

;; symbol -> macrosymbol
#+obsolete                              ; replaced by entry-and-normalize-id
(defun get-macrosymbol (sym)
  (let ((macname (sc2c:identifier-sc2c sym)))
    (when macname
      (intern macname *scpp-macro-package*))))

;; フィールドアクセスをC風に書けるようにする
;; ~sc::f->x ==> ~(fref f -> x)
(defun invert-case-if-mixed (str)
  (if (case-mixed-p str)
      (string-invertcase str)
    str))
(defun expand-fref-symbol (sym)
  (let* ((name (invert-case-if-mixed (symbol-name sym)))
         (pack (symbol-package sym))
         (symbol-list ())
         (j 0))
    (loop for i from 0 upto (1- (length name))
        as ch = (aref name i)
        do (case ch
             ((#\-)
              (when (and (< (+ i 1) (1- (length name)))
                         (char= (aref name (+ i 1)) #\>))
                (push (intern (invert-case-if-mixed (subseq name j i)) pack) symbol-list)
                (push ~-> symbol-list)
                (incf i)
                (setq j (+ i 1))))
             ((#\.)
              (push (intern (invert-case-if-mixed (subseq name j i)) pack) symbol-list)
              (setq j (+ i 1))))
        finally (when (< j i)
                  (push (intern (invert-case-if-mixed (subseq name j i)) pack) symbol-list)))
    (if (cdr symbol-list)
        (values (cons ~fref (nreverse symbol-list)) t)
      (values sym nil))))

;; 検索: symbol-> {'macro, 'constant, nil}
(defun macro-find (macsymbol)
  (gethash macsymbol *macro-entries*))

;; 登録
;; dm: a %defmacro/%defconstant form
(defun entry-macro (dm &key (redefine-warn t) &aux entry-type entry-value)
  (let ((macsymbol (entry-and-normalize-id (second dm))))
    (case (first dm)
      ((sc::%defmacro)
       (eval `(defmacro ,macsymbol ,@(cddr dm)))
       (setq entry-type 'macro
             entry-value (cons :macro (cddr dm))))
      ((sc::%defconstant)
       ;; (eval `(defparameter ,macsymbol ',(third dm)))
       (setq entry-type 'constant
             entry-value (third dm)))
      (otherwise
       (error "~S is not a defmacro/defconstant directive." dm)))
    (awhen (macro-find macsymbol)
      (when redefine-warn
        (warn "Macro ~S is being redefined: ~S --> ~S"
              macsymbol (macro-entry-value it) entry-value))
      (delete-macro macsymbol))
    (setf (gethash macsymbol *macro-entries*) 
      (create-macro-entry entry-type entry-value))))

;; 削除
(defun delete-macro (macsymbol)
  ;; (makunbound macsymbol)
  (fmakunbound macsymbol)
  (remhash macsymbol *macro-entries*))

;; Macro expansion and recursive application of scpp-exp1.
;; Simple annotation for field references (e.g., "obj.a->b") is
;; also expanded here (to "(fref (mref (fref obj a)) b)").
;; When there is any expansion applied, scpp-exp1/scpp-list is applied
;; recursively.
(defun scpp-macroexpand (x
                         &aux macsymbol macentry)
  (cond
   ((symbolp x)                         ; constant macro
    (multiple-value-bind (fref-exp expanded-p) (expand-fref-symbol x)
      (cond
       ;; Simple annotation for field references
       (expanded-p
	(scpp-1exp fref-exp))
       (t
        (setq macsymbol (entry-and-normalize-id x))
        (if (and (not (member macsymbol *extracting-macro*))
                 (setq macentry (macro-find macsymbol))
                 (eq 'constant (macro-entry-type macentry)))
	    ;; Symbol macro (by %defconstant)
	    (let ((*extracting-macro* (cons macsymbol *extracting-macro*)))
	      (scpp-1exp (macro-entry-value macentry)))
	  ;; Other symbol
          (list macsymbol))))))
   ((consp x)                           ; (<macroname> ... )
    (if (and (symbolp (car x))
             (setq macsymbol (entry-and-normalize-id (car x)))
             (not (member macsymbol *extracting-macro*))
             (setq macentry (macro-find macsymbol))
             (eq 'macro (macro-entry-type macentry)))
        ;; Macro (by %defmacro)
	(let ((*extracting-macro* (cons macsymbol *extracting-macro*)))
	  (scpp-1exp (macroexpand-1 (cons macsymbol (cdr x)))))
      ;; Cons to which no macro can be applied.
      (list (scpp-list x))))
   ;; Atom other than symbol (e.g., number string)
   (t (list x))))
