#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include <sys/types.h> 
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include<sys/wait.h>



#define PORTNUM 12931
#define BACKLOG 5
#define BUFLEN 80
#define MAXNAME 20


int received_signal = 0;
char *buf = NULL;
int sockfd;
int pid;        //son_pid

void sigkill_handler_father(int n){
    shutdown(sockfd, SHUT_RDWR);
    free(buf);
    close(sockfd);
    kill(pid, SIGKILL);
    exit(1);
}

void sigkill_handler_son(int n){
    send(sockfd, "\\quit  \0", 8, 0);
    shutdown(sockfd, SHUT_RDWR);
    free(buf);
    close(sockfd);
    kill(getppid(), SIGKILL);
    exit(1);
}



int get_name(char* buf){
    int i=0, c;

    for (;(c=getchar())!= '\n';){
        buf[i] = c;
        ++i;
        if (i>MAXNAME){
            while((c=getchar())!= '\n');
            printf("Too long nickname. Please, write another: ");
            return 0;
        }
    }
    if (i == 0)
        return 0;
    buf[i] = '\0';
    ++i;
    return i;
}

int get_new_message(char* buf){
    int i=0, c;

    for (;(c=getchar())!='\n';){
        buf[i] = c;
        ++i;
        if (i>BUFLEN-1){
            printf("Too long message.");
            return 0;
        }
    }

    buf[i] = '\0';
    ++i;
    return i;
}

int main(int argc, char* argv[]){
    struct sockaddr_in own_addr, party_addr;
    int newsockfd, filefd;
    int party_len;
    int len;
    int i;


    signal(SIGINT, sigkill_handler_father);

    /*Создаем сокет*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
        fprintf(stderr, "can`t create socket\n");
        return 0;

    }
    memset(&own_addr, 0, sizeof(own_addr));
    own_addr.sin_family = AF_INET;
    own_addr.sin_addr.s_addr = INADDR_ANY;
    own_addr.sin_port = htons(PORTNUM);

    //Подключение сокета
    if (connect(sockfd, (struct sockaddr *) &own_addr, sizeof(own_addr))<0){
        fprintf(stderr, "Error in connect\n");
        return 1;
    }

    buf = calloc(BUFLEN, sizeof(char));
    printf("Write your nickname: ");
    do{
        len = get_name(buf);
        if (len == 0) continue;
        send(sockfd, buf, len, 0);
        if ((i = recv(sockfd, buf, BUFLEN, 0)) == 0)
            kill(getpid(), SIGINT);

        if (strcmp(buf, "OK") != 0)
            printf("This nick is taken. Please write another: ");
        else{
            printf("You are connected.\n");
            break;
        }
    } while (1);


    if (pid=fork()){                    //слушающий процесс
        // shutdown(sockfd, SHUT_WR);
        
        
        while(1){
            if(recv(sockfd, buf, BUFLEN, 0) && !received_signal)
                printf("%s\n", buf);
            else{
                break;
            }
        }
        kill(pid, SIGINT);
        wait(NULL);
        free(buf);
        shutdown(sockfd, SHUT_RDWR);
        close(sockfd);
        return 0;

    }
    else{                           //пишущий процесс
        // shutdown(sockfd, SHUT_RD);
        int message_len;

        signal(SIGINT, sigkill_handler_son);

        message_len = get_new_message(buf);
        while(1){
            if(send(sockfd, buf, message_len, 0)==0){
                free(buf);
                return 0;
            }
            if (strstr(buf, "\\quit") == buf){
                free(buf);
                return 0;
            }
            message_len = get_new_message(buf);
        }
    }
}