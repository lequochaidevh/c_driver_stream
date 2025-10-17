// server_udp.c -- simple UDP echo server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFSIZE 1024

int main(void) {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buf[BUFSIZE];
    socklen_t len;

    // 1. create socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Bind
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %d...\n", PORT);

    // 3. recv data and send back client (echo)
    while (1) {
        len = sizeof(cliaddr);
        ssize_t n = recvfrom(sockfd, buf, BUFSIZE - 1, 0,
                             (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }

        buf[n] = '\0';
        printf("Received from %s:%d: %s",
               inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buf);

        // Send feedback (echo) // Can not need
        if (sendto(sockfd, buf, n, 0, (struct sockaddr *)&cliaddr, len) != n) {
            perror("sendto");
        }
    }

    close(sockfd);
    return 0;
}
