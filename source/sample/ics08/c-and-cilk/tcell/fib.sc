(%include "worker.sh")
(%rule (:tcell-type :tcell :lightweight :untype))

(def (task fib)
  (def n int :in)
  (def r int :out))

(def (fib n) (wfn int int)
  (if (<= n 2)
      (return 1)
    (begin
      (defs int s1 s2)
      (do-two
	  (= s1 (fib (- n 1)))
	  (= s2 (fib (- n 2)))
	(fib
	 (:set (= @n (- n 2)))
	 (:get (= s2 @r))))
      (return (+ s1 s2)))))

#| 以下，自動生成

void send_int(int n){ fprintf(stdout, " %d", n); }
void recv_int(int *n){ fscanf(stdin, " %ld", n); }
(def (struct task-fib)
  (def n int)
  (def r int))

(def (send-task-body thr x0)
  (fn void (ptr (struct thread-data) (ptr void)))
  (def x0 x (ptr (struct task-fib)) x0)
  (send-int (fref x -> n)))

;; malloc と send-rslt-bodyのfreeをどう扱うか？
(def (recv-task-body thr)
  (fn (ptr void) (ptr (struct thread-data)))
  (def x (ptr (struct task-fib)))
  (= x (cast (ptr (struct task-fib))
	 (csym::malloc (sizeof (struct task-fib)))))
  (recv-int (ptr (fref x -> n)))
  (return x))

(def (send-rslt-body thr x0)
  (fn void (ptr (struct thread-data)) (ptr void))
  (def x (ptr (struct task-fib)) x0)
  (send-int (fref x -> r))
  (csym::free x))

(def (recv-rslt-body)
  (fn void (ptr (struct thread-data)) (ptr void))
  (def x (ptr (struct task-fib)) x0)
  (recv-int (ptr (fref x -> r))))

(def (do-task-body thr x0)
  (fn void (ptr (struct thread-data)) (ptr void))
  (def (_bk) (lightweight int void) (return 0))
  (def x (ptr (struct task-fib)) x0)
  (def n int (fref x-> n))
  (csym::fprintf stderr "Start %d~%" n)
  (= (fref x -> r) (fib _bk _thr n)))
  
|#


