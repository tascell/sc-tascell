(def (h i g) (fn int int (ptr (lightweight int int)))
  (return (g (g i))))

(def (foo a) (fn int int)
  (def x int 0)
  (def (g1 b) (lightweight int int)
    (inc x)
    (return (+ a b)))
  (return (+ (h (+ a 1) g1) x)))

(def (main) (fn int)
  (return (foo 1)))
