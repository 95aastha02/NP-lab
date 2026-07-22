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

    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(PORT);
    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    printf("UDP server listening on 127.0.0.1:%d\n", PORT);
    fflush(stdout);

    struct sockaddr_in client = {0};
    socklen_t client_length = sizeof(client);
    char buffer[BUFFER_SIZE] = {0};
    ssize_t received = recvfrom(socket_fd, buffer, sizeof(buffer) - 1, 0,
                                (struct sockaddr *)&client, &client_length);
    if (received < 0) {
        perror("recvfrom");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    printf("Datagram received: %s\n", buffer);
    const char *reply = "Hello from Aastha's UDP server";
    sendto(socket_fd, reply, strlen(reply), 0,
           (struct sockaddr *)&client, client_length);
    printf("Reply datagram sent\n");

    close(socket_fd);
    return EXIT_SUCCESS;
}
