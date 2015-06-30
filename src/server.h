#ifndef SOCKET_SAMPLES_SERVER_H
#define SOCKET_SAMPLES_SERVER_H

#include <stdint.h>
#include <stdbool.h>

enum command_type { GET, ADD, SUB, SET, END, };
enum variable_type { COUNTER, DELAY, };

typedef struct {
    enum command_type type;
    int arg1;
    int arg2;
    int client_fd;
} command_t;

typedef struct {
    uint16_t port;
    int counter;
    int delay;
    int listen_fd;
    bool quit;
} server_t;

void server_listen(server_t *server);
command_t get_next_command(int fd);
void process_command(server_t *server, command_t *command);

void loop_plain(server_t *server);
void loop_thread(server_t *server);
void loop_select(server_t *server);

#endif //SOCKET_SAMPLES_SERVER_H
