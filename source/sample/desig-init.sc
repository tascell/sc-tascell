(%rule ())

(decl (csym::printf) (csym::fn int (ptr (const char)) va-arg))

(deftype div-t struct
  (def quot int)
  (def rem int))

(def (union uni)
  (def any-member int)
  (def flt-member float))

(%defconstant MAX 10)

(def (struct sab)
  (def a (array int 3))
  (def b int))

(def (main) (fn int void)
  (def s1 (array char) "abc")
  (def t1 (array char 3) "abc")
  (def p (ptr char) "abc")
  (def nm (array (ptr (const char)) 2)
    (array ((aref-this 0) "member zero")
	   ((aref-this 1) "member one")))
  (def answer div-t
    (struct ((fref-this quot) 2) ((fref-this rem) -1)))
  (def w (array (struct sab) 3)
    (array ((fref (aref-this 0) a) (array 1))
	   ((aref (fref (aref-this 1) a) 0) 2)))
  (def a (array int MAX)
    (array 1 3 5 7 9 ((aref-this (- MAX 5)) 8) 6 4 2 0))
  (def u (union uni) (struct ((fref-this any-member) 42)))
  
  (return 0))
