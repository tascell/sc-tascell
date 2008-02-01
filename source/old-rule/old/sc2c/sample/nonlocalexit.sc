;;;; 1^2 + 2^2 + ... + n^2 の計算
;;;; 結果がlimitを超えたら -1 を返す。
(def (square-sum n) (fn int int)
  (def @O-FLOW __label__)  ; この宣言はgccでも必須、2パス解析すればはずせる?
  (static limit int 8192)

  (def (square-sum-t n acc) (lightweight int int int)
    (def (square-t n acc) (lightweight int int int)
      (if (== n 0)
	  (return acc)
	  (if (> acc limit)	
	      (goto @O-FLOW)
	      (return (square-t (- n 1) (+ acc -1 (* 2 n)))))))

    (if (== n 0)
	(return acc)
	(if (> acc limit)
	    (goto @O-FLOW)
	    (return
	     (square-sum-t (- n 1) (+ acc (square-t n 0)))))))
  
  (return (square-sum-t n 0))
     
  (label @O-FLOW (return -1)))

(def (main) (fn int)
  (return (square-sum 100)))
