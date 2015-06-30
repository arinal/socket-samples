#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/socket.h>
#include "server.h"

typedef struct {
    server_t *server;
    int fd;
} args_t;

static void *handle_request(void* args);

void loop_thread(server_t *server)
{
    pthread_t threads[50];

    while (!server->quit) {
        args_t* args = malloc(sizeof(args_t));
        args->fd = accept(server->listen_fd, NULL, NULL);
        args->server = server;
        pthread_create(&threads[args->fd], NULL, &handle_request, args);
    }
}

static void *handle_request(void* args)
{
    args_t* params = args;
    bool end = false;
    while (!end) {
        command_t command = get_next_command(params->fd);
        process_command(params->server, &command);
        end = command.type == END;
    }
    free(args);
}