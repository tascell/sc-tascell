;;; SC Preprocessor
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
;;; (%elif <constant>)
;;; (%endif)
;;;;; #line, #error
;;; (%line <digit> {<string>})
;;; (%error <string> {form}*)
;;;;; #pragma 
;;; (%pragma <string> {<string>}) 

(defconstant *scpp-macro-package* (find-package "SCPP-MACRO"))
(defconstant *scpp-package* (find-package "SCPP"))

(defparameter *cinclude-h-file* "cinclude_tmp") ; Cのヘッダをincludeする一時ヘッダ
(defparameter *delete-c2sc-intermediate* nil) ; cinclude_tmp.h, *.se, *.se? を削除するか
(defparameter *include-path-list* '("./"))

(defvar *macro-entries* (make-hash-table :test #'eq)) ; macro-entries
(defvar *rule-modifier* #'identity)     ; sc-main から渡される関数．rule-listを変更する．
(defvar *sc2c-modifier* #'identity)     ; sc-main から渡される関数．sc2cに使うルールを変更する．
(defvar *ofile-modifier* #'identity)    ; sc-main から渡される関数．出力ファイル名を変更する．

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
               ((:include-path *include-path-list*) *include-path-list*))
  (initialize)
  (when (stringp x)
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

(defvar *scpp-abort* nil)
(defvar *scpp-section* nil)             ; one of {nil, then, else}

(defun scpp-list (x)
  (reduce #'nconc (mapcar #'scpp-1exp x) :from-end t))

(defun scpp-1exp (x)
  (cond
   ((and (listp x)
         (symbolp (car x))
         (char= #\% (aref (symbol-name (car x)) 0))) 
    (case (car x)
      ((sc::%splice)
       (scpp-list (cdr x)))
      ((sc::%include)
       (scpp-list (sc-file:read-sc-file 
                   (path-search (car (scpp-list (cdr x)))
                                *include-path-list*
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
       (if (let ((macsymbol (entry-and-normalize-id (second x)))) 
             (case (car x)
               ((sc::%ifdef)  (macro-find macsymbol))
               ((sc::%ifndef) (not (macro-find macsymbol)))
               ((sc::%if)     (eval (scpp-macroexpand (second x))))))
           (scpp-list (third x))
         (scpp-list (fourth x))))
      ((sc::%ifdef* sc::%ifndef* sc::%if*) ; (%ifdef* ... %else ...) = (%ifdef (...) (...))
       (multiple-value-bind (then-forms else-forms)
           (list-until (cddr x) ~%else :key #'car)
         (let ((tag (cdr (assoc (car x) ~((%ifdef* . %ifdef) (%ifndef* . %ifndef)
                                                             (%if* . %if))))))
           (assert tag)
           (scpp-1exp ~(,tag ,(second x)
                             ,then-forms ,(cdr else-forms))))))
      ((sc::%line)
       (list
        `(sc::c-exp ,(format nil "#line ~D \"~a\""
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
   (t 
    (list (scpp-macroexpand x)))))

;; *cinclude-h-file* を作成
(defun make-cinclude-h (header-list
                        &optional (output-filename *cinclude-h-file*))
  (let ((output-file (merge-pathnames
                      (make-pathname :name output-filename
                                     :type "h")
                      (car *include-path-list*))))
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
(defun expand-fref-symbol (sym)
  (let ((name (symbol-name sym))
        (pack (symbol-package sym))
        (symbol-list ())
        (j 0))
    (loop for i from 0 upto (1- (length name))
        as ch = (aref name i)
        do (case ch
             ((#\-)
              (when (and (< (+ i 1) (1- (length name)))
                         (char= (aref name (+ i 1)) #\>))
                (push (intern (subseq name j i) pack) symbol-list)
                (push ~-> symbol-list)
                (incf i)
                (setq j (+ i 1))))
             ((#\.)
              (push (intern (subseq name j i) pack) symbol-list)
              (setq j (+ i 1))))
        finally (when (< j i)
                  (push (intern (subseq name j i) pack) symbol-list)))
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

;; 適用
(defun scpp-macroexpand (x
                         &key (extracting-macro '()) ; 展開を行わないマクロ
                         &aux macsymbol macentry)
  (cond
   ((symbolp x)                         ; constant macro
    (multiple-value-bind (fref-exp expand-p) (expand-fref-symbol x)
      (cond
       (expand-p (scpp-macroexpand fref-exp))
       (t
        (setq macsymbol (entry-and-normalize-id x))
        (if (and (not (member macsymbol extracting-macro))
                 (setq macentry (macro-find macsymbol))
                 (eq 'constant (macro-entry-type macentry)))
            (scpp-macroexpand (macro-entry-value macentry)
                              :extracting-macro (cons macsymbol extracting-macro))
          macsymbol)))))
   ((consp x)
    (if (and (symbolp (car x))
             (setq macsymbol (entry-and-normalize-id (car x)))
             (not (member macsymbol extracting-macro))
             (setq macentry (macro-find macsymbol))
             (eq 'macro (macro-entry-type macentry)))
        ;; マクロ
        (scpp-macroexpand (macroexpand-1 (cons macsymbol (cdr x)))
                          :extracting-macro (cons macsymbol extracting-macro))
      ;; それ以外(再帰的に適用)
      (mapcar #'scpp-macroexpand x)))
   ;; symbol以外のatom
   (t x)))
