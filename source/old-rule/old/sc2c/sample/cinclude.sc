(%cinclude "stdio.h")

(def (main argc argv) (fn int int (ptr (ptr char)))
  (c::printf "argc = %d~%" argc)
  (return 0))
