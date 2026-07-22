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
    const char *message = "data for a blocking read";
    write(pair[0], message, strlen(message));
    char buffer[128] = {0};
    printf("Blocking read waits until data is available...\n");
    read(pair[1], buffer, sizeof(buffer) - 1);
    printf("Blocking read received: %s\n", buffer);
    close(pair[0]);
    close(pair[1]);
    return EXIT_SUCCESS;
}
