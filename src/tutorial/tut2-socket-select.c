#include <sys/socket.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int port = 8088;
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    bind(listen_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
    listen(listen_sock, 1024);
    printf("Listened to port %d\n", port);

    int max_fd = listen_sock;
    fd_set active_fds, read_fds;
    FD_ZERO(&active_fds);
    FD_SET(listen_sock, &active_fds);

    while (true) {
        read_fds = active_fds;
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        };
        int fd;
        for (fd = 0; fd <= max_fd; ++fd) {
            if (FD_ISSET(fd, &read_fds)) {
                if (fd == listen_sock) {
                    struct sockaddr rem_addr;
                    socklen_t addr_len = sizeof rem_addr;
                    int new_fd = accept(listen_sock, &rem_addr, &addr_len);
                    FD_SET(new_fd, &active_fds);
                    if (new_fd > max_fd) max_fd = new_fd;
                    char remote_ip[INET6_ADDRSTRLEN];
                    printf("server: new connection from %s on socket %d\n",
                           inet_ntop(rem_addr.sa_family, get_in_addr(&rem_addr), remote_ip, INET6_ADDRSTRLEN),
                           new_fd);
                } else {
                    char buffer[128];
                    int bytes = recv(fd, buffer, sizeof buffer, 0);
                    if (bytes <= 0) {
                        close(fd);
                        FD_CLR(fd, &active_fds);
                        printf("server: socket %d closed\n", fd);
                    } else {
                        printf("server: sending %s to client\n", buffer);
                        send(fd, buffer, bytes, 0);
                    }
                }
            }
        }
    }
}