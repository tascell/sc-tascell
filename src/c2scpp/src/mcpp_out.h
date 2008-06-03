/* mcpp_out.h: declarations of OUTDEST data types for MCPP  */
#ifndef _MCPP_OUT_H
#define _MCPP_OUT_H

/* Choices for output destination */
typedef enum {
    OUT,                        /* ~= fp_out    */
    ERR,                        /* ~= fp_err    */
    DBG,                        /* ~= fp_debug  */
    SE2,                        // C2SC: fp_se2
    SE3,                        // C2SC: fp_se3
    NUM_OUTDEST
} OUTDEST;

#endif  /* _MCPP_OUT_H  */
