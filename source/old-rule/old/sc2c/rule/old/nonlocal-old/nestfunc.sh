; 追加する型、マクロ定義
(deftype nestfn-t (ptr (fn (ptr char) (ptr char) (ptr void))))

(deftype closure-t struct
  (def fun nestfn-t)
  (def fr (ptr void)))

(defmacro special (tp)
  `(- (cast ,tp 0) 1))

(deftype align-t double)

(defmacro aligned-add (base size)
  `(cast (ptr char)
	 (+ (cast (ptr align-t) ,base)
	    (/ (+ ,size (sizeof align-t) -1) (sizeof align-t)))))

(defmacro aligned-sub (base size)
  `(cast (ptr char)
	 (- (cast (ptr align-t) ,base)
	    (/ (+ ,size (sizeof align-t) -1) (sizeof align-t)))))

(defmacro mref-t (tp p)
  `(mref (cast (ptr ,tp) ,p)))

(defmacro push-arg (tp v argp)
  `(exps (= (mref-t ,tp ,argp) ,v)
	 (= ,argp (aligned-add ,argp (sizeof ,tp)))))

(defmacro pop-arg (tp argp)
  `(exps (= ,argp (aligned-sub ,argp (sizeof ,tp)))
	 (mref-t ,tp ,argp)))

#|
(def (lw-call esp) (fn void (ptr char))
     (def tmp-esp (ptr char))
     (def clos (ptr closure-t) (mref-t (ptr closure-t) esp))
     (def new-esp (ptr char) esp)
     (while (= tmp-esp ((fref clos -> fun) new-esp (fref clos -> fr)))
       (lw-call tmp-esp)
       (= new-esp (+ esp 1))))
|#

; nonlocalexit-goto に対応させてみる
(def (lw-call esp) (fn int (ptr char))
  (def tmp-esp (ptr char))
  (def clos (ptr closure-t) (mref-t (ptr closure-t) esp))
  (def new-esp (ptr char) esp)
  (while (= tmp-esp ((fref clos -> fun) new-esp (fref clos -> fr)))
    (if (bit-and (cast size-t tmp-esp) 2)
	(return 2))
    ;; 入れ子関数の実行が nonlocal-goto で終了した場合
    ;; new-esp のフラグ(下位2ビット目)をonにする
    ;; => 呼び出し元入れ子関数の再実行のとき
    ;;    ・自分が脱出先ならそこから実行再開
    ;;    ・そうでなければ即座にreturn
    (= new-esp (+ esp 1 (lw-call tmp-esp))))
  (return 0))
