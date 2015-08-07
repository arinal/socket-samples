#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <malloc.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ev.h>

#define BUFFER_SIZE 1024

int total_clients;

void read_handler(struct ev_loop *loop, struct ev_io *watcher, int revents);
void accept_handler(struct ev_loop *loop, struct ev_io *watcher, int revents);

int main()
{
        struct sockaddr_in addr;
        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8088);
        addr.sin_addr.s_addr = INADDR_ANY;

        int server_d = socket(PF_INET, SOCK_STREAM, 0);
        bind(server_d, (struct sockaddr*) &addr, sizeof(addr));
        listen(server_d, 2);

        struct ev_loop *loop = ev_default_loop(0); // make our main event loop
        struct ev_io w_accept; // watcher for new connection

        // register accept_handler that will be called on every new connection
        ev_io_init(&w_accept, accept_handler, server_d, EV_READ); // bind the watcher to the handler
        ev_io_start(loop, &w_accept); // register the watcher to the event loop
        ev_loop(loop, 0); // and finally, start the event loop

        return 0;
}

void read_handler(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
        if (EV_ERROR & revents) {
                perror("got invalid event");
                return;
        }
        char buffer[BUFFER_SIZE];
        int n = recv(watcher->fd, buffer, BUFFER_SIZE, 0);

        if (n < 0) {
                perror("read error");
                return;
        }
        if (n == 0) {
                ev_io_stop(loop, watcher);
                close(watcher->fd);
                free(watcher);
                printf("Client closed, %d client(s) connected.\n", --total_clients);
                return;
        }
        else
                printf("Message: %s\n", buffer);
        send(watcher->fd, buffer, n, 0);
        bzero(buffer, n);
}

void accept_handler(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
        if (EV_ERROR & revents) {
                perror("got invalid event");
                return;
        }

        int client_sd = accept(watcher->fd, NULL, NULL);
        printf("%d client(s) connected.\n", ++total_clients);

        // register read_handler that will be called when client send any data
        struct ev_io *w_client = (struct ev_io*) malloc(sizeof(struct ev_io));
        ev_io_init(w_client, read_handler, client_sd, EV_READ);
        ev_io_start(loop, w_client);
}
