#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      // read(), write(), close()
#include <arpa/inet.h>   // sockaddr_in, inet_ntoa()

#define PORT 8281
#define BUFFER_SIZE 1024

int main() {
    printf("\nServer is starting ... \n");

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    const char *hello = "Hello from server\n";

    // 1. Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket create failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 2. Asigning address (bind)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // get with every IP
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Listen
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);

    // 4. Accept connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Client connected: %s\n", inet_ntoa(address.sin_addr));

    // 5. Send and Get data
    read(new_socket, buffer, BUFFER_SIZE);
    printf("Client: %s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);

    // 6. Close socket
    close(new_socket);
    close(server_fd);
    return 0;
}
