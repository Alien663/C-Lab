#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void main(int argc, char *argv[]) {
    if(argc > 1){
        int Flag = 1;
        for(int i=1;i<argc;i++){
            // get the input from argv
            int the_input = atoi(argv[i]);
            int j=0;
            pid_t cid;


            // start to make fork tree
            if(Flag){
                Flag = 0;
                for(;j<the_input;j++){
                    cid = fork();
                    if(cid == 0) break;
                    else {
                        i = argc;// let parent don't fork grant child
                        wait(NULL);
                    }
                }
            }
            if(j==0) Flag = 1; // setting the left leaf flag
        }
    }
    while(wait(NULL)!=-1);
    printf("I'm %d, my parent is %d\n", getpid(), getppid());
}