#include <stdbool.h>
#include <netinet/in.h>
#include <stdio.h>

#include "server.h"

int main()
{
    server_t server;
    server.port = 8088;
    server.counter = 66;
    server.quit = false;

    server_listen(&server);
    printf("Listened to port %d\n", server.port);

//    loop_plain(&server);
//    loop_thread(&server);
    loop_select(&server);

    return 0;
}

void loop_plain(server_t *server)
{
    while (!server->quit) {
        int fd = accept(server->listen_fd, NULL, NULL);
        command_t command = get_next_command(fd);
        process_command(server, &command);
    }
}



//    echo_server_using_plain(&server);
//    echo_server_using_select();
//    echo_client_using_plain(&server);
