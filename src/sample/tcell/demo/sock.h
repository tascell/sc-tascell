int connect_to (char *hostname, unsigned short port);
void close_socket (int socket);

int send_char (char, int socket);
int send_string (char *str, int socket);
int send_fmt_string (int socket, char *fmt_string, ...);
int send_binary (void *src, int elm_size, int n_elm, int socket);

int receive_char (int socket);
char* receive_line (char *buf, int maxlen, int socket);
int receive_binary (void *dst, int elm_size, int n_elm, int socket);
