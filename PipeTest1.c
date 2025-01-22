#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void main(){
    int pipe_fd[2];
    if(pipe(pipe_fd) < 0) printf("Pipe Create Error\n");
    signal(SIGCHLD,SIG_IGN);
    pid_t pid = fork();
    if(pid < 0) printf("Fork Process Error\n");
    else if( pid == 0){
        printf("I'm child, %d\n", getpid());
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        execlp("cat","cat",NULL);
        exit(0);
    }
    else{
        printf("I'm parent, %d\n", getpid());
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        printf("Can you hear me?");
        fflush(stdout);
    }   
}