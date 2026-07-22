#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8084

int main(void) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
    if (socket_fd < 0 || connect(socket_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        return EXIT_FAILURE;
    }
    const char *message = "Hello from the threaded client";
    send(socket_fd, message, strlen(message), 0);
    char reply[256] = {0};
    recv(socket_fd, reply, sizeof(reply) - 1, 0);
    printf("Server replied: %s\n", reply);
    close(socket_fd);
    return EXIT_SUCCESS;
}
