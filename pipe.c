//Piping a basic message back and forth etween a parent and child process
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int fh[2];
    pipe(fh);
    FILE* reader = fdopen(fh[0],"r");
    FILE* writer = fdopen(fh[1],"w");
    int i;
    int n=2;
    pid_t p[n];
    for(i=0;i<n;i++)
    {
        p[i] = fork();
        if(p[i]>0) 
        {
            int score;
            waitpid(p[i],NULL,0);
            fscanf(reader,"Score %d", &score);
            printf("The child says the score is %d\n", score);
        }
        else 
        {
            fprintf(writer,"Score %d",10+10);
            fprintf(writer,"%d\n",10);
            return 0;
        }
    }
    return 0;
}
