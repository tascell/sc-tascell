;;;
(deftype size_t unsigned-int)
(decl (csym::printf format) (fn int (ptr (const char)) va-arg))
(decl (csym::atoi nptr) (fn int (ptr (const char))))
(def (struct timeval)
  (def tv_sec long)
  (def tv_usec long) )
(def (struct timezone)
  (def tz_minuteswest int)
  (def tz_dsttime int) )
(decl (csym::gettimeofday tp tzp)
      (fn int (ptr (struct timeval)) (ptr (struct timezone))))
;;;

(def (csym::save-int i) (fn void int)
  (csym::printf "int: %d~%" i))
(def (csym::save-pc pc) (fn void int)
  (csym::printf "pc: %d~%" pc))

(def n-in int)

(def (csym::fib n) (fn int int)
  (def s int)
  (if (<= n 2)
      (return 1)
      (begin
       (= s 0)
       (+= s (fib (- n 1)))
       (+= s (fib (- n 2)))
       (return s))))

(def done int 0)

(def (cpfib scan0 n) (fn int (ptr (lightweight void)) int)
  (def pc int 0)
  (def s int 0)
  (def (save1) (lightweight void)
    (scan0)
    (csym::save-pc pc)
    (csym::save-int s)
    (csym::save-int n))
  (if (<= n 2)
      (return 1)
      (begin
       (= pc 1)
       (+= s (cpfib save1 (- n 1)))
       (= pc 2)
       (+= s (cpfib save1 (- n 2)))
       (return s)))
  )
  

(def (main argc argv) (fn int int (ptr (ptr char)))
  (def tp1 (struct timeval))
  (def tp2 (struct timeval))
  (def i int)
  (def (save1) (lightweight void)
    )
  
  (= n-in (if-exp (> argc 1) (csym::atoi (aref argv 1)) 30))

  (csym::printf "cpfib(%d)~%" n-in)
  
  ;; time
  (csym::gettimeofday (ptr tp1) 0)
  (= i (cpfib save1 n-in))
  (csym::gettimeofday (ptr tp2) 0)

  (csym::printf "fib(%d)=%d~%" n-in i)

  (csym::printf 
   "%f~%"
   (+    (- (fref tp2 tv_sec)  (fref tp1 tv_sec))
      (* (- (fref tp2 tv_usec) (fref tp1 tv_usec)) 0.000001)) )

  (return 0)
  )
