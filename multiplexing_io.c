#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void) {
    int pair[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair) < 0) {
        perror("socketpair");
        return EXIT_FAILURE;
    }
    const char *message = "descriptor became ready";
    write(pair[0], message, strlen(message));
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(pair[1], &read_set);
    printf("select() is monitoring the descriptor...\n");
    select(pair[1] + 1, &read_set, NULL, NULL, NULL);
    if (FD_ISSET(pair[1], &read_set)) {
        char buffer[128] = {0};
        read(pair[1], buffer, sizeof(buffer) - 1);
        printf("Multiplexed read received: %s\n", buffer);
    }
    close(pair[0]);
    close(pair[1]);
    return EXIT_SUCCESS;
}
