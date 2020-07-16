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

int connect_to (char *hostname, unsigned short port);
void close_socket (int socket);

int send_char (char, int socket);
int send_string (char *str, int socket);
int send_fmt_string (int socket, char *fmt_string, ...);
int send_binary (void *src, unsigned long elm_size, unsigned long n_elm,
                 int socket);
void show_mpisend_buf (int socket);

int receive_char (int socket);
char* receive_line (char *buf, int maxlen, int socket);
int receive_binary (void *dst, unsigned long elm_size, unsigned long n_elm,
                    int socket);

void set_rank_and_gid (int rank, int gid);
void send_block_start(void);
void send_block_end(int rank);
void sendrecv(void);

extern char *receive_buf;
extern char *receive_buf_p;
extern __thread struct send_block *sq;
extern __thread int RANK;
