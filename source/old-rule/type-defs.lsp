;;; type.ruleで使用する関数、定数の定義
(provide "TYPE-DEFS")
(in-package "TYPE")
(scr:require "SC-UTIL")

;;; leave only type-entry from *var-alist*
(defun leave-typename (&optional (alist *var-alist*))
  (remove-if-not
   #'(lambda (x) (and (consp (cdr x))
                      (eq '$type (cadr x))))
   alist))

;;; get type of variable
(defun assoc-vartype (id)
  ;; (format t "(assoc-vartype ~s)~%" id)
  (assoc-typedef-name-rec
   (cdr
    (or (assoc (par-identifier id) *var-alist* 
               :key #'par-identifier :test #'string=)
	(progn
	  (warn "variable ~s is undefined." id)
          (break)
	  (return-from assoc-vartype '$undefined))))))

;;; get struct member list
(defun assoc-struct (id)
  (cdr
   (or (assoc (identifier-sc2c id) *str-alist*
	      :key #'(lambda (x) (identifier-sc2c x))
	      :test #'string=)
       (error "struct-or-union ~s is undefined." id))))

;;; get type of typedef-name
(defun assoc-typedef-name (typedef-name)
  ;; (format t "(assoc-typedef-name ~s)~%" typedef-name)
  (unless (par-identifier typedef-name) 
    (return-from assoc-typedef-name typedef-name))
  (let ((nametype (assoc-vartype typedef-name)))
    (if (and (consp nametype) 
	     (eq '$type (car nametype)))
	(cadr (assoc-typedef-name nametype))
      (progn
	(warn "typedef-name ~s is undefined." typedef-name)
	(break)
	typedef-name))))

;;; get type of typedef-name recursively
(defun assoc-typedef-name-rec (texp)
  ;; (format t "(assoc-typedef-name-rec ~s)~%" texp)
  (if (listp texp)
      (if (or (eq 'enum (car texp))
	      (eq 'struct (car texp))
	      (eq 'union (car texp)))
	  texp
	  (mapcar #'assoc-typedef-name-rec texp))
      (let ((texp2 (assoc-typedef-name texp)))
	(if (eq texp texp2)
	    texp
	    (assoc-typedef-name-rec texp2)))))
