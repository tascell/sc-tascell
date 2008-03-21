/* n_24.t:  # operator in macro definition. */

#define str( a)     # a

/* 24.1:    */
/*  "a+b";  */
    str( a+b);

/* 24.2:    White spaces between tokens of operand are converted to one space.
 */
/*  "ab + cd";  */
    str(    ab  /* comment */   +
        cd  );

/* 24.3:    \ is inserted before \ and " in or surrounding literals and no
        other character is inserted to anywhere.    */
/*  "'\"' + \"' \\\"\"";    */
    str( '"' + "' \"");

/* 24.4:    Line splicing by <backslash><newline> is done prior to token
        parsing.   */
/*  "\"abc\"";  */
    str( "ab\
c");

