#include <stdio.h>
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

/* #define DEBUG */

#ifdef DEBUG
int dbg_printf (char *fmt_string, ...)
{
    va_list args;
    int len;
    char *str;
    char *str2;
    int i, i2;

    va_start (args, fmt_string);
    len = vasprintf (&str, fmt_string, args);
    va_end (args);
    if (str == NULL) { perror ("allocation"); exit (1); }

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
    free(str); free(str2);
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
    char *str;
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
            len = vasprintf (&str, fmt_string, args);
            va_end (args);

            if (str == NULL) { perror ("allocation"); exit (1); }
            ret = send (socket, str, len, 0);
#ifdef DEBUG
            dbg_printf ("send_fmt_string: %s", str);
#endif
            free (str);
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
