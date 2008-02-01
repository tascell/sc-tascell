;; utilitis for sc2c.rule
(provide "SC2C-DEFS")
(defpackage "SC2C"
  (:use "CL" "RULE" "SC-MISC")
  (:export :parenthesize :parenthesize-t :cond->if
           :identifier-sc2c :char-sc2c :string-sc2c)
  (:shadow cl:declaration))

(in-package "SC2C")
;;(in-package "SCT-USER")

(defun add-paren (str)
  (list "(" str ")"))

(defun parenthesize (exp-retval &optional (assoc -1))
  (funcall
   (if (and (numberp (cadr exp-retval))
	    (numberp assoc)
	    (> assoc (cadr exp-retval)))
       #'identity
       #'add-paren)
   (car exp-retval)))

(defun parenthesize-t (texp-str outer-assoc inner-assoc)
  (funcall
   (if (and (numberp outer-assoc)
	    (numberp inner-assoc)
	    (<= inner-assoc outer-assoc))
       #'identity
       #'add-paren)
   texp-str))

;; sc1拡張
(defun cond->if (clauses)
  (cond
   ((endp clauses) ~())
   ((eq ~else (caar clauses)) ~(begin ,@(cdar clauses)))
   ((not (cdr clauses)) ~(if ,(caar clauses) (begin ,@(cdar clauses))))
   (t
    ~(if ,(caar clauses) (begin ,@(cdar clauses)) ,(cond->if (cdr clauses))))))
   
;; identifierの処理
(defun get-idstring (symstr)
  #+readtable-case
  (funcall
   (if (case-mixed-p symstr)
       #'identity
       #'string-invertcase)
   (substitute #\_ #\- (remove #\@ symstr)) )
  #-readtable-case
  (let ((idstr (make-string (length symstr) :initial-element #\Space)) 
	(getcase (if (eq *print-case* :downcase)
		     'char-downcase
		     'char-upcase))
	(m 0))
    (dotimes (n (length symstr) (remove #\Space idstr))
      (let ((s (aref symstr n)))
        (if (char= s #\@)
            (if (eq getcase #'char-upcase)
                (setq getcase #'char-downcase)
                (setq getcase #'char-upcase))
            (progn
              (when (char= s #\-) (setq s #\_))
              (setf (aref idstr m) (funcall getcase s))
              (incf m)))))))

(defun identifier-sc2c (x &aux idstr)
  (when (and (symbolp x) (not (null x)))    
    (setq idstr (get-idstring (symbol-name x)))
    (if (or (string= "" (remove #\_ idstr))
            (not 
             (let ((s0 (aref idstr 0)))
               (or (alpha-char-p s0)
                   (char= #\_ s0))))
            (notevery #'(lambda (x)
                          (or (alphanumericp x)
                              (char= x #\_)))
                      idstr))
        nil
        idstr)))

(setf (symbol-function 'identifier-sc2c) 
  (memoize #'identifier-sc2c :test #'eq :size 20000 :rehash-size 2))

;;; SC文字->C文字
;;; 返値はstring
(defun char-sc2c (ch)
  (when (characterp ch)
    (if (graphic-char-p ch)
      (case ch
        (#\\ "\\\\")
        (#\' "\\'")
        (#\" "\\\"")
        (otherwise (make-string 1 :initial-element ch)))
      (case ch
        (#\BackSpace "\\b")
        (#\Tab "\\t")
        (#\NewLine "\\n")
        (#\Page "\\f")
        (#\Return "\\r")
        (otherwise
         (format nil "\\x~x" (char-code ch)))))))

;;; string-sc2c用の拡張format関数。
;;; ~|xxx| (xxx=は8進の整数) で、その数字コードの文字が埋め込まれる 
(defun sc-format (format-string &rest args)
  (labels ((sc-format-list (str-list)
             (multiple-value-bind (before-~ after-~)
                 (sc-misc:list-until str-list nil
                                     :test #'(lambda (x y)
					       (declare (ignore y))
                                               (and (eql #\~ (first x))
                                                    (eql #\| (second x)))))
               (if after-~
                   (progn
                     (setq after-~ (cddr after-~)) 
                     (multiple-value-bind (code step)
                         (parse-integer (concatenate 'string after-~)
                                        :radix 8 :junk-allowed t)
                       `(,@before-~
                         ,(code-char code)
                         ,@(sc-format-list (nthcdr (1+ step) after-~)))))
                   before-~))))
    (let ((retstr
           (concatenate 'string
                        (sc-format-list (concatenate 'list format-string)))))
      (apply #'format nil retstr args) )))

;;; SC文字列->C文字列( princ でCのstring-literalが出力される)
(defun string-sc2c (str &rest args)
  (when (stringp str)
    (let ((f-str (apply #'sc-format str args)))
      (apply #'string+ (map 'list #'char-sc2c f-str)))))
