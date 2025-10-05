#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

#include "helper.h"

#define PORT 8080
#define BUFFER_SIZE 1024
char catching_buff[255];


int sock;

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int valread = 0;
        valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) break;

        if (valread >= BUFFER_SIZE) valread = BUFFER_SIZE - 1;
        buffer[valread] = '\0';
        printf("\r SENDER [%c]: %s", buffer[0], buffer+1);
        int iClear;
        for(iClear = 0; iClear < strlen(catching_buff); iClear++) printf(" ");
        printf("\n\n"); // clean line
        printf(" Input ---> %s", catching_buff);
        fflush(stdout);
        if (strcmp(buffer, "stop chat") == 0) {
            printf("\nChat ended by server.\n");
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            exit(0);
        }
    }
    return NULL;
}

int main() {
    struct sockaddr_in serv_addr;
    char message[BUFFER_SIZE];
    pthread_t recv_thread;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server!\n");

    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    printf("\n\n\n");
    // get input and send data
    while (1) {
        char *buffer_ptr = get_message_and_jump_start_line("Me: ", catching_buff, sizeof(catching_buff));

        strncpy(message, buffer_ptr, sizeof(message));
        message[strcspn(message, "\n")] = 0;
        
        send(sock, message, strlen(message), 0);

        if (strcmp(message, "stop chat") == 0) {
            printf("Chat ended by client.\n");
            break;
        }
    }

    close(sock);
    return 0;
}
