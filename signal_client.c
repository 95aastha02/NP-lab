#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static volatile sig_atomic_t acknowledgement_received = 0;

static void handle_acknowledgement(int signal_number) {
    (void)signal_number;
    acknowledgement_received = 1;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server-pid>\n", argv[0]);
        return EXIT_FAILURE;
    }
    pid_t server_pid = (pid_t)strtol(argv[1], NULL, 10);
    struct sigaction action = {0};
    action.sa_handler = handle_acknowledgement;
    sigemptyset(&action.sa_mask);
    sigaction(SIGUSR2, &action, NULL);

    printf("Sending SIGUSR1 to server PID %d\n", server_pid);
    if (kill(server_pid, SIGUSR1) < 0) {
        perror("kill");
        return EXIT_FAILURE;
    }
    while (!acknowledgement_received) pause();
    printf("Received SIGUSR2 acknowledgement from server\n");
    return EXIT_SUCCESS;
}
