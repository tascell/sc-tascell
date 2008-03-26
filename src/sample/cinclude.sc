(%cinclude "<stdio.h>" (:macro NULL stdout __TIME__ __DATE__))

(def (main argc argv) (fn int int (ptr (ptr char)))
  (csym::fprintf csym::stdout "%s %s~%" csym::__TIME__ csym::__DATE__)
  (return csym::NULL))
