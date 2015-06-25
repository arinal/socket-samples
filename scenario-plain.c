#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "common.h"

void client_using_plain()
{
    int port = 8088;

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_aton("127.0.0.1", &server_addr.sin_addr);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    connect(client_socket, (const struct sockaddr *) &server_addr, sizeof(server_addr));

    printf("String to echo: ");
    char buffer[512];
    fgets(buffer, 512, stdin);

    write_line(client_socket, buffer, strlen(buffer));
    read_line(client_socket, buffer, 512);

    printf("Server replied: %s\n", buffer);
}

void server_using_plain()
{
    uint16_t port = 8088;
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));

    listen(server_sock, 1024);

    bool quit = false;
    while (!quit) {
        int client_sock = accept(server_sock, NULL, NULL);
        char buffer[512];
        int n = read_line(client_sock, buffer, 512);
        write_line(client_sock, buffer, n);
        close(client_sock);
    }
}

