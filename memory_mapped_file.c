#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

pid_t child;

void cleanup(int signal) {
  int status;
  waitpid(child, &status, 0);
  fprintf(stdout,"zombies reaped!\n");
}
int fail(char*filename, int linenumber) { 
    fprintf(stderr, "%s:%d %s\n",filename,linenumber,strerror(errno) ); 
    exit(1);
    return 0; /*Make compiler happy */
}
#define QUIT fail(__FILE__, __LINE__ )

int main() {
    // We want a file big enough to hold 10 integers  
    int size = sizeof(int) * 10;

    int fd = open("data",O_RDWR| O_CREAT|O_TRUNC, 0600); //6 = read+write for me!

    lseek(fd, size, SEEK_SET);
    write(fd, "A",1);

    void* addr = mmap (0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("Mapped at %p\n", addr);
    if(!addr) QUIT;

    int* array = addr;
    array[0] = 0x12345678;
    array[1] = 0xdeadc0de;

    signal(SIGCHLD, cleanup); // or better - sigaction
    child = fork();
    if(child == -1 ) { exit(1);}

    if( child ==0 ) { /* I am the child!*/
        printf("child sees: "); 
        printf("%x and %x\n", array[0], array[1]);
        //change the shared memory
        array[0] = 0x99999999;
    } else { /* I'm the parent! */
        sleep(5); // so we can see the cleanup
        printf("parent sees: "); 
        printf("%x and %x\n", array[0], array[1]);
    }

    munmap(addr,size);
    return 0;
}
