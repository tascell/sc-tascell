(provide "sc2c")
(in-package "sc2c")
(export '(@sc2c @translation-unit @external-declaration @declaration
                @inlined-declaration @function-attribute
                @register-declarator @function-identifier @def-or-decl
                @init-declarator @storage-class-specifier
                @compound-storage-class-specifier @struct-declaration
                @enumerator @enumeration-constant @compound-initializer
                @initializer @type-expression @type-vaarg
                @type-specifier @struct-or-union-specifier
                @struct-or-union @enum-specifier @type-qualifier
                @typedef-name @statement @compound-statement
                @block-item @labeled-statement @expression-statement
                @selection-statement @iteration-statement
                @jump-statement @expression @field-identifier
                @c-expression))
(require "SC-MISC")
(require "SC-UTIL")
(require "SCT-LIB")
(require "SC2C-DEFS" #"./sc2c-defs.lsp")
(defconstant *rule-package* *package*)
(use-package "SC-TRANSFORMER")
(use-package "SC-MISC")
(use-package "SC-UTIL")
(use-package "SC-UTIL")
(defun initiate (sc-transformer::x &key (package *package*))
  (@sc2c sc-transformer::x :package package))
(defun @sc2c
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (sc2c (immigrate-package sc-transformer::x *rule-package*
                package))
      package *rule-package*))
