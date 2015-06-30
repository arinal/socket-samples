void echo_client_using_plain(uint16_t port)
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_aton("127.0.0.1", &server_addr.sin_addr);

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    connect(client_fd, (const struct sockaddr *) &server_addr, sizeof(server_addr));

    printf("String to echo: ");
    char buffer[512];
    fgets(buffer, 512, stdin);

    write_line(client_fd, buffer, strlen(buffer));
    read_line(client_fd, buffer, 512);

    printf("Server replied: %s\n", buffer);
}

void echo_server_using_plain(int port)
{
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listen_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));

    listen(listen_fd, 1024);

    bool quit = false;
    while (!quit) {
        int fd = accept(listen_fd, NULL, NULL);
        char buffer[512];
        int n = read_line(fd, buffer, 512);
        quit = strncmp("exit", buffer, strlen("exit")) == 0;
        write_line(fd, buffer, n);
        close(fd);
    }
}

