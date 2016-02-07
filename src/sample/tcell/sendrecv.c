/*
Copyright (c) 2008 Tasuku Hiraishi <hiraisi@kuis.kyoto-u.ac.jp>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "sock.h"
#include "sendrecv.h"

// #define DEBUG

extern int sv_socket;     /* defined in worker.sc */

/**/
void send_int(int n)
{
    char buf[16];
    snprintf (buf, 16, "%d\n", n);
    send_string (buf, sv_socket);
}
int recv_int(void)
{
    char buf[16];
    receive_line (buf, 16, sv_socket);
    return atoi(buf);
}

/**/
void send_long(long n)
{
    char buf[16];
    snprintf (buf, 16, "%ld\n", n);
    send_string (buf, sv_socket);
}
long recv_long(void)
{
    char buf[16];
    receive_line (buf, 16, sv_socket);
    return atol(buf);
}

/**/
void send_longlong(long long int n)
{
  char buf[24];
  snprintf(buf, 24, "%lld\n", n);
  send_string(buf, sv_socket);
}
long long int recv_longlong(void)
{
  char buf[24];
  receive_line(buf, 24, sv_socket);
  return atoll(buf);
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
    if (strcmp ("(\n", buf))
        { fprintf (stderr, "recv_binary_header: format error\n"); exit (1); }
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
        {
            fprintf (stderr, "recv_binary_terminator: format error\n");
            exit (1);
        }
}

/**/
void swap_int32s (INT32 *a, int n)
{
  /* !!! 未実装 !!! */
}

/**/
int send_int32s (INT32 *a, int nelm)
{
  int ret;
  send_binary_header (sizeof(INT32), nelm);
  ret = send_binary (a, sizeof(INT32), nelm, sv_socket);
  send_binary_terminator ();
  return ret;
}
    
int recv_int32s (INT32 *a, int nelm)
{
  int ret, swp;
  swp = recv_binary_header (0, 0);
  ret = receive_binary (a, sizeof(INT32), nelm, sv_socket);
  if (swp) swap_int32s (a, nelm);
  recv_binary_terminator ();
  return ret;
}

/* reverse byte order of double values */
/* assuming that long is 32-bit, and double is 64-bit */
void swap_doubles (double *a, int n)
{
    int i;
    /*     fprintf(stderr, "swapping...\n"); */
    for( i = 0 ; i < n ; i++ )
        {
            unsigned long *xp = (unsigned long *)(a+i);
            unsigned long x0 = xp[0];  /* 前32bit */
            unsigned long x1 = xp[1];  /* 後32bit */
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

/**/
int send_double_seq (double *a, int nelm)
{
    int ret;
    ret = send_binary (a, sizeof(double), nelm, sv_socket);
#ifdef DEBUG
    {
      int i;
      fprintf (stderr, "send(%d): ",ret);
      for (i=0; i<nelm; i++)
        fprintf (stderr, "%lf ", a[i]);
      fputc ('\n',stderr);
    }
#endif
    return ret;
}
int recv_double_seq (double *a, int nelm)
{
    int ret;
    ret = receive_binary (a, sizeof(double), nelm, sv_socket);
#ifdef DEBUG
    {
      int i;
      fprintf (stderr, "recv(%d): ",ret);
      for (i=0; i<nelm; i++)
        fprintf (stderr, "%lf ", a[i]);
      fputc ('\n',stderr);
    }
#endif
    return ret;
}

/**/
int send_doubles (double *a, int nelm)
{
  int ret;
  send_binary_header (sizeof(double), nelm);
  ret = send_binary (a, sizeof(double), nelm, sv_socket);
  send_binary_terminator ();
  return ret;
}

int recv_doubles (double *a, int nelm)
{
  int ret, swp;
  swp = recv_binary_header (0, 0);
  ret = receive_binary (a, sizeof(double), nelm, sv_socket);
  if (swp) swap_doubles (a, nelm);
  recv_binary_terminator ();
  return ret;
}

/**/
int send_double (double d)
{
  int ret;
  send_binary_header (sizeof(double), 1);
  ret = send_binary (&d, sizeof(double), 1, sv_socket);
  send_binary_terminator ();
  return ret;
}

int recv_double (void)
{
  double d[1];
  int swp = recv_binary_header (0, 0);
  receive_binary (d, sizeof(double), 1, sv_socket);
  if (swp) swap_doubles (d, 1);
  recv_binary_terminator ();
  return d[0];
}
