#include <sys/wait.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server.h"
#include "common/common.h"

static void grim_reaper(int sig)
{
    int old = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) continue;
    errno = old;
}

void loop_fork_process(server_t *server)
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grim_reaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) syslog_exit("sigaction");

    while (1) {
        int cfd = accept(server->listen_fd, NULL, NULL);
        if (cfd == -1) syslog_exit("Failure in accept");

        switch (fork()) {
            case -1:
                syslog_nx("Can't create child");
                close(cfd);
                break;
            case 0: // this is child
                close(server->listen_fd);
                command_t command = get_next_command(cfd);
                process_command(server, &command);
                _exit(EXIT_SUCCESS);
            default:
                close(cfd);
                break;
        }
    }
}
