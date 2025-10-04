#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/stat.h> //filesize

#define PORT 8182
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    FILE *fp;
    int bytes_received;
    long file_size;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server is waiting for connection...\n");
    int addrlen = sizeof(address);
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    printf("Client connected!\n");

    recv(new_socket, &file_size, sizeof(file_size), 0);
    printf("Receiving file (%ld bytes)...\n", file_size);

    fp = fopen("receive_video.mp4", "wb");
    if (!fp) {
        perror("File open error");
        exit(1);
    }

    while ((bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, sizeof(char), bytes_received, fp);
    }

    printf("File received successfully.\n");
    fclose(fp);
    close(new_socket);
    close(server_fd);
    return 0;
}
