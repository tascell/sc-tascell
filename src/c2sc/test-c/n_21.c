/* n_21.c:  Tokenization (No preprocessing tokens are merged implicitly).   */

#include    "defs.h"

main( void)
{
    int     a = 1;

    fputs( "started\n", stderr);

/* 21.1:    */
#define MINUS   -
    assert( -MINUS-a == -1);

    fputs( "success\n", stderr);
    return  0;
}

