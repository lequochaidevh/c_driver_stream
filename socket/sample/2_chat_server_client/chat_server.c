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
int new_socket;

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
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // thread recive messages from client
    char message[BUFFER_SIZE];
    pthread_t recv_thread;
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
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
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server is running on port %d...\n", PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected!\n");

    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    printf("\n\n\n");
    while (1) {
        
        char *buffer_ptr = get_message_and_jump_start_line("Server: ", catching_buff, sizeof(catching_buff));

        strncpy(message, buffer_ptr, sizeof(message));
        message[strcspn(message, "\n")] = 0;

        send(new_socket, message, strlen(message), 0);

        if (strcmp(message, "stop chat") == 0) {
            printf("Chat ended by server.\n");
            break;
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
