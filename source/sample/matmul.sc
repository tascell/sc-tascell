(deftype mat (array int 2 2))
(decl printf (fn int (ptr (const char)) va-arg))

;(private pa int)
;(private pb long)

((matmul a b c) (fn void mat mat mat)
  (defs int i j k)
  (for ( (def i int 0) (<= i 1) (inc i) )
   (for ( (def j int 0) (<= j 1) (inc j) )
    (= (aref c i j) 0)))
  (for ( (def i int 0) (<= i 1) (inc i) )
   (for ( (def j int 0) (<= j 1) (inc j) )
    (for ( (def k int 0) (<= k 1) (inc k) )
     (deftype l int)
     (+= (aref c i j) (* (aref a i k) (aref b k j))))))
  
  (def k__2 unsigned-char)

  (return))

((main) (fn int)
 (let ((defs mat (a (array (array 1 2) (array 3 4)))
                 (b (array (array 5 6) (array 7 8))) c))
  (matmul a b c)
  (printf "a:%d %d %d %d \\n" 
    (aref a 0 0) (aref a 0 1) (aref a 1 0) (aref a 1 1))
  (printf "b:%d %d %d %d \\n" 
    (aref b 0 0) (aref b 0 1) (aref b 1 0) (aref b 1 1))
  (printf "c:%d %d %d %d \\n" 
    (aref c 0 0) (aref c 0 1) (aref c 1 0) (aref c 1 1))
  (return)))