#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include "list.h"
#include "tree.h"
#include "exec.h"

extern char** lst;
extern char* plex;
char ** argvs;

void inv() {
	printf("%s", "\x1b[32m"); /*здесь изменяется цвет на зеленый */
	char s[100]; /* ограничение: имя хоста и текущей директории не должно быть слишком длинным! */ gethostname(s, 100);
	printf("%s@%s", getenv("USER"), s);
	printf("%s", "\x1B[34m"); /* здесь изменяется цвет на серый */
	getcwd(s, 100);
	printf(":%s$ ", s);
    printf("%s", "\x1B[37m");
    fflush(stdout);
}
tree T;

void sigint_handler(int s){
    clear_tree(T);
    T=NULL;
    clearlist();
    printf("\n");
    inv();
}

void sigusr1_handler(int s){
    clear_tree(T);
    clearlist();
    printf("\n");
    exit(0);
}

void sigusr2_handler(int s){
    printf("\nbcgrnd pid= %d\n", wait(NULL));
    inv();
}


int main(int argc, char** argv){
    argvs=argv;
    signal(SIGINT, sigint_handler);
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    if (argc == 1){
        while(1){
            inv();
            make_list();
            if (lst==NULL){
                continue;}
            else if (strcmp("cd", lst[0]) == 0){
                cd(lst);
            }
            else if (strcmp(lst[0], "exit") == 0){
                clearlist();
                return 0;
            }
            else{
                T=build_tree();
                // print_tree(T, 1);
                start(T);
                clear_tree(T);
                T=NULL;
                clearlist();
            }
        }
    }    
    else{
        lst = argv+1;
        T=build_tree();
        start(T);
        clear_tree(T);
        T=NULL;
    }
}
