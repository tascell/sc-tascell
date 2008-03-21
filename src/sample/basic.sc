(%defconstant NULL 0)

(decl malloc (fn (ptr void) int))

((struct list)
  (data int)
  (next (ptr (struct list))))

(main (fn int)
  (let ((fst (ptr (struct list)) NULL)
        (cur (ptr (struct list))))
    (for ((defs int (i 0) (j 0))
            (<= i 10) (exps (inc i) (+= j 2)))
       (= cur (malloc (sizeof (struct list))))
       (= (fref cur -> data) (* i j))
       (= (fref cur -> next) fst)
       (= fst cur))))
