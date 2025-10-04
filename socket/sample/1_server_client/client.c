#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     
#include <arpa/inet.h>  

#define PORT 8281
#define BUFFER_SIZE 1024

int main() {

    printf("\n Client is started. Send message to Server ... \n");
     int server_fd, new_socket;
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    const char *hello = "Hello from client\n";

    // 1. Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 2. Convert IP from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // 3. Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // 4. Send and Get data
    send(sock, hello, strlen(hello), 0);
    read(sock, buffer, BUFFER_SIZE);
    printf("Server: %s\n", buffer);

    // 5. Close socket
    close(sock);
    return 0;
}
