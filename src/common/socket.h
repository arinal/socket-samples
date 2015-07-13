/*
 * Read characters from 'fd' until a newline is encountered. If a newline character is not encountered in the first
 * (n - 1) bytes, then the excess characters are discarded. The returned string placed in 'buf' is null-terminated and
 * includes the newline character if it was read in the first (n - 1) bytes. The function return value is the number of
 * bytes placed in buffer (which includes the newline character if encountered, but excludes the terminating null byte).
 */
int read_line(int fd, void *buffer, int len);
int write_line(int fd, char buffer[], int len);

void *get_in_addr(struct sockaddr *sock_addr);
int probe_local_addr(int listen_port);

