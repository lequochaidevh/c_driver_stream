// client_udp.c -- simple UDP client
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

    int sockfd;
    struct sockaddr_in servaddr;
    char sendbuf[BUFSIZE], recvbuf[BUFSIZE];
    socklen_t len = sizeof(servaddr);

    // 1. create socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Config server
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &servaddr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Client ready to send to %s:%d\n", server_ip, port);

    // 3. send and recv (echo)
    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL) {
        size_t len_send = strlen(sendbuf);
        if (sendto(sockfd, sendbuf, len_send, 0,
                   (struct sockaddr *)&servaddr, sizeof(servaddr)) != (ssize_t)len_send) {
            perror("sendto");
            break;
        }

        ssize_t n = recvfrom(sockfd, recvbuf, sizeof(recvbuf) - 1, 0, NULL, NULL);
        if (n < 0) {
            perror("recvfrom");
            break;
        }
        recvbuf[n] = '\0';
        printf("Echo from server: %s", recvbuf);
    }

    close(sockfd);
    return 0;
}
