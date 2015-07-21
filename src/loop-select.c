#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "server.h"
#include "common/common.h"

void loop_select(server_t *server)
{
	int max_fd = server->listen_fd;
	fd_set active_fds, read_fds;
	FD_ZERO(&active_fds);
	FD_SET(server->listen_fd, &active_fds);

	bool quit = false;
	while (!quit) {
		read_fds = active_fds;
		if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
			perror("select");
			exit(EXIT_FAILURE);
		};
		int fd;
		for (fd = 0; fd <= max_fd; ++fd) {
			if (FD_ISSET(fd, &read_fds)) {
				if (fd == server->listen_fd) {
					struct sockaddr rem_addr;
					socklen_t addr_len = sizeof rem_addr;
					int new_fd = accept(server->listen_fd, &rem_addr, &addr_len);
					FD_SET(new_fd, &active_fds);
					if (new_fd > max_fd) max_fd = new_fd;
					printf("server: new connection on socket %d\n", new_fd);
				} else {
					command_t command = get_next_command(fd);
					process_command(server, &command);
					if (command.type == END) {
						FD_CLR(fd, &active_fds);
						printf("server: socket %d closed\n", fd);
					}
				}
			}
		}
	}
}