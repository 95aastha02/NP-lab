#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8084

static void *handle_client(void *argument) {
    int client_fd = *(int *)argument;
    free(argument);
    char message[256] = {0};
    recv(client_fd, message, sizeof(message) - 1, 0);
    printf("Worker thread received: %s\n", message);
    const char *reply = "Reply from the worker thread";
    send(client_fd, reply, strlen(reply), 0);
    close(client_fd);
    return NULL;
}

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
    printf("Threaded server listening on port %d\n", PORT);
    fflush(stdout);

    int *client_fd = malloc(sizeof(*client_fd));
    if (client_fd == NULL) return EXIT_FAILURE;
    *client_fd = accept(server_fd, NULL, NULL);
    if (*client_fd < 0) {
        perror("accept");
        free(client_fd);
        return EXIT_FAILURE;
    }

    pthread_t worker;
    if (pthread_create(&worker, NULL, handle_client, client_fd) != 0) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }
    printf("Main thread created a worker for the client\n");
    pthread_join(worker, NULL);
    printf("Worker thread completed\n");
    close(server_fd);
    return EXIT_SUCCESS;
}
