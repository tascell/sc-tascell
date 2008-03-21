(%rule (:multithread-sc1 :multithread-type :multithread-temp :multithread :untype))

(c-exp "#include<stdio.h>")
(c-exp "#include<stdlib.h>")

(def (pfib n) (fn int int)
  (def x int) (def y int) (def nn int 0) (def c cont 0)
  (if (<= n 2) 
      (return 1)
      (begin
        (thread-create nil
          (= x (pfib (- n 1)))
	  (inc nn)
	  (if (== nn 0)
	      (thread-resume c)))
	(= y (pfib (- n 2)))
	(= nn (- nn 1))
	(if (< nn 0)
	    (thread-suspend c0
	      (= c c0)))
	(return (+ x y)))))

(def (msc-main argc argv) (fn int int (ptr (ptr char)))
  (def n int 4)
  (if (> argc 1)
      (= n (c-exp "atoi(~A)" (aref argv 1))))  
  (c-exp "printf(~A, ~A, ~A);" "fib(%d)=%d~%" n (pfib n))
  (return n))

