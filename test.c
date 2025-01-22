#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int get_treesize(char *s) {
    int i, level;

    for(level=0, i=0; s[i]; i++){
        if(s[i] == 'd') level += 1;
        if(s[i] == 'u') level -= 1;
        if(level == 0) return i+1;
    }

    return i;
}

void main(int argc, char *argv[]){
    char label, *labels, *t_begin, *t_end;
    int  j;
    pid_t cid;
    int generation = 0;
    char ch[80];
    int fd[2];
    int children[10];
    int children_counts = 0;
    int parent_pipe[1];
    t_begin = argv[1]; 
    t_end   = t_begin + strlen(t_begin) - 1;
    labels  = argv[2];

newborn:
    label = *labels;  labels += 1;

    t_begin = t_begin + 1;
    while(t_begin < t_end ) {
        j = get_treesize(t_begin);
        pipe(fd);
        cid = fork();
        if( cid == 0 ){
            close(fd[1]); // child close write site to parent
            parent_pipe[0] = fd[0]; // child record parent's read site of pipe

            generation++;
            t_end  = t_begin + j - 1;
            goto newborn;
        } else {
            close(fd[0]); // parent close read site of child
            children[children_counts++] = fd[1]; // parent record child's write side

            // wait(NULL);
            t_begin += j;
            labels += j/2;
        }
    }
  
    fprintf(stderr, "I'm %c, my pid=%d, and my ppid=%d\n", label, getpid(), getppid());
    while(1){
        if(generation == 0) fgets(ch, 80, stdin); // root read from stdin()
        else read(parent_pipe[0], ch, sizeof(ch)); // node and leaf read from parent

        for(int i=0;i<children_counts;i++){
            write(children[i], ch, sizeof(ch)); // write to all children
        }

        if(ch[0] == 'q'){ // if ch == 'q' then break from while loop
            close(parent_pipe[0]);
            for(int i=0;i<children_counts;i++){
                close(children[i]);
                wait(NULL);
            }
            break;
        }
    }
}