(private x int)
(private y long)

(def (g a) (fn int int)
  (return (- a)))

(def (f a b) (fn int int int)
  (return (* (g a) b x y)))


  