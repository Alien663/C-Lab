#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int get_treesize(char *s)
{
  int i, level;

  for(level=0, i=0; s[i]; i++){
    if(s[i] == 'd') level += 1;
    if(s[i] == 'u') level -= 1;
    if(level == 0) return i+1;
  }

  return i;
}

int main(int argc, char *argv[]) {
    char label, *labels, *t_begin, *t_end;
    int  j;
    pid_t pid;
    t_begin = argv[1]; 
    t_end   = t_begin + strlen(t_begin) - 1;
    labels  = argv[2];

newborn:
    label = *labels;  labels += 1;

    t_begin = t_begin + 1;
    while(t_begin < t_end ) {
        j = get_treesize(t_begin);
        pid = fork();
        if( pid == 0 ){
            t_end  = t_begin + j - 1;
            goto newborn;
        }
        else {
            t_begin += j;
            labels  += j/2;
        }
    }
    fprintf(stderr, "I'm %c, my pid=%d, and my ppid=%d\n", label, getpid(), getppid());

    int sum = 0;
    if(pid == 0){
        while(1) pause();
    }
    else{
        while(1){
            int status, temp_sig=0;
            pid_t my_children = wait(&status);
            if(my_children == -1) break;

            if(WIFEXITED(status)){ // node's life go to the end
                sum += WEXITSTATUS(status);
                printf("Child %d exits with value %d.\n", getpid(), sum);
            }
            else if(WIFSIGNALED(status)){ // leaf or node is killed with signal
                temp_sig = WTERMSIG(status);
                sum += temp_sig;
                printf("Child %d is terminated with signal : %d.\n", my_children, temp_sig);
            }
        }
    }
    return(sum);
}