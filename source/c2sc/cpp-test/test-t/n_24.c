/* n_24.c:  # operator in macro definition / 1. */

#define str( a)     # a

/* 24.1:    */
/*  "a+b";  */
    str( a+b);

/* 24.2:    White spaces between tokens of operand are converted to one space.
 */
/*  "ab + cd";  */
    str(    ab  /* comment */   +
        cd  );

/* { dg-do preprocess }
   { dg-final { if ![file exist n_24.i] { return }                      } }
   { dg-final { if \{ [grep n_24.i "\"a\\+b\""] != ""           \} \{   } }
   { dg-final { if \{ [grep n_24.i "\"ab \\+ cd\""] != ""       \} \{   } }
   { dg-final { return \} \}                                            } }
   { dg-final { fail "n_24.c: #operator"                                } }
 */

