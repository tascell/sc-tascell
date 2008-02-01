#|
#include<stdio.h>
#include<sys/time.h>
#include<math.h>
#include"xccmem.h"
#include"systhr.h"
|#

(defmacro @MAXNPROC () 64)
(defmacro @DPRINT0 ())

(decl (struct _proc_locals0))
(deftype proc-env (ptr (struct _proc_locals)))

((struct _task_buf)
 (f (ptr (fn void proc-env (ptr (struct _tasuk_buf)))))
 (sender int)  #| optional |#
 (why int)     #| optional |#
 ((struct str-t1)
  (result-buf-p (ptr int))
  (result-port-p (ptr int))
  (k int))
 ((union uni-a)
  (t1 (struct str-t1)))
 (a (union uni-a)))
(deftype task-buf-t (struct _task_buf))

((struct _req_buf)
 (task-buf-p (ptr task-buf-t))
 (task-port-p (ptr int))
 (req-port-p (ptr (ptr void))))        #| (struct _req_buf **) |#
(deftype req-buf-t (struct _req_buf))

((struct _proc_locals)
 (n_proc int)
 (myid int)
 (task-buf task-buf-t)
 (task-port int)
 (req-buf req-buf-t)
 (dumy (array (ptr void) 64))
 (req-port (ptr void))                 #| (req_buf_t *) |#
 (dumy2 (array (ptr void) 64))
 (dumy3 (array (ptr void) 64)))
(pe (array (struct _proc_locals) (@MAXNPROC)))

((init-proc-locals pr id num-proc) (fn void proc-env int int)
 (def i int)
 (= (fref pr -> n-proc) num-proc)
 (= (fref pr -> myid) id)
 (= (fref pr -> req-port) 0))

((tf-do-nothing pr task-buf-p) (fn void proc-env (ptr task-buf-p)))

((refuse-req pr) (fn void proc-env)
 #| transfer a null task to refuse a request |#
 (def req-buf-p (ptr void) (atomic-read-ptr (fref pr -> req-port)))
 (if req-buf-p
     (let ((rbp (ptr req-buf-t) (cast (ptr req-buf-t) req-buf-p)))
       (start-read-after-read)
       (atomic-write-ptr (fref pr -> req-port) 0)
       (= (fref rbp -> task-buf-p -> f) tf-do-nothing)
       (finish-write-before-write)
       (atomic-write-int (ptr (fref rbp -> task-port-p)) 1)))
)

#| for idle processor |#
((steal-run-task pr req-port-p why) (fn void proc-env (ptr (ptr void)) int)
 (= (fref pr -> task-port) 0)
 (= (fref pr -> task-buf why) why)
 #| request a task |#
 (= (fref pr -> req-buf task-buf-p)  (mref (fref pr -> task-buf)))
 (= (fref pr -> req-buf task-port-p) (mref (fref pr -> task-port)))
 (= (fref pr -> req-buf req-port-p)  (mref (fref pr -> req-port)))
 (finish-write-before-write)
 (while (cas-ptr (ptr req-port-p) 0 
		 (cast (ptr void) (mref (fref pr -> req-buf))))
   (refuse-req pr))
 #| wait for a task |#
 (while (== (atomic-read-int (fref pr -> task-port)) 9)
   (refuse-req pr))
 (start-read-after-read)
 #| run the stolen task |#
 ((fref pr -> task-buf f) pr (mref (fref pr -> task->buf)))
)

(bv int)

((helper p) (fn (ptr void) (ptr void))
 (def pr proc-env (cast proc-env p))
 (defs double r q)
 (def n int)
 (systhr-bind-to-proc (fref pr -> myid))
 (systhr-bind-to-proc (fref pr -> myid))
 (do-while (cas-int bv n (+ n 1))  ;(when (== bv n) (= bv (+ n 1)))
   (= n bv))
 (for ((= r (sqrt (+ 0.5 (fref pr -> myid))))
       (= q (sqrt (+ r (fref pr -> myid))))
       (-= r (cast int r))
       1
       (exps (=  r (+ (* r 3.0) g))
	     (-= r (cast int r))))
      (def tg int (cast int (* (fref pr -> n-proc) r)))
      (if (!= tg (fref pr -> myid))
	  (steal-run-task pr (mref (fref (aref pe tg) req-port)) 0))
      )
)

((init-parallel num-proc (fn void int))
 (def i int)
 (for ((= i 0) (< i num-proc) (inc i))
      (init-proc-locals (+ pe i) i num-proc))
 (systhr-init num-proc)  ;initialize global thread setting for "np" threads.
 (systhr-bind-to-proc 0) ;bind the current thread to a specific processor.
 (= bv 1)
 (for ((= i 1) (< i num-proc) (inc i))
      ;spawn a new thread to execute "start_func(arg)".
      (systhr-create helper (+ pe i))) 					
 (while (< (atomic-read-int bv) num-proc)
   (start-read-after-read)))

