void select_basic()
{
	fd_set readfds, writefds;
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	FD_SET(STDIN_FILENO, &readfds);
	FD_SET(STDOUT_FILENO, &writefds);

	char n;
	read(STDIN_FILENO, &n, sizeof(n));

	int count = select(2, &readfds, &writefds, NULL, NULL);
	printf("count = %d\n", count);

	int fd;
	for (fd = 0; fd < 2; ++fd) {
		if (FD_ISSET(fd, &readfds))
			printf("%d is ready to read\n", fd);
		if (FD_ISSET(fd, &writefds))
			printf("%d is ready to write\n", fd);
	}
}

void poll_basic()
{
	struct pollfd fds[] = {
			{STDIN_FILENO, 0 | POLLIN, 0},
			{STDOUT_FILENO, 0 | POLLOUT, 0},
	};

	char n;
	read(STDOUT_FILENO, &n, sizeof(n));
	poll(fds, 2, -1);

	int i;
	for (i = 0; i < 2; ++i) {
		if (fds[i].revents | POLLIN)
			printf("File %d ready to be read\n", i);
		if (fds[i].revents | POLLOUT)
			printf("File %d ready to be written\n", i);
	}

	return 0;
}