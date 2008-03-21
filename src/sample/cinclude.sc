(%cinclude "stdio.h")

(def (main argc argv) (fn int int (ptr (ptr char)))
  (c::fprintf c::stdout "%s %s~%" c::__@time__ c::__@date__)
  (return c::@null))
