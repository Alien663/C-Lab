#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

void main(){
    int pipe_fd1[2];
    int pipe_fd2[2];
    int num;
    int stdout_copy = dup(STDOUT_FILENO);
    char pipe_buff[10000] = {0};

    if(pipe(pipe_fd1) < 0) printf("Pipe 1 Create Error\n");
    if(pipe(pipe_fd2) < 0) printf("Pipe 2 Create Error\n");
    signal(SIGCHLD,SIG_IGN);
    pid_t pid = fork();
    if(pid < 0) printf("Fork Process Error\n");
    else if( pid == 0){
        printf("I'm child, %d\n", getpid());
        close(pipe_fd1[1]);
        dup2(pipe_fd1[0], STDIN_FILENO);
        close(pipe_fd1[0]);
        
        close(pipe_fd2[0]);
        dup2(pipe_fd2[1], STDOUT_FILENO);
        close(pipe_fd2[1]);

        execlp("cat","cat",NULL);
        exit(0);
    }
    else{
        printf("I'm parent, %d\n", getpid());
        
        close(pipe_fd1[0]);
        dup2(pipe_fd1[1], STDOUT_FILENO);
        close(pipe_fd1[1]);

        printf("Can you hear me?");
        fflush(stdout);

        close(pipe_fd2[1]);
        dup2(stdout_copy,STDOUT_FILENO);
        memset(pipe_buff, 0, sizeof(pipe_buff));
        num = read(pipe_fd2[0], pipe_buff, sizeof(pipe_buff));
        
        printf("num : %d\n", num);
        printf("pipe_buff : %s\n", pipe_buff);

        close(pipe_fd2[0]);

    }   
}