;; 先頭の#include は手動ではずして
;(%rule (:basic :type :tmp (:nestfunc) :untype))

(c-exp "#include<sys/time.h>")
(c-exp "#include<stdio.h>")
(c-exp "#include<stdlib.h>")
(c-exp "#include<string.h>")
(%include "rule/hsc-setrule.sh")

;;; 構造体定義
(def (struct _Bintree)
  (def key int)
  (def val int)
  (def left (ptr (struct _Bintree)))
  (def right (ptr (struct _Bintree))) )
(deftype Bintree (struct _Bintree))

(def (struct _KVpair)
  (def key int)
  (def val int) )
(deftype KVpair (struct _KVpair))

(def (struct _Alist)
  (def kv (ptr (struct _KVpair)))
  (def cdr (ptr (struct _Alist))) )
(deftype Alist (struct _Alist))

;;; 計測パラメータ
(static-def maxins int)
(static-def maxsearch int)

(def (newBintree k v) (fn (ptr Bintree) int int)
  (def p (ptr Bintree))
  (= p (new (init Bintree (struct k v 0 0))))
  (return p))

;;; x がすでに存在する場合
(def (insert x k v) (fn void (ptr Bintree) int int)
  (def y (ptr Bintree) 0)

  (do-while 1
    (if (== (fref (mref x) key) k)
        (begin (= (fref (mref x) val) v)
               (return))
        (if (< k (fref (mref x) key))
            (begin (= y (fref (mref x) left))
                   (if (not y)
                       (begin
                        (= (fref (mref x) left)
			   (newBintree k v))
                        (return))))
            (begin (= y (fref (mref x) right))
                   (if (not y)
                       (begin
                        (= (fref (mref x) right)
			   (newBintree k v))
                        (return))))))
    (= x y))
  )

(def (search x k v0) (fn int (ptr Bintree) int int)
  (while x
   (if (== (fref (mref x) key) k)
    (begin (return (fref (mref x) val)))
    (if (< k (fref (mref x) key))
     (begin (= x (fref (mref x) left)))
     (begin (= x (fref (mref x) right))))))
  (return v0))


(def (bin2list x rest) (fn (ptr Alist) (ptr Bintree) (ptr Alist))
  (def a (ptr Alist) 0)
  (def kv (ptr KVpair) 0)

  (if (fref (mref x) right)
      (begin
       (= rest (bin2list (fref (mref x) right) rest))
       (= (fref (mref x) right) 0) ; this subtree becomes a garbage
       ))
   
  (= kv (new (init KVpair (struct (fref (mref x) key) (fref (mref x) val)))))
  (= a (new (init Alist (struct kv rest))))


  (= rest a)

  (if (fref (mref x) left)
      (begin
       (= rest (bin2list (fref (mref x) left) rest))
       (= (fref (mref x) left) 0)) )

  (return rest) )
  

(def (randinsert this n) (fn void (ptr Bintree) int)
  (decl i int) (decl k int)
  (decl seed (array unsigned-int))
  (= seed (new-array 3 unsigned-int 3 4 5))
  (for ((= i 0) (< i n) (inc i))
   (= k (csym::rand-r (aref seed)))
   (insert this k k)))


(def (randsearch this n) (fn void (ptr Bintree) int)
  (decl i int) (decl k int)
  (decl seed (array unsigned-int))
  (= seed (new-array 3 unsigned-int 8 9 10))
  (for ((= i 0) (< i n) (inc i))
       (= k (csym::rand-r (aref seed)))
       (search this k 0)))



(def (hsc-main argc argv) (fn int int (array (array char)))
  (def tp1 (struct timeval))
  (def tp2 (struct timeval))
  (def i int)
  (def index int 1)
  (def searchskip int 0)
  (def root (ptr Bintree))
  (def ttime double)

  (if (and (> argc index)
           (== (csym::strcmp (aref (aref argv 1)) "-s") 0))
    (begin
      (-- argc)
      (++ index)
      (= searchskip 1)))

  (= maxins    (if-exp (> argc 1) (csym::atoi (aref (aref argv index))) 100000))
  (= maxsearch (if-exp (> argc 2) (csym::atoi (aref (aref argv (+ 1 index)))) 300000))

  (csym::printf "Bintree=%d, maxins=%d, maxsearch=%d~%"
          (sizeof Bintree) maxins maxsearch)

  (= root (new (init Bintree (struct 0 0 0 0))))

  (randinsert root maxins)

;  (if (>= gctp 0)
;    (gc scan1))
  
  (csym::gettimeofday (ptr tp1) 0)  
  (if (not searchskip)
    (bin2list root 0))
  (csym::gettimeofday (ptr tp2) 0)  


  (= ttime
     (+
      (- (fref tp2 tv_sec) (fref tp1 tv_sec))
      (*
       (- (fref tp2 tv_usec) (fref tp1 tv_usec))
       0.000001)))

  (csym::printf "total=%f~%"		; gc=%f other=%f~%"
                ttime)			; (- ttime gc-ttime))

  (return 0)
)
