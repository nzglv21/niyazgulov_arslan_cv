#include <string.h>
#include <errno.h> 
#include <fcntl.h> 
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>

int cp(int argc, char* argv[]){

    if (argc==1){
        fprintf(stderr,"cp: missing file operand\n");
        return 1;
    }
    if (argc == 2){
        fprintf(stderr, "cp: missing destination file operand after '%s'\n", argv[1]);
        return 1;
    }
    if (strcmp(argv[1], argv[2])==0){
        fprintf(stderr, "cp: '%s' and '%s' are the same file\n", argv[1], argv[2]);
        return 1;
    }
    int f1=open(argv[1], O_RDONLY);

    if (f1==-1){
        fprintf(stderr, "cp: cannot stat '%s': No such file or directory\n", 
                argv[1]);
        exit(1);}

    int f2=open(argv[2], O_WRONLY|O_TRUNC|O_CREAT, 0666);
    int cnt;
    char *buf=calloc(1024, sizeof(char));

    while ((cnt=read(f1, buf, 1024))!= 0)
        write(f2, buf, cnt);
    free(buf);
    close(f1);
    close(f2); 
}