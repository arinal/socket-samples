#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include "server.h"
#include "common.h"

void process_command(server_t *server, command_t *command)
{
    char buffer[128];
    int c, is_add;
    switch (command->type) {
        case GET:
            sprintf(buffer, "^? %d \r\n", server->counter);
            break;
        case ADD:
        case SUB:
            is_add = command->type == ADD;
            c = (is_add)? '+' : '-';
            server->counter += command->arg1 * (is_add? 1 : -1);
            sprintf(buffer, "^%c/r/n", c);
            break;
        case SET:
            if (command->arg1 == COUNTER) server->counter = command->arg2;
            else server->delay = command->arg2;
            sprintf(buffer, "^=\r\n");
            break;
        case END:
            close(command->client_fd);
            return;
    }
    write_line(command->client_fd, buffer, (int)strlen(buffer));
}

command_t get_next_command(int fd)
{
    char buffer[128];
    ssize_t bytes = recv(fd, buffer, sizeof buffer, 0);

    command_t res;
    res.client_fd = fd;
    switch (*buffer) {
        case '?':
            res.type = GET;
            break;
        case '+':
        case '-':
            res.type = (*buffer == '+')? ADD : SUB;
            sscanf(buffer + 1, "%d", &res.arg1);
            break;
        case '=':
            res.type = SET;
            sscanf(buffer + 1, "%d %d", &res.arg1, &res.arg2);
            break;
        case '~':
            res.type = END;
            break;
        default:
            if (bytes <= 0) res.type = END;
            break;
    }

    return res;
}


void server_listen(server_t *server)
{
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(server->port);

    server->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bind(server->listen_fd, (struct sockaddr*) &addr, sizeof(addr));

    listen(server->listen_fd, 1024);
}
