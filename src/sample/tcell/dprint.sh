;;; Debug print
(%ifndef* VERBOSE
  (%defconstant VERBOSE 1))             ; non-nil: Debug print ON
(%if* VERBOSE
 (%defmacro DEBUG-STMTS (n &rest stmts) `(if (>= option.verbose ,n) (begin ,@stmts)))
 (%defmacro DEBUG-PRINT (n &rest args)
  `(if (>= option.verbose ,n) (csym::fprintf stderr ,@args)))
 %else
 (%defmacro DEBUG-STMTS (n &rest stmts) `(begin))
 (%defmacro DEBUG-PRINT (n &rest args) `(begin)))

;;; Profiler
(%ifndef* PROFILE
  (%defconstant PROFILE 0))             ; non-nil: Profiler ON
(%if* PROFILE
 (%defmacro PROF-CODE (&rest stmts) `(%splice ,@stmts))
 %else
 (%defmacro PROF-CODE (n &rest stmts) `(%splice))
 )
