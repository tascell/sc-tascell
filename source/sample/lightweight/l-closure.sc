(%rule (:nestfunc-sc1 :nestfunc-type :nestfunc-temp :nestfunc :untype))

(deftype size-t long)

(def (h i g) (fn int int (ptr (lightweight int int)))
  (return (g (g i))))

(def (foo a) (fn int int)
  (def x int 0)
  (def y int 0)
  (def (g1 b) (lightweight int int)
    (inc x)
    (return (+ a b)))
  (= y (h 10 g1))
  (return (+ x y))) 

(def (main) (fn int)
  (return (foo 1)))
