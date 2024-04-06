#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int read_count =0;
int cat(int argc, char* argv[]){
    int f, i, j, e_flag=0, c;
    int temp_e=0;
    int start=-1;

    for (i=1; i<argc&& argv[i][0]=='-';++i){
        temp_e=e_flag;
        for (j=1;argv[i][j]!='\0';++j)
            if (argv[i][j]=='n'){
                temp_e=1;
            }
            else{
                start=i;
                break;
            }
        
        if (start==-1)
            e_flag=temp_e;
        else
            break;
    }
    start=i;

    j=1;
    for (i=start; i<argc; ++i){
        f=open(argv[i], O_RDONLY);
        int state=0;
        if (e_flag) printf("%d ", j);
        while (read(f,&c,1)!=0){
            read_count+=1;
            if (state==0){
                printf("%6d  ",j);
                state=1;
            }
            printf("%c",c);
            if (c=='\n'&& e_flag){
                ++j;
                state=0;
            }
        }
    }
    printf("\n%d\n", read_count);
}