#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <sys/types.h>
#include <sys/wait.h>

void pwd(int argc, char* argv[]){
    char *buf=malloc(sizeof(char)*400);
    getcwd(buf, 400);
    printf("%s\n", buf);
    free(buf);

}