#ifndef INT32
#define INT32 int
#endif

void send_int(int n);
int recv_int(void);
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
