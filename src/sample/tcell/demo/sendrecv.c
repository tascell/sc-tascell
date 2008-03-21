#include <stdio.h>
#include <string.h>
#include "sock.h"
#include "sendrecv.h"

extern int sv_socket;     /* defined in worker[>=3].sc */

/**/
void send_int(int n)
{
    char buf[16];
    snprintf (buf, 16, "%ld\n", n);
    send_string (buf, sv_socket);
}
int recv_int(void)
{
    char buf[16];
    receive_line (buf, 16, sv_socket);
    return atoi(buf);
}

/**/
union int_4 {
    int i;
    char c[4];
} endian = {1};

int send_binary_header (int elmsize, /* size of an element */
                        int nelm )   /* number of elements */
{
    char buf[64];
    snprintf (buf, 64, "(\n%d %d %d\n",
              elmsize*nelm, elmsize, (int)endian.c[0]);
    send_string (buf, sv_socket);
    return endian.c[0];              /* endian */
}
int recv_binary_header (int *pelmsize, /* size of an element */
                        int *pnelm)    /* number of elements */
{
    char buf[64]; char *p=buf;
    int size, elmsize, e;
    receive_line (buf, 64, sv_socket);
    if (strcmp (")\n", buf))
        { perror ("recv_binary_header"); exit (1); }
    receive_line (buf, 64, sv_socket);
    size = strtol (p, &p, 10);
    elmsize = strtol (p, &p, 10);
    e = strtol (p, &p, 10);
    if (pelmsize) *pelmsize = elmsize;
    if (pnelm)    *pnelm    = size/elmsize;
    return e != endian.c[0];           /* swap required? */
}
/**/
void send_binary_terminator (void){ send_string (")\n", sv_socket); }
void recv_binary_terminator (void)
{
    char buf[16];
    receive_line (buf, 16, sv_socket);
    if (strcmp (")\n", buf))
        { perror ("recv_binary_terminator"); exit (1); }
}

/**/
int send_double_seq (double *a, int nelm)
{
    return send_binary (a, sizeof(double), nelm, sv_socket);
}
int recv_double_seq (double *a, int nelm)
{
    return receive_binary (a, sizeof(double), nelm, sv_socket);
}

/* reverse byte order of double values */
/* assuming that long is 32-bit, and double is 64-bit */
void swap_doubles (double *a, int n)
{
    int i;
    /* fprintf(stderr, "swapping...\n")	 */
    for( i = 0 ; i < n ; i++ )
        {
            unsigned long *xp = (unsigned long *)(a+i);
            unsigned long x0 = xp[0];  /* ‘O32bit */
            unsigned long x1 = xp[1];  /* Œã32bit */
            unsigned long y0 = x0;
            unsigned long y1 = x1;
                                       /* y0 y1 y2 y3 */
            y0 = y0<<8|(x0 >>= 8)&255; /* y1 y2 y3 x2 (x= 0 x0 x1 x2) */
            y0 = y0<<8|(x0 >>= 8)&255; /* y2 y3 x2 x1 (x= 0  0 x0 x1) */
            y0 = y0<<8|x0>>8;          /* y3 x2 x1 x0 */
            y1 = y1<<8|(x1 >>= 8)&255;
            y1 = y1<<8|(x1 >>= 8)&255;
            y1 = y1<<8|x1>>8;
            xp[1] = y0;
            xp[0] = y1;
        }
    /* fprintf(stderr, "swapping...done\n"); */
}

