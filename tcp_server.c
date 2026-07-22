#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        close(server_fd);
        return EXIT_FAILURE;
    }

    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("TCP server listening on 127.0.0.1:%d\n", PORT);
    fflush(stdout);

    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE] = {0};
    ssize_t received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (received < 0) {
        perror("recv");
        close(client_fd);
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Client says: %s\n", buffer);
    const char *reply = "Hello from Aastha's TCP server";
    send(client_fd, reply, strlen(reply), 0);
    printf("Reply sent successfully\n");

    close(client_fd);
    close(server_fd);
    return EXIT_SUCCESS;
}
