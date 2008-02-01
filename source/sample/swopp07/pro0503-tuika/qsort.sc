;; got from 
;; http://chat.carleton.ca/~xnie/Myresearch/route_table_code/c/qsort.c
(%defconstant NESTFUNC-TAG lightweight)
(%rule (%if (cl::eq 'lightweight 'NESTFUNC-TAG)
	    ((:basic :type :tmp :nestfunc :untype))
	    (:default)))

;;;;;;;;;;
(deftype size-t long)
(deftype WORD long)

(decl (csym::malloc size) (fn (ptr void) size-t))
(decl (csym::printf) (fn int (ptr (const char)) va-arg))
(decl (csym::atoi)   (fn int (ptr (const char))))

(def (struct timeval)
  (def tv_sec long)
  (def tv_usec long) )
(def (struct timezone)
  (def tz_minuteswest int)
  (def tz_dsttime int) )
(decl (csym::gettimeofday tp tzp)
      (fn int (ptr (struct timeval)) (ptr (struct timezone))))
;;;;;;;;;;

(%defconstant W (sizeof WORD))
(%defmacro SWAPINIT (a es)
  `(= swaptype (if-exp (% (bit-or (- ,a (cast (ptr char) 0)) ,es) W)
		2
		(if-exp (> ,es W) 1 0))))

(%defmacro exch (a b t)
  `(exps (= ,t ,a) (= ,a ,b) (= ,b ,t)))

(%defmacro swap (a b)
  `(if-exp (!= swaptype 0)
           (swapfunc ,a ,b es swaptype)
           (cast void (exch (mref (cast (ptr WORD) ,a))
		            (mref (cast (ptr WORD) ,b))
                            t))))

(%defmacro vecswap (a b n)
  `(if (> ,n 0) (swapfunc ,a ,b ,n swaptype)))


(static-def (swapfunc a b n swaptype) 
            (fn void (ptr char) (ptr char) size-t int)
  (if (<= swaptype 1)
      (begin
       (def t WORD)
       (for ( 0 (> n 0) (exps (+= a W) (+= b W) (-= n W)))
	 (exch (mref (cast (ptr WORD) a))
	       (mref (cast (ptr WORD) b))
	       t)))
      (begin
       (def t2 char)
       (for ( (> n 0) (exps (+= a 1) (+= b 1) (-= n 1)))
	 (exch (mref a) (mref b) t2)))))

(%defmacro PVINIT (pv pm)
  `(if (!= swaptype 0)
       (begin (= ,pv a) (swap ,pv ,pm))
       (begin (= ,pv (cast (ptr char) (ptr v)))
	      (= v (mref (cast (ptr WORD) ,pm))))))

(%defmacro min (a b)
  `(if-exp (< ,a ,b) ,a ,b))

(static-def (med3 a b c cmp)
            (fn (ptr char) (ptr char) (ptr char) (ptr char) 
		(ptr (NESTFUNC-TAG int 
				  (ptr (const void)) (ptr (const void)))))
  (return (if-exp (< (cmp a b) 0)
		  (if-exp (< (cmp b c) 0)
			  b 
			  (if-exp (< (cmp a c) 0) c a))
		  (if-exp (> (cmp b c) 0)
			  b
			  (if-exp (> (cmp a c) 0) c a)))))

