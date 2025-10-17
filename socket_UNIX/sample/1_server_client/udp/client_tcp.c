// client.c -- simple TCP client (IPv4)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); return EXIT_FAILURE; }

    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &serv.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        return EXIT_FAILURE;
    }

    // Connect to server TCP
    if (connect(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("connect");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Connected to %s:%d\n", server_ip, port);
    char sendbuf[BUFSIZE], recvbuf[BUFSIZE];
    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL) {
        size_t len = strlen(sendbuf);
        if (len == 0) continue;
        if (send(sockfd, sendbuf, len, 0) != (ssize_t)len) {
            perror("send");
            break;
        }
        ssize_t n = recv(sockfd, recvbuf, sizeof(recvbuf) - 1, 0);
        if (n <= 0) { if (n == 0) printf("Server closed connection\n"); else perror("recv"); break; }
        recvbuf[n] = '\0';
        printf("Echo from server: %s", recvbuf);
    }

    close(sockfd);
    return 0;
}
