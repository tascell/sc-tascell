(def (task_fib n) (task int int)
  (reply_result (fib n)))

(def (fib n) (fn int int)
  (def s int 0)
  (def r (task_data int int))
  (def restp int)
  (if (<= n 2) (return 1))
  (= restp 1)
  (handle
     (split 
        (if restp (begin (= restp 0) (spawn r (task_fib (- n 2))))))
     (+= s (fib (- n 1))))
  (if restp
      (+= s (fib (- n 2)))
      (begin (wait_result r) (+= s (result_value r))))
  (return s))
