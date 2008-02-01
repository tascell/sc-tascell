(provide "type")
(in-package "type")
(export '(@tp0 @tp1 @tpi @tpb @tpe))
(require "SC-MISC")
(require "SC-UTIL")
(require "SCT-LIB")
(require "TYPE-DEFS" #"rule/type-defs.lsp")
(defconstant *rule-package* *package*)
(use-package "SC-TRANSFORMER")
(use-package "SC-MISC")
(use-package "SC-UTIL")
(defun initiate (sc-transformer::x &key (package *package*))
  (@tp0 sc-transformer::x :package package))
(defun @tp0
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (tp0 (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @tp1
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (tp1 (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @tpi
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (tpi (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @tpb
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (tpb (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun @tpe
       (sc-transformer::x &key (package *package*) &aux
           (*package* *rule-package*))
  (immigrate-package
      (tpe (immigrate-package sc-transformer::x *rule-package* package))
      package *rule-package*))
(defun tp0 (x)
  (block sc-transformer::pattern-matched
    (let (declaration-list
             (matched-symbol (list (list 'declaration-list))))
      (if (and (listp x)
               (let ((sc-transformer::crest x))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff x sc-transformer::crest)))
                       (progn
                         (setq declaration-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (progn
              (defvar *str-alist* nil)
              (defvar *var-alist* nil)
              (defvar *last-var-alist* nil)
              (let ((*keyword-list* (cons 'lightweight *keyword-list*))
                    (*str-alist* *str-alist*) (*var-alist* *var-alist*)
                    (*last-var-alist* *last-var-alist*))
                (tp1 declaration-list))))
          nil))
    (return-from sc-transformer::pattern-matched
      (error "No pattern matched."))))
(defun tp1 (x)
  (block sc-transformer::pattern-matched
    (let (decl-list init texp id scs
             (matched-symbol
                 (list (list 'decl-list) (list 'init) (list 'texp)
                       (list 'id) (list 'scs))))
      (if (and (listp x) x (listp (car x)) (car x) (cdr (car x))
               (cdr (cdr (car x)))
               (let ((sc-transformer::retval
                         (multiple-value-list (scs (car (car x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (car (car x))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list (id (car (cdr (car x)))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (car (cdr (car x)))) t)
               (progn (setq texp (car (cdr (cdr (car x))))) t)
               (let ((sc-transformer::crest (cdr (cdr (cdr (car x))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr (car x))))
                                      sc-transformer::crest)))
                       (progn
                         (setq init sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq decl-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((*var-alist*
                      (cons (list* id (remove-type-qualifier texp))
                            *var-alist*)))
              (cons (list* scs id texp (mapcar #'tpi init))
                    (tp1 decl-list))))
          nil))
    (let (decl-list body texp-list id-list scs
             (matched-symbol
                 (list (list 'decl-list) (list 'body) (list 'texp-list)
                       (list 'id-list) (list 'scs))))
      (if (and (listp x) x (listp (car x)) (car x) (cdr (car x))
               (listp (car (cdr (car x)))) (cdr (cdr (car x)))
               (listp (car (cdr (cdr (car x)))))
               (car (cdr (cdr (car x))))
               (eq 'fn (car (car (cdr (cdr (car x))))))
               (let ((sc-transformer::retval
                         (multiple-value-list (scs (car (car x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (car (car x))) t)
               (let ((sc-transformer::crest (car (cdr (car x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (car (cdr (car x)))
                                      sc-transformer::crest)))
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
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest
                         (cdr (car (cdr (cdr (car x)))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (car (cdr (cdr (car x)))))
                                      sc-transformer::crest)))
                       (progn
                         (setq texp-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr (cdr (cdr (car x))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr (car x))))
                                      sc-transformer::crest)))
                       (progn
                         (setq body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq decl-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let* ((fn-tag (car (third (car x))))
                   (texp-list2
                       (mapcar #'remove-type-qualifier texp-list))
                   (*var-alist*
                       (cons (list (first id-list) 'ptr
                                   (list* fn-tag texp-list2))
                             *var-alist*))
                   (new-body nil))
              (let ((bind-list
                        (cmpd-list (cdr id-list) (cdr texp-list2))))
                (setq new-body
                      (let ((*var-alist*
                                (append bind-list *var-alist*)))
                        (tpb body))))
              (cons (list* scs id-list (list* fn-tag texp-list)
                           new-body)
                    (tp1 decl-list))))
          nil))
    (let (decl-list body texp-list id-list scs
             (matched-symbol
                 (list (list 'decl-list) (list 'body) (list 'texp-list)
                       (list 'id-list) (list 'scs))))
      (if (and (listp x) x (listp (car x)) (car x) (cdr (car x))
               (listp (car (cdr (car x)))) (cdr (cdr (car x)))
               (listp (car (cdr (cdr (car x)))))
               (car (cdr (cdr (car x))))
               (eq 'lightweight (car (car (cdr (cdr (car x))))))
               (let ((sc-transformer::retval
                         (multiple-value-list (scs (car (car x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'scs matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq scs (car (car x))) t)
               (let ((sc-transformer::crest (car (cdr (car x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (car (cdr (car x)))
                                      sc-transformer::crest)))
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
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest
                         (cdr (car (cdr (cdr (car x)))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (car (cdr (cdr (car x)))))
                                      sc-transformer::crest)))
                       (progn
                         (setq texp-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr (cdr (cdr (car x))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr (car x))))
                                      sc-transformer::crest)))
                       (progn
                         (setq body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq decl-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let* ((fn-tag (car (third (car x))))
                   (texp-list2
                       (mapcar #'remove-type-qualifier texp-list))
                   (*var-alist*
                       (cons (list (first id-list) 'ptr
                                   (list* fn-tag texp-list2))
                             *var-alist*))
                   (new-body nil))
              (let ((bind-list
                        (cmpd-list (cdr id-list) (cdr texp-list2))))
                (setq new-body
                      (let ((*var-alist*
                                (append bind-list *var-alist*)))
                        (tpb body))))
              (cons (list* scs id-list (list* fn-tag texp-list)
                           new-body)
                    (tp1 decl-list))))
          nil))
    (let (decl-list sdeclist id soru dd
             (matched-symbol
                 (list (list 'decl-list) (list 'sdeclist) (list 'id)
                       (list 'soru) (list 'dd))))
      (if (and (listp x) x (listp (car x)) (car x) (cdr (car x))
               (listp (car (cdr (car x))))
               (= 2 (length (car (cdr (car x)))))
               (let ((sc-transformer::retval
                         (multiple-value-list (def-decl (car (car x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'dd matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq dd (car (car x))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (s-or-u (nth 0 (car (cdr (car x))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'soru matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq soru (nth 0 (car (cdr (car x))))) t)
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (id (nth 1 (car (cdr (car x))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 1 (car (cdr (car x))))) t)
               (let ((sc-transformer::crest (cdr (cdr (car x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car x)))
                                      sc-transformer::crest)))
                       (progn
                         (setq sdeclist sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq decl-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((sdecl-with-type nil) (member-list nil))
              (let ((*var-alist* nil))
                (setq sdecl-with-type (tp1 sdeclist) member-list
                      *last-var-alist*))
              (let ((*str-alist*
                        (cons (cons id member-list) *str-alist*)))
                (cons (list* dd (list soru id) sdecl-with-type)
                      (tp1 decl-list)))))
          nil))
    (let (decl-list bit
             (matched-symbol (list (list 'decl-list) (list 'bit))))
      (if (and (listp x) x (eq ':bit (car x)) (cdr x)
               (progn (setq bit (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq decl-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (list* :bit bit (tp1 decl-list)))
          nil))
    (let (decl-list enum-list id
             (matched-symbol
                 (list (list 'decl-list) (list 'enum-list) (list 'id))))
      (if (and (listp x) x (listp (car x)) (car x)
               (eq 'def (car (car x))) (cdr (car x))
               (listp (car (cdr (car x))))
               (= 2 (length (car (cdr (car x)))))
               (eq 'enum (nth 0 (car (cdr (car x)))))
               (let ((sc-transformer::retval
                         (multiple-value-list
                             (id (nth 1 (car (cdr (car x))))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 1 (car (cdr (car x))))) t)
               (let ((sc-transformer::crest (cdr (cdr (car x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car x)))
                                      sc-transformer::crest)))
                       (progn
                         (setq enum-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq decl-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((vadd-list nil))
              (dolist (en enum-list)
                (when (consp en) (setq en (car en)))
                (push (cons en (list 'enum id)) vadd-list))
              (let ((*var-alist* (append vadd-list *var-alist*)))
                (cons (list* 'def (list 'enum id) enum-list)
                      (tp1 decl-list)))))
          nil))
    (let (decl-list sdecllist id
             (matched-symbol
                 (list (list 'decl-list) (list 'sdecllist) (list 'id))))
      (if (and (listp x) x (listp (car x)) (car x)
               (eq 'deftype (car (car x))) (cdr (car x))
               (cdr (cdr (car x)))
               (eq 'struct (car (cdr (cdr (car x)))))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (car (cdr (car x)))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (car (cdr (car x)))) t)
               (let ((sc-transformer::crest (cdr (cdr (cdr (car x))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr (car x))))
                                      sc-transformer::crest)))
                       (progn
                         (setq sdecllist sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq decl-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((sdecl-with-type nil) (member-list nil))
              (let ((*var-alist* nil))
                (setq sdecl-with-type (tp1 sdeclist) member-list
                      *var-alist*))
              (let ((*str-alist*
                        (cons (cons (cat-symbol '$ id) member-list)
                              *str-alist*))
                    (*var-alist*
                        (cons (cons id
                                    (list '$type
                                     (list 'struct (cat-symbol '$ id))))
                              *var-alist*)))
                (cons (list* 'deftype id 'struct sdecl-with-type)
                      (tp1 decl-list)))))
          nil))
    (let (decl-list enum-list id
             (matched-symbol
                 (list (list 'decl-list) (list 'enum-list) (list 'id))))
      (if (and (listp x) x (listp (car x)) (car x)
               (eq 'deftype (car (car x))) (cdr (car x))
               (cdr (cdr (car x))) (eq 'enum (car (cdr (cdr (car x)))))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (car (cdr (car x)))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (car (cdr (car x)))) t)
               (let ((sc-transformer::crest (cdr (cdr (cdr (car x))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr (car x))))
                                      sc-transformer::crest)))
                       (progn
                         (setq enum-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq decl-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((vadd-list nil))
              (dolist (en enum-list)
                (when (consp en) (setq en (car en)))
                (push (list* en id) vadd-list))
              (push (list id '$type (list 'enum (cat-symbol '$ id)))
                    vadd-list)
              (let ((*var-alist* (append vadd-list *var-alist*)))
                (cons (list* 'deftype id 'enum enum-list)
                      (tp1 decl-list)))))
          nil))
    (let (decl-list texp id
             (matched-symbol
                 (list (list 'decl-list) (list 'texp) (list 'id))))
      (if (and (listp x) x (listp (car x)) (= 3 (length (car x)))
               (eq 'deftype (nth 0 (car x)))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (nth 1 (car x))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 1 (car x))) t)
               (progn (setq texp (nth 2 (car x))) t)
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq decl-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((*var-alist*
                      (cons (cons id
                                  (list '$type
                                        (remove-type-qualifier texp)))
                            *var-alist*)))
              (cons (list 'deftype id texp) (tp1 decl-list))))
          nil))
    (let (decl-list rem
             (matched-symbol (list (list 'decl-list) (list 'rem))))
      (if (and (listp x) x (listp (car x)) (car x)
               (eq 'c-exp (car (car x)))
               (let ((sc-transformer::crest (cdr (car x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (car x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq decl-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons (car x) (tp1 decl-list)))
          nil))
    (let ((matched-symbol (list)))
      (if (and (listp x) (endp x))
          (return-from sc-transformer::pattern-matched
            (progn (setq *last-var-alist* *var-alist*) nil))
          nil))
    (let (rem-body (matched-symbol (list (list 'rem-body))))
      (if (and (listp x) x (eq '$body (car x))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq rem-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched (tpb rem-body))
          nil))
    (let (otherwise (matched-symbol (list (list 'otherwise))))
      (if (and (progn (setq otherwise x) t))
          (return-from sc-transformer::pattern-matched
            (error "syntax error in ~s." (car otherwise)))
          nil))
    (return-from sc-transformer::pattern-matched
      (error "No pattern matched."))))
(defun tpi (x)
  (block sc-transformer::pattern-matched
    (let (init (matched-symbol (list (list 'init))))
      (if (and (listp x) x (eq 'array (car x))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq init sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (list* (car x) (mapcar #'tpi init)))
          nil))
    (let (init (matched-symbol (list (list 'init))))
      (if (and (listp x) x (eq 'struct (car x))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq init sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (list* (car x) (mapcar #'tpi init)))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (progn (setq exp x) t))
          (return-from sc-transformer::pattern-matched (tpe exp)) nil))
    (return-from sc-transformer::pattern-matched
      (error "No pattern matched."))))
(defun tpb (x)
  (block sc-transformer::pattern-matched
    (let (body atr (matched-symbol (list (list 'body) (list 'atr))))
      (if (and (listp x) x (eq ':attr (car x)) (cdr x)
               (progn (setq atr (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (list* :attr atr (tpb body)))
          nil))
    (let (body regarg
               (matched-symbol (list (list 'body) (list 'regarg))))
      (if (and (listp x) x (listp (car x)) (car x)
               (eq 'register (car (car x)))
               (let ((sc-transformer::crest (cdr (car x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (car x))
                                      sc-transformer::crest)))
                       (progn
                         (setq regarg sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (list* (list* 'register regarg) (tpb body)))
          nil))
    (let (remain-body body
             (matched-symbol (list (list 'remain-body) (list 'body))))
      (if (and (listp x) x (listp (car x)) (car x)
               (eq 'begin (car (car x)))
               (let ((sc-transformer::crest (cdr (car x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (car x))
                                      sc-transformer::crest)))
                       (progn
                         (setq body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons (list* 'begin (tpb body)) (tpb remain-body)))
          nil))
    (let (remain-body stat id
             (matched-symbol
                 (list (list 'remain-body) (list 'stat) (list 'id))))
      (if (and (listp x) x (listp (car x)) (= 3 (length (car x)))
               (eq 'label (nth 0 (car x)))
               (progn (setq id (nth 1 (car x))) t)
               (progn (setq stat (nth 2 (car x))) t)
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons (list* 'label id (tpb (list stat)))
                  (tpb remain-body)))
          nil))
    (let (remain-body exp
             (matched-symbol (list (list 'remain-body) (list 'exp))))
      (if (and (listp x) x (listp (car x)) (= 2 (length (car x)))
               (eq 'case (nth 0 (car x)))
               (progn (setq exp (nth 1 (car x))) t)
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons (list 'case (tpe exp)) (tpb remain-body)))
          nil))
    (let (remain-body (matched-symbol (list (list 'remain-body))))
      (if (and (listp x) x (listp (car x)) (= 1 (length (car x)))
               (eq 'default (nth 0 (car x)))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons '(default) (tpb remain-body)))
          nil))
    (let (remain-body stat2 stat1 exp
             (matched-symbol
                 (list (list 'remain-body) (list 'stat2) (list 'stat1)
                       (list 'exp))))
      (if (and (listp x) x (listp (car x)) (car x)
               (eq 'if (car (car x))) (cdr (car x)) (cdr (cdr (car x)))
               (progn (setq exp (car (cdr (car x)))) t)
               (progn (setq stat1 (car (cdr (cdr (car x))))) t)
               (let ((sc-transformer::crest (cdr (cdr (cdr (car x))))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr (car x))))
                                      sc-transformer::crest)))
                       (progn
                         (setq stat2 sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons (if (null stat2)
                      (list* 'if (tpe exp) (tpb (list stat1)))
                      (list* 'if (tpe exp)
                             (append (tpb (list stat1)) (tpb stat2))))
                  (tpb remain-body)))
          nil))
    (let (remain-body body exp
             (matched-symbol
                 (list (list 'remain-body) (list 'body) (list 'exp))))
      (if (and (listp x) x (listp (car x)) (car x)
               (eq 'switch (car (car x))) (cdr (car x))
               (progn (setq exp (car (cdr (car x)))) t)
               (let ((sc-transformer::crest (cdr (cdr (car x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car x)))
                                      sc-transformer::crest)))
                       (progn
                         (setq body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons (list* 'switch (tpe exp) (tpb body))
                  (tpb remain-body)))
          nil))
    (let (remain-body body exp
             (matched-symbol
                 (list (list 'remain-body) (list 'body) (list 'exp))))
      (if (and (listp x) x (listp (car x)) (car x)
               (eq 'do-while (car (car x))) (cdr (car x))
               (progn (setq exp (car (cdr (car x)))) t)
               (let ((sc-transformer::crest (cdr (cdr (car x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car x)))
                                      sc-transformer::crest)))
                       (progn
                         (setq body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons (list* 'do-while (tpe exp) (tpb body))
                  (tpb remain-body)))
          nil))
    (let (remain-body id
             (matched-symbol (list (list 'remain-body) (list 'id))))
      (if (and (listp x) x (listp (car x)) (= 2 (length (car x)))
               (eq 'goto (nth 0 (car x)))
               (progn (setq id (nth 1 (car x))) t)
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons (car x) (tpb remain-body)))
          nil))
    (let (remain-body (matched-symbol (list (list 'remain-body))))
      (if (and (listp x) x (listp (car x)) (= 1 (length (car x)))
               (eq 'continue (nth 0 (car x)))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons (car x) (tpb remain-body)))
          nil))
    (let (remain-body (matched-symbol (list (list 'remain-body))))
      (if (and (listp x) x (listp (car x)) (= 1 (length (car x)))
               (eq 'break (nth 0 (car x)))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons (car x) (tpb remain-body)))
          nil))
    (let (remain-body exp
             (matched-symbol (list (list 'remain-body) (list 'exp))))
      (if (and (listp x) x (listp (car x)) (car x)
               (eq 'return (car (car x)))
               (let ((sc-transformer::crest (cdr (car x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (car x))
                                      sc-transformer::crest)))
                       (progn
                         (setq exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons (if (null exp) '(return)
                      (list 'return (tpe (car exp))))
                  (tpb remain-body)))
          nil))
    (let (remain-body body decl-list
             (matched-symbol
                 (list (list 'remain-body) (list 'body)
                       (list 'decl-list))))
      (if (and (listp x) x (listp (car x)) (car x)
               (eq 'thread-create (car (car x))) (cdr (car x))
               (progn (setq decl-list (car (cdr (car x)))) t)
               (let ((sc-transformer::crest (cdr (cdr (car x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car x)))
                                      sc-transformer::crest)))
                       (progn
                         (setq body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let* ((n-decls (length decl-list))
                   (tpb-x (tpb (append decl-list body)))
                   (new-body (nthcdr n-decls tpb-x))
                   (new-decl-list (firstn tpb-x n-decls)))
              (cons (list* 'thread-create new-decl-list new-body)
                    (tpb remain-body))))
          nil))
    (let (remain-body body id
             (matched-symbol
                 (list (list 'remain-body) (list 'body) (list 'id))))
      (if (and (listp x) x (listp (car x)) (car x)
               (eq 'thread-suspend (car (car x))) (cdr (car x))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (car (cdr (car x)))))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (car (cdr (car x)))) t)
               (let ((sc-transformer::crest (cdr (cdr (car x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (car x)))
                                      sc-transformer::crest)))
                       (progn
                         (setq body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest))))))
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((*var-alist* (cons (cons id 'thst-ptr) *var-alist*)))
              (cons (list* 'thread-suspend id (tpb body))
                    (tpb remain-body))))
          nil))
    (let (remain-body exp
             (matched-symbol (list (list 'remain-body) (list 'exp))))
      (if (and (listp x) x (listp (car x)) (= 2 (length (car x)))
               (eq 'thread-resume (nth 0 (car x)))
               (progn (setq exp (nth 1 (car x))) t)
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cons (list 'thread-resume (tpe exp)) (tpb remain-body)))
          nil))
    (let (remain-body otherwise
             (matched-symbol
                 (list (list 'remain-body) (list 'otherwise))))
      (if (and (listp x) x (progn (setq otherwise (car x)) t)
               (let ((sc-transformer::crest (cdr x)))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr x) sc-transformer::crest)))
                       (progn
                         (setq remain-body sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((expression-stat (tpe otherwise)))
              (if (eq '$not-expression expression-stat)
                  (tp1 (list* otherwise '$body remain-body))
                  (list* expression-stat (tpb remain-body)))))
          nil))
    (let ((matched-symbol (list)))
      (if (and (listp x) (endp x))
          (return-from sc-transformer::pattern-matched nil) nil))
    (return-from sc-transformer::pattern-matched
      (error "No pattern matched."))))
(defun tpe (x)
  (block sc-transformer::pattern-matched
    (let (id (matched-symbol (list (list 'id))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (id x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id x) t))
          (return-from sc-transformer::pattern-matched
            (list 'the (assoc-vartype id) id))
          nil))
    (let (num (matched-symbol (list (list 'num))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (integerp x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'num matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq num x) t))
          (return-from sc-transformer::pattern-matched
            (list 'the 'int num))
          nil))
    (let (cha (matched-symbol (list (list 'cha))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (characterp x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'cha matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq cha x) t))
          (return-from sc-transformer::pattern-matched
            (list 'the 'char cha))
          nil))
    (let (num (matched-symbol (list (list 'num))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (numberp x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'num matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq num x) t))
          (return-from sc-transformer::pattern-matched
            (list 'the 'double num))
          nil))
    (let (str (matched-symbol (list (list 'str))))
      (if (and (let ((sc-transformer::retval
                         (multiple-value-list (stringp x))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'str matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq str x) t))
          (return-from sc-transformer::pattern-matched
            (list 'the '(array char) str))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq 'aref (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (cond
              ((cdr rem-exp)
               (tpe (list* 'aref (list 'aref exp1 (car rem-exp))
                           (cdr rem-exp))))
              ((null rem-exp) (tpe exp1))
              (t (let* ((exp-with-type (tpe exp1))
                        (exp-type (cadr exp-with-type)))
                   (unless (and (listp exp-type)
                                (or (eq 'ptr (car exp-type))
                                    (eq 'array (car exp-type))))
                     (error "Can't apply aref to ~s." exp1))
                   (list 'the (deref-type exp-type)
                         (list 'aref exp-with-type (tpe (car rem-exp))))))))
          nil))
    (let (rem-id id1 exp1
                 (matched-symbol
                     (list (list 'rem-id) (list 'id1) (list 'exp1))))
      (if (and (listp x) x (eq 'fref (car x)) (cdr x) (cdr (cdr x))
               (progn (setq exp1 (car (cdr x))) t)
               (progn (setq id1 (car (cdr (cdr x)))) t)
               (let ((sc-transformer::crest (cdr (cdr (cdr x)))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr (cdr x)))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-id sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (if rem-id (tpe (list* 'fref (list 'fref exp1 id1) rem-id))
                (let* ((exp1-with-type (tpe exp1))
                       (exp1-type (cadr exp1-with-type)))
                  (unless (and (listp exp1-type)
                               (or (eq 'struct (car exp1-type))
                                   (eq 'union (car exp1-type))))
                    (error "~s is neither struct or union" exp1))
                  (let* ((*var-alist* (assoc-struct (cadr exp1-type)))
                         (id1-with-type (tpe id1)))
                    (list 'the (cadr id1-with-type)
                          (list 'fref exp1-with-type id1))))))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq 'inc (nth 0 x))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-with-type (tpe exp)))
              (list 'the (cadr exp-with-type)
                    (list (car x) exp-with-type))))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq 'dec (nth 0 x))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-with-type (tpe exp)))
              (list 'the (cadr exp-with-type)
                    (list (car x) exp-with-type))))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq '++ (nth 0 x))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-with-type (tpe exp)))
              (list 'the (cadr exp-with-type)
                    (list (car x) exp-with-type))))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq '-- (nth 0 x))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-with-type (tpe exp)))
              (list 'the (cadr exp-with-type)
                    (list (car x) exp-with-type))))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq 'bit-not (nth 0 x))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-with-type (tpe exp)))
              (list 'the (cadr exp-with-type)
                    (list (car x) exp-with-type))))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq 'ptr (nth 0 x))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((exp-with-type (tpe exp)))
              (list 'the (list 'ptr (cadr exp-with-type))
                    (list 'ptr exp-with-type))))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq 'mref (nth 0 x))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let* ((exp-with-type (tpe exp))
                   (exp-type (cadr exp-with-type)))
              (unless (and (listp exp-type)
                           (or (eq 'ptr (car exp-type))
                               (eq 'array (car exp-type))))
                (error "Can't apply mref to ~s." exp))
              (list 'the (deref-type exp-type)
                    (list 'mref exp-with-type))))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq 'not (nth 0 x))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (list 'the 'int (tpe exp)))
          nil))
    (let (id (matched-symbol (list (list 'id))))
      (if (and (listp x) (= 2 (length x)) (eq 'sizeof (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (id (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'id matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq id (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (let ((idtype (assoc-vartype id)))
              (break)
              (list 'the 'int
                    (list 'sizeof
                          (if (eq '$type (car idtype)) id
                              (list 'the idtype id))))))
          nil))
    (let (exp (matched-symbol (list (list 'exp))))
      (if (and (listp x) (= 2 (length x)) (eq 'sizeof (nth 0 x))
               (let ((sc-transformer::retval
                         (multiple-value-list (expr (nth 1 x)))))
                 (and (car sc-transformer::retval)
                      (rplacd (find 'exp matched-symbol :key #'car)
                              sc-transformer::retval)))
               (progn (setq exp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (list 'the 'int (list 'sizeof (tpe exp))))
          nil))
    (let (texp (matched-symbol (list (list 'texp))))
      (if (and (listp x) (= 2 (length x)) (eq 'sizeof (nth 0 x))
               (progn (setq texp (nth 1 x)) t))
          (return-from sc-transformer::pattern-matched
            (list 'the 'int (list 'sizeof texp)))
          nil))
    (let (exp texp (matched-symbol (list (list 'exp) (list 'texp))))
      (if (and (listp x) (= 3 (length x)) (eq 'cast (nth 0 x))
               (progn (setq texp (nth 1 x)) t)
               (progn (setq exp (nth 2 x)) t))
          (return-from sc-transformer::pattern-matched
            (list 'the texp (list 'cast texp (tpe exp))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '* (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '/ (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '% (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '+ (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '- (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq 'bit-xor (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq 'bit-and (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq 'bit-or (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '<< (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '>> (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq 'and (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq 'or (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '< (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '> (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '<= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '>= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '== (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '!= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '*= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '/= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '%= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '+= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '-= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '<<= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq '>>= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq 'bit-and= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq 'bit-xor= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq 'bit-or= (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (rem-exp exp1
                  (matched-symbol (list (list 'rem-exp) (list 'exp1))))
      (if (and (listp x) x (eq 'exps (car x)) (cdr x)
               (progn (setq exp1 (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq rem-exp sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let ((op (car x)))
              (cond
                ((cdr rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too many arguments." x))
                   (otherwise
                       (tpe (list* op (list op exp1 (car rem-exp))
                                   (cdr rem-exp))))))
                ((null rem-exp)
                 (case op
                   ((< > <= >= == != = *= /= %= += -= <<= >>= bit-and=
                       bit-xor= bit-or=)
                    (error "~s:too few arguments." x))
                   (otherwise
                       (let ((exp1-with-type (tpe exp1)))
                         (list 'the (cadr exp1-with-type)
                               (list op exp1-with-type))))))
                (t (let* ((exp2 (car rem-exp))
                          (exp-with-type1 (tpe exp1))
                          (exp-with-type2 (tpe exp2))
                          (exp-type1 (cadr exp-with-type1))
                          (exp-type2 (cadr exp-with-type2))
                          (whole-op
                              (case op
                                ((* / % + - bit-xor bit-and bit-or)
                                 (type-conversion exp-type1 exp-type2))
                                ((<< >> = *= /= %= += -= <<= >>=
                                     bit-and= bit-xor= bit-or=)
                                 (cadr exp-with-type1))
                                ((exps) (cadr exp-with-type2))
                                ((and or < > <= >= == !=) 'int)
                                (otherwise
                                    (error
                                     "unexpected operator in Tpe.")))))
                     (list 'the whole-op
                           (list op exp-with-type1 exp-with-type2)))))))
          nil))
    (let (exp3 exp2 exp1
               (matched-symbol
                   (list (list 'exp3) (list 'exp2) (list 'exp1))))
      (if (and (listp x) (= 4 (length x)) (eq 'if-exp (nth 0 x))
               (progn (setq exp1 (nth 1 x)) t)
               (progn (setq exp2 (nth 2 x)) t)
               (progn (setq exp3 (nth 3 x)) t))
          (return-from sc-transformer::pattern-matched
            (let* ((exp-with-type1 (tpe exp1))
                   (exp-with-type2 (tpe exp2))
                   (exp-with-type3 (tpe exp3))
                   (exp-type2 (cadr exp-with-type2))
                   (exp-type3 (cadr exp-with-type3))
                   (whole-op (type-conversion exp-type2 exp-type3)))
              (list 'the whole-op
                    (list 'if-exp exp-with-type1 exp-with-type2
                          exp-with-type3))))
          nil))
    (let (exp-list str
             (matched-symbol (list (list 'exp-list) (list 'str))))
      (if (and (listp x) x (eq 'c-exp (car x)) (cdr x)
               (progn (setq str (car (cdr x))) t)
               (let ((sc-transformer::crest (cdr (cdr x))))
                 (loop
                   (when (funcall #'(lambda (sc-transformer::y)
                                      (and (endp sc-transformer::y)))
                                  sc-transformer::crest)
                     (let ((sc-transformer::prev
                               (ldiff (cdr (cdr x))
                                      sc-transformer::crest)))
                       (progn
                         (setq exp-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (list* 'c-exp str (mapcar #'tpe exp-list)))
          nil))
    (let (arg-list fexp
             (matched-symbol (list (list 'arg-list) (list 'fexp))))
      (if (and (listp x) x
               (let ((sc-transformer::retval
                         (multiple-value-list (expr (car x)))))
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
                       (progn
                         (setq arg-list sc-transformer::prev)
                         (return t))))
                   (progn
                     (if (endp sc-transformer::crest) (return nil)
                         (setq sc-transformer::crest
                               (cdr sc-transformer::crest)))))))
          (return-from sc-transformer::pattern-matched
            (let* ((fexp-with-type (tpe fexp))
                   (fexp-type (cadr fexp-with-type)) (type-fn nil))
              (unless (and (listp fexp-type) (eq 'ptr (car fexp-type))
                           (listp (setq type-fn (cadr fexp-type)))
                           (or (eq 'fn (car type-fn))
                               (eq 'lightweight (car type-fn))))
                (error "~s is not a function." fexp))
              (list 'the (cadr type-fn)
                    (list* 'call
                           (list 'the type-fn (caddr fexp-with-type))
                           (mapcar #'tpe arg-list)))))
          nil))
    (let ((matched-symbol (list)))
      (if (and (listp x) (endp x))
          (return-from sc-transformer::pattern-matched nil) nil))
    (let (otherwise (matched-symbol (list (list 'otherwise))))
      (if (and (progn (setq otherwise x) t))
          (return-from sc-transformer::pattern-matched
            '$not-expression)
          nil))
    (return-from sc-transformer::pattern-matched
      (error "No pattern matched."))))
