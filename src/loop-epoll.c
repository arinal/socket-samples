#include <sys/socket.h>
#include <sys/epoll.h>
#include "server.h"
#include "common/common.h"

#define LOOP_EPOLL_MAXEVENTS 5000

static int to_nonblock(int sfd)
{
    int flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) err_exit("fcntl(F_GETFL)");
    flags |= O_NONBLOCK;
    if (fcntl(sfd, F_SETFL, flags) == -1) err_exit("fcntl(F_SETFL");
    return 0;
}

static void add_to_epoll(int epollfd, int fd)
{
    int res = to_nonblock(fd);
    if (res == -1) abort();
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    res = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    if (res == -1) err_exit("epoll_ctl");
}

void loop_epoll(server_t *server)
{
    int epollfd = epoll_create1(0);
    if (epollfd == -1) err_exit("epoll_create1");

    add_to_epoll(epollfd, server->listen_fd);

    struct epoll_event *events = calloc(LOOP_EPOLL_MAXEVENTS, sizeof(struct epoll_event));

    while (!server->quit) {
        int n = epoll_wait(epollfd, events, LOOP_EPOLL_MAXEVENTS, -1);
        int i;
        for (i = 0; i < n; i++) {
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
                err_message("epoll error, closing %d\n", events[i].data.fd);
                close(events[i].data.fd);
                continue;
            }
            else if (server->listen_fd == events[i].data.fd) { /* incoming connections. */
                struct sockaddr addr;
                socklen_t in_len = sizeof addr;
                int newfd = accept(server->listen_fd, &addr, &in_len);
                printf("Accepted connection on descriptor %d ", newfd);
                add_to_epoll(epollfd, newfd);
            }
            else { /* incoming data */
                command_t command = get_next_command(events[i].data.fd);
                process_command(server, &command);
            }
        }
    }

    free(events);
    close(server->listen_fd);
}


//int done = 0;
//while (1) {
//char buff[512];
//int count = read_line(events[i].data.fd, buff, sizeof buff);
//if (count == -1) {
//if (errno == EAGAIN) break;
//else {
//perror("read");
//done = 1;
//}
//}
//else if (count == 0) {
///* End of file. The remote has closed the connection. */
//done = 1;
//break;
//}
///* Write data back to the client */
//
//}
//
//if (done) {
//printf("Closed connection on descriptor %d\n", events[i].data.fd);
//close(events[i].data.fd);
//}