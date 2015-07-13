#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int read_line(int fd, void *buffer, int n)
{
	if (n <= 0 || buffer == NULL) {
		errno = EINVAL;
		return -1;
	}

	char *buf = buffer;
	ssize_t total = 0;
	while (1) {
		char ch;
		ssize_t num_read = read(fd, &ch, 1);
		if (num_read == -1) {
			if (errno == EINTR) continue; /* Interrupted --> restart read() */
			else return -1;               /* Some other error */
		} else if (num_read == 0) {       /* EOF */
			if (total == 0) return 0;  /* No bytes read; return 0 */
			else break;                   /* Some bytes read; add '\0' */
		} else {                          /* 'num_read' must be 1 if we get here */
			if (total < n - 1) {       /* Discard > (n - 1) bytes */
				total++;
				*buf++ = ch;
			}
			if (ch == '\n') break;
		}
	}
	*buf = '\0';
	return total;
}

int write_line(int sock, char buffer[], int len)
{
	int left = len;
	while (left > 0) {
		ssize_t n = write(sock, buffer, (size_t) left);
		left -= n;
		buffer += n;
	}
	return len;
}

void *get_in_addr(struct sockaddr *sock_addr)
{
	return (sock_addr->sa_family == AF_INET)? &(((struct sockaddr_in*) sock_addr)->sin_addr) :
		   &(((struct sockaddr_in6*) sock_addr)->sin6_addr);
}

int probe_local_addr(int listen_port)
{
	struct addrinfo hint_addr, *res_addr;
	memset(&hint_addr, 0, sizeof hint_addr);
	hint_addr.ai_family = AF_UNSPEC;
	hint_addr.ai_socktype = SOCK_STREAM;
	hint_addr.ai_flags =  AI_PASSIVE;

	char port_string[6];
	snprintf(port_string, 5, "%d", listen_port);
	if (getaddrinfo(NULL, port_string, &hint_addr, &res_addr) != 0) {
		fprintf(stderr, "server: error getting address\n");
		return -1;
	}

	int listen_fd = -1;
	struct addrinfo *p_addr;
	for (p_addr = res_addr; p_addr != NULL; p_addr = p_addr->ai_next) {
		listen_fd = socket(p_addr->ai_family, p_addr->ai_socktype, p_addr->ai_protocol);
		if (listen_fd < 0) continue;

		bool yes = true;
		setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
		if (bind(listen_fd, p_addr->ai_addr, p_addr->ai_addrlen) < 0) {
			perror("bind");
			close(listen_fd);
			continue;
		}
		break;
	}

	if (p_addr == NULL) return -2;

	freeaddrinfo(res_addr);

	return listen_fd;
}