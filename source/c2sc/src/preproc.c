/* preproc.c:   to "pre-preprocess" header files.   */

#pragma __preprocess

#include    "system.H"
#include    "internal.H"

#ifdef  __STDC__
#pragma __put_defines
#else
#ifdef  __cplusplus
#pragma __put_defines
#else
#put_defines
#endif
#endif

