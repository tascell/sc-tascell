;;; type.ruleで使用する関数、定数の定義
(provide "TYPE-DEFS")
(in-package "TYPE")
(require "SC-UTIL")
(use-package "SC-TRANSFORMER")
(use-package "SC-UTIL")

;;; get type of variable
(defun assoc-vartype (id)
  ;; (format t "(assoc-vartype ~s)~%" id)
  (assoc-typedef-name-rec
   (cdr
    (or (assoc (par-identifier id) *var-alist* :key #'par-identifier :test #'string=)
	(progn
	  (warn "variable ~s is undefined." id)
	  (return-from assoc-vartype '$undefined))))))

;;; get struct member list
(defun assoc-struct (id)
  (cdr
   (or (assoc (get-idstring (symbol-name id)) *str-alist*
	      :key #'(lambda (x) (get-idstring (symbol-name x)))
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
