;;; goto による入れ子関数脱出例
;;; (複雑なケース)
;;; g -> f -> g1 -> g -> g1 ==goto==> g

(def (f pg) (fn int (ptr (lightweight void int)))
  (pg 0)
  (return 0))

(def (g pg) (fn int (ptr (lightweight void int)))
  (def @L1 __label__)
  (def (g1 m) (lightweight void int)
    (if (> m 0)
	(goto @L1)
	(g g1)))
  
  (if (== pg 0)
      (return (f g1))
      (begin
       (g1 1)  ; goto先は後で呼び出されたg
       (return 0)))
  
  (label @L1
	 (if (== pg 0)
	     (return 0)
	     (pg 1)  ; goto先は最初に呼び出されたg
	     )))
  
(def (main) (fn int)
  (return (g 0)))

