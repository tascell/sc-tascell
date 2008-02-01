(defun shell () (system "$SHELL"))
(setq *print-case* :downcase)

(let ((compile-file-list
       '("sc-defs" "sc-misc" "sct")))
  (dolist (cf compile-file-list)
    (compile-file cf)))

(setq rule
      '("rule/basic.rule" "rule/rename.rule" "rule/hoist.rule"
	"rule/type.rule" "rule/tmp.rule" "rule/multithread.rule"
	"rule/untype.rule" "rule/basic.rule" "rule/rename.rule"
	"rule/hoist.rule" "rule/type.rule" "rule/tmp.rule"
	"rule/nestfunc.rule" "rule/untype.rule"))
(setq rule2 
      '("rule/basic.rule" "rule/type.rule" "rule/tmp.rule"
	"rule/nestfunc.rule" "rule/untype.rule"))
(setq rule3
      '("rule/basic.rule" "rule/type.rule" "rule/tmp.rule"
	"rule/untype.rule"))

(setq rule4
      '("rule/basic.rule" "rule/rename.rule" "rule/hoist.rule"))

(setq rule5 '("rule/labeled-break.rule"))

(setq rule6 '("rule/basic.rule"
	      "rule/type.rule" "rule/tmp.rule" "rule/multithread.rule"
	      "rule/untype.rule"))

(setq scl '("sample/multithread-orig.sc" "sample/fib.sc"
	    "sample/tmptest.sc" "sample/l-closure.sc"
	    "sample/nonlocalexit.sc" "sample/nonlocalexit2.sc"
	    "sample/hoisting.sc" "sample/matmul.sc"
	    "sample/lbreak.sc"))
(setq scn scl sc (car scn))

(setq ruleb (butlast rule))
(setq rulel (last rule))

(setq untype "rule/untype.rule")

(setq temp "temp.sc")
(setq ctemp "temp.c")

(defun indent (cfile)
  (system (format nil "sleep 1;indent ~A" cfile)))

(defun ev1 (&optional (rule rule) (sc sc))
  (sc2c sc :rule rule :output-file temp))
(defun ev2 (&optional (sc-temp temp))
  (format t "applying sc2c...~%")
  (sc2c sc-temp :rule "sc2c.rule" :output-file ctemp))
(defun ev3 ()
  (indent ctemp))

(defun evall (&optional (rule rule))
  (ev1 rule) (ev2) (ev3))
(defun csc (&optional (n 1))
  (cond ((= n 0)
	 (values (setq sc (car scn)) scl))
	((< n 0)
	 (csc (+ n (length scl))))
	(t
	 (progn
	   (setq scn (or (cdr scn) scl))
	   (csc (1- n))))))

(defun dbg ()
  (setq *sct-debug* 
	(if *sct-debug* nil t)))

(defun gcc (file) (system (concatenate 'string "gcc -Wall -g -O4 " file)))

(print `(scl ,scl))
(print `(sc ,sc))
(print `(rule ,rule))
(print `(rule2 ,rule2))
(print `(temp ,temp))
(print `(ev1 ,#'ev1))
(print `(ev2 ,#'ev2))
(print `(ev3 ,#'ev3))
