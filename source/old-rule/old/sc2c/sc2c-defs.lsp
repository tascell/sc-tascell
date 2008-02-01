;; utilitis for sc2c.rule
(provide "SC2C-DEFS")
(in-package "SC2C")

(require "SC-UTIL")
(use-package "SC-UTIL")

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

(defun operator (x)
  (when (member x '(* / % + - << >> 
		    bit-and bit-xor bit-or and or)
		:test #'eq) 
    (values
     (operator-sc2c x)          ; sc-1 に対応する C言語の文字列
     (case x                    ; 結合優先度
       ((* / %) 4)
       ((+ -) 5)
       ((<< >>) 6)
       ((bit-and) 9)
       ((bit-xor) 10)
       ((bit-or) 11)
       ((and) 12)
       ((or) 13)
       (otherwise nil))
     (case x                    ; 単位元
       ((* bit-and and) '1)
       ((+ - bit-or or) '0)
       (otherwise nil)))))

(defun comparator (x)
  (when (member x '(< > <= >= == !=) :test #'eq)
    (values
     (operator-sc2c x)
     (case x
       ((< > <= >=) 7)
       ((== !=) 8)
       (otherwise nil)))))

(defun assignment-operator (x)
  (when (member x '(= *= /= %= += -= <<=
		    >>= bit-and= bit-xor= bit-or=)
		:test #'eq)
    (values (operator-sc2c x) 15)))

(defun unary-operator (x)
  (when (member x  '(ptr mref bit-not not) :test #'eq)
    (values (operator-sc2c x) 2)))
