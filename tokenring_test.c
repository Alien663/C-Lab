#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,  char *argv[ ]) {
    pid_t childpid;
    pid_t root_pid = getpid();
    int fd[2]; 
    int i;
    int nprocs;

    if ( (argc != 2) || ((nprocs = atoi (argv[1])) <= 0) ) {
        fprintf (stderr, "Usage: %s nprocs\n", argv[0]);
        return 1; 
    }  

    pipe(fd);

    
    dup2(fd[0], STDIN_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    for (i = 1; i < nprocs;  i++) {
        pipe(fd);
        childpid = fork();
        if(childpid) dup2(fd[1], STDOUT_FILENO);
        else dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        if(childpid) break;
    }

    char ack[80], order[80];
    while(1){
        //fprintf(stderr, "This is process %d with ID %ld and parent id %ld\n", i, (long)getpid(), (long)getppid());
        if(root_pid == getpid()){
           fprintf(stderr, "I'm root");
           fgets(order, 80, stdin);
           write(fd[1], "Hello", 5);
           fprintf(stderr, ", number : %d, %s\n", i, ack);
        }
        else{
            read(fd[0], order, sizeof(order));
            if(strcmp(order, "Hello") == 0){
                fprintf(stderr, "I'm process, number : %d, %s\n", i, order);
                write(fd[1], order, sizeof(order));
            }
        }
    }
    return 0;
}     