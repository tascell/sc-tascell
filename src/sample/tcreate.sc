(def (f x) (fn int int)
  (thread-create ((a int 10) (b int 20))
     (= x (+ a b)))
  (return x))