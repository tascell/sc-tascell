;;;;;;;;; pentomino ;;;;;;;;;;;;

(private a (array int 12))
(private b (array int 70))

(decl search (fn int int))

(def (try_piece k i) (fn int int int)
  (def s int 0)
  (def d int)
  (def n int (fref (aref ps i) n))
  (for ((= d 0) (< d n) (inc d))
     (def kk int k)
     (def l int)
     (def pss (ptr int) (aref (fref (aref ps i) pss) d))
     ;; room available?
     (for ((= l 0) (< l 4) (inc l))
        (if (or (>= (+= kk (aref pss l)) 70) (!= (aref b kk) 0)) (goto Ln)))
     ;; put the piece
     (= (aref b (= kk k)) (+ i #\a))
     (for ((= l 0) (< l 4) (inc l)) 
        (= (aref b (+= kk (aref pss l))) (+ i #\a)))
     (= (aref a i) 1)
     (handle
        (undo-redo
           (def kk int k)
           (def l int)
           (def pss (ptr int) (aref (fref (aref ps i) pss) d))
           ;; remove the piece (backtrack)
           (= (aref a i) 0)
           (= (aref b (= kk k)) 0)
           (for ((= l 0) (< l 4) (inc l)) 
              (= (aref b (+= kk (aref pss l))) 0))
           (try-to-spawn)
           ;; put the piece (cancel backtrack)
           (= (aref b (= kk k)) (+ i #\a))
           (for ((= l 0) (< l 4) (inc l)) 
              (= (aref b (+= kk (aref pss l))) (+ i #\a)))
           (= (aref a i) 1))
        ;; find the first empty location
        (for ((= kk k) (< kk 70) (inc kk)) (if (== (aref b kk) 0) (break)))
        ;; recursive search
        (+= s (search kk)))
     ;; remove the piece (backtrack)
     (= (aref a i) 0)
     (= (aref b (= kk k)) 0)
     (for ((= l 0) (< l 4) (inc l)) 
        (= (aref b (+= kk (aref pss l))) 0))
     (label Ln continue))
  (return s))

(def (struct arrange_data)
  (def a (array int 12))
  (def b (array int 70))

(def (task_backup_and_try_piece ad k i)
        (task int (struct arrange_data) int int)
   ....
   (reply_result (try_piece k i))
   ....)

(def (search i) (fn int int)
  (def s int 0)
  (def i int 0)
  (def si int 12)
  (def r (array (result int) 12))
  (if (== k 70) (return 1))
  (if (..prunable..) (return 0))
  (while (< i si)
     (def ii int (inc i))
     (if (== (aref a ii) 0)
         (handle
           (split
              ... 
              (spawn (aref r ..) (task_backup_and_try_piece ...))
              ...
              )
           (+= s (try_piece k ii)))))
  (while (< i 12)
     (def ii int (inc i))
     (wait_result (aref r ii))
     (+= s (result_value (aref r ii))))
  (return s))

