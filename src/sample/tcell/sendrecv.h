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

#ifndef INT32
#define INT32 int
#endif

/**/
void send_int(int n);
int recv_int(void);

/**/
void send_longlong(long long int n);
long long int recv_longlong(void);

/**/
int send_binary_header (int elmsize, /* size of an element */
                        int nelm )   /* number of elements */;
int recv_binary_header (int *pelmsize, /* size of an element */
                        int *pnelm)    /* number of elements */;
/**/
void send_binary_terminator (void);
void recv_binary_terminator (void);

/**/
void swap_int32s (INT32 *a, int n);

/**/
int send_int32s (INT32 *a, int nelm);
int recv_int32s (INT32 *a, int nelm);

/**/
void swap_doubles (double *a, int n);

/**/
int send_double_seq (double *a, int nelm);
int recv_double_seq (double *a, int nelm);

/* ヘッダの送受信も含む */
int send_doubles (double *a, int nelm);
int recv_doubles (double *a, int nelm);
