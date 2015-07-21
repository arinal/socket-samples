#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common/socket.h"

void run_server(int port);
void connect_to_server(int port);

int main()
{
    printf("1. Run as server\n"
                   "2. Connect to server\n"
                   "Option: ");
    char option = getchar(); getchar();
    switch (option) {
        case '1':
            run_server(8088);
            break;
        case '2':
            connect_to_server(8088);
            break;
    }
}

void run_server(int port)
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listen_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));

    listen(listen_fd, 1024);
    printf("Socket %d now listening at port %d\n", listen_fd, port);

    bool quit = false;
    while (!quit) {
        int fd = accept(listen_fd, NULL, NULL);
        char buffer[512];
        int n = read_line(fd, buffer, 512);
        quit = strncmp("exit", buffer, strlen("exit")) == 0;
        write_line(fd, buffer, n);
        close(fd);
    }
}

void connect_to_server(int port)
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_aton("127.0.0.1", &server_addr.sin_addr);

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    connect(client_fd, (const struct sockaddr *) &server_addr, sizeof(server_addr));

    printf("String to echo: ");
    char input[64];
    fgets(input, 64, stdin);

    write_line(client_fd, input, strlen(input));
    char reply[64];
    read_line(client_fd, reply, 64);
    printf("Server replied: %s\n", reply);
}
