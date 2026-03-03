#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
   int nprocs;
   int i;
   int idx = 0;
   pid_t root_pid = getpid();

   if ((argc != 2) || ((nprocs = atoi(argv[1])) <= 0)) {
      fprintf(stderr, "Usage: %s nprocs\n", argv[0]);
      return 1;
   }

   int (*cw)[2] = malloc((size_t)nprocs * sizeof(*cw));
   int (*acw)[2] = malloc((size_t)nprocs * sizeof(*acw));
   if (cw == NULL || acw == NULL) {
      perror("Failed to allocate pipe arrays");
      free(cw);
      free(acw);
      return 1;
   }

   for (i = 0; i < nprocs; i++) {
      if (pipe(cw[i]) == -1 || pipe(acw[i]) == -1) {
         fprintf(stderr, "[%ld]: failed to create pipe %d: %s\n", (long)getpid(), i, strerror(errno));
         for (int j = 0; j <= i; j++) {
            close(cw[j][0]);
            close(cw[j][1]);
            close(acw[j][0]);
            close(acw[j][1]);
         }
         free(cw);
         free(acw);
         return 1;
      }
   }

   for (i = 1; i < nprocs; i++) {
      pid_t childpid = fork();
      if (childpid < 0) {
         fprintf(stderr, "[%ld]: failed to fork child %d: %s\n", (long)getpid(), i, strerror(errno));
         for (int j = 0; j < nprocs; j++) {
            close(cw[j][0]);
            close(cw[j][1]);
            close(acw[j][0]);
            close(acw[j][1]);
         }
         free(cw);
         free(acw);
         return 1;
      }
      if (childpid == 0) {
         idx = i;
         break;
      }
   }

   int prev = (idx - 1 + nprocs) % nprocs;
   int next = (idx + 1) % nprocs;

   int cw_in = cw[prev][0];
   int cw_out = cw[idx][1];
   int acw_in = acw[next][0];
   int acw_out = acw[idx][1];

   for (i = 0; i < nprocs; i++) {
      if (i != prev) {
         close(cw[i][0]);
      }
      if (i != idx) {
         close(cw[i][1]);
      }
      if (i != next) {
         close(acw[i][0]);
      }
      if (i != idx) {
         close(acw[i][1]);
      }
   }

   fd_set readfds;
   int maxfd = (cw_in > acw_in ? cw_in : acw_in);
   if (idx == 0 && STDIN_FILENO > maxfd) {
      maxfd = STDIN_FILENO;
   }
   maxfd += 1;

   int running = 1;
   while (running) {
      FD_ZERO(&readfds);
      if (idx == 0) {
         FD_SET(STDIN_FILENO, &readfds);
      }
      FD_SET(cw_in, &readfds);
      FD_SET(acw_in, &readfds);

      int ret = select(maxfd, &readfds, NULL, NULL, NULL);
      if (ret < 0) {
         perror("select");
         break;
      }

      if (idx == 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
         char ch;
         if (read(STDIN_FILENO, &ch, 1) > 0) {
            if (ch == 'q') {
               int token = -1;
               write(acw_out, &token, sizeof(token));
            } else if (ch == 'p') {
               int token = 1;
               write(acw_out, &token, sizeof(token));
            } else if (ch == 'f') {
               int a = 0;
               int b = 1;
               write(cw_out, &a, sizeof(a));
               write(cw_out, &b, sizeof(b));
            }
         }
      }

      if (FD_ISSET(cw_in, &readfds)) {
         int a;
         int b;
         if (read(cw_in, &a, sizeof(a)) > 0 && read(cw_in, &b, sizeof(b)) > 0) {
            printf("I am P%d %ld, got : %d %d\n", idx, (long)getpid(), a, b);
            if (idx != 0) {
               int sum = a + b;
               write(cw_out, &b, sizeof(b));
               write(cw_out, &sum, sizeof(sum));
            }
         }
      }

      if (FD_ISSET(acw_in, &readfds)) {
         int token;
         if (read(acw_in, &token, sizeof(token)) > 0) {
            if (token == -1) {
               if (idx != 0) {
                  write(acw_out, &token, sizeof(token));
               }
               running = 0;
            } else {
               if (idx == 0) {
                  printf("I'm P%d %ld, the root\n", idx, (long)getpid());
               } else {
                  printf("I'm P%d %ld\n", idx, (long)getpid());
                  write(acw_out, &token, sizeof(token));
               }
            }
         }
      }
   }

   close(cw_in);
   close(cw_out);
   close(acw_in);
   close(acw_out);

   if (idx == 0 && root_pid == getpid()) {
      while (wait(NULL) > 0) {
      }
   }

   free(cw);
   free(acw);
   return 0;
}