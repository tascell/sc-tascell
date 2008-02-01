(decl printf (fn int (const int) va-arg))

(%defmacro ntimes (n &body form)
    `(for ((def i int 0) (< i ,n) (inc i)) ,@form)) 

(main (fn int)
  (ntimes 10 
    (def str (array char 3 10) 
               (array "abcdefg~%" "hijklmn~%" "opqrstu~%"))
    (printf "~%")
    (ntimes 3
      (printf (aref str i)))))


(%defmacro defswap (fname texp)
   `(def (,fname a b) (fn void (ptr ,texp) (ptr ,texp))
       (def buffer ,texp)
       (= buffer (ptr a))
       (= (ptr a) (ptr b))
       (= (ptr b) buffer)
       (return)))


(defswap swap int)
(defswap swap-w (ptr char))

(%defmacro ggtype () '(ptr (fn (ptr void) int int)))

(decl (gg ff) (fn (ggtype) (ggtype)))
(deftype gg-t (fn (ggtype) (ggtype)))

(%ifdef ggtype
	((%error "ggtype defined"))
	((%error "ggtypee undefined")))