;; (%cinclude "<stdio.h>" "<sys/time.h>" "<string.h>")
(%include "rule/hsc-setrule.sh")
(c-exp "#include<stdio.h>")
(c-exp "#include<stdlib.h>")
(c-exp "#include<string.h>")
;; (c-exp "#include<sys/time.h>")
       
;;; 構造体定義
(def (struct _Bintree)
  (def key int)
  (def val int)
  (def left (ptr (struct _Bintree)))
  (def right (ptr (struct _Bintree))) )

(deftype Bintree (struct _Bintree))

;;; 計測パラメータ
(static-def maxins int)
(static-def maxsearch int)

(def (newBintree k v) (fn (ptr Bintree) int int)
  (def p (ptr Bintree) 0)
  ;; (def ar (array int 20))
  (= p (new (init Bintree (struct k v 0 0)))) ;ポインタはちゃんと初期化しないとエラーにする。
  ;; (= ar (new (init (array int 20) (array 0))))
  ;; ((aref-this 15) 3)))))
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

;;; insertと同様。
(def (search x k v0) (fn int (ptr Bintree) int int)
  (while x
   (if (== (fref (mref x) key) k)
    (begin (return (fref (mref x) val)))
    (if (< k (fref (mref x) key))
     (begin (= x (fref (mref x) left)))
     (begin (= x (fref (mref x) right))))))
  (return v0))

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
  (= (aref seed 2) 100)
  (for ((= i 0) (< i n) (inc i))
       (= k (csym::rand-r (aref seed)))
       (search this k 0)))


(def (hsc-main argc argv) (fn int int (array (array char)))
  ;; (def tp1 (struct csym::timeval))
  ;; (def tp2 (struct csym::timeval)) 
  ;; (def ttime double)
  (def searchskip int 0)
  (def index int 1)
  (def root (ptr Bintree))

  (if (and (> argc 1)
           (== (csym::strcmp (aref (aref argv index)) "-s") 0))
      (begin
       (-- argc)
       (++ index)
       (= searchskip 1)))

  (= maxins    (if-exp (> argc 1) (atoi (aref (aref argv index))) 100000))
  (= maxsearch (if-exp (> argc 2) (atoi (aref (aref argv (+ 1 index)))) 300000))

  (printf "Bintree=%d, masins=%d, maxsearch=%d~%"
          (sizeof Bintree) maxins maxsearch)

  (= root (new (init Bintree (struct 0 0 0 0))))

  ;; (csym::gettimeofday (ptr tp1) 0)
  (randinsert root maxins)
  ;; (csym::gettimeofday (ptr tp2) 0)

  (if (not searchskip)
      (randsearch root maxsearch))

  ;;   (= ttime
  ;;      (+
  ;;       (- (fref tp2 tv_sec) (fref tp1 tv_sec))
  ;;       (*
  ;;        (- (fref tp2 tv_usec) (fref tp1 tv_usec))
  ;;        0.000001)))
  
  ;;   (csym::printf "total=%f gc=%f other=%f~%" ttime gc_ttime (- ttime gc-ttime))
  (return 0)
  )
