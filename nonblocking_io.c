#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void) {
    int pair[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair) < 0) {
        perror("socketpair");
        return EXIT_FAILURE;
    }
    fcntl(pair[1], F_SETFL, fcntl(pair[1], F_GETFL) | O_NONBLOCK);
    char buffer[128] = {0};
    if (read(pair[1], buffer, sizeof(buffer) - 1) < 0 && errno == EAGAIN)
        printf("Non-blocking read returned immediately with EAGAIN\n");
    const char *message = "data after EAGAIN";
    write(pair[0], message, strlen(message));
    read(pair[1], buffer, sizeof(buffer) - 1);
    printf("Next non-blocking read received: %s\n", buffer);
    close(pair[0]);
    close(pair[1]);
    return EXIT_SUCCESS;
}
