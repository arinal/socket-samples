#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
	printf("Choose one from the following loops:"
		"1. Plain\n"
		"2. Fork new process\n"
		"3. Spawn new thread\n"
		"4. Multiplexed I/O (select)\n"
		"5. epoll\n"
		"Your options: ");
	char opt = getc(stdin);

	switch (opt) {
		case '1':
			loop_plain(&server);
			break;
		case '2':
			loop_fork_process(&server);
			break;
		case '3':
			loop_thread(&server);
			break;
		case '4':
			loop_select(&server);
			break;
//        case '5':
//            loop_epoll(&server);
//            break;
	}

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