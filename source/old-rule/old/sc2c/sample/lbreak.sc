(def (f x) (fn int int)
  (label l1 
   (do-while (> x 0)
     (do-while (<= x 10)
       (inc x)
       (if (< x 5)
	   (continue l1)
	   (break l1))
       (= x (g x)))
     (= x (h x))))
  (return x))

#|
(def (f x) (fn int int)
  (label l1 
   (do-while (> x 0)
    (label l2 
     (do-while (<= x 10)
       (if (== x 0)
	   (break l1)     ; break     (do-while (>  x 0)  ...)
	   (continue l2)) ; continue  (do-while (<= x 10) ...)
       (g x))))))
|#