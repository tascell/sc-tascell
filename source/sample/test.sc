(decl (printf) (fn int (const char) va-arg)) 

(def (main) (fn int)
 (let ((a char #\Newline) (b char "abcdefg"))
   (printf "%d%s~%" a b)
   (return (+ a b))))
