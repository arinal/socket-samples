#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "common/common.h"
#include "server.h"

int main()
{
//    if (daemonize(0) == -1) err_exit("daemonize");

	server_t server;
	server.port = 8088;
	server.counter = 66;
	server.quit = false;

	server_listen(&server);
	printf("Listened to port %d\n", server.port);

//    loop_plain(&server);
//    loop_fork_process(&server);
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