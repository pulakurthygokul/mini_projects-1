/* 
A basic shell that runs within a shell.
Features:
* logging and searching
* runs standard shell executables using fork 
* timing of tasks with -t flag
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "log.h"

FILE* shell_args(int argc, char ** argv, int* tbit);
void dump_history();
void myfree(char* a, char* b);
void rm_zombie();
void cleanup();

int zombies=0; 
log_t Log;

int main(int argc, char ** argv) 
{
    signal(SIGINT, SIG_IGN);
    signal(SIGCHLD, rm_zombie);
    int t_bit=0;
    FILE* f=NULL;
    f = shell_args(argc, argv, &t_bit);

    //log initialization
    log_init(&Log);

    int repeat_flag=0;
    char* match=NULL; 
    char* line=NULL;
    char* copy=NULL;
    while(1)
    {
        char dir[100];
        size_t len = 0;
        ssize_t read = -1;
        if(!repeat_flag)
        {
            if(f==stdin)
                printf("(pid=%d)%s$ ", getpid(), getcwd(dir, sizeof(dir)));
            read = getline(&line, &len, f);
            if(read==-1 && f!=stdin)
            {
                free(line);
                line=NULL;
                fclose(f);
                f=stdin;
                continue;
            }
            //blank line case
            if(read==1 && line[0]=='\n')
            {
                free(line);
                line=NULL;
                continue;
            }
        }
        if(repeat_flag)
        {
            read=-1;
            line = match; 
            repeat_flag = 0;
        }
        //remove newline
        if(line[read-1]== '\n')
            line[read-1]=0;
        copy = malloc(strlen(line)+1);
        strcpy(copy, line);
        copy[strlen(line)]=0;

        char* space = " ";
        char *word = NULL;
        word = strtok(line, space);

        if(strcmp(word,"exit")==0)
        {
            cleanup();
            break;
        }
        if(strcmp(word,"cd")==0)
        {
            //get next word
            word = strtok(NULL, space);
            if(word==NULL)
                printf("xxx: No such file or directory\n");
            else
                if(chdir(word))
                    printf("xxx: No such file or directory\n");
            log_push(&Log, copy);
            myfree(copy, line);
            continue;
        }
        if(word[0]=='!')
        {
            if(strcmp(word,"!#")==0)
                dump_history();
            else
            {
                match = log_search(&Log, word+1);
                if(!match)
                    printf("No Match\n");
                else
                {
                    printf("%s matches %s\n", word+1, match);
                    repeat_flag=1;
                }
            }
            free(copy);
            copy=NULL;
        }
        //exec command
        else
        {
            struct timeval start;
            struct timeval end;
            gettimeofday(&start, 0);

            log_push(&Log, copy);

            char* args[10];
            int i;
            for(i=0;i<10;i++)
                args[i]=NULL;
             
            //get args 
            args[0] = word;
            for(i=1;word!=NULL;i++)
            {
                word=strtok(NULL,space);
                args[i]=word;
            }
            zombies+=1;
            if(strcmp(args[i-2],"&")==0)
            {
                int pid = fork();
                if(pid==0)
                    execvp(args[0], args);
                return 1;
            }
            int status;
            gettimeofday(&end, 0);
            int pid = fork();
            if(pid == 0)
            {
                execvp(args[0], args);
                printf("%s: not found\n", args[0]); 
                return 1;
            }
            else
            {
                printf("Command executed by pid=%d\n", pid);
                waitpid(pid, &status,0);
                long elapsed = (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
                if(t_bit)
                    printf("Execution took %lu microseconds.\n", elapsed) ;
            }
        }
        myfree(copy,line);
    }
    myfree(copy, line);
    log_destroy(&Log);
    return 0;
}

FILE* shell_args(int argc, char ** argv, int* tbit)
{
    FILE* f=stdin;
    if(argc>1)
    {
        //-h getlogin
        if(strcmp(argv[1],"-h")==0)
        {
            char *p = getlogin();
            if(p) 
                printf("Shell by %s\n",p);
            else
                printf("Can't get login!\n");
        }
        //-f run file
        if(strcmp(argv[1],"-f")==0)
        {
            if(argv[2])
                f=fopen(argv[2], "r");
            else
            {
                printf("Invalid file\n");
                f=stdin;
            }
        }
        //= print environ variables
        if(strcmp(argv[1],"=")==0)
        {
            extern char **environ;
            char **var;
            for(var=environ; *var!=NULL;++var)
                printf("%s\n",*var);
        }
        //-t time enabled
        if(strcmp(argv[1],"-t")==0)
            *tbit=1;
    }
    return f;
}

void dump_history()
{
    int i;
    for(i=1; i<=Log.size; i++)
        printf("%s\n", Log.data[i]);
}

void myfree(char* a, char* b)
{
    free(a); free(b);
    a=b=NULL;
}
void cleanup()
{
    int status=0;
    int i;
    for(i=0; i<zombies; i++)
        wait(&status);
}
void rm_zombie()
{
    zombies-=1;
}
