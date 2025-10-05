#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/stat.h>

#define PORT 8182
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    FILE *fp;
    int bytes_read;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr); // hoặc IP của server container

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Connected to server.\n");

    fp = fopen("test_video.mp4", "rb");
    if (!fp) {
        perror("File open error");
        exit(1);
    }

    struct stat st;
    stat("test_video.mp4", &st);
    long file_size = st.st_size;
    send(sock, &file_size, sizeof(file_size), 0);

    while ((bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
        send(sock, buffer, bytes_read, 0);
    }

    printf("File sent successfully.\n");
    fclose(fp);
    close(sock);
    return 0;
}
