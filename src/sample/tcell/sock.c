/*
Copyright (c) 2008-2014 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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
#ifdef USEMPI
#include <mpi.h>
#endif

#include "sock.h"

/* #define DEBUG */

#define SEND_MAX 65536
#define RECV_MAX 1024
#define DATA_TYPE MPI_CHAR
#define DATA_TAG  0

static char *mpirecv_buf = NULL;
static int mpirecv_buf_size = 0;
static int mpirecv_buf_len = 0;
static int mpirecv_buf_start = 0;
char **mpisend_buf;             /* ptr to current send buffer for MPI */
int *mpisend_buf_len;           /* ptr to length of the current buffer */
static int *mpisend_buf_size;   /* ptr to capacity of the current buffer */

struct send_block
{
    char *buf; // ���M����f�[�^
    int len;   // buf�ɓ����Ă���f�[�^�̑傫��
    int size;  // buf�Ɋm�ۂ��ꂽ�q�[�v�T�C�Y
    int rank;  // ���M��rank
    struct send_block *next;
};

static struct send_block *send_queue_head = NULL;
static struct send_block *send_queue_tail = NULL;
static struct send_block *send_queue_temp = NULL;
static pthread_mutex_t send_lock;

/* SunOS does not support vasprintf() */
#ifdef NO_VASPRINTF
#define VSNPRINTF_LEN 1000
#endif

/* If receive_buf points a string, Tascell reads this as if this is an external message
   before reading a sokcet/stdin (until receive_buf_p points the end of the string) */
char *receive_buf = 0;
char *receive_buf_p = 0;

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

// Prepare for adding a message to the send queue.
void send_block_start (void)
{
  struct send_block *sq = (struct send_block*)malloc(sizeof(struct send_block));
  sq->buf = malloc(sizeof(char)*32768);
  sq->len = 0;
  sq->size = 32768;
  sq->next = NULL;
  mpisend_buf = &(sq->buf);
  mpisend_buf_size = &(sq->size);
  mpisend_buf_len = &(sq->len);
  send_queue_temp = sq;
}

// Finish adding the message to the send queue.
void send_block_end(int rank)
{
  send_queue_temp->rank = rank;
  pthread_mutex_lock(&send_lock);
  {
    if (send_queue_head == NULL) send_queue_head = send_queue_temp;
    if (send_queue_tail != NULL) send_queue_tail->next = send_queue_temp;
    send_queue_tail = send_queue_temp;
    send_queue_temp = NULL;
  }
  pthread_mutex_unlock(&send_lock);
}

// Append contents in buf whose length is len to *mpisend_buf
int
append_to_mpisend_buf(const void *buf, int len)
{
    int ns = len + *mpisend_buf_len;
    if (ns > *mpisend_buf_size)
    {
        *mpisend_buf = realloc(*mpisend_buf, ns+1);
        if (*mpisend_buf == NULL)
        {
            fprintf(stderr, "Error: Not enough memory.\n");
            exit(1);
        }
        *mpisend_buf_size = ns+1;
    }
    memcpy((*mpisend_buf)+(*mpisend_buf_len), buf, len);
    *mpisend_buf_len = ns;
}

/* send */
int send_char (char ch, int socket)
{
#ifdef DEBUG
  dbg_printf ("send_char: '%s' (%d)", (char[]){ch,0}, ch);
#endif
  if (socket<0) {           // MPI
    append_to_mpisend_buf ((char[]){ch,0}, 1);
    return 0;
  } else if (socket == 0) { // stdout
    return putc (ch, stdout);
  } else {                  // TCP/IP
    return send (socket, &ch, 1, 0);
  }
}

