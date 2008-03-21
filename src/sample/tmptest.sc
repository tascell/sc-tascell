(def (g x) (fn int int)
  (return (* x x)))

(def (h x) (fn int int)
  (return (+ x x)))

(def (i x) (fn int int)
  (return (- x (* 2 x))))


(def (f1 x) (fn int int)
  (return (if-exp (> (g x) 100) 
		  (h x)
		  (i x))))

(def (f2 x) (fn int int)
  (def y int)
  (if (and  (> (g x) 100)
	    (< (h x) 200))
      (if (or (> (g x) 200)
	      (> (h x) 300))
	  (return (g (i (h x))))
	  (return (= y 300)))
      (return (exps (= y (g x)) (= y (h y)) (= x (g (i (h y))))))))
      
(def (f3 x) (fn int int)
  (def i int)
  (for ((= i 0) (> x (f2 i)) (inc i))
    (if (> i 10)
	(continue))
    (if (> i 100)
	(break)
	(+= i 3)))
  (return (f1 i)))
