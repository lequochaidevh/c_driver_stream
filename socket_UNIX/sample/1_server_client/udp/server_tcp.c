// server.c -- simple TCP echo server (IPv4), iterative (handle 1 client tại một thời điểm)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BACKLOG 5
#define BUFSIZE 1024

int main(void) {
    int server_fd = -1, client_fd = -1;
    struct sockaddr_in addr;
    char buf[BUFSIZE];

    // 1) Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2) allow reuse address (restart server fastly)
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3) Config server and bind
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // nghe trên mọi interface
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4) Listen
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);

    // 5) Loop: accept -> recv -> send -> close
    for (;;) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue; // 
        }

        printf("Accepted connection from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        ssize_t n;
        while ((n = recv(client_fd, buf, sizeof(buf) - 1, 0)) > 0) {
            buf[n] = '\0'; // đảm bảo null-terminated nếu là text
            printf("Received: %s", buf); // in ra console server
            // Echo lại client
            if (send(client_fd, buf, n, 0) != n) {
                perror("send");
                break;
            }
        }

        if (n == 0) {
            printf("Client disconnected.\n");
        } else if (n < 0) {
            perror("recv");
        }

        close(client_fd);
        client_fd = -1;
    }

    close(server_fd);
    return 0;
}
