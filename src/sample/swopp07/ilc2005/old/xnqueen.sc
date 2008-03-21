;; 先頭の#include は手動ではずして

(%include "copygc.sh")
(%include "copygc.sc")

(def (csym::extend-ok j k a) (fn int int int (ptr char))
  (def i int)
  (for ((= i 0) (< i k) (inc i))
    (if (or (== j (aref a i))
            (== j (- (aref a i) (- k i)))
            (== j (+ (aref a i) (- k i))))
        (return 0)))
  (return 1))


(def (nqueens m n k a) (fn int (ptr (lightweight int)) int int (ptr char))
  (def (mon) (lightweight int)
    (return (+ (m) n k)))
  (def b (ptr char))
  (if (== n k)
      (return 1)
      (begin
       (def s int 0)
       (def i int)
       ;; try each possible postition for queen <k> 
       (for ((= i 0) (< i n) (inc i))
         (if (csym::extend-ok i k a)
             (begin
              ;; allocate a temporary array and copy <a> in it
              (= b (csym::malloc (* (+ k 1) (sizeof char))))
              (csym::memcpy b a (* k (sizeof char)))
              (= (aref b k) i)
              (+= s (nqueens mon n (+ k 1) b))
              (csym::free b) )))
       (return s) )))

(def (main argc argv) (fn int int (ptr (ptr char)))
  (def (mon) (lightweight int)
    (return 0))
  (def n int)
  (def s int)
  (def a (ptr char))
  (def tp1 (struct timeval))
  (def tp2 (struct timeval))
  (if (< argc 2)
      (begin
       (csym::printf "%s: number of queens required~%" (aref argv 0))
       (return 1)))
  (= n (csym::atoi (aref argv 1)))
  (= a (csym::malloc (* n (sizeof char))))
  
  (csym::printf "running queens %d~%" n)
  (csym::gettimeofday (ptr tp1) 0)  
  (= s (nqueens mon n 0 a))
  (csym::gettimeofday (ptr tp2) 0)  
  (csym::printf "%d answers~%" s)

  (csym::printf "time=%f~%"
                (+
                 (- (fref tp2 tv_sec) (fref tp1 tv_sec))
                 (*
                  (- (fref tp2 tv_usec) (fref tp1 tv_usec))
                  0.000001)))

  (csym::free a)
  (return 0))
