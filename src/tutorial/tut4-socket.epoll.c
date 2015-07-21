#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "common/errors.h"
#include "common/socket.h"

static int to_nonblock(int sfd)
{
    int flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) err_exit("fcntl(F_GETFL)");
    flags |= O_NONBLOCK;
    if (fcntl(sfd, F_SETFL, flags) == -1) err_exit("fcntl(F_SETFL");
    return 0;
}

#define MAXEVENTS 64

int main(int argc, char *argv[])
{
    int sfd = probe_local_addr(8088);
    if (sfd == -1) abort();

    int s = to_nonblock(sfd);
    if (s == -1) abort();

    s = listen(sfd, SOMAXCONN);
    if (s == -1) err_exit("listen");

    int efd = epoll_create1(0);
    if (efd == -1) err_exit("epoll_create1");

    struct epoll_event event;
    event.data.fd = sfd;
    event.events = EPOLLIN | EPOLLET;
    s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
    if (s == -1) err_exit("epoll_ctl");

    struct epoll_event *events = calloc(MAXEVENTS, sizeof event);

    bool unfinished = true;
    while (unfinished) {
        int n = epoll_wait(efd, events, MAXEVENTS, -1);
        int i;
        for (i = 0; i < n; i++) {
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
                /* An error has occured on this fd, or the socket is not ready for reading (why were we notified then?) */
                fprintf(stderr, "epoll error\n");
                close(events[i].data.fd);
                continue;
            }
            else if (sfd == events[i].data.fd) {
                /* We have a notification on the listening socket, which means one or more incoming connections. */
                while (1) {
                    struct sockaddr in_addr;
                    socklen_t in_len = sizeof in_addr;
                    int infd = accept(sfd, &in_addr, &in_len);
                    if (infd == -1)
                    if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
                        /* We have processed all incoming connections. */
                        break;
                    else err_exit("accept");
                    printf("Accepted connection on descriptor %d ", infd);

                    s = to_nonblock(infd);
                    if (s == -1) abort();
                    event.data.fd = infd;
                    event.events = EPOLLIN | EPOLLET;
                    s = epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event);
                    if (s == -1) err_exit("epoll_ctl");
                }
                continue;
            }
            else {
                /* We have data on the fd waiting to be read. Read and display it. We must read whatever
                 * data is available completely, as we are running in edge-triggered mode and won't get a
                 * notification again for the same data. */
                int done = 0;
                while (1) {
                    char buff[512];
                    int count = read_line(events[i].data.fd, buff, sizeof buff);
                    if (count == -1) {
                        /* If errno == EAGAIN, that means we have read all data. So go back to the main loop. */
                        if (errno != EAGAIN) {
                            perror("read");
                            done = 1;
                        }
                        break;
                    }
                    else if (count == 0) {
                        /* End of file. The remote has closed the connection. */
                        done = 1;
                        break;
                    }
                    /* Write data back to the client */
                    write_line(events[i].data.fd, buff, strlen(buff));
                    unfinished = strncmp(buff, "exit", strlen("exit"));
                }

                if (done) {
                    printf("Closed connection on descriptor %d\n", events[i].data.fd);
                    /* Closing the descriptor will make epoll remove it from the set of descriptors which are monitored. */
                    close(events[i].data.fd);
                }
            }
        }
    }

    free(events);
    close(sfd);

    return 0;
}
