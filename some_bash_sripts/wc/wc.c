#include <string.h>
#include <errno.h> 
#include <fcntl.h> 
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
void wc_f(int f, int total[]){
    int sym, word, line, c, state;
    sym=word=line=0;
    state=1;
    while ((read(f,&c,sizeof(char)))!= 0){
        sym+=1;
        if (c=='\n') line+=1;
        if (c==' '|| c=='\n' || c=='\t') state=1;
        else if (state==1){
            state=0;
            word+=1;
        }
        }
    printf("%5d %5d %5d ", line, word, sym);
    total[0]+=line;
    total[1]+=word;
    total[2]+=sym;
}

int wc(int argc, char* argv[]){
    int c, i;
    int f;
    int total[3]={0, 0, 0};

    if (argc==1){
        wc_f(1, total);
        printf("\n");
        return 0;}
    
    for (i=1; i<argc; ++i){
        f=open(argv[i], O_RDONLY);
        if (f==-1){
            printf("Not access");
            return 1;
        }
        wc_f(f, total);
        printf("%s\n", argv[i]);
    }    

    if (argc>2){
        printf("%5d %5d %5d ", total[0], total[1], total[2]);
        printf("total\n");
    }

    

}


