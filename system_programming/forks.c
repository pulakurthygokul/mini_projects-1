#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

pid_t child;

void cleanup(int signal) {
  int status;
  waitpid(child, &status, 0);
  write(1,"cleanup!\n",9);
}
int main() {
   // Register signal handler BEFORE the child can finish
   int x=10;
   signal(SIGCHLD, cleanup); // or better - sigaction
   child = fork();
   if(child == -1 ) { exit(1);}

   if( child ==0 ) { /* I am the child!*/
     // Do background stuff e.g. call exec   
      printf("Child here, x is %d\n", x);
   } else { /* I'm the parent! */
      sleep(4); // so we can see the cleanup
      printf("Parent here, x is %d\n", x);
   }
   return 0;
} 
