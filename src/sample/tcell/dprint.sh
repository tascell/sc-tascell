;;; Debug print用マクロ
;;(%defconstant VERBOSE 1)                ; コメントアウトするとデバッグ出力のためのコードを無効化
(%ifdef* VERBOSE
 (%defmacro DEBUG-STMTS (n &rest stmts) `(if (>= option.verbose ,n) (begin ,@stmts)))
 (%defmacro DEBUG-PRINT (n &rest args)
  `(if (>= option.verbose ,n) (csym::fprintf stderr ,@args)))
 %else
 (%defmacro DEBUG-STMTS (n &rest stmts) `(begin))
 (%defmacro DEBUG-PRINT (n &rest args) `(begin)))