(def (print-array a n) (fn void (ptr int) int)
  (def i int)
  (for ((= i 0) (< i n) (inc i))
    (csym::printf "%d%c" (aref a i) (if-exp (< (+ i 1) n) #\Space #\Newline))))

(def (quicksort a n es cmp)
     (fn void (ptr char) size-t size-t 
	(ptr (NESTFUNC-TAG int (ptr (const void)) (ptr (const void)))))
  (defs (ptr char) pa pb pc pd pl pm pn pv)
  (defs int r swaptype)
  (defs WORD t v)
  (def s size-t)

  ;;(print-array a n)
  (SWAPINIT a es)
  ;;(csym::printf "swaptype=%d~%" swaptype)
  (if (< n 7)        ; insertion sort on smallest arrays
      (begin
       (= pl a)
       (for ((= pm (+ a es)) (< pm (+ a (* n es))) (+= pm es))
	 (for ((= pl pm) 
	       (and (> pl a) (> (cmp (- pl es) pl) 0))
	       (-= pl es))
	   ;;(csym::printf "%d %d~%" (mref (cast (ptr int) pl))
	   ;;	 (mref (cast (ptr int) (- pl es))))
	   (swap pl (- pl es))
	   ;;(csym::printf "%d %d~%" (mref (cast (ptr int) pl))
	   ;;	 (mref (cast (ptr int) (- pl es))))
	   ))
       (return)))
  (= pm  (+ a (* (/ n 2) es)))  ; Small arrays middle element
  (if (> n 7)
      (begin
       (= pl a)
       (= pn (+ a (* (- n 1) es)))
       (if (> n 40)             ; Big arrays, psudomedian of 9
	   (begin
	    (= s (* (/ n 8) es))
	    (= pl (med3 pl             (+ pl s) (+ pl (* 2 s)) cmp))
	    (= pm (med3 (- pm s)       pm       (+ pm s)       cmp))
	    (= pn (med3 (- pn (* 2 s)) (- pn s) pn             cmp))))
       (= pm (med3 pl pm pn cmp))))  ; Mid-size, med of 3
  ;; (csym::printf "%d~%" (mref (cast (ptr int) pm)))
  (PVINIT pv pm)  ; pv points to partition value
  (= pa (= pb a))
  (= pc (= pd (+ a (* (- n 1) es))))
  (loop
   (while (and (<= pb pc) 
	       (<= (= r (cmp pb pv)) 0))
     (if (== r 0) (begin (swap pa pb) (+= pa es)))
     (+= pb es))
   (while (and (>= pc pb)
	       (>= (= r (cmp pc pv)) 0))
     (if (== r 0) (begin (swap pc pd) (-= pd es)))
     (-= pc es))
   (if (> pb pc) (break))
   (swap pb pc)
   (+= pb es)
   (-= pc es))
  (= pn (+ a (* n es)))
  (= s (min (- pa a)  (- pb pa)   )) (vecswap a  (- pb s) s)
  (= s (min (- pd pc) (- pn pd es))) (vecswap pb (- pn s) s)
  (if (> (= s (- pb pa)) es) (quicksort a        (/ s es) es cmp))
  (if (> (= s (- pd pc)) es) (quicksort (- pn s) (/ s es) es cmp)))


(def (modsort a n m) (fn void (ptr int) int int)
  (def (comp-mod pp pq) (NESTFUNC-TAG int (ptr int) (ptr int))
      (return (if-exp (< (% (mref pp) m) (% (mref pq) m))
		      1
		      (if-exp (== (% (mref pp) m) (% (mref pq) m))
			      0
			      -1))))
  (quicksort a n (sizeof int) comp-mod))




(def aa (ptr int))
(def nn int)
(def mm int)

(def (foo n) (fn void int)
  (if n
      (foo (- n 1))
      (modsort aa nn mm)))

(def (main argc argv) (fn int int (ptr (ptr char)))
  (def n int 30)
  (def m int 10)
  (def i int)
  (def nfoo int 0)
  (def a (ptr int))
  (defs (struct timeval) tp1 tp2)
  (def ttime double)

  (if (> argc 1) (= n (csym::atoi (aref argv 1))))
  (if (> argc 2) (= m (csym::atoi (aref argv 2))))
  (if (> argc 3) (= nfoo (csym::atoi (aref argv 3))))
  (= a (csym::malloc (* (sizeof int) n)))
  (for ((= i 0) (< i n) (inc i))
    (= (aref a i) i))
  (= aa a) (= nn n) (= mm m)
  (if (> argc 4) (print-array a n))
  (csym::gettimeofday (ptr tp1) 0)
  (if nfoo (foo nfoo) (modsort a n m))
  (csym::gettimeofday (ptr tp2) 0)
  (if (> argc 4) (print-array a n))

  (= ttime (+    (- (fref tp2 tv_sec) (fref tp1 tv_sec))
	      (* (- (fref tp2 tv_usec) (fref tp1 tv_usec))
		 0.000001)))
  
  (csym::printf "%f~%" ttime)

  (return 0))
