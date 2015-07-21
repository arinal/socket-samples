#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>
#include "common/errors.h"

int main()
{
    int epfd = epoll_create(2);
    if (epfd == -1) err_exit("epoll_create");

    struct epoll_event event1;
    event1.events = EPOLLIN;
    event1.data.fd = STDIN_FILENO;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event1) == -1) err_exit("epoll_ctl");

    struct epoll_event event2;
    event2.events = EPOLLOUT;
    event2.data.fd = STDOUT_FILENO;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDOUT_FILENO, &event2) == -1) err_exit("epoll_ctl");

    struct epoll_event evt_results[10];
    int n = epoll_wait(epfd, evt_results, 2, -1);
    int i;
    for (i = 0; i < n; ++i) {
        if (evt_results[i].events | EPOLLIN)
            printf("FD %d is ready to read\n", evt_results[i].data.fd);
        if (evt_results[i].events | EPOLLOUT)
            printf("FD %d is ready to write\n", evt_results[i].data.fd);
    }

    close(epfd);

    return 0;
}
