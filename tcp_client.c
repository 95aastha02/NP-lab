#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) != 1) {
        fprintf(stderr, "Invalid server address\n");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    if (connect(socket_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    const char *message = "Hello from Aastha's TCP client";
    send(socket_fd, message, strlen(message), 0);
    printf("Sent: %s\n", message);

    char buffer[BUFFER_SIZE] = {0};
    ssize_t received = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
    if (received < 0) {
        perror("recv");
        close(socket_fd);
        return EXIT_FAILURE;
    }
    printf("Server replied: %s\n", buffer);

    close(socket_fd);
    return EXIT_SUCCESS;
}
