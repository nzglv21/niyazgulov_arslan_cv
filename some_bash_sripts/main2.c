#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include "echo/echo.h"
#include "pwd.h"
#include "ls/ls.h"
#include "cat/cat.h"
#include "cp/cp.h"
#include "wc/wc.h"



int main(int argc, char* argv[]){
    if (argc==1) return 0;
    if (strcmp(argv[1], "echo")==0){
        echo(argc-1, argv+1);
    }
    else if (strcmp(argv[1], "pwd")==0){
        pwd(argc-1, argv+1);}
    else if (strcmp(argv[1], "ls")==0){
        ls(argc-1, argv+1);
    }
    else if (strcmp(argv[1], "wc")==0){
        wc(argc-1, argv+1);
    }
    else if (strcmp(argv[1], "cp")==0){
        cp(argc-1, argv+1);
    }
    else if (strcmp(argv[1], "cat")==0){
        cat(argc-1, argv+1);
    }
    else{
        printf("Error command\n");
        return 1;
    }
}