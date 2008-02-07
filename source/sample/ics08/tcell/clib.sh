(decl (csym::printf) (csym::fn int (ptr (const char)) va-arg))
(decl (csym::fprintf) (csym::fn int (ptr FILE) (ptr (const char)) va-arg))

(decl (csym::malloc) (csym::fn (ptr void) size-t))
(decl (csym::free) (csym::fn void (ptr void)))

(decl (csym::sqrt) (csym::fn double double))
(decl (csym::fabs) (csym::fn double double))
