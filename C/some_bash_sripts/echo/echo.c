#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <sys/types.h>
#include <sys/wait.h>

void  echo(int argc, char *argv[]){
    int i, j, it_c;
    int e_flag, n_flag, E_flag;
    int e_temp, n_temp, its_word;
    typedef enum {Start, backslash} vertex;
    e_flag = n_flag = 0;
    its_word = -1;
    for (i=1; (i<argc) && (argv[i][0] == '-');++i){
        e_temp = e_flag;
        n_temp = n_flag;
        for (int j=1;(argv[i][j]!='\0') && (its_word== -1);++j){
            char c = argv[i][j];
            switch (c){
                case 'e': e_temp = 1; break;
                case 'n': n_temp = 1; break;
                case 'E': e_temp = 0; break;
                default:
                    its_word = i;
            }
        }
        if (its_word == -1){
            e_flag = e_temp;
            n_flag = n_temp;
        }
        else break;
    }
    its_word = i;
    if ((e_flag==0)){
        for (i=its_word; (i<argc);++i)
            printf(i==argc-1? "%s":"%s ", argv[i]);
    }
    else{

        vertex v=Start;
        for (i=its_word; (i<argc);++i){
            for (j=0;argv[i][j]!='\0';++j){
                switch (v){
                    case Start:
                        if (argv[i][j]!='\\') printf("%c", argv[i][j]);
                        else v = backslash;
                        break;
                                   
                    case backslash:
                        v = Start;
                        switch (argv[i][j])
                        {
                        case 'a': printf("\a");
                            break;
                        case 'b': printf("\b");
                            break;
                        case 'e': printf("\e");
                            break;
                        case 'f': printf("\f");
                            break;
                        case 'n': printf("\n");
                            break;
                        case 'r': printf("\r");
                            break;
                        case 't': printf("\t");
                            break;
                        case 'v': printf("\v");
                            break;
                        case '\\': 
                            printf("\\");
                            break;   
                        case 'c':
                            it_c = 1;
                            n_flag = 1;
                            break;
                        default:
                            printf("\\%c", argv[i][j]);
                            break;
                        }
                }  
                if (it_c==1) break;   
            }
            if (it_c==1) break;
        }
    }
    if (n_flag == 0) printf("\n");
}