(defun @translation-unit
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (translation-unit
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @external-declaration
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (external-declaration
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @declaration
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (declaration
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @inlined-declaration
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (inlined-declaration
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @function-attribute
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (function-attribute
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @register-declarator
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (register-declarator
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @function-identifier
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (function-identifier
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @def-or-decl
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (def-or-decl
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @init-declarator
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (init-declarator
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @storage-class-specifier
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (storage-class-specifier
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @compound-storage-class-specifier
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (compound-storage-class-specifier
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @struct-declaration
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (struct-declaration
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @enumerator
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (enumerator
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @enumeration-constant
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (enumeration-constant
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @compound-initializer
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (compound-initializer
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @initializer
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (initializer
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @type-expression
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (type-expression
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @type-vaarg
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (type-vaarg
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @type-specifier
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (type-specifier
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @struct-or-union-specifier
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (struct-or-union-specifier
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @struct-or-union
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (struct-or-union
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @enum-specifier
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (enum-specifier
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @type-qualifier
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (type-qualifier
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @typedef-name
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (typedef-name
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @statement
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (statement
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @compound-statement
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (compound-statement
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @block-item
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (block-item
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @labeled-statement
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (labeled-statement
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @expression-statement
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (expression-statement
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @selection-statement
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (selection-statement
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @iteration-statement
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (iteration-statement
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @jump-statement
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (jump-statement
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @expression
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (expression
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @field-identifier
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (field-identifier
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @c-expression
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (c-expression
          (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun sc2c (x)
  (block sc-transformer::pattern-matched
    (let (sc-program (matched-symbol (list (list 'sc-program))))
      (if (and (listp x)
               (let ((sc-transformer::crest x))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff x sc-transformer::crest)))
                       (progn
                         (setq sc-program sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (progn
              (let ((*print-case* :downcase)
                    (*keyword-list* (cons 'c-exp *keyword-list*))
                    (*otherwise-default* nil))
                (strcat (translation-unit sc-program)))))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun translation-unit (x)
  (block sc-transformer::pattern-matched
    (let (ext-decl-list (matched-symbol (list (list 'ext-decl-list))))
      (if (and (listp x)
               (let ((sc-transformer::crest x))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff x sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar
                                       #'external-declaration
                                       sc-transformer::prev)))
                         (setq ext-decl-list sc-transformer::prev)
                         (rplacd (find 'ext-decl-list matched-symbol
                                       :key #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (strcat (get-retval 'ext-decl-list)))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun external-declaration (x)
  (block sc-transformer::pattern-matched
    (let (decl (matched-symbol (list (list 'decl))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (declaration x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'decl matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq decl x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'decl))
          nil))
    (let (cexp (matched-symbol (list (list 'cexp))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (c-expression x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'cexp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq cexp x) t))
          (return-from sc-transformer::pattern-matched
            (format nil "~A~%" (get-retval 'cexp)))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun declaration (x)
  (block sc-transformer::pattern-matched
    (let (i-decl (matched-symbol (list (list 'i-decl))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (inlined-declaration x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'i-decl matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq i-decl x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'i-decl))
          nil))
    (let (init texp id
               (matched-symbol
                   (list (list 'init) (list 'texp) (list 'id))))
      (if (and (listp x) (= 3 (length x))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (nth 0 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 0 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (nth 1 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (initializer (nth 2 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'init matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq init (nth 2 x)) t))
          (return-from sc-transformer::pattern-matched
            (inlined-declaration (list* 'def x)))
          nil))
    (let (texp id (matched-symbol (list (list 'texp) (list 'id))))
      (if (and (listp x) (= 2 (length x))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (nth 0 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 0 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (inlined-declaration (list* 'def x)))
          nil))
    (let (bi-list ft-list f-id
                  (matched-symbol
                      (list (list 'bi-list) (list 'ft-list)
                            (list 'f-id))))
      (if (and (listp x) x (cdr x) (listp (car (cdr x))) (car (cdr x))
               (eq 'fn (car (car (cdr x))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-identifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'f-id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq f-id (car x)) t)
               (let ((sc-transformer::crest (cdr (car (cdr x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (car (cdr x)))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'type-expression
                                       sc-transformer::prev)))
                         (setq ft-list sc-transformer::prev)
                         (rplacd (find 'ft-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (inlined-declaration (list* 'def x)))
          nil))
    (let (bi-list va-arg ft-list f-id
                  (matched-symbol
                      (list (list 'bi-list) (list 'va-arg)
                            (list 'ft-list) (list 'f-id))))
      (if (and (listp x) x (cdr x) (listp (car (cdr x))) (car (cdr x))
               (eq 'fn (car (car (cdr x))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-identifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'f-id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq f-id (car x)) t)
               (let ((sc-transformer::crest (cdr (car (cdr x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and
                                       (= 1 (length sc-transformer::y))
                                       (let
                                        ((sc-transformer::retval
                                          (multiple-value-list
                                           (type-vaarg
                                            (nth 0 sc-transformer::y)))))
                                         (and
                                          (car sc-transformer::retval)
                                          (rplacd
                                           (find 'va-arg matched-symbol
                                            :key #'car)
                                           sc-transformer::retval)))
                                       (progn
                                         (setq va-arg
                                          (nth 0 sc-transformer::y))
                                         t)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (car (cdr x)))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar
                                       #'function-type-list
                                       sc-transformer::prev)))
                         (setq ft-list sc-transformer::prev)
                         (rplacd (find 'ft-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (inlined-declaration (list* 'def x)))
          nil))
    (let (sdecl-list su-spec
             (matched-symbol (list (list 'sdecl-list) (list 'su-spec))))
      (if (and (listp x) x
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (struct-or-union-specifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'su-spec matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq su-spec (car x)) t)
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar
                                       #'struct-declaration
                                       sc-transformer::prev)))
                         (setq sdecl-list sc-transformer::prev)
                         (rplacd (find 'sdecl-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (inlined-declaration (list* 'def x)))
          nil))
    (let (en-list en-spec
                  (matched-symbol
                      (list (list 'en-list) (list 'en-spec))))
      (if (and (listp x) x
               (let ((sc-transformer::retval
                         (multiple-value-list (enum-specifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'en-spec matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq en-spec (car x)) t)
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'enumerator
                                       sc-transformer::prev)))
                         (setq en-list sc-transformer::prev)
                         (rplacd (find 'en-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (inlined-declaration (list* 'def x)))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun inlined-declaration (x)
  (block sc-transformer::pattern-matched
    (let (init texp id scs
               (matched-symbol
                   (list (list 'init) (list 'texp) (list 'id)
                         (list 'scs))))
      (if (and (listp x) (= 4 (length x))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (storage-class-specifier (nth 0 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (nth 0 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (id (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 1 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression (nth 2 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (nth 2 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (initializer (nth 3 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'init matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq init (nth 3 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((scs-str (get-retval 'scs))
                  (idtexp-str
                      (format nil (get-retval 'texp) (get-retval 'id)))
                  (init-str
                      (if (pattern-variable-p 'init)
                          (format nil " = ~A"
                                  (parenthesize
                                      (multiple-value-list
                                       (get-retval 'init))
                                      15))
                          "")))
              (format nil "~A ~A~A;~%" scs-str idtexp-str init-str)))
          nil))
    (let (texp id scs
               (matched-symbol
                   (list (list 'texp) (list 'id) (list 'scs))))
      (if (and (listp x) (= 3 (length x))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (storage-class-specifier (nth 0 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (nth 0 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (id (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 1 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression (nth 2 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (nth 2 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((scs-str (get-retval 'scs))
                  (idtexp-str
                      (format nil (get-retval 'texp) (get-retval 'id)))
                  (init-str
                      (if (pattern-variable-p 'init)
                          (format nil " = ~A"
                                  (parenthesize
                                      (multiple-value-list
                                       (get-retval 'init))
                                      15))
                          "")))
              (format nil "~A ~A~A;~%" scs-str idtexp-str init-str)))
          nil))
    (let (bi-list r-decl f-attr va-arg texp-list texp func-id scs
                  (matched-symbol
                      (list (list 'bi-list) (list 'r-decl)
                            (list 'f-attr) (list 'va-arg)
                            (list 'texp-list) (list 'texp)
                            (list 'func-id) (list 'scs))))
      (if (and (listp x) x (cdr x) (cdr (cdr x))
               (listp (car (cdr (cdr x)))) (car (cdr (cdr x)))
               (eq 'fn (car (car (cdr (cdr x)))))
               (cdr (car (cdr (cdr x)))) (cdr (cdr (cdr x)))
               (eq ':attr (car (cdr (cdr (cdr x)))))
               (cdr (cdr (cdr (cdr x))))
               (cdr (cdr (cdr (cdr (cdr x)))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (storage-class-specifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (car x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-identifier (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'func-id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq func-id (car (cdr x))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression
                                 (car (cdr (car (cdr (cdr x)))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (car (cdr (car (cdr (cdr x)))))) t)
               (let ((sc-transformer::crest
                         (cdr (cdr (car (cdr (cdr x)))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and
                                       (= 1 (length sc-transformer::y))
                                       (let
                                        ((sc-transformer::retval
                                          (multiple-value-list
                                           (type-vaarg
                                            (nth 0 sc-transformer::y)))))
                                         (and
                                          (car sc-transformer::retval)
                                          (rplacd
                                           (find 'va-arg matched-symbol
                                            :key #'car)
                                           sc-transformer::retval)))
                                       (progn
                                         (setq va-arg
                                          (nth 0 sc-transformer::y))
                                         t)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car (cdr (cdr x)))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'type-expression
                                       sc-transformer::prev)))
                         (setq texp-list sc-transformer::prev)
                         (rplacd (find 'texp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-attribute
                                 (car (cdr (cdr (cdr (cdr x)))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'f-attr matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq f-attr (car (cdr (cdr (cdr (cdr x)))))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (register-declarator
                                 (car (cdr (cdr (cdr (cdr (cdr x))))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'r-decl matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn
                 (setq r-decl (car (cdr (cdr (cdr (cdr (cdr x)))))))
                 t)
               (let ((sc-transformer::crest
                         (cdr (cdr (cdr (cdr (cdr (cdr x))))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr
                                       (cdr (cdr (cdr (cdr (cdr x))))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((scs-retval (multiple-value-list (get-retval 'scs)))
                  (fid-str (car (get-retval 'func-id)))
                  (texp-str (get-retval 'texp))
                  (arg-list
                      (combine-each-nth
                          (list (get-retval 'texp-list)
                                (cdr (get-retval 'func-id)))
                          :n
                          (max (length texp-list)
                               (length (cdr (get-retval 'func-id))))))
                  (inline-str
                      (if (pattern-variable-p 'f-attr)
                          (format nil "~A " (get-retval 'f-attr)) ""))
                  (register-list
                      (if (pattern-variable-p 'r-decl)
                          (get-retval 'r-decl) nil))
                  (vaarg-str
                      (if (pattern-variable-p 'va-arg) ", ..." "")))
              (let ((scs-str (first scs-retval))
                    (arg-str (format nil "~A~A"
                                     (strcat
                                      (mapcar
                                       #'(lambda (xx)
                                           (format nil "~A~A"
                                            (if
                                             (member (second xx)
                                              register-list :test
                                              #'string=)
                                             "register " "")
                                            (if (first xx)
                                             (format nil (first xx)
                                              (if (second xx)
                                               (second xx) ""))
                                             (second xx))))
                                       arg-list)
                                      ", ")
                                     vaarg-str))
                    (body-str
                        (if (and (eq 'decl (second scs-retval))
                                 (null bi-list))
                            ";"
                            (format nil "~%{~%~A}"
                                    (strcat (get-retval 'bi-list))))))
                (format nil "~%~A~A ~A~A~%" inline-str scs-str
                        (format nil texp-str
                                (format nil "~A( ~A )" fid-str arg-str))
                        body-str))))
          nil))
    (let (bi-list f-attr va-arg texp-list texp func-id scs
                  (matched-symbol
                      (list (list 'bi-list) (list 'f-attr)
                            (list 'va-arg) (list 'texp-list)
                            (list 'texp) (list 'func-id) (list 'scs))))
      (if (and (listp x) x (cdr x) (cdr (cdr x))
               (listp (car (cdr (cdr x)))) (car (cdr (cdr x)))
               (eq 'fn (car (car (cdr (cdr x)))))
               (cdr (car (cdr (cdr x)))) (cdr (cdr (cdr x)))
               (eq ':attr (car (cdr (cdr (cdr x)))))
               (cdr (cdr (cdr (cdr x))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (storage-class-specifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (car x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-identifier (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'func-id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq func-id (car (cdr x))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression
                                 (car (cdr (car (cdr (cdr x)))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (car (cdr (car (cdr (cdr x)))))) t)
               (let ((sc-transformer::crest
                         (cdr (cdr (car (cdr (cdr x)))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and
                                       (= 1 (length sc-transformer::y))
                                       (let
                                        ((sc-transformer::retval
                                          (multiple-value-list
                                           (type-vaarg
                                            (nth 0 sc-transformer::y)))))
                                         (and
                                          (car sc-transformer::retval)
                                          (rplacd
                                           (find 'va-arg matched-symbol
                                            :key #'car)
                                           sc-transformer::retval)))
                                       (progn
                                         (setq va-arg
                                          (nth 0 sc-transformer::y))
                                         t)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car (cdr (cdr x)))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'type-expression
                                       sc-transformer::prev)))
                         (setq texp-list sc-transformer::prev)
                         (rplacd (find 'texp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-attribute
                                 (car (cdr (cdr (cdr (cdr x)))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'f-attr matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq f-attr (car (cdr (cdr (cdr (cdr x)))))) t)
               (let ((sc-transformer::crest
                         (cdr (cdr (cdr (cdr (cdr x)))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr (cdr (cdr x)))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((scs-retval (multiple-value-list (get-retval 'scs)))
                  (fid-str (car (get-retval 'func-id)))
                  (texp-str (get-retval 'texp))
                  (arg-list
                      (combine-each-nth
                          (list (get-retval 'texp-list)
                                (cdr (get-retval 'func-id)))
                          :n
                          (max (length texp-list)
                               (length (cdr (get-retval 'func-id))))))
                  (inline-str
                      (if (pattern-variable-p 'f-attr)
                          (format nil "~A " (get-retval 'f-attr)) ""))
                  (register-list
                      (if (pattern-variable-p 'r-decl)
                          (get-retval 'r-decl) nil))
                  (vaarg-str
                      (if (pattern-variable-p 'va-arg) ", ..." "")))
              (let ((scs-str (first scs-retval))
                    (arg-str (format nil "~A~A"
                                     (strcat
                                      (mapcar
                                       #'(lambda (xx)
                                           (format nil "~A~A"
                                            (if
                                             (member (second xx)
                                              register-list :test
                                              #'string=)
                                             "register " "")
                                            (if (first xx)
                                             (format nil (first xx)
                                              (if (second xx)
                                               (second xx) ""))
                                             (second xx))))
                                       arg-list)
                                      ", ")
                                     vaarg-str))
                    (body-str
                        (if (and (eq 'decl (second scs-retval))
                                 (null bi-list))
                            ";"
                            (format nil "~%{~%~A}"
                                    (strcat (get-retval 'bi-list))))))
                (format nil "~%~A~A ~A~A~%" inline-str scs-str
                        (format nil texp-str
                                (format nil "~A( ~A )" fid-str arg-str))
                        body-str))))
          nil))
    (let (bi-list r-decl va-arg texp-list texp func-id scs
                  (matched-symbol
                      (list (list 'bi-list) (list 'r-decl)
                            (list 'va-arg) (list 'texp-list)
                            (list 'texp) (list 'func-id) (list 'scs))))
      (if (and (listp x) x (cdr x) (cdr (cdr x))
               (listp (car (cdr (cdr x)))) (car (cdr (cdr x)))
               (eq 'fn (car (car (cdr (cdr x)))))
               (cdr (car (cdr (cdr x)))) (cdr (cdr (cdr x)))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (storage-class-specifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (car x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-identifier (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'func-id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq func-id (car (cdr x))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression
                                 (car (cdr (car (cdr (cdr x)))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (car (cdr (car (cdr (cdr x)))))) t)
               (let ((sc-transformer::crest
                         (cdr (cdr (car (cdr (cdr x)))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and
                                       (= 1 (length sc-transformer::y))
                                       (let
                                        ((sc-transformer::retval
                                          (multiple-value-list
                                           (type-vaarg
                                            (nth 0 sc-transformer::y)))))
                                         (and
                                          (car sc-transformer::retval)
                                          (rplacd
                                           (find 'va-arg matched-symbol
                                            :key #'car)
                                           sc-transformer::retval)))
                                       (progn
                                         (setq va-arg
                                          (nth 0 sc-transformer::y))
                                         t)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car (cdr (cdr x)))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'type-expression
                                       sc-transformer::prev)))
                         (setq texp-list sc-transformer::prev)
                         (rplacd (find 'texp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (register-declarator
                                 (car (cdr (cdr (cdr x))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'r-decl matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq r-decl (car (cdr (cdr (cdr x))))) t)
               (let ((sc-transformer::crest (cdr (cdr (cdr (cdr x))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr (cdr x))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((scs-retval (multiple-value-list (get-retval 'scs)))
                  (fid-str (car (get-retval 'func-id)))
                  (texp-str (get-retval 'texp))
                  (arg-list
                      (combine-each-nth
                          (list (get-retval 'texp-list)
                                (cdr (get-retval 'func-id)))
                          :n
                          (max (length texp-list)
                               (length (cdr (get-retval 'func-id))))))
                  (inline-str
                      (if (pattern-variable-p 'f-attr)
                          (format nil "~A " (get-retval 'f-attr)) ""))
                  (register-list
                      (if (pattern-variable-p 'r-decl)
                          (get-retval 'r-decl) nil))
                  (vaarg-str
                      (if (pattern-variable-p 'va-arg) ", ..." "")))
              (let ((scs-str (first scs-retval))
                    (arg-str (format nil "~A~A"
                                     (strcat
                                      (mapcar
                                       #'(lambda (xx)
                                           (format nil "~A~A"
                                            (if
                                             (member (second xx)
                                              register-list :test
                                              #'string=)
                                             "register " "")
                                            (if (first xx)
                                             (format nil (first xx)
                                              (if (second xx)
                                               (second xx) ""))
                                             (second xx))))
                                       arg-list)
                                      ", ")
                                     vaarg-str))
                    (body-str
                        (if (and (eq 'decl (second scs-retval))
                                 (null bi-list))
                            ";"
                            (format nil "~%{~%~A}"
                                    (strcat (get-retval 'bi-list))))))
                (format nil "~%~A~A ~A~A~%" inline-str scs-str
                        (format nil texp-str
                                (format nil "~A( ~A )" fid-str arg-str))
                        body-str))))
          nil))
    (let (bi-list va-arg texp-list texp func-id scs
                  (matched-symbol
                      (list (list 'bi-list) (list 'va-arg)
                            (list 'texp-list) (list 'texp)
                            (list 'func-id) (list 'scs))))
      (if (and (listp x) x (cdr x) (cdr (cdr x))
               (listp (car (cdr (cdr x)))) (car (cdr (cdr x)))
               (eq 'fn (car (car (cdr (cdr x)))))
               (cdr (car (cdr (cdr x))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (storage-class-specifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (car x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-identifier (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'func-id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq func-id (car (cdr x))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression
                                 (car (cdr (car (cdr (cdr x)))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (car (cdr (car (cdr (cdr x)))))) t)
               (let ((sc-transformer::crest
                         (cdr (cdr (car (cdr (cdr x)))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and
                                       (= 1 (length sc-transformer::y))
                                       (let
                                        ((sc-transformer::retval
                                          (multiple-value-list
                                           (type-vaarg
                                            (nth 0 sc-transformer::y)))))
                                         (and
                                          (car sc-transformer::retval)
                                          (rplacd
                                           (find 'va-arg matched-symbol
                                            :key #'car)
                                           sc-transformer::retval)))
                                       (progn
                                         (setq va-arg
                                          (nth 0 sc-transformer::y))
                                         t)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car (cdr (cdr x)))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'type-expression
                                       sc-transformer::prev)))
                         (setq texp-list sc-transformer::prev)
                         (rplacd (find 'texp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr (cdr (cdr x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr x)))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((scs-retval (multiple-value-list (get-retval 'scs)))
                  (fid-str (car (get-retval 'func-id)))
                  (texp-str (get-retval 'texp))
                  (arg-list
                      (combine-each-nth
                          (list (get-retval 'texp-list)
                                (cdr (get-retval 'func-id)))
                          :n
                          (max (length texp-list)
                               (length (cdr (get-retval 'func-id))))))
                  (inline-str
                      (if (pattern-variable-p 'f-attr)
                          (format nil "~A " (get-retval 'f-attr)) ""))
                  (register-list
                      (if (pattern-variable-p 'r-decl)
                          (get-retval 'r-decl) nil))
                  (vaarg-str
                      (if (pattern-variable-p 'va-arg) ", ..." "")))
              (let ((scs-str (first scs-retval))
                    (arg-str (format nil "~A~A"
                                     (strcat
                                      (mapcar
                                       #'(lambda (xx)
                                           (format nil "~A~A"
                                            (if
                                             (member (second xx)
                                              register-list :test
                                              #'string=)
                                             "register " "")
                                            (if (first xx)
                                             (format nil (first xx)
                                              (if (second xx)
                                               (second xx) ""))
                                             (second xx))))
                                       arg-list)
                                      ", ")
                                     vaarg-str))
                    (body-str
                        (if (and (eq 'decl (second scs-retval))
                                 (null bi-list))
                            ";"
                            (format nil "~%{~%~A}"
                                    (strcat (get-retval 'bi-list))))))
                (format nil "~%~A~A ~A~A~%" inline-str scs-str
                        (format nil texp-str
                                (format nil "~A( ~A )" fid-str arg-str))
                        body-str))))
          nil))
    (let (bi-list r-decl f-attr texp-list texp func-id scs
                  (matched-symbol
                      (list (list 'bi-list) (list 'r-decl)
                            (list 'f-attr) (list 'texp-list)
                            (list 'texp) (list 'func-id) (list 'scs))))
      (if (and (listp x) x (cdr x) (cdr (cdr x))
               (listp (car (cdr (cdr x)))) (car (cdr (cdr x)))
               (eq 'fn (car (car (cdr (cdr x)))))
               (cdr (car (cdr (cdr x)))) (cdr (cdr (cdr x)))
               (eq ':attr (car (cdr (cdr (cdr x)))))
               (cdr (cdr (cdr (cdr x))))
               (cdr (cdr (cdr (cdr (cdr x)))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (storage-class-specifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (car x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-identifier (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'func-id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq func-id (car (cdr x))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression
                                 (car (cdr (car (cdr (cdr x)))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (car (cdr (car (cdr (cdr x)))))) t)
               (let ((sc-transformer::crest
                         (cdr (cdr (car (cdr (cdr x)))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car (cdr (cdr x)))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'type-expression
                                       sc-transformer::prev)))
                         (setq texp-list sc-transformer::prev)
                         (rplacd (find 'texp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-attribute
                                 (car (cdr (cdr (cdr (cdr x)))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'f-attr matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq f-attr (car (cdr (cdr (cdr (cdr x)))))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (register-declarator
                                 (car (cdr (cdr (cdr (cdr (cdr x))))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'r-decl matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn
                 (setq r-decl (car (cdr (cdr (cdr (cdr (cdr x)))))))
                 t)
               (let ((sc-transformer::crest
                         (cdr (cdr (cdr (cdr (cdr (cdr x))))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr
                                       (cdr (cdr (cdr (cdr (cdr x))))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((scs-retval (multiple-value-list (get-retval 'scs)))
                  (fid-str (car (get-retval 'func-id)))
                  (texp-str (get-retval 'texp))
                  (arg-list
                      (combine-each-nth
                          (list (get-retval 'texp-list)
                                (cdr (get-retval 'func-id)))
                          :n
                          (max (length texp-list)
                               (length (cdr (get-retval 'func-id))))))
                  (inline-str
                      (if (pattern-variable-p 'f-attr)
                          (format nil "~A " (get-retval 'f-attr)) ""))
                  (register-list
                      (if (pattern-variable-p 'r-decl)
                          (get-retval 'r-decl) nil))
                  (vaarg-str
                      (if (pattern-variable-p 'va-arg) ", ..." "")))
              (let ((scs-str (first scs-retval))
                    (arg-str (format nil "~A~A"
                                     (strcat
                                      (mapcar
                                       #'(lambda (xx)
                                           (format nil "~A~A"
                                            (if
                                             (member (second xx)
                                              register-list :test
                                              #'string=)
                                             "register " "")
                                            (if (first xx)
                                             (format nil (first xx)
                                              (if (second xx)
                                               (second xx) ""))
                                             (second xx))))
                                       arg-list)
                                      ", ")
                                     vaarg-str))
                    (body-str
                        (if (and (eq 'decl (second scs-retval))
                                 (null bi-list))
                            ";"
                            (format nil "~%{~%~A}"
                                    (strcat (get-retval 'bi-list))))))
                (format nil "~%~A~A ~A~A~%" inline-str scs-str
                        (format nil texp-str
                                (format nil "~A( ~A )" fid-str arg-str))
                        body-str))))
          nil))
    (let (bi-list f-attr texp-list texp func-id scs
                  (matched-symbol
                      (list (list 'bi-list) (list 'f-attr)
                            (list 'texp-list) (list 'texp)
                            (list 'func-id) (list 'scs))))
      (if (and (listp x) x (cdr x) (cdr (cdr x))
               (listp (car (cdr (cdr x)))) (car (cdr (cdr x)))
               (eq 'fn (car (car (cdr (cdr x)))))
               (cdr (car (cdr (cdr x)))) (cdr (cdr (cdr x)))
               (eq ':attr (car (cdr (cdr (cdr x)))))
               (cdr (cdr (cdr (cdr x))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (storage-class-specifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (car x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-identifier (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'func-id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq func-id (car (cdr x))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression
                                 (car (cdr (car (cdr (cdr x)))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (car (cdr (car (cdr (cdr x)))))) t)
               (let ((sc-transformer::crest
                         (cdr (cdr (car (cdr (cdr x)))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car (cdr (cdr x)))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'type-expression
                                       sc-transformer::prev)))
                         (setq texp-list sc-transformer::prev)
                         (rplacd (find 'texp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-attribute
                                 (car (cdr (cdr (cdr (cdr x)))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'f-attr matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq f-attr (car (cdr (cdr (cdr (cdr x)))))) t)
               (let ((sc-transformer::crest
                         (cdr (cdr (cdr (cdr (cdr x)))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr (cdr (cdr x)))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((scs-retval (multiple-value-list (get-retval 'scs)))
                  (fid-str (car (get-retval 'func-id)))
                  (texp-str (get-retval 'texp))
                  (arg-list
                      (combine-each-nth
                          (list (get-retval 'texp-list)
                                (cdr (get-retval 'func-id)))
                          :n
                          (max (length texp-list)
                               (length (cdr (get-retval 'func-id))))))
                  (inline-str
                      (if (pattern-variable-p 'f-attr)
                          (format nil "~A " (get-retval 'f-attr)) ""))
                  (register-list
                      (if (pattern-variable-p 'r-decl)
                          (get-retval 'r-decl) nil))
                  (vaarg-str
                      (if (pattern-variable-p 'va-arg) ", ..." "")))
              (let ((scs-str (first scs-retval))
                    (arg-str (format nil "~A~A"
                                     (strcat
                                      (mapcar
                                       #'(lambda (xx)
                                           (format nil "~A~A"
                                            (if
                                             (member (second xx)
                                              register-list :test
                                              #'string=)
                                             "register " "")
                                            (if (first xx)
                                             (format nil (first xx)
                                              (if (second xx)
                                               (second xx) ""))
                                             (second xx))))
                                       arg-list)
                                      ", ")
                                     vaarg-str))
                    (body-str
                        (if (and (eq 'decl (second scs-retval))
                                 (null bi-list))
                            ";"
                            (format nil "~%{~%~A}"
                                    (strcat (get-retval 'bi-list))))))
                (format nil "~%~A~A ~A~A~%" inline-str scs-str
                        (format nil texp-str
                                (format nil "~A( ~A )" fid-str arg-str))
                        body-str))))
          nil))
    (let (bi-list r-decl texp-list texp func-id scs
                  (matched-symbol
                      (list (list 'bi-list) (list 'r-decl)
                            (list 'texp-list) (list 'texp)
                            (list 'func-id) (list 'scs))))
      (if (and (listp x) x (cdr x) (cdr (cdr x))
               (listp (car (cdr (cdr x)))) (car (cdr (cdr x)))
               (eq 'fn (car (car (cdr (cdr x)))))
               (cdr (car (cdr (cdr x)))) (cdr (cdr (cdr x)))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (storage-class-specifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (car x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-identifier (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'func-id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq func-id (car (cdr x))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression
                                 (car (cdr (car (cdr (cdr x)))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (car (cdr (car (cdr (cdr x)))))) t)
               (let ((sc-transformer::crest
                         (cdr (cdr (car (cdr (cdr x)))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car (cdr (cdr x)))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'type-expression
                                       sc-transformer::prev)))
                         (setq texp-list sc-transformer::prev)
                         (rplacd (find 'texp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (register-declarator
                                 (car (cdr (cdr (cdr x))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'r-decl matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq r-decl (car (cdr (cdr (cdr x))))) t)
               (let ((sc-transformer::crest (cdr (cdr (cdr (cdr x))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr (cdr x))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((scs-retval (multiple-value-list (get-retval 'scs)))
                  (fid-str (car (get-retval 'func-id)))
                  (texp-str (get-retval 'texp))
                  (arg-list
                      (combine-each-nth
                          (list (get-retval 'texp-list)
                                (cdr (get-retval 'func-id)))
                          :n
                          (max (length texp-list)
                               (length (cdr (get-retval 'func-id))))))
                  (inline-str
                      (if (pattern-variable-p 'f-attr)
                          (format nil "~A " (get-retval 'f-attr)) ""))
                  (register-list
                      (if (pattern-variable-p 'r-decl)
                          (get-retval 'r-decl) nil))
                  (vaarg-str
                      (if (pattern-variable-p 'va-arg) ", ..." "")))
              (let ((scs-str (first scs-retval))
                    (arg-str (format nil "~A~A"
                                     (strcat
                                      (mapcar
                                       #'(lambda (xx)
                                           (format nil "~A~A"
                                            (if
                                             (member (second xx)
                                              register-list :test
                                              #'string=)
                                             "register " "")
                                            (if (first xx)
                                             (format nil (first xx)
                                              (if (second xx)
                                               (second xx) ""))
                                             (second xx))))
                                       arg-list)
                                      ", ")
                                     vaarg-str))
                    (body-str
                        (if (and (eq 'decl (second scs-retval))
                                 (null bi-list))
                            ";"
                            (format nil "~%{~%~A}"
                                    (strcat (get-retval 'bi-list))))))
                (format nil "~%~A~A ~A~A~%" inline-str scs-str
                        (format nil texp-str
                                (format nil "~A( ~A )" fid-str arg-str))
                        body-str))))
          nil))
    (let (bi-list texp-list texp func-id scs
                  (matched-symbol
                      (list (list 'bi-list) (list 'texp-list)
                            (list 'texp) (list 'func-id) (list 'scs))))
      (if (and (listp x) x (cdr x) (cdr (cdr x))
               (listp (car (cdr (cdr x)))) (car (cdr (cdr x)))
               (eq 'fn (car (car (cdr (cdr x)))))
               (cdr (car (cdr (cdr x))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (storage-class-specifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (car x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (function-identifier (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'func-id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq func-id (car (cdr x))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression
                                 (car (cdr (car (cdr (cdr x)))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (car (cdr (car (cdr (cdr x)))))) t)
               (let ((sc-transformer::crest
                         (cdr (cdr (car (cdr (cdr x)))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car (cdr (cdr x)))))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'type-expression
                                       sc-transformer::prev)))
                         (setq texp-list sc-transformer::prev)
                         (rplacd (find 'texp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr (cdr (cdr x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr x)))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((scs-retval (multiple-value-list (get-retval 'scs)))
                  (fid-str (car (get-retval 'func-id)))
                  (texp-str (get-retval 'texp))
                  (arg-list
                      (combine-each-nth
                          (list (get-retval 'texp-list)
                                (cdr (get-retval 'func-id)))
                          :n
                          (max (length texp-list)
                               (length (cdr (get-retval 'func-id))))))
                  (inline-str
                      (if (pattern-variable-p 'f-attr)
                          (format nil "~A " (get-retval 'f-attr)) ""))
                  (register-list
                      (if (pattern-variable-p 'r-decl)
                          (get-retval 'r-decl) nil))
                  (vaarg-str
                      (if (pattern-variable-p 'va-arg) ", ..." "")))
              (let ((scs-str (first scs-retval))
                    (arg-str (format nil "~A~A"
                                     (strcat
                                      (mapcar
                                       #'(lambda (xx)
                                           (format nil "~A~A"
                                            (if
                                             (member (second xx)
                                              register-list :test
                                              #'string=)
                                             "register " "")
                                            (if (first xx)
                                             (format nil (first xx)
                                              (if (second xx)
                                               (second xx) ""))
                                             (second xx))))
                                       arg-list)
                                      ", ")
                                     vaarg-str))
                    (body-str
                        (if (and (eq 'decl (second scs-retval))
                                 (null bi-list))
                            ";"
                            (format nil "~%{~%~A}"
                                    (strcat (get-retval 'bi-list))))))
                (format nil "~%~A~A ~A~A~%" inline-str scs-str
                        (format nil texp-str
                                (format nil "~A( ~A )" fid-str arg-str))
                        body-str))))
          nil))
    (let (sdcl-list su-spec dd
             (matched-symbol
                 (list (list 'sdcl-list) (list 'su-spec) (list 'dd))))
      (if (and (listp x) x (cdr x)
               (let ((sc-transformer::retval
                         (multiple-value-list (def-or-decl (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'dd matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq dd (car x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (struct-or-union-specifier (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'su-spec matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq su-spec (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar
                                       #'struct-declaration
                                       sc-transformer::prev)))
                         (setq sdcl-list sc-transformer::prev)
                         (rplacd (find 'sdcl-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (if (and (eq 'decl dd) (null sdcl-list))
                (format nil "~A;~%" (get-retval 'su-spec))
                (format nil "~%~A {~%~A};~%" (get-retval 'su-spec)
                        (strcat (get-retval 'sdcl-list)))))
          nil))
    (let (en-list espec
                  (matched-symbol (list (list 'en-list) (list 'espec))))
      (if (and (listp x) x (eq 'def (car x)) (cdr x)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (enum-specifier (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'espec matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq espec (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'enumerator
                                       sc-transformer::prev)))
                         (setq en-list sc-transformer::prev)
                         (rplacd (find 'en-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (format nil "~A { ~A };~%" (get-retval 'espec)
                    (strcat (get-retval 'en-list) ", ")))
          nil))
    (let (idec-list texp spec
             (matched-symbol
                 (list (list 'idec-list) (list 'texp) (list 'spec))))
      (if (and (listp x) x (cdr x)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (compound-storage-class-specifier (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'spec matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq spec (car x)) t)
               (progn (setq texp (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'init-declarator
                                       sc-transformer::prev)))
                         (setq idec-list sc-transformer::prev)
                         (rplacd (find 'idec-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((spec-retval (get-retval 'spec))
                  (idec-retval-list (get-retval 'idec-list)))
              (strcat (mapcar #'(lambda (xx)
                                  (inlined-declaration
                                      (list* spec-retval (first xx)
                                       texp (cdr xx))))
                              idec-retval-list))))
          nil))
    (let (texp id (matched-symbol (list (list 'texp) (list 'id))))
      (if (and (listp x) (= 3 (length x)) (eq 'deftype (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 1 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression (nth 2 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (nth 2 x)) t))
          (return-from sc-transformer::pattern-matched
            (format nil "typedef ~A;~%"
                    (format nil (get-retval 'texp) (get-retval 'id))))
          nil))
    (let (sdcl-list su id
             (matched-symbol
                 (list (list 'sdcl-list) (list 'su) (list 'id))))
      (if (and (listp x) x (eq 'deftype (car x)) (cdr x) (cdr (cdr x))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (car (cdr x))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (struct-or-union (car (cdr (cdr x)))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'su matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq su (car (cdr (cdr x)))) t)
               (let ((sc-transformer::crest (cdr (cdr (cdr x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr x)))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar
                                       #'struct-declaration
                                       sc-transformer::prev)))
                         (setq sdcl-list sc-transformer::prev)
                         (rplacd (find 'sdcl-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (format nil "typedef ~A {~%~A } ~A;~%" (get-retval 'su)
                    (strcat (get-retval 'sdcl-list)) (get-retval 'id)))
          nil))
    (let (en-list id
                  (matched-symbol (list (list 'en-list) (list 'id))))
      (if (and (listp x) x (eq 'deftype (car x)) (cdr x) (cdr (cdr x))
               (eq 'enum (car (cdr (cdr x))))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr (cdr x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr x)))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'enumerator
                                       sc-transformer::prev)))
                         (setq en-list sc-transformer::prev)
                         (rplacd (find 'en-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (format nil "typedef enum { ~A } ~A;~%"
                    (strcat (get-retval 'en-list) ", ")
                    (get-retval 'id)))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun function-attribute (x)
  (block sc-transformer::pattern-matched
    (let ((matched-symbol (list)))
      (if (and (eq 'inline x))
          (return-from sc-transformer::pattern-matched "inline") nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun register-declarator (x)
  (block sc-transformer::pattern-matched
    (let (id-list (matched-symbol (list (list 'id-list))))
      (if (and (listp x) x (eq 'register (car x))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'id
                                       sc-transformer::prev)))
                         (setq id-list sc-transformer::prev)
                         (rplacd (find 'id-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (get-retval id-list))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun function-identifier (x)
  (block sc-transformer::pattern-matched
    (let (id (matched-symbol (list (list 'id))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (id x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id x) t))
          (return-from sc-transformer::pattern-matched
            (list (get-retval 'id)))
          nil))
    (let (id-list (matched-symbol (list (list 'id-list))))
      (if (and (listp x)
               (let ((sc-transformer::crest x))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff x sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'id
                                       sc-transformer::prev)))
                         (setq id-list sc-transformer::prev)
                         (rplacd (find 'id-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (get-retval 'id-list))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun def-or-decl (x)
  (block sc-transformer::pattern-matched
    (let ((matched-symbol (list)))
      (if (and (eq 'def x))
          (return-from sc-transformer::pattern-matched
            (values "" 'def))
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'decl x))
          (return-from sc-transformer::pattern-matched
            (values "" 'decl))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun init-declarator (x)
  (block sc-transformer::pattern-matched
    (let (id (matched-symbol (list (list 'id))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (id x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id x) t))
          (return-from sc-transformer::pattern-matched (list id)) nil))
    (let (init id (matched-symbol (list (list 'init) (list 'id))))
      (if (and (listp x) (= 2 (length x))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (nth 0 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 0 x)) t)
               (progn (setq init (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched (list id init))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun storage-class-specifier (x)
  (block sc-transformer::pattern-matched
    (let ((matched-symbol (list)))
      (if (and (eq 'def x))
          (return-from sc-transformer::pattern-matched
            (values "" 'def))
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'decl x))
          (return-from sc-transformer::pattern-matched
            (values "" 'decl))
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'extern x))
          (return-from sc-transformer::pattern-matched
            (values "extern" 'def))
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'extern-def x))
          (return-from sc-transformer::pattern-matched
            (values "extern" 'def))
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'extern-decl x))
          (return-from sc-transformer::pattern-matched
            (values "extern" 'decl))
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'static x))
          (return-from sc-transformer::pattern-matched
            (values "static" 'def))
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'static-def x))
          (return-from sc-transformer::pattern-matched
            (values "static" 'def))
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'auto x))
          (return-from sc-transformer::pattern-matched
            (values "auto" 'def))
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'auto-def x))
          (return-from sc-transformer::pattern-matched
            (values "auto" 'def))
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'register x))
          (return-from sc-transformer::pattern-matched
            (values "register" 'def))
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'register-def x))
          (return-from sc-transformer::pattern-matched
            (values "register" 'def))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun compound-storage-class-specifier (x)
  (block sc-transformer::pattern-matched
    (let ((matched-symbol (list)))
      (if (and (eq 'defs x))
          (return-from sc-transformer::pattern-matched 'def) nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'extern-defs x))
          (return-from sc-transformer::pattern-matched 'extern) nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'static-defs x))
          (return-from sc-transformer::pattern-matched 'static) nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'auto-defs x))
          (return-from sc-transformer::pattern-matched 'auto) nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'register-defs x))
          (return-from sc-transformer::pattern-matched 'register) nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun struct-declaration (x)
  (block sc-transformer::pattern-matched
    (let (decl (matched-symbol (list (list 'decl))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (declaration x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'decl matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq decl x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'decl))
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq ':bit x))
          (return-from sc-transformer::pattern-matched " : ") nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (expression x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp x) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval (multiple-value-list (get-retval 'exp)))
                  (assoc 14))
              (format nil "~A~%" (parenthesize exp-retval assoc))))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun enumerator (x)
  (block sc-transformer::pattern-matched
    (let (en-const (matched-symbol (list (list 'en-const))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (enumeration-constant x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'en-const matched-symbol :key
                                    #'car)
                              sc-transformer::retval)))
               (progn (setq en-const x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'en-const))
          nil))
    (let (exp en-const
              (matched-symbol (list (list 'exp) (list 'en-const))))
      (if (and (listp x) (= 2 (length x))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (enumeration-constant (nth 0 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'en-const matched-symbol :key
                                    #'car)
                              sc-transformer::retval)))
               (progn (setq en-const (nth 0 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval (multiple-value-list (get-retval 'exp))))
              (format nil "~A=~A" (get-retval 'en-const)
                      (parenthesize exp-retval 15))))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun enumeration-constant (x)
  (block sc-transformer::pattern-matched
    (let (id (matched-symbol (list (list 'id))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (id x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'id))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun compound-initializer (x)
  (block sc-transformer::pattern-matched
    (let (init-list (matched-symbol (list (list 'init-list))))
      (if (and (listp x) x (eq 'array (car x))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'initializer
                                       sc-transformer::prev)))
                         (setq init-list sc-transformer::prev)
                         (rplacd (find 'init-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((init-retval-list
                      (combine-each-nth
                          (multiple-value-list (get-retval 'init-list)))))
              (values (format nil "{ ~A }"
                              (strcat (mapcar
                                       #'(lambda (xx)
                                           (parenthesize xx 16))
                                       init-retval-list)
                                      ", "))
                      0)))
          nil))
    (let (init-list (matched-symbol (list (list 'init-list))))
      (if (and (listp x) x (eq 'struct (car x))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'initializer
                                       sc-transformer::prev)))
                         (setq init-list sc-transformer::prev)
                         (rplacd (find 'init-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((init-retval-list
                      (combine-each-nth
                          (multiple-value-list (get-retval 'init-list)))))
              (values (format nil "{ ~A }"
                              (strcat (mapcar
                                       #'(lambda (xx)
                                           (parenthesize xx 16))
                                       init-retval-list)
                                      ", "))
                      0)))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun initializer (x)
  (block sc-transformer::pattern-matched
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (expression x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'exp))
          nil))
    (let (cmpd-init (matched-symbol (list (list 'cmpd-init))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (compound-initializer x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'cmpd-init matched-symbol :key
                                    #'car)
                              sc-transformer::retval)))
               (progn (setq cmpd-init x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'cmpd-init))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun type-expression (x)
  (block sc-transformer::pattern-matched
    (let (tspec (matched-symbol (list (list 'tspec))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (type-specifier x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'tspec matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq tspec x) t))
          (return-from sc-transformer::pattern-matched
            (values (format nil "~A ~~A" (get-retval 'tspec)) 17))
          nil))
    (let (texp (matched-symbol (list (list 'texp))))
      (if (and (listp x) (= 2 (length x)) (eq 'array (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((retval-texp
                      (multiple-value-list (get-retval 'texp)))
                  (assoc 1))
              (values (format nil (first retval-texp)
                              (parenthesize-t (format nil "~~A[]")
                                  (second retval-texp) assoc))
                      assoc)))
          nil))
    (let (exp-list texp
             (matched-symbol (list (list 'exp-list) (list 'texp))))
      (if (and (listp x) x (eq 'array (car x)) (cdr x)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'expression
                                       sc-transformer::prev)))
                         (setq exp-list sc-transformer::prev)
                         (rplacd (find 'exp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((retval-texp
                      (multiple-value-list (get-retval 'texp)))
                  (assoc 1))
              (values (format nil (first retval-texp)
                              (parenthesize-t
                                  (format nil "~~A[~A]"
                                          (strcat
                                           (get-retval 'exp-list) "]["))
                                  (second retval-texp) assoc))
                      assoc)))
          nil))
    (let (texp (matched-symbol (list (list 'texp))))
      (if (and (listp x) (= 2 (length x)) (eq 'ptr (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((retval-texp
                      (multiple-value-list (get-retval 'texp)))
                  (assoc 2))
              (values (format nil (first retval-texp)
                              (parenthesize-t (format nil "*~~A")
                                  (second retval-texp) assoc))
                      assoc)))
          nil))
    (let (texp-list texp1
             (matched-symbol (list (list 'texp-list) (list 'texp1))))
      (if (and (listp x) x (eq 'fn (car x)) (cdr x)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp1 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'type-expression
                                       sc-transformer::prev)))
                         (setq texp-list sc-transformer::prev)
                         (rplacd (find 'texp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((retval-texp1
                      (multiple-value-list (get-retval 'texp1)))
                  (assoc 1))
              (values (format nil (first retval-texp1)
                              (parenthesize-t
                                  (format nil "~~A( ~A~A )"
                                          (strcat
                                           (mapcar
                                            #'(lambda (xx)
                                                (format nil xx ""))
                                            (get-retval 'texp-list))
                                           ", ")
                                          (if
                                           (pattern-variable-p 'va-arg)
                                           (format nil ", ~A"
                                            (get-retval 'va-arg))
                                           ""))
                                  (second retval-texp1) assoc))
                      assoc)))
          nil))
    (let (va-arg texp-list texp1
                 (matched-symbol
                     (list (list 'va-arg) (list 'texp-list)
                           (list 'texp1))))
      (if (and (listp x) x (eq 'fn (car x)) (cdr x)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp1 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and
                                       (= 1 (length sc-transformer::y))
                                       (let
                                        ((sc-transformer::retval
                                          (multiple-value-list
                                           (type-vaarg
                                            (nth 0 sc-transformer::y)))))
                                         (and
                                          (car sc-transformer::retval)
                                          (rplacd
                                           (find 'va-arg matched-symbol
                                            :key #'car)
                                           sc-transformer::retval)))
                                       (progn
                                         (setq va-arg
                                          (nth 0 sc-transformer::y))
                                         t)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'type-expression
                                       sc-transformer::prev)))
                         (setq texp-list sc-transformer::prev)
                         (rplacd (find 'texp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((retval-texp1
                      (multiple-value-list (get-retval 'texp1)))
                  (assoc 1))
              (values (format nil (first retval-texp1)
                              (parenthesize-t
                                  (format nil "~~A( ~A~A )"
                                          (strcat
                                           (mapcar
                                            #'(lambda (xx)
                                                (format nil xx ""))
                                            (get-retval 'texp-list))
                                           ", ")
                                          (if
                                           (pattern-variable-p 'va-arg)
                                           (format nil ", ~A"
                                            (get-retval 'va-arg))
                                           ""))
                                  (second retval-texp1) assoc))
                      assoc)))
          nil))
    (let (texp tq-list
               (matched-symbol (list (list 'texp) (list 'tq-list))))
      (if (and (listp x)
               (let ((sc-transformer::crest x))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and
                                       (= 1 (length sc-transformer::y))
                                       (let
                                        ((sc-transformer::retval
                                          (multiple-value-list
                                           (type-expression
                                            (nth 0 sc-transformer::y)))))
                                         (and
                                          (car sc-transformer::retval)
                                          (rplacd
                                           (find 'texp matched-symbol
                                            :key #'car)
                                           sc-transformer::retval)))
                                       (progn
                                         (setq texp
                                          (nth 0 sc-transformer::y))
                                         t)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff x sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'type-qualifier
                                       sc-transformer::prev)))
                         (setq tq-list sc-transformer::prev)
                         (rplacd (find 'tq-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (values (format nil (get-retval 'texp)
                            (format nil " ~A ~~A"
                                    (strcat (get-retval 'tq-list) " ")))
                    17))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun type-vaarg (x)
  (block sc-transformer::pattern-matched
    (let ((matched-symbol (list)))
      (if (and (eq 'va-arg x))
          (return-from sc-transformer::pattern-matched "...") nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun type-specifier (x)
  (block sc-transformer::pattern-matched
    (let ((matched-symbol (list)))
      (if (and (eq 'void x))
          (return-from sc-transformer::pattern-matched "void") nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'char x))
          (return-from sc-transformer::pattern-matched "char") nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'signed-char x))
          (return-from sc-transformer::pattern-matched "signed char")
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'unsigned-char x))
          (return-from sc-transformer::pattern-matched
            "unsigned char")
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'short x))
          (return-from sc-transformer::pattern-matched "short") nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'signed-short x))
          (return-from sc-transformer::pattern-matched "signed short")
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'unsigned-short x))
          (return-from sc-transformer::pattern-matched
            "unsigned short")
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'int x))
          (return-from sc-transformer::pattern-matched "int") nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'signed-int x))
          (return-from sc-transformer::pattern-matched "signed int")
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'unsigned-int x))
          (return-from sc-transformer::pattern-matched "unsigned int")
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'long x))
          (return-from sc-transformer::pattern-matched "long") nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'signed-long x))
          (return-from sc-transformer::pattern-matched "signed long")
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'unsigned-long x))
          (return-from sc-transformer::pattern-matched
            "unsigned long")
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'long-long x))
          (return-from sc-transformer::pattern-matched "long long")
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'signed-long-long x))
          (return-from sc-transformer::pattern-matched
            "signed long long")
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'unsigned-long-long x))
          (return-from sc-transformer::pattern-matched
            "unsigned long long")
          nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'float x))
          (return-from sc-transformer::pattern-matched "float") nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'double x))
          (return-from sc-transformer::pattern-matched "double") nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'long-double x))
          (return-from sc-transformer::pattern-matched "long double")
          nil))
    (let (other (matched-symbol (list (list 'other))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list
                             (struct-or-union-specifier x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'other matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq other x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'other))
          nil))
    (let (other (matched-symbol (list (list 'other))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (enum-specifier x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'other matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq other x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'other))
          nil))
    (let (other (matched-symbol (list (list 'other))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (typedef-name x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'other matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq other x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'other))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun struct-or-union-specifier (x)
  (block sc-transformer::pattern-matched
    (let (id s-u (matched-symbol (list (list 'id) (list 's-u))))
      (if (and (listp x) (= 2 (length x))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (struct-or-union (nth 0 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 's-u matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq s-u (nth 0 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (id (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (format nil "~A ~A" (get-retval 's-u) (get-retval 'id)))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun struct-or-union (x)
  (block sc-transformer::pattern-matched
    (let ((matched-symbol (list)))
      (if (and (eq 'struct x))
          (return-from sc-transformer::pattern-matched "struct") nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'union x))
          (return-from sc-transformer::pattern-matched "union") nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun enum-specifier (x)
  (block sc-transformer::pattern-matched
    (let (id (matched-symbol (list (list 'id))))
      (if (and (listp x) (= 2 (length x)) (eq 'enum (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (format nil "enum ~A" (get-retval 'id)))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun type-qualifier (x)
  (block sc-transformer::pattern-matched
    (let ((matched-symbol (list)))
      (if (and (eq 'const x))
          (return-from sc-transformer::pattern-matched "const") nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'restrict x))
          (return-from sc-transformer::pattern-matched "restrict") nil))
    (let ((matched-symbol (list)))
      (if (and (eq 'volatile x))
          (return-from sc-transformer::pattern-matched "volatile") nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun typedef-name (x)
  (block sc-transformer::pattern-matched
    (let (id (matched-symbol (list (list 'id))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (id x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'id))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun statement (x)
  (block sc-transformer::pattern-matched
    (let (stat (matched-symbol (list (list 'stat))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (compound-statement x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'stat matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq stat x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'stat))
          nil))
    (let (stat (matched-symbol (list (list 'stat))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (expression-statement x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'stat matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq stat x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'stat))
          nil))
    (let (stat (matched-symbol (list (list 'stat))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (selection-statement x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'stat matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq stat x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'stat))
          nil))
    (let (stat (matched-symbol (list (list 'stat))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (iteration-statement x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'stat matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq stat x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'stat))
          nil))
    (let (stat (matched-symbol (list (list 'stat))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (jump-statement x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'stat matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq stat x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'stat))
          nil))
    (let (stat (matched-symbol (list (list 'stat))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (labeled-statement x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'stat matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq stat x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'stat))
          nil))
    (let ((matched-symbol (list)))
      (if (and (listp x) (endp x))
          (return-from sc-transformer::pattern-matched
            (format nil ";~%"))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun compound-statement (x)
  (block sc-transformer::pattern-matched
    (let (bi-list (matched-symbol (list (list 'bi-list))))
      (if (and (listp x) x (eq 'begin (car x))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (format nil "{~%~A}~%" (strcat (get-retval 'bi-list))))
          nil))
    (let (bi-list decl-list
                  (matched-symbol
                      (list (list 'bi-list) (list 'decl-list))))
      (if (and (listp x) x (eq 'let (car x)) (cdr x)
               (listp (car (cdr x)))
               (let ((sc-transformer::crest (car (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (car (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'declaration
                                       sc-transformer::prev)))
                         (setq decl-list sc-transformer::prev)
                         (rplacd (find 'decl-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (format nil "{~%~A~%~A}~%" (strcat (get-retval 'decl-list))
                    (strcat (get-retval 'bi-list))))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun block-item (x)
  (block sc-transformer::pattern-matched
    (let (bi (matched-symbol (list (list 'bi))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (inlined-declaration x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'bi matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq bi x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'bi))
          nil))
    (let (bi (matched-symbol (list (list 'bi))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (statement x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'bi matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq bi x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'bi))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun labeled-statement (x)
  (block sc-transformer::pattern-matched
    (let (stat id (matched-symbol (list (list 'stat) (list 'id))))
      (if (and (listp x) (= 3 (length x)) (eq 'label (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 1 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (statement (nth 2 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'stat matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq stat (nth 2 x)) t))
          (return-from sc-transformer::pattern-matched
            (format nil "~A:~%~A" (get-retval 'id) (get-retval 'stat)))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq 'case (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (format nil "case ~A:~%" (get-retval 'exp)))
          nil))
    (let ((matched-symbol (list)))
      (if (and (listp x) (= 1 (length x)) (eq 'default (nth 0 x)))
          (return-from sc-transformer::pattern-matched
            (format nil "default:~%"))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun expression-statement (x)
  (block sc-transformer::pattern-matched
    (let (exp-stat (matched-symbol (list (list 'exp-stat))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (expression x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp-stat matched-symbol :key
                                    #'car)
                              sc-transformer::retval)))
               (progn (setq exp-stat x) t))
          (return-from sc-transformer::pattern-matched
            (format nil "~A;~%" (get-retval 'exp-stat)))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun selection-statement (x)
  (block sc-transformer::pattern-matched
    (let (stat1 exp (matched-symbol (list (list 'stat1) (list 'exp))))
      (if (and (listp x) (= 3 (length x)) (eq 'if (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (statement (nth 2 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'stat1 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq stat1 (nth 2 x)) t))
          (return-from sc-transformer::pattern-matched
            (format nil "if( ~A )~% ~A ~A" (get-retval 'exp)
                    (get-retval 'stat1)
                    (if (pattern-variable-p 'stat2)
                        (format nil " else ~A" (get-retval 'stat2)) "")))
          nil))
    (let (stat2 stat1 exp
                (matched-symbol
                    (list (list 'stat2) (list 'stat1) (list 'exp))))
      (if (and (listp x) (= 4 (length x)) (eq 'if (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (statement (nth 2 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'stat1 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq stat1 (nth 2 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (statement (nth 3 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'stat2 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq stat2 (nth 3 x)) t))
          (return-from sc-transformer::pattern-matched
            (format nil "if( ~A )~% ~A ~A" (get-retval 'exp)
                    (get-retval 'stat1)
                    (if (pattern-variable-p 'stat2)
                        (format nil " else ~A" (get-retval 'stat2)) "")))
          nil))
    (let (bi-list exp
                  (matched-symbol (list (list 'bi-list) (list 'exp))))
      (if (and (listp x) x (eq 'switch (car x)) (cdr x)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (expression (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (format nil "switch( ~A )~%{~%~A}~%" (get-retval 'exp)
                    (strcat (get-retval 'bi-list))))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun iteration-statement (x)
  (block sc-transformer::pattern-matched
    (let (bi-list exp
                  (matched-symbol (list (list 'bi-list) (list 'exp))))
      (if (and (listp x) x (eq 'while (car x)) (cdr x)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (expression (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((prev nil) (post nil))
              (case (car x)
                ((while)
                 (setq prev
                       (format nil "while( ~A )" (get-retval 'exp))
                       post ""))
                ((do-while)
                 (setq prev "do " post
                       (format nil "while( ~A );" (get-retval 'exp))))
                (otherwise
                    (error "unexpected keyword ~S in iteration-statement."
                           (car x))))
              (format nil "~A~%{~%~A}~A~%" prev
                      (strcat (get-retval 'bi-list)) post)))
          nil))
    (let (bi-list exp
                  (matched-symbol (list (list 'bi-list) (list 'exp))))
      (if (and (listp x) x (eq 'do-while (car x)) (cdr x)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (expression (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((prev nil) (post nil))
              (case (car x)
                ((while)
                 (setq prev
                       (format nil "while( ~A )" (get-retval 'exp))
                       post ""))
                ((do-while)
                 (setq prev "do " post
                       (format nil "while( ~A );" (get-retval 'exp))))
                (otherwise
                    (error "unexpected keyword ~S in iteration-statement."
                           (car x))))
              (format nil "~A~%{~%~A}~A~%" prev
                      (strcat (get-retval 'bi-list)) post)))
          nil))
    (let (bi-list (matched-symbol (list (list 'bi-list))))
      (if (and (listp x) x (eq 'loop (car x))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (iteration-statement (list* 'while 1 bi-list)))
          nil))
    (let (bi-list exp2 exp1 exp-list
                  (matched-symbol
                      (list (list 'bi-list) (list 'exp2) (list 'exp1)
                            (list 'exp-list))))
      (if (and (listp x) x (eq 'for (car x)) (cdr x)
               (listp (car (cdr x)))
               (let ((sc-transformer::crest (car (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and
                                       (= 2 (length sc-transformer::y))
                                       (let
                                        ((sc-transformer::retval
                                          (multiple-value-list
                                           (expression
                                            (nth 0 sc-transformer::y)))))
                                         (and
                                          (car sc-transformer::retval)
                                          (rplacd
                                           (find 'exp1 matched-symbol
                                            :key #'car)
                                           sc-transformer::retval)))
                                       (progn
                                         (setq exp1
                                          (nth 0 sc-transformer::y))
                                         t)
                                       (let
                                        ((sc-transformer::retval
                                          (multiple-value-list
                                           (expression
                                            (nth 1 sc-transformer::y)))))
                                         (and
                                          (car sc-transformer::retval)
                                          (rplacd
                                           (find 'exp2 matched-symbol
                                            :key #'car)
                                           sc-transformer::retval)))
                                       (progn
                                         (setq exp2
                                          (nth 1 sc-transformer::y))
                                         t)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (car (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'expression
                                       sc-transformer::prev)))
                         (setq exp-list sc-transformer::prev)
                         (rplacd (find 'exp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval-list
                      (combine-each-nth
                          (multiple-value-list (get-retval 'exp-list)))))
              (format nil "for( ~A ; ~A ; ~A )~%{~%~A}~%"
                      (strcat (mapcar #'(lambda (xx)
                                          (parenthesize xx 16))
                                      exp-retval-list)
                              ", ")
                      (get-retval 'exp1) (get-retval 'exp2)
                      (strcat (get-retval 'bi-list)))))
          nil))
    (let (bi-list exp2 exp1 idecl-list
                  (matched-symbol
                      (list (list 'bi-list) (list 'exp2) (list 'exp1)
                            (list 'idecl-list))))
      (if (and (listp x) x (eq 'for (car x)) (cdr x)
               (listp (car (cdr x)))
               (let ((sc-transformer::crest (car (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and
                                       (= 2 (length sc-transformer::y))
                                       (let
                                        ((sc-transformer::retval
                                          (multiple-value-list
                                           (expression
                                            (nth 0 sc-transformer::y)))))
                                         (and
                                          (car sc-transformer::retval)
                                          (rplacd
                                           (find 'exp1 matched-symbol
                                            :key #'car)
                                           sc-transformer::retval)))
                                       (progn
                                         (setq exp1
                                          (nth 0 sc-transformer::y))
                                         t)
                                       (let
                                        ((sc-transformer::retval
                                          (multiple-value-list
                                           (expression
                                            (nth 1 sc-transformer::y)))))
                                         (and
                                          (car sc-transformer::retval)
                                          (rplacd
                                           (find 'exp2 matched-symbol
                                            :key #'car)
                                           sc-transformer::retval)))
                                       (progn
                                         (setq exp2
                                          (nth 1 sc-transformer::y))
                                         t)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (car (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar
                                       #'inlined-declaration
                                       sc-transformer::prev)))
                         (setq idecl-list sc-transformer::prev)
                         (rplacd (find 'idecl-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'block-item
                                       sc-transformer::prev)))
                         (setq bi-list sc-transformer::prev)
                         (rplacd (find 'bi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (format nil "{~%~A~A}~%" (strcat (get-retval 'idecl-list))
                    (iteration-statement
                        (list* 'for (list exp1 exp2) bi-list))))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun jump-statement (x)
  (block sc-transformer::pattern-matched
    (let (id (matched-symbol (list (list 'id))))
      (if (and (listp x) (= 2 (length x)) (eq 'goto (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (format nil "goto ~A;~%" (get-retval 'id)))
          nil))
    (let ((matched-symbol (list)))
      (if (and (listp x) (= 1 (length x)) (eq 'continue (nth 0 x)))
          (return-from sc-transformer::pattern-matched
            (format nil "continue;~%"))
          nil))
    (let ((matched-symbol (list)))
      (if (and (listp x) (= 1 (length x)) (eq 'break (nth 0 x)))
          (return-from sc-transformer::pattern-matched
            (format nil "break;~%"))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq 'return (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (format nil "return ~A;~%"
                    (if (pattern-variable-p 'exp) (get-retval 'exp) "")))
          nil))
    (let ((matched-symbol (list)))
      (if (and (listp x) (= 1 (length x)) (eq 'return (nth 0 x)))
          (return-from sc-transformer::pattern-matched
            (format nil "return ~A;~%"
                    (if (pattern-variable-p 'exp) (get-retval 'exp) "")))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun expression (x)
  (block sc-transformer::pattern-matched
    (let (id (matched-symbol (list (list 'id))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (id x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id x) t))
          (return-from sc-transformer::pattern-matched
            (values (get-retval 'id) 0))
          nil))
    (let (int (matched-symbol (list (list 'int))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (integerp x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'int matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq int x) t))
          (return-from sc-transformer::pattern-matched
            (values (write-to-string x) 0))
          nil))
    (let (flt (matched-symbol (list (list 'flt))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (floatp x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'flt matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq flt x) t))
          (return-from sc-transformer::pattern-matched
            (values (write-to-string x) 0))
          nil))
    (let (char (matched-symbol (list (list 'char))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (characterp x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'char matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq char x) t))
          (return-from sc-transformer::pattern-matched
            (values (format nil "'~A'" (char-sc2c char)) 0))
          nil))
    (let (str (matched-symbol (list (list 'str))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (stringp x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'str matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq str x) t))
          (return-from sc-transformer::pattern-matched
            (values (format nil "\"~A\"" (string-sc2c str)) 0))
          nil))
    (let (exp-list fexp
             (matched-symbol (list (list 'exp-list) (list 'fexp))))
      (if (and (listp x) x
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'fexp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq fexp (car x)) t)
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'expression
                                       sc-transformer::prev)))
                         (setq exp-list sc-transformer::prev)
                         (rplacd (find 'exp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((fexp-retval
                      (multiple-value-list (get-retval 'fexp)))
                  (exp-retval-list
                      (combine-each-nth
                          (multiple-value-list (get-retval 'exp-list))))
                  (assoc 1))
              (values (format nil "~A( ~A )"
                              (parenthesize fexp-retval assoc)
                              (strcat (mapcar
                                       #'(lambda (xx)
                                           (parenthesize xx 16))
                                       exp-retval-list)
                                      ", "))
                      assoc)))
          nil))
    (let (exp-list exp1
             (matched-symbol (list (list 'exp-list) (list 'exp1))))
      (if (and (listp x) x (eq 'aref (car x)) (cdr x)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (expression (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp1 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'expression
                                       sc-transformer::prev)))
                         (setq exp-list sc-transformer::prev)
                         (rplacd (find 'exp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (if (null exp-list) (get-retval 'exp1)
                (let ((exp1-retval
                          (multiple-value-list (get-retval 'exp1)))
                      (exp-retval-list
                          (combine-each-nth
                              (multiple-value-list
                                  (get-retval 'exp-list))))
                      (assoc 1))
                  (values (format nil "~A[~A]"
                                  (parenthesize exp1-retval assoc)
                                  (strcat (mapcar
                                           #'(lambda (xx)
                                               (parenthesize xx 17))
                                           exp-retval-list)
                                          "]["))
                          assoc))))
          nil))
    (let (fi-list exp
                  (matched-symbol (list (list 'fi-list) (list 'exp))))
      (if (and (listp x) x (eq 'fref (car x)) (cdr x)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (expression (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'field-identifier
                                       sc-transformer::prev)))
                         (setq fi-list sc-transformer::prev)
                         (rplacd (find 'fi-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval (multiple-value-list (get-retval 'exp)))
                  (fi-str-list0 (get-retval 'fi-list))
                  (fi-str-list nil) (assoc 1))
              (do ((str fi-str-list0 (cdr str)))
                  ((endp str) (setq fi-str-list (reverse fi-str-list)))
                (if (eq '-> (car str))
                    (progn
                      (push "->" fi-str-list)
                      (setq str (cdr str)))
                    (push "." fi-str-list))
                (push (car str) fi-str-list))
              (values (format nil "~A~A"
                              (parenthesize exp-retval assoc)
                              (strcat fi-str-list))
                      assoc)))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq 'inc (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval (multiple-value-list (get-retval 'exp)))
                  (op-str (operator-sc2c (car x))) (assoc 1))
              (values (format nil "~A~A"
                              (parenthesize exp-retval assoc) op-str)
                      assoc)))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq 'dec (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval (multiple-value-list (get-retval 'exp)))
                  (op-str (operator-sc2c (car x))) (assoc 1))
              (values (format nil "~A~A"
                              (parenthesize exp-retval assoc) op-str)
                      assoc)))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq '++ (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval (multiple-value-list (get-retval 'exp)))
                  (op-str (operator-sc2c (car x))) (assoc 2))
              (values (format nil "~A~A" op-str
                              (parenthesize exp-retval assoc))
                      assoc)))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq '-- (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval (multiple-value-list (get-retval 'exp)))
                  (op-str (operator-sc2c (car x))) (assoc 2))
              (values (format nil "~A~A" op-str
                              (parenthesize exp-retval assoc))
                      assoc)))
          nil))
    (let (exp uop (matched-symbol (list (list 'exp) (list 'uop))))
      (if (and (listp x) (= 2 (length x))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (unary-operator (nth 0 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'uop matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq uop (nth 0 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval (multiple-value-list (get-retval 'exp)))
                  (uop-retval (multiple-value-list (get-retval 'uop))))
              (let ((uop-str (first uop-retval))
                    (uop-assoc (second uop-retval)))
                (values (format nil "~A~A" uop-str
                                (parenthesize exp-retval uop-assoc))
                        uop-assoc))))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq '+ (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval (multiple-value-list (get-retval 'exp)))
                  (assoc 2))
              (values (format nil "~A~A" (operator-sc2c (car x))
                              (parenthesize exp-retval assoc))
                      assoc)))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq '- (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval (multiple-value-list (get-retval 'exp)))
                  (assoc 2))
              (values (format nil "~A~A" (operator-sc2c (car x))
                              (parenthesize exp-retval assoc))
                      assoc)))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq '/ (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (expression (list '/ 1 exp)))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq 'sizeof (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval (get-retval 'exp)) (assoc 2))
              (values (format nil "sizeof(~A)" exp-retval) assoc)))
          nil))
    (let (texp (matched-symbol (list (list 'texp))))
      (if (and (listp x) (= 2 (length x)) (eq 'sizeof (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((texp-str (format nil (get-retval 'texp) ""))
                  (assoc 2))
              (values (format nil "sizeof(~A)" texp-str) assoc)))
          nil))
    (let (exp texp (matched-symbol (list (list 'exp) (list 'texp))))
      (if (and (listp x) (= 3 (length x)) (eq 'cast (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (type-expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'texp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq texp (nth 1 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 2 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 2 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((texp-str (format nil (get-retval 'texp) ""))
                  (exp-retval (multiple-value-list (get-retval 'exp)))
                  (assoc 3))
              (values (format nil "(~A)~A" texp-str
                              (parenthesize exp-retval assoc))
                      assoc)))
          nil))
    (let (op (matched-symbol (list (list 'op))))
      (if (and (listp x) (= 1 (length x))
               (let ((sc-transformer::retval
                         (multiple-value-list (operator (nth 0 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'op matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq op (nth 0 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((op-noarg
                      (third (multiple-value-list (get-retval 'op)))))
              (if op-noarg (expression op-noarg)
                  (error "~A requires at least one argument" op))))
          nil))
    (let (exp-list op
             (matched-symbol (list (list 'exp-list) (list 'op))))
      (if (and (listp x) x
               (let ((sc-transformer::retval
                         (multiple-value-list (operator (car x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'op matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq op (car x)) t)
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'expression
                                       sc-transformer::prev)))
                         (setq exp-list sc-transformer::prev)
                         (rplacd (find 'exp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op-retval (multiple-value-list (get-retval 'op)))
                  (exp-retval-list
                      (combine-each-nth
                          (multiple-value-list (get-retval 'exp-list)))))
              (let ((op-str (first op-retval))
                    (op-assoc (second op-retval)))
                (values (strcat (cons (parenthesize
                                       (first exp-retval-list)
                                       (if op-assoc (1+ op-assoc) nil))
                                      (mapcar
                                       #'(lambda (xx)
                                           (parenthesize xx op-assoc))
                                       (cdr exp-retval-list)))
                                op-str)
                        op-assoc))))
          nil))
    (let (exp2 exp1 comp
               (matched-symbol
                   (list (list 'exp2) (list 'exp1) (list 'comp))))
      (if (and (listp x) (= 3 (length x))
               (let ((sc-transformer::retval
                         (multiple-value-list (comparator (nth 0 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'comp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq comp (nth 0 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp1 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp1 (nth 1 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 2 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp2 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp2 (nth 2 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((comp-retval
                      (multiple-value-list (get-retval 'comp)))
                  (exp1-retval
                      (multiple-value-list (get-retval 'exp1)))
                  (exp2-retval
                      (multiple-value-list (get-retval 'exp2))))
              (let ((comp-str (first comp-retval))
                    (comp-assoc (second comp-retval)))
                (values (format nil "~A ~A ~A"
                                (parenthesize exp1-retval comp-assoc)
                                comp-str
                                (parenthesize exp2-retval comp-assoc))
                        comp-assoc))))
          nil))
    (let (exp3 exp2 exp1
               (matched-symbol
                   (list (list 'exp3) (list 'exp2) (list 'exp1))))
      (if (and (listp x) (= 4 (length x)) (eq 'if-exp (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp1 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp1 (nth 1 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 2 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp2 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp2 (nth 2 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 3 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp3 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp3 (nth 3 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp1-retval
                      (multiple-value-list (get-retval 'exp1)))
                  (exp2-retval
                      (multiple-value-list (get-retval 'exp2)))
                  (exp3-retval
                      (multiple-value-list (get-retval 'exp3)))
                  (assoc 14))
              (values (format nil "~A?~A:~A"
                              (parenthesize exp1-retval assoc)
                              (parenthesize exp2-retval assoc)
                              (parenthesize exp3-retval assoc))
                      assoc)))
          nil))
    (let (exp2 exp1 as-op
               (matched-symbol
                   (list (list 'exp2) (list 'exp1) (list 'as-op))))
      (if (and (listp x) (= 3 (length x))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (assignment-operator (nth 0 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'as-op matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq as-op (nth 0 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp1 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp1 (nth 1 x)) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (expression (nth 2 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp2 matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp2 (nth 2 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((as-retval (multiple-value-list (get-retval 'as-op)))
                  (exp1-retval
                      (multiple-value-list (get-retval 'exp1)))
                  (exp2-retval
                      (multiple-value-list (get-retval 'exp2))))
              (let ((as-str (first as-retval))
                    (as-assoc (second as-retval)))
                (values (format nil "~A ~A ~A"
                                (parenthesize exp1-retval as-assoc)
                                as-str
                                (parenthesize exp2-retval
                                    (1+ as-assoc)))
                        as-assoc))))
          nil))
    (let (exp-list (matched-symbol (list (list 'exp-list))))
      (if (and (listp x) x (eq 'exps (car x))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'expression
                                       sc-transformer::prev)))
                         (setq exp-list sc-transformer::prev)
                         (rplacd (find 'exp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval-list
                      (combine-each-nth
                          (multiple-value-list (get-retval 'exp-list))))
                  (assoc 16))
              (values (strcat (mapcar #'(lambda (xx)
                                          (parenthesize xx assoc))
                                      exp-retval-list)
                              ",")
                      assoc)))
          nil))
    (let (cexp (matched-symbol (list (list 'cexp))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (c-expression x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'cexp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq cexp x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'cexp))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun field-identifier (x)
  (block sc-transformer::pattern-matched
    (let ((matched-symbol (list)))
      (if (and (eq '-> x))
          (return-from sc-transformer::pattern-matched '->) nil))
    (let (id (matched-symbol (list (list 'id))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (id x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id x) t))
          (return-from sc-transformer::pattern-matched
            (get-retval 'id))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
(defun c-expression (x)
  (block sc-transformer::pattern-matched
    (let (exp-list str
             (matched-symbol (list (list 'exp-list) (list 'str))))
      (if (and (listp x) x (eq 'c-exp (car x)) (cdr x)
               (let ((sc-transformer::retval
                         (multiple-value-list (stringp (car (cdr x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'str matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq str (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (awhen (or (and (endp sc-transformer::prev)
                                       (list nil))
                                  (multiple-value-list
                                      (check-mapcar #'expression
                                       sc-transformer::prev)))
                         (setq exp-list sc-transformer::prev)
                         (rplacd (find 'exp-list matched-symbol :key
                                       #'car)
                                 it)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((exp-retval-list
                      (combine-each-nth
                          (multiple-value-list (get-retval 'exp-list)))))
              (apply #'format nil str
                     (mapcar #'(lambda (xx) (parenthesize xx 1))
                             exp-retval-list))))
          nil))
    (return-from sc-transformer::pattern-matched 'nil)))
