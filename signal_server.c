#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static volatile sig_atomic_t request_received = 0;
static volatile sig_atomic_t client_pid = 0;

static void handle_request(int signal_number, siginfo_t *info, void *context) {
    (void)signal_number;
    (void)context;
    client_pid = info->si_pid;
    request_received = 1;
}

int main(void) {
    struct sigaction action = {0};
    action.sa_sigaction = handle_request;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);
    sigaction(SIGUSR1, &action, NULL);

    printf("Signal server PID: %d\n", getpid());
    printf("Waiting for SIGUSR1 from a client...\n");
    fflush(stdout);
    while (!request_received) pause();

    printf("Received SIGUSR1 from client PID %d\n", client_pid);
    if (kill(client_pid, SIGUSR2) < 0) {
        perror("kill");
        return EXIT_FAILURE;
    }
    printf("Sent SIGUSR2 acknowledgement to client\n");
    return EXIT_SUCCESS;
}
