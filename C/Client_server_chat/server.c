#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <signal.h>



#define PORTNUM 12931
#define BACKLOG 5
#define BUFLEN 80
#define MAX_CLIENTS 5
#define NAMELEN 20

char *buf, *client_names[MAX_CLIENTS];
int *users_priv_mes[MAX_CLIENTS];
int client_sockets[MAX_CLIENTS]; /**/


void sigint_handler(int n){
    free(buf);
    for(int i=0; i<MAX_CLIENTS; ++i){
        free(client_names[i]);
        free(users_priv_mes[i]);
        shutdown(client_sockets[i], SHUT_RDWR);
        close(client_sockets[i]);
    }
    exit(0);
}

int send_users_private(int n, char* user_names[], int user_names_len[], int user_sockets[], int* users_priv_mes[]){
    if (users_priv_mes[n] == NULL){
        send(client_sockets[n], "You didn`t send private messages.", 34, 0);
        return 0;
    }

    int mes_len = 0;
    int i = 0;

    for(;i<MAX_CLIENTS;++i){
        if ((users_priv_mes[n][i] !=0) && (n!=i)){
            mes_len += user_names_len[i] + 1;
        }
    }

    if (mes_len == 0){
        send(client_sockets[n], "You didn`t send private messages.", 34, 0);
        return 0; 
    }
    char *message = calloc(mes_len+5, sizeof(char));
    for (i=0;i<MAX_CLIENTS;++i){
        if (users_priv_mes[n][i] != 0){
            strcat(message, user_names[i]);
            strcat(message, ", ");
            }
    }
    message[mes_len-2] = '\0';
    send(user_sockets[n], message, mes_len, 0);
    free(message);
}


int sendusers(int n, int cs, char* users_names[], int users_names_len[]) {
    int len = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if ((users_names_len[i] != 0) && (i != n))
            len += users_names_len[i] + 1;
    }

    char *message = calloc(len+5, sizeof(char));
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (users_names[i] != NULL && (i != n)) {
            strcat(message, users_names[i]);
            strcat(message, ", ");
        }
    }
    
    if (len > 0) {
        message[len-2] = '\0';  // Устанавливаем нулевой символ перед последней запятой
    }
    send(cs, message, len, 0);
    free(message);
}


int send2chanel(char* message, int message_len, char* user_name, int name_len, int sockets[], int n){
    int fin_message_len = message_len +name_len+3;
    char* fin_message = calloc(fin_message_len, sizeof(char));
    int i=0;
    int j=0;
    fin_message[i] = '<';
    for (i=1; i<name_len; ++i){
        fin_message[i] = user_name[i-1];
    }
    fin_message[i] = '>';
    ++i;
    fin_message[i] = ' ';
    ++i;
    for (;j<message_len;++j){
        fin_message[i] =  message[j];
        ++i;
    }
    for (i=0; i<MAX_CLIENTS;++i){
        if (n!=i)
            send(sockets[i], fin_message, fin_message_len, 0);
    }
    free(fin_message);
    return fin_message_len;
}

int new_user_in(char* name, int name_len, int client_sockets[], int n){
    int i=0;
    int message_len = name_len + 22;
    char* message = calloc(message_len, sizeof(char));
    strcpy(message, name);
    strcat(message, " come in.");
    
    for (;i<MAX_CLIENTS; ++i){
        if (i != n){
            send(client_sockets[i], message, message_len, 0);
        }
    }
    free(message);
}

void user_exit(int n, char* message, int user_sockets[], char* name){
    char last_message[256];
    snprintf(last_message, sizeof(last_message), "%s left. Last message: %s",name, message+6);
    int len = strlen(last_message);
    last_message[len - 1] = '\0';
    for (int i=0; i<MAX_CLIENTS; ++i){
        if ((i!=n) && (user_sockets[i] != 0))
            send(user_sockets[i], last_message, len, 0);
    }

}

int private_message(int from, char *message, char* user_names[], int user_sockets[], int *users_priv_mes[]){
    message = message + 9;
    int i=0;
    char * split = strchr(message, ' '); //находим пробел между nickname и Message
    if (split != NULL){
        char * nick_name = message;
        *split = '\0';
        char * pr_mes = split + 1;

        for (i=0; i<MAX_CLIENTS; ++i){
            if (user_names[i] != NULL)
                if (strcmp(nick_name, user_names[i]) ==0)
                    break;
        }
        if (i == MAX_CLIENTS){
            send(user_sockets[from], "Error. No such name.", 22, 0);
            return from;
        }
        char private_message[256];
        snprintf(private_message, 256, "Private message from <%s>: %s", user_names[from], pr_mes);
        private_message[strlen(private_message)] = '\0';
        send(user_sockets[i], private_message, strlen(private_message), 0);
        return i;

    }
    else{
        send(user_sockets[from], "Error. You forgot message.", 28, 0);
        return from;
    }
}

int is_free_name(char* name, char* names[]){
    for (int i=0; i<MAX_CLIENTS; ++i){
        printf("%d", (int)names[i]);
        if (names[i] != NULL){
            if (strcmp(names[i], name) == 0){
                return 0;
            }
        }
    }
    return 1;
}

int help(int n){
    char message[33] = "\\users\n\\quit <message>\n\\privates";
    send(n, message, 33, 0);
}

