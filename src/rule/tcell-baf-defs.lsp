;;; Copyright (c) 2019 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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

(defpackage "TCELL-BAF"
  (:shadow cl:declaration)
  (:use "RULE" "CL" "SC-MISC")
  (:export :ref-value :set-value
	   :poly-plus :poly-unary-minus :poly-minus :poly-positive-p :poly-negative-p
	   :poly-zerop :poly-true-p :print-friendly-poly
	   :baf-compatible-p
	   ))
(in-package "TCELL-BAF")

;; Initial value of Location specifier: list of (<loc> . <val>)
(defparameter *init* (list))
;; Current value of Location specifier: list of (<loc> . <val>)
(defparameter *env* (list))

(defun diff-env (env1 env2)
  (if (null env1)
      (loop for lv2 in (print-friendly-env env2) collect `(:env2-only ,lv2))
    (let* ((lv1 (car env1))
	   (loc1 (car lv1))
	   (val1 (cdr lv1)))
      (aif (find loc1 env2 :key #'car :test #'equal)
	   (let* ((loc2 (car it))
		  (val2 (cdr it)))
	     (if (equal val1 val2)
		 (diff-env (cdr env1) (remove it env2))
	       (cons `(,loc1 (:env1 ,(print-friendly-poly val1))
			     (:env2 ,(print-friendly-poly val2)))
		     (diff-env (cdr env1) (remove it env2)))))
	   (cons `(:env1-only ,(cons loc1 (print-friendly-poly val1)))
		 (diff-env (cdr env1) env2))))))

(defun ref-value (loc)
  (aif (find loc *env* :key #'car :test #'equal)
       (cdr it)
       (let* ((isym (gensym (if (stringp loc) loc (format nil "~A" loc))))
	      (ival (list (cons isym 1) (cons nil 0))))
	 (push (cons loc ival) *init*)
	 (push (cons loc ival) *env*)
	 ival)))
       
(defun set-value (loc val)
  (aif (find loc *env* :key #'car :test #'equal)
       (rplacd it val)
       (let* ((isym (gensym (if (stringp loc) loc (format nil "~A" loc))))
	      (ival (list (cons isym 1) (cons nil 0))))
	 (push (cons loc ival) *init*)
	 (push (cons loc val) *env*)
	 val)))

(defun print-friendly-env (env)
  (loop for lv in env
	collect (cons (car lv) (print-friendly-poly (cdr lv)))))


;; n元1次多項式の加減算
;; Polynominals are represented as a list of (<symbol> . <number>). <symbol> is nil for dimensionless value
;; The list is sorted by <symbol> so that the equality of two polynominals can be judged just by #'equal
(defun poly-plus (val1 val2)
  (sort (poly-plus-aux val1 val2)
	#'(lambda (str1 str2)
	    (cond
	     ((string= str1 "NIL")
	      nil)
	     ((string= str2 "NIL")
	      t)
	     (t
	      (string< str1 str2))))
	:key (compose #'symbol-name #'car)))

(defun poly-plus-aux (val1 val2)
  (if (null val1)
      val2
    (let* ((term1 (car val1))
	   (sym1 (car term1))
	   (num1 (cdr term1)))
      (aif (find sym1 val2 :key #'car)
	   (let ((factor (+ num1 (cdr it))))
	     (if (and (= factor 0) sym1)     ; 係数0の項を消す（無次元の項を除く）
		 (poly-plus-aux (cdr val1) (remove it val2))
	       (cons (cons sym1 factor)
		     (poly-plus-aux (cdr val1) (remove it val2)))))
	   (cons term1
		 (poly-plus-aux (cdr val1) val2))))))

(defun poly-unary-minus (val)
  (loop for term in val
	collect (cons (car term) (- (cdr term)))))

(defun poly-minus (val1 val2)
  (poly-plus val1 (poly-unary-minus val2)))

(defun poly-positive-p (val)
  (if (or (second val)
	  (caar val))
      (throw :undefined-behavior (cons :undefined :undeterministic-value))
    (> (cdar val) 0)))
(defun poly-negative-p (val)
  (if (or (second val)
	  (caar val))
      (throw :undefined-behavior (cons :undefined :undeterministic-value))
    (< (cdar val) 0)))

(defun poly-zerop (val)
  (if (or (second val)
	  (caar val))
      (throw :undefined-behavior (cons :undefined :undeterministic-value))
    (= (cdar val) 0)))
	
(defun poly-true-p (val)
  (if (or (second val)
	  (caar val))
      (throw :undefined-behavior (cons :undefined :undeterministic-value))
    (not (= (cdar val) 0))))

(defun print-friendly-poly (val)
  (setq val
	(loop for term in val
	      collect (cond
		       ((null (car term))
			(cdr term))
		       ((= 1 (cdr term))
			(car term))
		       (t
			term))))
  (if (cdr val)
      val
    (car val)))



(defun baf-compatible-p (before after)
  (let ((bef (apply-rule before :untype))
        (aft (apply-rule after :untype)))
    (with-package :sc
      (format t "<<<back-and-forth compatibility test>>>~%[before:] ~S~%[after:] ~S~%" bef aft))
    (let ((*init* (list))
	  (*env* (list)))
      (format t "<<before => after>>~%")
      (apply-rule bef :sc-interpret)
      (apply-rule aft :sc-interpret)
      (with-package :sc
        (format t "[init:] ~S~%" (print-friendly-env *init*))
	(format t "[env:] ~S~%"  (print-friendly-env *env*))
	(format t "[diff:] ~S~%" (diff-env *init* *env*))))
    (let ((*init* (list))
	  (*env* (list)))
      (format t "<<after => before>>~%")
      (apply-rule aft :sc-interpret)
      (apply-rule bef :sc-interpret)
      (with-package :sc
        (format t "[init:] ~S~%" (print-friendly-env *init*))
	(format t "[env:] ~S~%"  (print-friendly-env *env*))
	(format t "[diff:] ~S~%" (diff-env *init* *env*))))))
