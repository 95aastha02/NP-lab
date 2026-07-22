#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main(void) {
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    const char *message = "Hello from Aastha's UDP client";
    sendto(socket_fd, message, strlen(message), 0,
           (struct sockaddr *)&server, sizeof(server));
    printf("Sent datagram: %s\n", message);

    char buffer[BUFFER_SIZE] = {0};
    socklen_t server_length = sizeof(server);
    ssize_t received = recvfrom(socket_fd, buffer, sizeof(buffer) - 1, 0,
                                (struct sockaddr *)&server, &server_length);
    if (received < 0) {
        perror("recvfrom");
        close(socket_fd);
        return EXIT_FAILURE;
    }
    printf("Server replied: %s\n", buffer);

    close(socket_fd);
    return EXIT_SUCCESS;
}
