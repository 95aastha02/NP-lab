#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 8082

int main(void) {
    signal(SIGCHLD, SIG_IGN);
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

    printf("Fork server listening on port %d\n", PORT);
    fflush(stdout);
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept");
        return EXIT_FAILURE;
    }

    pid_t child = fork();
    if (child < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (child == 0) {
        close(server_fd);
        char message[256] = {0};
        recv(client_fd, message, sizeof(message) - 1, 0);
        printf("Child process %d received: %s\n", getpid(), message);
        const char *reply = "Request handled by a forked child";
        send(client_fd, reply, strlen(reply), 0);
        close(client_fd);
        return EXIT_SUCCESS;
    }

    printf("Parent process %d created child %d\n", getpid(), child);
    close(client_fd);
    close(server_fd);
    waitpid(child, NULL, 0);
    return EXIT_SUCCESS;
}
