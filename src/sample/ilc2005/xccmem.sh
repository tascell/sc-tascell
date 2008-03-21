(decl (csym::atomic_read_ptr) (fn (ptr void)))
(decl (csym::atomic_read_int) (fn int))

(decl (csym::atomic_write_ptr) (fn void))
(decl (csym::atomic_write_int) (fn void))

(decl (csym::start_read_after_read) (fn void))
(decl (csym::finish_write_before_write) (fn void))

(decl (csym::cas_ptr) (fn int))
(decl (csym::cas_int) (fn int))
