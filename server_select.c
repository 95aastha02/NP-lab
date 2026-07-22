#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8083

int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(PORT);
    if (server_fd < 0 || bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0 ||
        listen(server_fd, 5) < 0) {
        perror("server setup");
        return EXIT_FAILURE;
    }

    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(server_fd, &read_set);
    printf("select() server waiting on port %d\n", PORT);
    fflush(stdout);
    if (select(server_fd + 1, &read_set, NULL, NULL, NULL) < 0) {
        perror("select");
        return EXIT_FAILURE;
    }

    int client_fd = accept(server_fd, NULL, NULL);
    printf("New connection is ready for reading\n");
    FD_ZERO(&read_set);
    FD_SET(client_fd, &read_set);
    if (select(client_fd + 1, &read_set, NULL, NULL, NULL) < 0) {
        perror("select");
        return EXIT_FAILURE;
    }

    char message[256] = {0};
    recv(client_fd, message, sizeof(message) - 1, 0);
    printf("select() marked client ready: %s\n", message);
    const char *reply = "Message processed using select()";
    send(client_fd, reply, strlen(reply), 0);
    close(client_fd);
    close(server_fd);
    return EXIT_SUCCESS;
}