(defmacro @POLL (req-port bk)
  `(do-while 0
    (let ((req-buf-p (ptr void) (atomic-read-ptr ,req-port)))
      (if req-buf-p
	  (let ((rbp (ptr req-buf-t) (cast (ptr req-buf-t) req-buf-p)))
	    (start-read-after-read)
	    (atomic-write-ptr ,req-port 0)
	    (if (== (,bk rbp) 0)
		(block
		  (= (fref rbp -> task-buf-p -> f) tf-do-nothing)
		  (finish-write-before-write)
		  (atomic-write-int (ptr (fref rbp -> task-port-p)) 1))))))))

(decl (fib pr req bkf k) (fn int proc-env (ptr (fn int (ptr req-buf-t))) int))

(tv0 (struct timeval))

((tf-fib pr task-buf-p) (fn void proc-env (ptr task-buf-t))
 (let (((bk req-buf-p) (fn int (ptr req-buf-t))
	 (return 0))
       (saved-result-port-p (ptr int))
       (saved-result-buf-p (ptr int))
       (tbp (ptr task-buf-t) task-buf-p))
   #| save info for "result back" |#
   (= saved-result-port-p (fref tbp -> a t1 result-port-p))
   (= saved-result-buf-p  (fref tbp -> a t1 result-bur-p))
   #| get task |#
;ifdef DPRINT
   (let ((tv1 (struct timeval)))
     (gettimeofday (mref tv1) 0)
     (printf "%9d %2d gets %2d of %2d for %d \\n"
	     (+ (* (- (fref tv1 tv-sec) (fref tv0 tv-sec)) 1000000)
		(- (fref tv1 tv-usec) (fref tv0 tv-usec)))
	     (fref pr -> myid) (fref tbp -> a t1 k) (fref tbp -> sender)
	     (fref tbp -> why)))
;endif
   (let ((k int (fref tbp -> a1 t1 k))
	 (s int 0))
     (= s (fib pr bk k))
     (= (ptr saved-result-buf-p) s))
   (finish-write-before-write)
   (atomic-write-int (ptr saved-result-port-p 1))))

((fib pr bkf k) (fn int proc-env (ptr (fn int (ptr req-buf-t))) int)
 (let ((s int 0)                 #| the sum of solutions |#
       (i int 0)
       (restp int 1)
       (saved-req-port-p (ptr (ptr void))) #| (req-buf-t**) |#
       (result-buf int)
       ((bk req-buf-p) (fn int (ptr req-buf-))
	(if ret (= ret (bkf req-buf-p)))
	(if ret (return 1))
	(if restp
	    (let ((tbp (ptr task-buf-t) (fref req-buf-p -> task-buf-p))
		  (tpp int              (fref req-buf-p -> task-port-p)))
	      (= restp 0)
	      #| save info for "request back" |#
	      (= saved-req-port-p (fref req-buf-p -> req-port-p))
	      #| result not available yet |#
	      (= result-port 0)
	      #| put a task |#
	      (= (fref tbp -> f) tf-fib)
	      (= (fref tbp -> sender) (fref pr -> myid))
	      (= (fref tbp -> a t1 result-buf-p) (mref result-buf))
	      (= (fref tbp -> a t1 result-port-p) (mref result-port))
	      (= (fref tbp -> a t1 k) (- k 2))
	      #| transfer the task |#
	      (finish-write-before-write)
	      (atomic-write-int (ptr tpp) 1)
	      (return 1)))
	(return 0)))
   (if (<= k 2) 
       (return 1)
       (block
         (= restp 1)
	 (@POLL (fref pr -> req-port) bk)
	 (+= s (fib pr bk (- k 1)))
	 (if restp
	     (block
	       (= restp 0)
	       (+= s (fib pr bk (- k 2))))
	     (block
	       (while (== (atomic-read-int result-port) 0)
		 #| request back a task and run |#
		 (steal-run-task pr saved-req-port-p 1))
	       (start-read-after-read)
	       (+= s result-buf)))
	 (return s)))))

((main argc argv) (fn int int (array (ptr char)))
 (let (((bk req-buf-p) (fn int (ptr req-buf-t))
	 (return 0))
       (tv (array (struct timeval) 2))
       (defs int (num-proc 2) (k 20) ans))
   (if (> argc 1) (= num-proc (atoi (aref argv 1))))
   (if (> argc 2) (= k (atoi (aref argv 2))))
   (init-parallel num-proc)
   (gettimeofday (+ tv 0) 0)  (= tv0 (aref tv 0))
   (= ans (fib (+ pe 0) bk k))
   (gettimeofday (+ tv 1) 0)
   (printf "fib(%d)=%d\\n" k ans)
   (printf "%lf\\n"
	   (+ (- (fref (aref tv 1) tv-sec) (fref (aref tv 0) tv-sec))
	      (* 0.000001
		 (- (fref (aref tv 1) tv-usec) (fref (aref tv 0) tv-usec)))))
   )
 )
 