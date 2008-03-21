/* n_21.c:  Tokenization (No preprocessing tokens are merged implicitly).   */

/* 21.1:    */
/*  - - -a; */
#define MINUS   -
    -MINUS-a;

/* { dg-do preprocess }
   { dg-final { if ![file exist n_21.i] { return }                      } }
   { dg-final { if \{ [grep n_21.i "- +- +- *a"] != ""          \} \{   } }
   { dg-final { return \}                                               } }
   { dg-final { fail "n_21.c: tokenization of expanded macro"           } }
 */

