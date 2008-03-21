(def (frac n) (fn int int)
     (def @L0 __label__)
     (def acc int 1)
     (def (frac-t n) (lightweight void int)
	  (if (== n 0)
	      (goto @L0)
	    (begin
	     (*= acc n)
	     (frac-t (- n 1)))))
     
     (frac-t n)
     (return -1)
     
     (label @L0 (return acc)))

(def (main) (fn int)
     (return (frac 10)))
