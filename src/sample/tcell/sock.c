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
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "sock.h"

#define SEND_MAX 65536
#define RECV_MAX 1024

/* SunOS does not support vasprintf() */
#ifdef NO_VASPRINTF
#define VSNPRINTF_LEN 1000
#endif

/* ここに文字列があれば優先的にこちらから読み込む */
char *receive_buf = 0;
char *receive_buf_p = 0;

/* #define DEBUG */

#ifdef DEBUG
int dbg_printf (char *fmt_string, ...)
{
    va_list args;
    int len;
#ifdef NO_VASPRINTF
    char str[VSNPRINTF_LEN];
#else
    char *str;
#endif
    char *str2;
    int i, i2;

    va_start (args, fmt_string);
#ifdef NO_VASPRINTF
    len = vsnprintf (str, VSNPRINTF_LEN, fmt_string, args);
#else
    len = vasprintf (&str, fmt_string, args);
#endif
    va_end (args);
#ifndef NO_VASPRINTF
    if (str == NULL) { perror ("allocation"); exit (1); }
#endif

    str2= (char*) malloc (sizeof(char)*(strlen(str)*2+3));
    fputc ('"',stderr);
    for (i=0,i2=0 ; str[i] ; i++,i2++)
        {
            if (str[i]=='\n')
                { str2[i2]='\\'; str2[++i2]='n'; }
            else
                str2[i2] = str[i];
        }
    strncpy (str2+i2, "\"\n", 2); str2[i2+2]='\0';
    fputs (str2, stderr);
#ifndef NO_VASPRINTF
    free(str);
#endif
    free(str2);
    return len;
}

#endif

/* 送信 */
int send_char (char ch, int socket)
{
    if (socket<0)
        return putc (ch, stdout);
    else
        {
#ifdef DEBUG
            dbg_printf ("send_char: '%s' (%d)", (char[]){ch,0}, ch);
#endif
        return send (socket, &ch, 1, 0);
        }
}

int send_string (char *str, int socket)
{
    if (socket<0)
        return fputs (str, stdout);
    else
        {
#ifdef DEBUG
            dbg_printf ("send_string: %s", str);
#endif
            return send (socket, str, strlen(str), 0);
        }
}
int send_fmt_string (int socket, char *fmt_string, ...)
{
    va_list args;
#ifdef NO_VASPRINTF
    char str[VSNPRINTF_LEN];
#else
    char *str;
#endif
    int len;
    int ret;

    if (socket<0)
        {
            va_start (args, fmt_string);
            len = vprintf (fmt_string, args);
            va_end (args);
            return len;
        }
    else
        {
            va_start (args, fmt_string);
#ifdef NO_VASPRINTF
            len = vsnprintf (str, VSNPRINTF_LEN, fmt_string, args);
#else
            len = vasprintf (&str, fmt_string, args);
#endif
            va_end (args);

#ifndef NO_VASPRINTF
            if (str == NULL) { perror ("allocation"); exit (1); }
#endif
            ret = send (socket, str, len, 0);
#ifdef DEBUG
            dbg_printf ("send_fmt_string: %s", str);
#endif
#ifndef NO_VASPRINTF
            free (str);
#endif
            return ret;
        }
}

int send_binary (void *src, unsigned long elm_size, unsigned long n_elm,
                 int socket)
{
    if (socket<0)
        {
            return fwrite (src, elm_size, n_elm, stdout);
        }
    else
        {
            unsigned long rest=elm_size*n_elm;
            unsigned long ret;
            while (rest>0) {
                ret = send (socket, src,
                            (rest>SEND_MAX)?SEND_MAX:rest,
                            0);
                if (ret<0)
                    { perror ("send_binary"); exit (1); }
                rest -= ret;
                src += ret;
#ifdef DEBUG
                dbg_printf ("send_binary: %ld bytes", ret);
#endif
            }
            return n_elm;
        }
}

/* 受信 */
/* 確実にlenだけ受信 */
ssize_t recv2 (int s, void *buf, size_t len, int flags)
{
    size_t rest=len;
    ssize_t ret;
    while (rest>0)
        {
            ret = recv (s, buf, (rest>RECV_MAX) ? RECV_MAX : rest, flags);
            if (ret<0)
                { perror ("recv2"); exit(1); }
            buf += ret;
            rest -= ret;
        }
    return len;
}

