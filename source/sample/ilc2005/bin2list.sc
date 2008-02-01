;; 先頭の#include は手動ではずして
(%rule (:basic :type :tmp (:nestfunc) :untype))

(%include "copygc.sh")
(%include "copygc.sc")

;;; 構造体定義
(def (struct _Bintree)
  (def d desc_t)
  (def key int)
  (def val int)
  (def left (ptr (struct _Bintree)))
  (def right (ptr (struct _Bintree))) )
(deftype Bintree (struct _Bintree))

(def (struct _KVpair)
  (def d desc_t)
  (def key int)
  (def val int) )
(deftype KVpair (struct _KVpair))

(def (struct _Alist)
  (def d desc_t)
  (def kv (ptr KVpair))
  (def cdr (ptr (struct _Alist))) )
(deftype Alist (struct _Alist))


;; 構造体定義: ポインタの位置

(def Bintree_node (array size_t)
  (array (offsetof Bintree left)
   (offsetof Bintree right)))
(def Bintree_d descriptor (DESC Bintree Bintree_node))

(def Alist_node (array size_t)
  (array (offsetof Alist kv) (offsetof Alist cdr)))
(def Alist_d descriptor (DESC Alist Alist_node))

(def KVpair_node (array size_t) (array))
(def KVpair_d descriptor (DESC KVpair KVpair_node))

;;; 計測パラメータ
(static-def maxins int)
(static-def maxsearch int)

(def (newBintree scan0 k v) (fn (ptr Bintree) sht int int)
  (def p (ptr Bintree))
  (= p
     (cast (ptr Bintree)
           (getmem scan0 (ptr Bintree_d))))
  (= (fref (mref p) key) k)
  (= (fref (mref p) val) v) (return p))


;;; x がすでに存在する場合
(def (insert scan0 x k v) (fn void sht (ptr Bintree) int int)
  (def y (ptr Bintree) 0)
  (def (scan1) (lightweight void void)
    (= x (csym::move x))
    (= y (csym::move y))
    (scan0))

  (do-while 1
    (if (== (fref (mref x) key) k)
        (begin (= (fref (mref x) val) v)
               (return))
        (if (< k (fref (mref x) key))
            (begin (= y (fref (mref x) left))
                   (if (not y)
                       (begin
                        (= (fref (mref x) left)
                           (newBintree scan1 k v))
                        (return))))
            (begin (= y (fref (mref x) right))
                   (if (not y)
                       (begin
                        (= (fref (mref x) right)
                           (newBintree scan1 k v))
                        (return))))))
    (= x y))
  )

(def (search scan0 x k v0) (fn int sht (ptr Bintree) int int)
  (while x
   (if (== (fref (mref x) key) k)
    (begin (return (fref (mref x) val)))
    (if (< k (fref (mref x) key))
     (begin (= x (fref (mref x) left)))
     (begin (= x (fref (mref x) right))))))
  (return v0))


(def (bin2list scan0 x rest) (fn (ptr Alist) sht (ptr Bintree) (ptr Alist))
  (def a (ptr Alist) 0)
  (def kv (ptr KVpair) 0)
  (def (scan1) (lightweight void void)
    (= x (csym::move x))
    (= rest (csym::move rest))
    (= a (csym::move a))
    (= kv (csym::move kv))
    (scan0))

  (if (fref (mref x) right)
      (begin
       (= rest (bin2list scan1 (fref (mref x) right) rest))
       (= (fref (mref x) right) 0)) )
  
  (= kv (getmem scan1 (ptr KVpair_d)))
  (= (fref (mref kv) key) (fref (mref x) key))
  (= (fref (mref kv) val) (fref (mref x) val))

  (= a (getmem scan1 (ptr Alist_d)))
  (= (fref (mref a) kv) kv)
  (= (fref (mref a) cdr) rest)
  (= rest a)

  (if (fref (mref x) left)
      (begin
       (= rest (bin2list scan1 (fref (mref x) left) rest))
       (= (fref (mref x) left) 0)) )

  (return rest) )
  

(def (randinsert scan0 this n) (fn void sht (ptr Bintree) int)
  (def (scan1) (lightweight void void)
    (= this (csym::move this))
    (scan0))

  (decl i int) (decl k int)
  (decl seed (array unsigned-short 3))
  (= (aref seed 0) 3) (= (aref seed 1) 4)
  (= (aref seed 2) 5)
  (for ((= i 0) (< i n) (inc i))
   (= k (csym::nrand48 seed))
   (insert scan1 this k k)))


(def (randsearch scan0 this n) (fn void sht (ptr Bintree) int)
  (def (scan1) (lightweight void void)
    (= this (csym::move this))
    (scan0))
  (decl i int) (decl k int)
  (decl seed (array unsigned-short 3))
  (= (aref seed 0) 8) (= (aref seed 1) 9)
  (= (aref seed 2) 10)
  (for ((= i 0) (< i n) (inc i))
       (= k (csym::nrand48 seed))
       (search scan1 this k 0)))


;;; gcパラメータの設定
(def (csym::init tp tosize stack_size limited_max) (fn void int int int int)
  (decl p gc_params) 
  (= (fref p gcv) 1)
  (= (fref p gctype) tp)
  (= (fref p tosize) tosize)
  (= (fref p stack_size) stack_size)
  (= (fref p limited_stack_max) limited_max)
  (csym::getmem_init p) )


(def (main argc argv) (fn int int (ptr (ptr char)))

  (def tp1 (struct timeval))
  (def tp2 (struct timeval))
  (def i int)
  (def gctp int)
  (def searchskip int 0)
  (def root (ptr Bintree))
  (def ttime double)

  (def (scan1) (lightweight void void)
    (= root (csym::move root)))

  (if (and (> argc 1)
           (== (csym::strcmp (aref argv 1) "-s") 0))
    (begin
      (-- argc)
      (++ argv)
      (++ searchskip)))

  (= maxins    (if-exp (> argc 1) (csym::atoi (aref argv 1)) 100000))
  (= maxsearch (if-exp (> argc 2) (csym::atoi (aref argv 2)) 300000))

  (csym::printf "Bintree=%d, maxins=%d, maxsearch=%d~%"
          (sizeof Bintree) maxins maxsearch)

  (= gctp (if-exp (> argc 3) (csym::atoi (aref argv 3)) 0))

  (csym::init (if-exp (< gctp 0) 0 gctp)
              (if-exp (> argc 4) (csym::atoi (aref argv 4)) 0) 
              (if-exp (> argc 5) (csym::atoi (aref argv 5)) 0) 
              (if-exp (> argc 6) (csym::atoi (aref argv 6)) 0) ) 

  (= root (getmem scan1 (ptr Bintree_d)))
  (= (fref (mref root) key) 0)
  (= (fref (mref root) val) 0)

  (randinsert scan1 root maxins)

  (if (>= gctp 0)
    (gc scan1))
  
  (= gc-ttime 0)
  (csym::gettimeofday (ptr tp1) 0)  
  (if (not searchskip)
    (bin2list scan1 root 0))
  (csym::gettimeofday (ptr tp2) 0)  


  (= ttime
     (+
      (- (fref tp2 tv_sec) (fref tp1 tv_sec))
      (*
       (- (fref tp2 tv_usec) (fref tp1 tv_usec))
       0.000001)))

  (csym::printf "total=%f gc=%f other=%f~%"
                ttime gc-ttime (- ttime gc-ttime))

  (return 0)
)
