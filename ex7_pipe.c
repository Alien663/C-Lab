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
    int j;
    
    /*
    *  fd1 : for parent to send message to child
    *  fd2 : for child to send ack to parent
    *  children : parent to record children
    *  parent_pipe : [0] to read from parent;  [1] to write to parent
    *  generation : record the generation(can judge is root)
    */
    int fd1[2], fd2[2], children[10][2], parent_pipe[2];
    int generation=0, children_counts=0;

    t_begin = argv[1];
    t_end   = t_begin + strlen(t_begin) - 1;
    labels  = argv[2];

newborn:
    children_counts = 0;
    label = *labels;  labels += 1;

    t_begin = t_begin + 1;
    while(t_begin < t_end ) {
        j = get_treesize(t_begin);
        pipe(fd1);
        pipe(fd2);
        if( fork() == 0 ){
            t_end  = t_begin + j - 1;
            generation++;
            parent_pipe[0] = fd1[0];
            parent_pipe[1] = fd2[1];
            close(fd1[1]);
            close(fd2[0]);
            goto newborn;
        }
        else {
            children[children_counts][1] = fd1[1];
            children[children_counts][0] = fd2[0];
            close(fd1[0]); 
            close(fd2[1]);
            children_counts++;
            t_begin += j;
            labels += j/2;
        }
    }

    pid_t pid = getpid(), ppid = getppid();
    fprintf(stderr, "I'm %c, the %d generation, my pid=%ld, and my ppid=%ld\n", label, generation, (long)pid, (long)ppid);

    char ch[80], ack[80];
    while(1){
        if(generation == 0) {
            ssize_t n = read(STDIN_FILENO, ch, sizeof(ch) - 1);
            if(n <= 0) break;
            ch[n] = '\0';
        } else {
            read(parent_pipe[0], ch, sizeof(ch));
        }

        if(ch[0] == label) strcpy(ch, "POST\0");

        for(int i=0;i<children_counts;i++){
            write(children[i][1], ch, sizeof(ch));
            read(children[i][0], ack, sizeof(ack));
        }

        if(strcmp(ch, "POST") == 0){
            fprintf(stderr, "I'm %c, the %d generation, my pid=%ld, and my ppid=%ld\n", label, generation, (long)pid, (long)ppid);
        }

        if(generation > 0) write(parent_pipe[1], "ack", 3);

        if(ch[0] == 'q'){
            close(parent_pipe[0]);
            close(parent_pipe[1]);
            for(int i=0;i<children_counts;i++){
                close(children[i][0]);
                close(children[i][1]);
                wait(NULL);
            }
            break;
        }
    }
}