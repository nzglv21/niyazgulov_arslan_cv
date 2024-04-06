#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 50500
#define QUEUE 10
#define BUF_SIZE 1024
#define USERNAME_LENGTH 256

struct User {
    char username[USERNAME_LENGTH];
    int socket;
} users[QUEUE];

/* Function to check if the username is valid */
bool isValidUsername(const char *username) {
    /* Check on correct syntax: latin letters and numbers only */
    for (long unsigned int i = 0; i < strlen(username) - 1; i++) 
        if (!(username[i] >= 'a' && username[i] <= 'z') 
            && !(username[i] >= 'A' && username[i] <= 'Z') 
            && !(username[i] >= '0' && username[i] <= '9'))
            return false;
    return true;
}

using namespace std;

int main(int argc, char **argv) {
    int server;

    struct sockaddr_in server_addr;

    /* Create a socket */
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("Can't create socket");
        return 1;
    }

    /* Set options */
    int opt = 1;
    if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        perror("Can't set options");
        return 1;
    }

    /* Set address and port of the server */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    /* Bind */
    if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Can't bind");
        return 1;
    }

    /* Information about the server */
    cout << "Server IP: " << inet_ntoa(server_addr.sin_addr) << endl;
    cout << "Server port: " << ntohs(server_addr.sin_port) << endl;

    /* Listen */
    if (listen(server, QUEUE) < 0) {
        perror("Can't listen");
        return 1;
    }

    cout << "Server started!" << endl;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(server, &readfds);
    int max_fd = server;

    while (1){
        fd_set temp = readfds;

        /* Select */
        int activity = select(max_fd + 1, &temp, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            printf("Can't select");
        }

        /* Server */
        int new_socket;
        if (FD_ISSET(server, &temp)) {
            if ((new_socket = accept(server, NULL, NULL)) < 0) {
                perror("Can't accept");
                return 1;
            }

            /* Add new socket to set */
            FD_SET(new_socket, &readfds);
            if (new_socket > max_fd) {
                max_fd = new_socket;
            }

            /* Get information about new client */
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof(client_addr);
            getpeername(new_socket, (struct sockaddr *) &client_addr, &client_addr_len);
            cout << "New client IP: " << inet_ntoa(client_addr.sin_addr) << endl;
            cout << "New client port: " << ntohs(client_addr.sin_port) << endl;

            /* Trying to get username from client */
            char username[USERNAME_LENGTH];
            memset(username, 0, USERNAME_LENGTH);
            char input[] = "Enter username (max 256 characters, allows only latin letters and numbers): ";
            send(new_socket, input, sizeof(input), 0);
            int username_length = recv(new_socket, username, USERNAME_LENGTH, 0);
            while (!isValidUsername(username)) {
                send(new_socket, input, sizeof(input), 0);
                username_length = recv(new_socket, username, USERNAME_LENGTH, 0);
            }
            if (username_length < 0) {
                perror("Error receiving username");
                return 1;
            }
            cout << "Username received: " << username << endl;

            /* Add new user to list */
            users[new_socket].socket = new_socket;
            strcpy(users[new_socket].username, username);

            /* Say hello to the client */
            char hello[] = "Hello, ";
            send(new_socket, hello, sizeof(hello), 0);
            send(new_socket, username, sizeof(username), 0);

            /* Say other clients that a new client has connected */
            char greetings[] = "This user has connected: ";
            for (int cd = 0; cd <= max_fd; cd++)
                if (cd != server && cd != new_socket){
                    send(cd, greetings, sizeof(greetings), 0);
                    send(cd, username, sizeof(username), 0);
                }
        }

        /* Client */
        else for (int cd = 0; cd <= max_fd; cd++) {
            if (cd != server && FD_ISSET(cd, &temp)) {
                char buffer[BUF_SIZE];
                memset(buffer, 0, BUF_SIZE);

                /* Receive message */
                int bytes_received = recv(cd, buffer, BUF_SIZE, 0);
                cout << "Message received: " << buffer << endl;

                /* Send message */
                send(cd, users[cd].username, sizeof(users[cd].username), 0);
                send(cd, ": ", sizeof(": "), 0);
                send(cd, buffer, bytes_received, 0);
                cout << "Message sent to: " << users[cd].username << endl;
            }
        }
    }
    return 0;
}
