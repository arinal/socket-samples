#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "common.h"

int read_line(int sock, char buffer[], int len)
{
    char* c; int n;
    for (n = 0, c = buffer; n < len; ++n, ++c) {
        read(sock, c, 1);
        if (*c == '\n') {
            *c = 0;
            break;
        }
    }
    return n;
}

int write_line(int sock, char buffer[], int len)
{
    int left = len;
    while (left > 0) {
        int n = write(sock, buffer, left);
        left -= n;
        buffer += n;
    }
    return len;
}

void *get_in_addr(const struct sockaddr *sock_addr)
{
    return (sock_addr->sa_family == AF_INET)? &(((struct sockaddr_in*) sock_addr)->sin_addr) :
           &(((struct sockaddr_in6*) sock_addr)->sin6_addr);
}

int probe_local_addr(int listen_port)
{
    struct addrinfo hint_addr, *res_addr;
    memset(&hint_addr, 0, sizeof hint_addr);
    hint_addr.ai_family = AF_UNSPEC;
    hint_addr.ai_socktype = SOCK_STREAM;
    hint_addr.ai_flags =  AI_PASSIVE;

    char port_string[6];
    snprintf(port_string, 5, "%d", listen_port);
    if (getaddrinfo(NULL, port_string, &hint_addr, &res_addr) != 0) {
        fprintf(stderr, "server: error getting address\n");
        return -1;
    }

    int listen_sock;
    struct addrinfo *p_addr;
    for (p_addr = res_addr; p_addr != NULL; p_addr = p_addr->ai_next) {
        listen_sock = socket(p_addr->ai_family, p_addr->ai_socktype, p_addr->ai_protocol);
        if (listen_sock < 0) continue;

        bool yes = true;
        setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
        if (bind(listen_sock, p_addr->ai_addr, p_addr->ai_addrlen) < 0) {
            perror("bind");
            close(listen_sock);
            continue;
        }
        break;
    }

    if (p_addr == NULL) return -2;

    freeaddrinfo(res_addr);

    return listen_sock;
}