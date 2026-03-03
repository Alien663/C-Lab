#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if(argc > 1){
        int forkFlag = 1;

        for(int i=1;i<argc && forkFlag;i++){
            int count = atoi(argv[i]);
            if(count <= 0) break;

            int is_child = 0;
            for(int j=0;j<count;j++){
                pid_t cid = fork();
                if(cid < 0){
                    perror("fork error");
                    exit(1);
                }
                if(cid == 0) {
                    forkFlag = (j == 0); // Only the first child will fork next generation
                    is_child = 1;
                    break;
                }
            }

            // Parent stops forking
            if(!is_child){
                forkFlag = 0;
            }
        }
    }

    for (;;) {
        if (wait(NULL) == -1) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }
    }

    printf("I'm %d, my parent is %d\n", getpid(), getppid());
    return 0;
}