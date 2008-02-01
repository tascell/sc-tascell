(load "~/work/sc/source/sc-misc.lsp")

(defvar *table* nil)  ;; list of ((<prog> <machine[-icc]> <impl>) time1 time2 ...)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; example
;; (make-tex-table (make-table "rslt-spao-06" "rslt-dougo-06" "rslt-wfs2-06" "rslt-totukawa-06"))
;; (make-tex-table (make-table "rslt-spao-13" "rslt-dougo-13" "rslt-wfs2-13" "rslt-totukawa-13"))

(defun make-table (&rest filenames)
  (let ((*table* nil))
    (loop for fname in filenames
	do (add-table fname))
    *table*
    ))

(defun add-table (file)
  (let (curprog curmachine curimpl curicc-p)
    (with-open-file (istr file :direction :input)
      (do-text-line (line istr)
	(acond
	 ((string-begin-with "prog:" line)
	  (setq curprog (read-from-string it)))
	 ((string-begin-with "machine:" line)
	  (setq curmachine (read-from-string it)))
	 ((string-begin-with "exefile:" line)
	  (let ((rem (string-left-trim " " it)))
	    (unless (setq rem (string-begin-with
			       (string+ "./" (string-downcase (symbol-name curprog)) "-")
			       rem))
	      (error "Unexpected exefile ~S: (progname ~S)" it curprog))
	    (aif (string-begin-with "icc-" rem)
		 (progn
		   (setq rem it)
		   (setq curicc-p t))
		 (setq curicc-p nil))
	    (awhen (search "-sol" rem)
	      (setq rem (subseq rem 0 it)))
	    (setq curimpl (read-from-string rem))))
	 ((string-begin-with "time:" line)
	  (assert (and curprog curmachine curimpl))
	  (let* ((curmachine-icc (if curicc-p (symbol+ curmachine '-icc) curmachine))
		 (time (read-from-string it))
		 (entry (list curprog curmachine-icc curimpl)))
	    (aif (find entry *table* :key #'car :test #'equal)
		 (push time (cdr it))
		 (push (cons entry (list time)) *table*))))
	 (t nil))))))

(defun yuukou-float (flt n)
  (let* ((zflt (abs flt))
	 (shousuu (loop for i from (+ n 10) downto 1
		      as e upfrom -10
		      as kurai = (expt 10 e)
		      when (< zflt kurai)
		      do (return i)
		      finally (return 0))))
    (format nil "~,VF" shousuu flt)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defparameter *prog-progname*
    '((bintree  "BinTree    " "copying    " "GC         ")
      (bin2list "Bin2List   " "copying    " "GC         ")
      (cpfib    "fib(37)    " "check-     " "pointing   ")
      (nqueen   "nqueens(13)" "load       " "balancing  ")
      (qsort    "Quick      " "sort       ")
      ;; tcell
      (fib      "fib(37)    " "load       " "balancing  ")
      (lu       "LU(1000)   " "load       " "balancing  ")
      (pen      "Pentomino  " "load       " "balancing  ")
      (cmp      "Comp(20000)" "load       " "balancing  ")
      ))
(defun progname (prog)
  (cdr (assoc prog *prog-progname*)))
(defun progrank (prog)
  (position prog *prog-progname* :key #'car))

(defparameter *machine-machinename*
    '((wfs2 "S(GCC)")
      (DOUGO.YLAB-LOCAL.KUIS.KYOTO-U.AC.JP "P(GCC)")
      (DOUGO.YLAB-LOCAL.KUIS.KYOTO-U.AC.JP-ICC "P(ICC)")
      (spao "X(GCC)")
      (spao-icc "X(ICC)")
      (totukawa "C(GCC)")
      (totukawa-icc "C(ICC)")
      ))
(defun machine-name (machine)
  (second (assoc machine *machine-machinename*)))
(defun machine-rank (machine)
  (position machine *machine-machinename* :key #'car))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(let ((current-prog-strings nil)
      (comment nil))
  (let ((base nil))
    (defun make-table-line (machine)
      (let ((mname (machine-name (second (caar machine))))
	    (time-array (make-array 6)))
	(loop for impl in '(c gcc clos xcccl lw xcc)
	    as i upfrom 0
	    do (awhen (find impl machine :key (compose #'third #'car))
		 (setf (aref time-array i) (median (cdr it)))))
	(unless (search "ICC" mname)
	  (setq base (aref time-array 0)))
	(assert base)
	(with-output-to-string (ostr)
	  (format ostr "~A" (or (pop current-prog-strings)
				"           "))
	  (format ostr " & ~A " mname)
	  (loop for time across time-array
	      do (if time
		     (format ostr "& ~A(~A) "
			     (yuukou-float time 3)
			     (yuukou-float (/ time base) 3))
		   (format ostr "& ---         ")
		   )))))
    )
  (defun make-tex-table (table)
    (loop for prog in (sort (assort table :key #'caar) #'<
			    :key (compose #'progrank #'caaar))
	do
	  (when comment (format t "~&%%% ~S" (caaar prog)))
	  (setq current-prog-strings (progname (caaar prog)))
	  (loop for machine in (sort (assort prog :key (compose #'second #'car)) #'<
				     :key (compose #'machine-rank #'second #'caar))
	      do  (when comment (format t "~&%% ~S~%" (second (caar machine))))
		  (format t "~A~%" (make-table-line machine)))))
  )