int send_string (char *str, int socket)
{
#ifdef DEBUG
  dbg_printf ("send_string: %s", str);
#endif
  if (socket<0) {           // MPI
    append_to_mpisend_buf (str, strlen(str));
    return 0;
  } else if (socket == 0) { // stdout
    return fputs (str, stdout);
  } else {                  // TCP/IP
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

    if (socket == 0) // stdout
        {
            va_start (args, fmt_string);
            len = vprintf (fmt_string, args);
            va_end (args);
            return len;
        }
    else             // MPI or TCP/IP
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
	    if (socket<0) { // MPI
	      append_to_mpisend_buf (str, len);
	      ret = 0;
	    }
	    else {          // TCP/IP
	      ret = send (socket, str, len, 0);
	    }
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
  if (socket < 0)     // MPI
    {
      append_to_mpisend_buf (src, elm_size*n_elm);
      return 0;
    }
  else if (socket==0) // stdout
    {
      return fwrite (src, elm_size, n_elm, stdout);
    }
  else                // TCP/IP
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

/* Print contents of *mpisend_buf to stderr */
void show_mpisend_buf (int socket) {
  if (socket < 0) { // MPI
    (*mpisend_buf)[*mpisend_buf_len-1] = 0;
    fprintf (stderr, "%s\n", *mpisend_buf);
  }
  /* do nothing unless MPI mode */
}

/* receive */
/* Get contents of the length len from mpirecv_buf */
static ssize_t
get_from_mpirecv_buf (void *buf, int len)
{
    int rest = len;
    int cpylen;

    for (;;)
    {
        cpylen = (mpirecv_buf_len < rest) ? mpirecv_buf_len : rest;
        if (cpylen > 0)
        {
            memcpy(buf, mpirecv_buf + mpirecv_buf_start, cpylen);
            buf += cpylen;
            mpirecv_buf_start += cpylen;
            mpirecv_buf_len -= cpylen;
            rest -= cpylen;
        }
        if (rest == 0) return len;
    }
}

/* Invoke recv() repeatedly to read len characters */
ssize_t recv2 (int socket, void *buf, size_t len, int flags)
{
    size_t rest=len;
    ssize_t ret;
    while (rest>0)
        {
            ret = recv (socket, buf, (rest>RECV_MAX) ? RECV_MAX : rest, flags);
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
      /* Finished reading receive_buf */
      receive_buf = 0;
    }
  } 
  if (socket<0)       // MPI
    {
      get_from_mpirecv_buf (&buf, sizeof(char));
      return buf;
    }
  else if (socket==0) // stdin
    {
      return fgetc (stdin);
    }
  else                // TCP/IP
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
        /* Finished reading receive_buf */
        receive_buf = 0;
      }
    } 
    if (socket<0)  // MPI
      {
	for (i = 0; i < maxlen-1; i++)
	  {
	    ret = get_from_mpirecv_buf (&ch, sizeof(char));
	    if (ret <= 0)
	      {
		perror("receive_line");
		return NULL;
	      }
	    else
	      {
		buf[i]=ch;
		if (ch == '\n') { i++; break; }
	      }
	  }
	buf[i]='\0';
	return buf;
      }
    if (socket==0) // stdin
      {
	return fgets (buf, maxlen, stdin);
      }
    else           // TCP/IP
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
      perror ("receive_binary from receive_buf");
    }
    if (socket<0)  // MPI
      {
	get_from_mpirecv_buf (dst, n_elm*elm_size);
	return n_elm;
      }
    if (socket==0) // stdin
      {
	return fread (dst, elm_size, n_elm, stdin);
      }
    else           // TCP/IP
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


/* Connect to the server */
/* <hostname, port> => socket */
int connect_to (char *hostname, unsigned short port)
{
    struct hostent *servhost;
    int dstSocket;              /* socket */
    struct sockaddr_in dstAddr; /* sockaddr_in structure object */

    /* 名前->IP address */
    servhost = gethostbyname(hostname);
    if (servhost == NULL) {
        perror ("gethostbyname");
        exit (1);
    }
    /* Initialize sockaddr_in object */
    memset(&dstAddr, 0, sizeof(dstAddr));      /* zero clear */
    dstAddr.sin_port = htons(port);            /* port */
    dstAddr.sin_family = AF_INET;              /* omajinai */
    memcpy((char*)&dstAddr.sin_addr,           /* address */
           servhost->h_addr, servhost->h_length);
    
    /* Generate a socket */
    dstSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0) {
        perror ("socket");
        exit (1);
    }

    /* Connect */
    fprintf(stderr, "Trying to connect to %s:%d \n", hostname, port);
    if (0 != connect(dstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr))) {
        perror ("connection");
        exit (1);
    }
    fprintf(stderr, "Ok\n");
    /* test */
    /* send(dstSocket, "Test", strlen("Test")+1, 0); */
    /* close(dstSocket); */

    return dstSocket;
}

void close_socket (int socket)
{
    close (socket);
}

// Infinite loop for messaging thread in MPI mode
void sendrecv()
{
    MPI_Status send_status, recv_status;
    MPI_Request req;
    int sflag, rflag;
    int sent;
    int readlen;

    for (sent = 0;;)
    {
        MPI_Iprobe(MPI_ANY_SOURCE, DATA_TAG, MPI_COMM_WORLD,
                &rflag, &recv_status);
        if (rflag)
        {
            MPI_Get_count(&recv_status, DATA_TYPE, &readlen);
            if (readlen > mpirecv_buf_size)
            {
                mpirecv_buf = realloc(mpirecv_buf, readlen);
                if (mpirecv_buf == NULL)
                {
                    fprintf(stderr, "Error: Not enough memory.\n");
                    exit(1);
                }
                mpirecv_buf_size = readlen;
            }
            MPI_Recv(mpirecv_buf, readlen, DATA_TYPE, recv_status.MPI_SOURCE,
                    DATA_TAG, MPI_COMM_WORLD, &recv_status);
            mpirecv_buf_len = readlen;
            mpirecv_buf_start = 0;
            proc_msg();
        }

        usleep(20);
        sched_yield();

        if (sent)
        {
            MPI_Test(&req, &sflag, &send_status);
            if (sflag)
            {
                pthread_mutex_lock(&send_lock);
                {
                    struct send_block *sq = send_queue_head;
                    free(sq->buf);
                    send_queue_head = sq->next;
                    if (sq->next == NULL) send_queue_tail = NULL;
                    free(sq);
                    sent = 0;
                }
                pthread_mutex_unlock(&send_lock);
            }
        }
        else
        {
            pthread_mutex_lock(&send_lock);
            {
                if (send_queue_head != NULL)
                {
                    struct send_block *sq = send_queue_head;
                    MPI_Issend(sq->buf, sq->len, DATA_TYPE, sq->rank,
                            DATA_TAG, MPI_COMM_WORLD, &req);
                    sent = 1;
                }
            }
            pthread_mutex_unlock(&send_lock);
        }
    }
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

