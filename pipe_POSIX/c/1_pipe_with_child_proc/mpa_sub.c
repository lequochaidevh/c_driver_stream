// mpa_sub.c
// Simple MPA-like subscriber reading length-prefixed messages.
//
// Usage: ./mpa_sub /run/mpa/mypipe

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s ./run/mpa/<pipe>\n <ID number>", argv[0]);
        return 1;
    }
    const char *fifo_path = argv[1];

    // Open FIFO for reading (block until there's a writer)
    int fd;
    printf("mpa_sub ID %s\n", argv[2]);
    while (1) {
        fd = open(fifo_path, O_RDONLY); // | O_NONBLOCK);
        if (fd >= 0) break;
        perror("waiting for fifo");
        sleep(1);
    }

    int count_life = 3;
    while (count_life) {
        uint32_t net_len;
        ssize_t r = read(fd, &net_len, sizeof(net_len));
        if (r == 0) { // EOF, publisher closed
            // reopen to keep listening for future publishers: simple strategy
            close(fd);
            fd = open(fifo_path, O_RDONLY);
            if (fd < 0) { perror("re-open"); sleep(1); continue; }
            continue;
        } else if (r < 0) {
            perror("read len");
            break;
        } else if (r != sizeof(net_len)) {
            fprintf(stderr, "short read for len\n");
            break;
        }

        uint32_t len = ntohl(net_len);
        char *buf = malloc(len + 1);
        if (!buf) { perror("malloc"); break; }

        ssize_t got = 0;
        while (got < (ssize_t)len) {
            ssize_t g = read(fd, buf + got, len - got);
            if (g <= 0) { break; }
            got += g;
        }
        if (got != (ssize_t)len) {
            free(buf);
            fprintf(stderr, "incomplete message\n");
            continue;
        }
        buf[len] = '\0';
        printf("Received: %s\n", buf);
        free(buf);
        count_life--;
    }

    close(fd);
    return 0;
}
