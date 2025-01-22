#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MaxChildren 13

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

main(int argc, char *argv[])
{
  char label, *labels, *t_begin, *t_end;
  int  num_children;
  int  subtree_size;
  int  rootpid, mypid;
  int  to_children[MaxChildren];
  int  fd[2];
  char buf[80];
  int  i, count;

  rootpid = getpid();

  t_begin = argv[1]; 
  t_end   = t_begin + strlen(t_begin) - 1;
  labels  = argv[2];

newborn:
  num_children = 0;
  label = *labels;  labels += 1;

  t_begin = t_begin + 1;
  while(t_begin < t_end ) {
    num_children++;
    subtree_size = get_treesize(t_begin);

    pipe(fd);
    if( fork() == 0 ){
      t_end  = t_begin + subtree_size - 1;

      dup2(fd[0], STDIN_FILENO);  close(fd[0]);  close(fd[1]);
      goto newborn;
    } else {
      t_begin += subtree_size;
      labels  += subtree_size/2;

      to_children[num_children-1] = fd[1];   close(fd[0]);
    }
  }
  
  fprintf(stderr, "I'm %c, my pid=%ld, and my ppid=%ld\n",
                  label, getpid(), getppid());

  /* The tree is created. */

  mypid = getpid();
  count = 1;
  while(1) {
    if( mypid == rootpid ) fgets(buf, 80, stdin);
    else                   read(STDIN_FILENO, buf, 1);

    if( buf[0] == 'q' ) {
      for(i=0; i<num_children; i++) {
        write( to_children[i], &buf, 1);
        wait(NULL);
      }
      exit(0);
    }

    if( buf[0] == label )
      fprintf(stderr, "hello, i'm %c, my pid is %d, counted %d times\n",
                      label, mypid, count++);
    else
      for(i=0; i<num_children; i++) 
        write( to_children[i], &buf, 1);
  }
}