#ifndef GRIFF_COMMON_H
#define GRIFF_COMMON_H

#include <stdint.h>
#include <sys/socket.h>

int read_line(int sock, char buffer[], int len);
int write_line(int sock, char buffer[], int len);
void *get_in_addr(struct sockaddr *sock_addr);
int probe_local_addr(int listen_port);

#endif //GRIFF_COMMON_H