//Удаляет пользоавтеля из массива приватных сообщений.
int zero_private(int n, int* private_message[]){ 
    for(int i=0; i<MAX_CLIENTS; ++i){
        if (private_message[i] != NULL ){
            if (i!=n)
                private_message[i][n] = 0;
        }
    }
}

int main(int argc, char* argv[]){
    struct sockaddr_in own_addr, client_addr;
    int server_fd, newserver_fd, filefd;
    int party_len;
    int len;
    int i, j;
    int old_client_sockets[MAX_CLIENTS];
    fd_set readfds; /*файловые дискриторы*/
    int max_cs;
    int cs;
    int activity;
    int client_names_len[MAX_CLIENTS];
    int clients_count = 0;

    socklen_t addrlen = MAX_CLIENTS;
    buf = calloc(BUFLEN, sizeof(char));

    signal(SIGINT, sigint_handler);
    /*Создаем сокет*/
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))<0){
        fprintf(stderr, "can`t create socket\n");
        return 0;

    }

    memset(&own_addr, 0, sizeof(own_addr));
    own_addr.sin_family = AF_INET;
    own_addr.sin_addr.s_addr = INADDR_ANY;
    own_addr.sin_port = htons(PORTNUM);
    printf("server is started\n");

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));



    if (bind(server_fd, (struct sockaddr *) &own_addr, sizeof(own_addr))<0){
        fprintf(stderr, "can`t bind soket\n");
        return 0;
    }
    /*обработка запросов на соединение*/
    if (listen(server_fd, BACKLOG)<0){
        fprintf(stderr, "can`t listen socket\n");
        return 0;
    }

    //инициализация массивов
    for (int i=0; i!=MAX_CLIENTS;++i){ 
        old_client_sockets[i] = 0;
        client_sockets[i] = 0;
        client_names[i] = NULL;
        client_names_len[i] = 0;
        users_priv_mes[i] = NULL;
    }

    while (1){
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_cs = server_fd;

        for (i=0; i<MAX_CLIENTS;i++){
            cs = client_sockets[i];

            if (cs>0){
                FD_SET(cs, &readfds);
            }

            if (cs>max_cs)
                max_cs = cs;
        }


        activity = select(max_cs+1, &readfds, NULL, NULL, NULL);
        if ((activity<0))
            perror("select error");

        if (FD_ISSET(server_fd, &readfds)){
            int new_socket;
            if ((new_socket=accept(server_fd, (struct sockaddr *)&client_addr, &addrlen))<0)
                fprintf(stderr, "error in accept");
            
            if (clients_count == MAX_CLIENTS){
                shutdown(new_socket, SHUT_RDWR);
                close(new_socket);
                printf("There are max clients. Can`t add new client.");
                continue;
            }
            else{
                printf("New connection\n");
            }



            for (i=0;i<MAX_CLIENTS; ++i){
                if (client_sockets[i] == 0){
                    client_sockets[i] = new_socket;
                    break;
                }
            }

        }

        for (i=0; i<MAX_CLIENTS; ++i){
            cs = client_sockets[i];
            if (FD_ISSET(cs, &readfds)){
                if ((len=recv(cs, buf, BUFLEN, 0))){ 
                    if (old_client_sockets[i]==0){
                        if (is_free_name(buf, client_names)){
                            ++clients_count;
                            send(cs, "OK", 3, 0);
                            fprintf(stderr, "new_user: %s\n", buf);
                            client_names[i] = calloc(len, sizeof(char));
                            client_names_len[i] = len;
                            strcpy(client_names[i], buf);
                            old_client_sockets[i] = cs;
                            new_user_in(buf, len, client_sockets, i);
                        }
                        else{
                            send(cs, "NO", 3, 0);
                        }

                    }
                    else{ 
                        if (strstr(buf, "\\quit")==buf){
                            --clients_count;
                            fprintf(stderr, "\\quit\n");
                            user_exit(i, buf, client_sockets, client_names[i]);
                            shutdown(cs, SHUT_RDWR);
                            close(cs);
                            client_sockets[i] = 0;
                            old_client_sockets[i] = 0;
                            free(client_names[i]);
                            free(users_priv_mes[i]);
                            zero_private(i, users_priv_mes);
                            users_priv_mes[i] = NULL;
                            client_names[i] = NULL;
                            client_names_len[i] = 0;
                            continue;
                        }
                        if (strcmp(buf, "\\users")==0){
                            sendusers(i, cs, client_names, client_names_len);
                            continue;
                        }
                        if (strcmp(buf, "\\privates") == 0){
                            send_users_private(i, client_names, client_names_len, client_sockets, users_priv_mes);
                            continue;
                        }
                        if (strstr(buf, "\\private") == buf){
                            j = private_message(i, buf, client_names, client_sockets, users_priv_mes);
                            if (j != i){
                                if (users_priv_mes[i] == NULL)
                                    users_priv_mes[i] = calloc(MAX_CLIENTS, sizeof(int));
                                users_priv_mes[i][j] = 1;
                            }
                            continue;
                        }
                        if (strcmp(buf, "\\help")==0){
                            help(cs);
                        }
                        else{
                            send2chanel(buf, len, client_names[i], client_names_len[i], client_sockets, i);
                            }
                    }
                }
            }
        }
    }    
}