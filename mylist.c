// My version of ls with standard list and recursive list with -r flag
// Execute: ./mylist [-r] dirname
// Recursive printing is very ugly, needs organization

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
void list(char* path);

int recurse; 
int main(int argc, char** argv) {
    //check for recursive option
    char dir[100];
    switch(argc) {
        case 1:
            strcpy(dir, "."); 
            recurse=0;
            break;
        case 2:
            if(strcmp(argv[1], "-r")==0){
                recurse=1;
                strcpy(dir, "."); 
            }
            else {
                recurse=0;
                strcpy(dir, argv[1]); 
            }
            break;
        case 3:
            recurse=1;
            strcpy(dir, argv[2]); 
            break;
    }
    list(dir);
    return 0;
}
 
void list(char* path){
    struct dirent* dp;
    DIR* dir = opendir(path); 
    struct stat s;
    while ((dp = readdir(dir)) != NULL) {
        char* newpath = malloc(strlen(path) + strlen(dp->d_name) + 2);
        sprintf(newpath,"%s/%s", path, dp->d_name);
        stat(newpath, &s);
        if(recurse && strcmp(dp->d_name,".") && strcmp(dp->d_name,"..") && S_ISDIR(s.st_mode)) {
            printf("%s:\n", newpath);
            list(newpath);
            printf("\n");
        }
        printf("%s ", dp->d_name);
        free(newpath);
    } 
    printf("\n");
}
