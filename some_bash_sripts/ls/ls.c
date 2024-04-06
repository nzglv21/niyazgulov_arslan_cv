#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>





void write_file_names(char* path){
    DIR* dir= opendir(path);
    if (dir == NULL){
        printf("ls: cannot access \'%s\': No such file ot directory", path);
        exit(1);
    }

    struct dirent* cur_file;

    while ((cur_file=readdir(dir))!=NULL){
        if ((cur_file->d_name)[0] != '.')
            printf("%s ", cur_file->d_name);
    }
    closedir(dir);
    printf("\n");
}






int is_dir(char* path){
    struct stat fileInfo;

    if (stat(path, &fileInfo)==0){
        if (fileInfo.st_mode & __S_IFDIR){
            return 1;
        }
        else
            return 0;
    }
    else
        fprintf(stderr, "ls: cannot access \'%s\': No such file ot directory\n", path);
        exit(1);
}

void modification_time(struct tm *mod_time){
            printf("%04d-%02d-%02d %02d:%02d:%02d",
               mod_time->tm_year + 1900,
               mod_time->tm_mon + 1,
               mod_time->tm_mday,
               mod_time->tm_hour,
               mod_time->tm_min,
               mod_time->tm_sec);
}

void just_ls(char* argv[], int start_path, int argc){

    int i=start_path;
    if (start_path==argc){
        write_file_names(".");
        return;}
    
    if (start_path+1 == argc){
        if (is_dir(argv[start_path]))
            write_file_names(argv[start_path]);
        else
            printf("%s\n", argv[start_path]);
        return;}

    for (i=start_path; i<argc; ++i){
        if (is_dir(argv[i])){
            printf("%s:\n", argv[i]);
            write_file_names(argv[i]);}
        else
            printf("%s\n", argv[i]);
        printf(i+1!=argc?"\n":"");}

}


void  access_rights(struct stat* p){
    printf(((*p).st_mode & __S_IFDIR) ? "d" : "-");
    printf(((*p).st_mode & S_IRUSR) ? "r" : "-");
    printf(((*p).st_mode & S_IWUSR) ? "w" : "-");
    printf(((*p).st_mode & S_IXUSR) ? "x" : "-");
    printf(((*p).st_mode & S_IRGRP) ? "r" : "-");
    printf(((*p).st_mode & S_IWGRP) ? "w" : "-");
    printf(((*p).st_mode & S_IXGRP) ? "x" : "-");
    printf(((*p).st_mode & S_IROTH) ? "r" : "-");
    printf(((*p).st_mode & S_IWOTH) ? "w" : "-");
    printf(((*p).st_mode & S_IXOTH) ? "x" : "-");
}

void l_file_info(char* full_path, char* file_name, int g){
    struct stat fileInfo;
    char time_str[20];
    stat(full_path, &fileInfo);
    struct passwd *pwd = getpwuid(fileInfo.st_uid);
    struct group *grp = getgrgid(fileInfo.st_gid);
    struct tm *mod_t= localtime(&fileInfo.st_mtime);
    access_rights(&fileInfo);
    
    
    printf(" %lu", fileInfo.st_nlink);
    printf(" %s", pwd->pw_name);
    if (g==0) printf(" %s", grp->gr_name);
    printf(" %5.5f", ((float)fileInfo.st_size)/1024);
    strftime(time_str, sizeof(time_str), "%b %d %H:%M", mod_t);
    printf(" %s", time_str);
    
    printf(" %s\n", file_name);

}


void ls_l_g(char *path, int g){
    int i;
    DIR* dir=opendir(path);
    struct dirent* cur_file;
    struct stat dir_stat;
    int status = stat(path, &dir_stat);
    if (dir == NULL && status==-1){
        fprintf(stderr,"ls: cannot access \'%s\': No such file ot directory\n", path);
        exit(1);
    }

    if (!(dir_stat.st_mode & __S_IFDIR)){
        l_file_info(path, path, g);
        return;
    }
    printf("%s:\n", path);
    while ((cur_file=readdir(dir))!= NULL){
        if (cur_file->d_name[0] != '.'){
            char *full_path;
            full_path = (char *)calloc(1,strlen(path) + strlen(cur_file->d_name) + 2); // +2 for '/' and '\0'
            sprintf(full_path, "%s/%s", path, cur_file->d_name);
            l_file_info(full_path, cur_file->d_name, g);
            free(full_path);
        }
    }
    closedir(dir);
}



void ls_R(char *path, int l, int g){
    DIR* dir=opendir(path);
    
    if (l+g==0){
        printf("%s:\n", path);
        write_file_names(path);
    }
    else{
        ls_l_g(path, g);
    }
    
    if (dir==NULL){
        fprintf(stderr, "ls: cannot access \'%s\': No such file ot directory\n", path);
        exit(1);
    }
    
    struct dirent* cur_file;
    struct stat cur_stat;
    while ((cur_file=readdir(dir))!=NULL){
        char *full_path;
        full_path = (char *)malloc(strlen(path) + strlen(cur_file->d_name) + 2); // +2 for '/' and '\0'
        sprintf(full_path, *(path+strlen(path)-1)!='/'? "%s/%s":"%s%s", path, cur_file->d_name);
        stat(full_path, &cur_stat);
        if (cur_stat.st_mode & __S_IFDIR && cur_file->d_name[0]!='.'){
            printf("\n");
            ls_R(full_path, l, g);
        }
        free(full_path);

    }
    closedir(dir);
    
}

int ls(int argc, char* argv[]){
    int i, j, it_c;
    int R_flag, l_flag, g_flag;
    int R_temp, l_temp, g_temp, its_word;
    R_flag = l_flag = g_flag= 0;
    its_word = -1;
    for (i=1; (i<argc) && (argv[i][0] == '-');++i){    /*flags detecting*/
        R_temp = R_flag;
        l_temp = l_flag;    
        g_temp = g_flag;
        for (int j=1;(argv[i][j]!='\0') && (its_word== -1);++j){
            char c = argv[i][j];
            switch (c){
                case 'R': R_temp = 1; break;
                case 'l': l_temp = 1; break;
                case 'g': g_temp = 1; break;
                default:
                    its_word = i;
            }
        }
        if (its_word == -1){
            R_flag = R_temp;
            l_flag = l_temp;
            g_flag =g_temp;
        }
        else break;
    }
    its_word = i;
    if (R_flag+g_flag+l_flag == 0){
        just_ls(argv, its_word, argc);
        return 0;}
    else if (R_flag == 0){
        if (its_word == argc)
            ls_l_g(".", g_flag);
        else
            for (i=its_word; i<argc; ++i) 
                ls_l_g(argv[i], g_flag);         
        }
    else{
        if (its_word==argc)
            ls_R(".", l_flag, g_flag);
        else
            for (i=its_word; i<argc; ++i){
                ls_R(argv[i], l_flag, g_flag);
                if (i+1<argc) printf("\n");
            }
    }
}