#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main(){
    int pipe_fd[2];
    if(pipe(pipe_fd) < 0) printf("Pipe Create Error\n");
    pid_t pid = fork();
    ssize_t num;
    char buff[32];
    if(pid<0) printf("Fork Process Error\n");
    else if(pid == 0){
        printf("I'm a child, %d\n", getpid());
        close(pipe_fd[1]); // close write
        read(pipe_fd[0], buff, 32);
        close(pipe_fd[0]); // close read after use
        printf("%s", buff);
        exit(0);
    }
    else{
        printf("I'm a parent, %d\n", getpid());
        close(pipe_fd[0]); // close read
        write(pipe_fd[1], "This is a message from parent.\n", 32);
        close(pipe_fd[1]); // close write after use
        exit(0);
    }
}