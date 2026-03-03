#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/select.h>

volatile sig_atomic_t flag_usr1 = 0;
volatile sig_atomic_t flag_usr2 = 0;
volatile sig_atomic_t flag_quit = 0;

void handler_usr1(int sig) {
    (void)sig;
    flag_usr1 = 1;
}

void handler_usr2(int sig) {
    (void)sig;
    flag_usr2 = 1;
}

void handler_quit(int sig) {
    (void)sig;
    flag_quit = 1;
}

int main(int argc, char *argv[]) {
    pid_t childpid;
    int cw[2];
    int cw_in, cw_out;
    int i;
    int nprocs;

    if ((argc != 2) || ((nprocs = atoi(argv[1])) <= 0)) {
        fprintf(stderr, "Usage: %s nprocs\n", argv[0]);
        return 1;
    }

    if (pipe(cw) == -1) {
        perror("Failed to create starting pipe");
        return 1;
    }

    cw_in = cw[0];
    cw_out = cw[1];

    for (i = 1; i < nprocs; i++) {
        if (pipe(cw) == -1) {
            fprintf(stderr, "[%ld]:failed to create pipe %d: %s\n", (long)getpid(), i, strerror(errno));
            return 1;
        }
        if ((childpid = fork()) == -1) {
            fprintf(stderr, "[%ld]:failed to create child %d: %s\n", (long)getpid(), i, strerror(errno));
            return 1;
        }
        if (childpid > 0) {
            cw_out = cw[1];
            close(cw[0]);
        } else {
            cw_in = cw[0];
            close(cw[1]);
        }
        if (childpid) {
            break;
        }
    }

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sa.sa_handler = handler_usr1;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction(SIGUSR1)");
        return 1;
    }

    sa.sa_handler = handler_usr2;
    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("sigaction(SIGUSR2)");
        return 1;
    }

    sa.sa_handler = handler_quit;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction(SIGINT)");
        return 1;
    }

    printf("I am P%d : %ld\n", i, (long)getpid());
    fd_set readfds;
    int running = 1;
    int is_root = 0;

    while (running) {
        if (flag_usr1) {
            is_root = 1;
            flag_usr1 = 0;
            int a = 0;
            int b = 1;
            write(cw_out, &a, sizeof(a));
            write(cw_out, &b, sizeof(b));
        }

        if (flag_usr2) {
            is_root = 1;
            flag_usr2 = 0;
            int token = -2;
            write(cw_out, &token, sizeof(token));
        }

        if (flag_quit) {
            is_root = 1;
            flag_quit = 0;
            int token = -1;
            write(cw_out, &token, sizeof(token));
        }

        FD_ZERO(&readfds);
        FD_SET(cw_in, &readfds);

        int ret = select(cw_in + 1, &readfds, NULL, NULL, NULL);
        if (ret < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("Failed to create select()");
            break;
        }

        if (FD_ISSET(cw_in, &readfds)) {
            int a, b;
            if (read(cw_in, &a, sizeof(a)) > 0) {
                if (a >= 0) {
                    read(cw_in, &b, sizeof(b));
                    printf("I am P%d %ld, got : %d %d\n", i, (long)getpid(), a, b);
                    int sum = a + b;
                    if (is_root) is_root = 0; 
                    else {
                        write(cw_out, &b, sizeof(b));
                        write(cw_out, &sum, sizeof(sum));
                    }
                } else if (a == -2) {
                    printf("I am P%d %ld\n", i, (long)getpid());
                    if (is_root) is_root = 0; 
                    else {
                        write(cw_out, &a, sizeof(a));
                    }
                } else {
                    if (is_root) is_root = 0; 
                    else {
                        write(cw_out, &a, sizeof(a));
                    }
                    running = 0;
                }
            }
        }
    }

    if ((close(cw_in) == -1) || (close(cw_out) == -1)) {
        perror("Failed to close final descriptors");
        return 1;
    }

    return 0;
}
