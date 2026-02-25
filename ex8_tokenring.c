#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>

int main(int argc,  char *argv[ ]) {
   pid_t root_pid = getpid();
   pid_t childpid;
   int cw[2], acw[2];
   int cw_in, cw_out, acw_in, acw_out;
   int i;
   int nprocs;
   
   /* check command line for a valid number of processes to generate */
   if ( (argc != 2) || ((nprocs = atoi (argv[1])) <= 0) ) {
      fprintf (stderr, "Usage: %s nprocs\n", argv[0]);
      return 1; 
   }
   
   if (pipe(cw) == -1 || pipe(acw) == -1) {
      perror("Failed to create starting pipe");
      return 1;
   }

   cw_in = cw[0];
   cw_out = cw[1];
   acw_in = acw[0];
   acw_out = acw[1];

   /* create the remaining processes */
   for (i = 1; i < nprocs;  i++) {
      if (pipe(cw) == -1 || pipe(acw) == -1) {
         fprintf(stderr, "[%ld]:failed to create pipe %d: %s\n", (long)getpid(), i, strerror(errno));
         return 1; 
      } 
      if ((childpid = fork()) == -1) {
         fprintf(stderr, "[%ld]:failed to create child %d: %s\n", (long)getpid(), i, strerror(errno));
         return 1; 
      }
      if (childpid > 0){
         cw_out = cw[1];
         acw_in = acw[0];
         close(cw[0]);
         close(acw[1]);
      }
      else{
         cw_in = cw[0];
         acw_out = acw[1];
         close(cw[1]);
         close(acw[0]);
      }
      if (childpid)
         break;
   }

   fd_set readfds;
   int maxfd = (STDIN_FILENO > cw_in ? STDIN_FILENO : cw_in);
   maxfd = (maxfd > acw_in ? maxfd : acw_in) + 1;
   int running = 1;
   int is_root = (root_pid == getpid());
   while(running){
      FD_ZERO(&readfds);
      if(is_root) FD_SET(STDIN_FILENO, &readfds);
      FD_SET(cw_in, &readfds);
      FD_SET(acw_in, &readfds);

      int ret = select(maxfd, &readfds, NULL, NULL, NULL);
      if (ret < 0) {
         perror("select");
         break;
      }

      if( is_root && FD_ISSET(STDIN_FILENO, &readfds)) {
         char ch;
         int token = 1;
         if(read(STDIN_FILENO, &ch, 1) > 0) {
            if(ch == 'q') {
               token = -1; // 結束信號
               write(acw_out, &token, sizeof(token));
            }
            else if (ch == 'p')
            {
               token = 1; // token
               write(acw_out, &token, sizeof(token));
            }
            else if (ch == 'f'){
               int a=0, b=1;      
               write(cw_out, &a, sizeof(a));
               write(cw_out, &b, sizeof(b));
            }
         }
      }

      if (FD_ISSET(cw_in, &readfds)) {
         // CW pipe 有資料
         int a, b;
         if (read(cw_in, &a, sizeof(a)) > 0 && read(cw_in, &b, sizeof(b)) > 0) {
            printf("I am P%d %ld, got : %d %d\n", i, (long)getpid(), a, b);
            // 計算下一步
            if(!is_root){
               int sum = a + b;
               write(cw_out, &b, sizeof(b));
               write(cw_out, &sum, sizeof(sum));
            }
         }
      }

      if (FD_ISSET(acw_in, &readfds)) {
         // ACW pipe 有 token
         int token;
         if (read(acw_in, &token, sizeof(token)) > 0) {
            if(token == -1){ // when receive termination signal
               if(!is_root)
                  write(acw_out, &token, sizeof(token)); // 傳遞結束信號
               running = 0;
            }
            else{
               if(is_root)
                  printf("I'm P%d %ld, the root\n", i, (long)getpid());
               else{
                  printf("I'm P%d %ld\n", i, (long)getpid());
                  write(acw_out, &token, sizeof(token)); // 繼續傳
               }
            }
         }
      }
   }

   if ((close(cw_in) == -1) || (close(cw_out) == -1) || (close(acw_in) == -1) || (close(acw_out) == -1)) {
      perror("Failed to close final descriptors");
      return 1; 
   }   

   return 0; 
}     