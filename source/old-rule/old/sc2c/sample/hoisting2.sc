((f) (fn void)
 (def x int)
 (def y int)
 (begin
  (def y (const int) 20)
  (begin
   (= x 5)
   (def x int)
   (= x 4))
  (= x 3)
  (def x int 5)
  (= x 2)))