int receive_char (int socket)
{
    char buf;
    int ret;

    if (receive_buf) {
      if ( *receive_buf_p )
        return ( *(receive_buf_p++) );
      else {
        receive_buf = 0;
        /* →socketから読み込む */
      }
    } 
    if (socket<0)
        {
            return fgetc (stdin);
        }
    else
        {
            ret=recv2 (socket, &buf, 1, 0);
#ifdef DEBUG
            dbg_printf ("receive_char: '%s' (%d)", (char[]){buf,0}, buf);
#endif
            if (ret <= 0) return EOF;
            else return buf;
        }
}

char* receive_line (char *buf, int maxlen, int socket)
{
    char ch;
    int ret;
    int i;

    if (receive_buf) {
      if ( *receive_buf_p ) {
        for (i=0 ; i<maxlen-1 ; i++) {
          if ( *receive_buf_p == '\n' ) {
            receive_buf_p++;
            break;
          } else if ( *receive_buf_p == 0 ) {
            break;
          } else {
            buf[i] = *receive_buf_p;
            receive_buf_p++;
          }
        }
        buf[i] = '\0';
        return buf;
      }
      else {
        receive_buf = 0;
        /* →socketから読み込む */
      }
    } 
    if (socket<0)
        {
            return fgets (buf, maxlen, stdin);
        }
    else
        {
            for (i=0; i<maxlen-1 ; i++) {
                ret=recv2 (socket, &ch, 1, 0);
                if (ret <= 0)
                    {
                        perror ("receive_line");
                        return NULL;
                    }
                else
                    {
                        buf[i]=ch;
                        if (ch == '\n') { i++; break; }
                    }
            }
            buf[i]='\0';
#ifdef DEBUG
            dbg_printf ("receive_line: %s", buf);
#endif
            return buf;
        }
}

int receive_binary (void *dst, unsigned long elm_size, unsigned long n_elm,
                    int socket)
{
    if (receive_buf) {
      perror ("receive_binary to receive_buf");
    }
    if (socket<0)
        {
            return fread (dst, elm_size, n_elm, stdin);
        }
    else
        {
            int i;
            for (i=0; i<elm_size ; i++)
                {
                    dst += recv2 (socket, dst, n_elm, 0);
#ifdef DEBUG
                    dbg_printf ("receive_binary: %ld bytes", n_elm);
#endif
                }
            return n_elm;
        }
}


/* 接続・切断 */
/* <hostname, port> => socket */
int connect_to (char *hostname, unsigned short port)
{
    struct hostent *servhost;
    int dstSocket;              /* socket */
    struct sockaddr_in dstAddr; /* sockaddr_in 構造体 */

    /* 名前->IP address */
    servhost = gethostbyname(hostname);
    if (servhost == NULL) {
        perror ("gethostbyname");
        exit (1);
    }
    /* sockaddr_in 構造体のセット */
    memset(&dstAddr, 0, sizeof(dstAddr));      /* zero clear */
    dstAddr.sin_port = htons(port);            /* port */
    dstAddr.sin_family = AF_INET;              /* omajinai */
    memcpy((char*)&dstAddr.sin_addr,           /* address */
           servhost->h_addr, servhost->h_length);
    
    /* ソケット生成 */
    dstSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0) {
        perror ("socket");
        exit (1);
    }

    /* 接続 */
    fprintf(stderr, "Trying to connect to %s:%d \n", hostname, port);
    if (0 != connect(dstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr))) {
        perror ("connection");
        exit (1);
    }
    fprintf(stderr, "Ok\n", hostname, port);
    /* test */
    /* send(dstSocket, "Test", strlen("Test")+1, 0); */
    /* close(dstSocket); */

    return dstSocket;
}

void close_socket (int socket)
{
    close (socket);
}

/* test */
#ifdef TEST
int main (int argc, char **argv)
{
    unsigned short port=9876;
    char hname[256];
    int socket;
    
    if (argc > 1)
        strcpy (hname, argv[1]);
    else
        strcpy (hname, "127.0.0.1");
    socket=connect_to (hname, port);
    sock_printf (socket, "send to %s:%d\n", hname, port);
    sleep(1);
    close_socket (socket);
}
#endif
