#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "helper.h"

#define PORT 8080
#define BUFFER_SIZE 1024
char catching_buff[255];


#define MAX_CLIENTS 10

int clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t lock;

void broadcast_message(char *msg, int sender_fd) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < client_count; i++) {
        if (clients[i] != sender_fd) { // Forward message for all client except client's message.
            send(clients[i], msg, strlen(msg), 0);
        }
    }
    pthread_mutex_unlock(&lock);
}

int new_socket;

void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    char buffer[BUFFER_SIZE];
    int valread;

    while ((valread = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[valread] = '\0';
        printf("Client %d: %s\n", client_fd, buffer);

        if (strncmp(buffer, "stop chat", 9) == 0) {
            printf("Client %d disconnected.\n", client_fd);
            close(client_fd);

            // Remove client list
            pthread_mutex_lock(&lock);
            for (int i = 0; i < client_count; i++) {
                if (clients[i] == client_fd) {
                    clients[i] = clients[client_count - 1];
                    client_count--;
                    break;
                }
            }
            pthread_mutex_unlock(&lock);
            return NULL;
        }

        // Send all other client.
        broadcast_message(buffer, client_fd);
    }

    return NULL;
}

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) break;

        buffer[valread] = '\0';
        printf("\r Client: %s", buffer);
        int iClear;
        for(iClear = 0; iClear < strlen(catching_buff); iClear++) printf(" ");
        printf("\n\n"); // clean line
        printf(" Input ---> %s", catching_buff);
        fflush(stdout);
        if (strcmp(buffer, "stop chat") == 0) {
            printf("\nChat ended by client.\n");
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            exit(0);
        }
    }
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    pthread_mutex_init(&lock, NULL);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server is running on port %d...\n", PORT);

    printf("\n\n\n");
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("New client connected: %d\n", new_socket);

        pthread_mutex_lock(&lock);
        clients[client_count++] = new_socket;
        pthread_mutex_unlock(&lock);

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, (void *)&new_socket);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}
