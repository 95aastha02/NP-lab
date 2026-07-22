#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static volatile sig_atomic_t data_ready = 0;

static void handle_sigio(int signal_number) {
    (void)signal_number;
    data_ready = 1;
}

int main(void) {
    int pair[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair) < 0) {
        perror("socketpair");
        return EXIT_FAILURE;
    }
    struct sigaction action = {0};
    action.sa_handler = handle_sigio;
    sigemptyset(&action.sa_mask);
    sigaction(SIGIO, &action, NULL);
    fcntl(pair[1], F_SETOWN, getpid());
    fcntl(pair[1], F_SETFL, fcntl(pair[1], F_GETFL) | O_ASYNC | O_NONBLOCK);

    const char *message = "kernel sent SIGIO";
    write(pair[0], message, strlen(message));
    for (int attempt = 0; attempt < 100 && !data_ready; ++attempt) usleep(1000);
    if (!data_ready) {
        fprintf(stderr, "SIGIO was not delivered\n");
        return EXIT_FAILURE;
    }
    char buffer[128] = {0};
    read(pair[1], buffer, sizeof(buffer) - 1);
    printf("SIGIO handler reported ready data\n");
    printf("Signal-driven read received: %s\n", buffer);
    close(pair[0]);
    close(pair[1]);
    return EXIT_SUCCESS;
}
