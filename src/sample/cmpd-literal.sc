(%rule ())

(decl (csym::printf) (csym::fn int (ptr (const char)) va-arg))

(def (f1) (fn void void)
  (def p (ptr int) (init (array int 5) (array 50 39)))
  (= (mref p) 30)
  (= p (init (array int 2) (array (mref p))))
  (csym::printf "%d~%" (aref p 0)))

(def (struct s)
  (def i int))

(def (f2) (fn int void)
  (defs (ptr (struct s)) (p 0) q)
  (def j int 0)

  (label again nil)
  (= q p)
  (= p (ptr (init (struct s) (struct (inc j)))))
  (if (< j 2) (goto again))

  (return (and (== p q) (== (fref q -> i) 1))))

 
(def (struct point)
  (def x int)
  (def y int))

(def (drawline from to) (fn void (struct point) (struct point))
  (csym::printf "draw:(%d, %d) <--> (%d, %d)~%"
		(fref from x) (fref from y) (fref to x) (fref to y))
  (return))

(def (p-drawline pfrom pto)
     (fn void (ptr (struct point)) (ptr (struct point)))
  (csym::printf "DRAW:(%d, %d) <--> (%d, %d)~%"
		(fref pfrom -> x) (fref pfrom -> y)
		(fref pto -> x)   (fref pto -> y))
  (return))


(def (struct int-list)
  (def car int)
  (def cdr (ptr (struct int-list))))

(def (main) (fn int void)
  (def endless-zeros (struct int-list)
       (init (struct int-list) (struct 0 (ptr endless-zeros))))
  (drawline (init (struct point) (struct ((fref-this x) 1) ((fref-this y) 1)))
	    (init (struct point) (struct ((fref-this y) 4) ((fref-this x) 3))))
  (p-drawline
   (ptr (init (struct point) (struct ((fref-this x) 1) ((fref-this y) 1))))
   (ptr (init (struct point) (struct ((fref-this y) 4) ((fref-this x) 3)))))
  
  (init (array (const float) 6)
	(array 1.0 2.0 3.0 4.0 5.0 6.0))

  (csym::printf "%s~%" (init (array char 20) (array "abcdefghij")))
  (f1)
  (csym::printf "(f2) = %d~%" (f2))
  (return 0))
