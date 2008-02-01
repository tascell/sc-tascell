(c-exp "#include<stdlib.h>")
(c-exp "#include<pthread.h>")

;;; 追加する型、マクロ定義
(deftype nestfn-t (ptr (fn (ptr char) (ptr char) (ptr void))))

(deftype closure-t struct
  (def fun nestfn-t)
  (def fr (ptr void)))
(deftype Align-t double)

(%defmacro mref-t (tp p)
  `(mref (cast (ptr ,tp) ,p)))

;;; nonlocalexit-goto に対応
(def (lw-call esp) (fn (ptr char) (ptr char))
  (def clos (ptr closure-t) (mref-t (ptr closure-t) esp))
  ;; 入れ子関数を呼び出す。
  ;; goto で脱出してきた場合は、脱出先のefp
  ;; (stackのreconstructionをefpのところのフレームで止める)
  ;; そうでない場合は、0 が返り値となる。 
  (return ((fref clos -> fun) esp (fref clos -> fr))))

;; for handling pthread
(def (struct func-arg)
  (def func (ptr (fn (ptr void) (ptr char) (ptr void))))
  (def arg (ptr void)))

(def (struct tho-frame)
    (def tmp-esp (ptr char)))

(def (thread-origin farg) (fn (ptr void) (ptr void))
  (def efp (ptr (struct tho-frame)))
  (def x (ptr (struct func-arg)) farg)
  (def func (ptr (fn (ptr void) (ptr char) (ptr void))) (fref x -> func))
  (def arg (ptr void) (fref x -> arg))
  (def ret (ptr void))
  (def new-esp (ptr char))
  (def estack (array char 8192))
  (def esp (ptr char) estack)
  
  (pthread-cleanup-push free farg)
  (= efp (cast (ptr (struct tho-frame)) esp))
  (= esp (cast (ptr char)
	       (+ (cast (ptr Align-t) esp)
              (/ (- (+ (sizeof (struct tho-frame)) (sizeof Align-t)) 1)
                 (sizeof Align-t)))))
  (= (mref-t (ptr char) esp) 0)
  (= new-esp esp)
  (while (and (== (= ret (func new-esp arg)) (- (cast (ptr void) 0) 1))
              (!= (= (fref (mref efp) tmp-esp)
                     (mref-t (ptr char) esp))
                  0))
    (lw-call (fref (mref efp) tmp-esp)) (= new-esp (+ esp 1)))
  (pthread-cleanup-pop 1)
  (return ret)
  )
