// Simple MPA-like publisher using named FIFO and length-prefixed messages.
//
// Usage: ./mpa_pub /run/mpa/mypipe "Hello subscribers"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <arpa/inet.h> // for htonl

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s ./run/mpa/<pipe> <message>\n", argv[0]);
        return 1;
    }
    const char *fifo_path = argv[1];
    const char *msg = argv[2];

    // create directory if needed (run as root or with permission)
    // mkfifo will fail if already exists - that's fine
    if (mkfifo(fifo_path, 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo");
            return 1;
        }
    }

    // Open FIFO for read/write so that open doesn't block waiting for a reader.
    // Opening O_RDWR means the FIFO stays usable even with no readers yet.
    int fd = open(fifo_path, O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        perror("open fifo");
        return 1;
    }

    // Prepare length-prefixed message
    uint32_t len = (uint32_t)strlen(msg);
    uint32_t net_len = htonl(len); // network byte order for portability

    int count_life = 3;
    while(count_life) {
        
        // Write length then payload
        if (write(fd, &net_len, sizeof(net_len)) != sizeof(net_len)) {
            perror("write len");
        }
        if (write(fd, msg, len) != (ssize_t)len) {
            perror("write msg");
        }

        printf("Published %u bytes to %s\n", len, fifo_path);

        // Keep the pipe open so that subscribers can continue reading
        printf("Sleep 2s and will exit publisher.\n");
        sleep(2);
        count_life--;
    }
    close(fd);
    return 0;
}

// char* result = malloc(strlen(s1) + strlen(s2) + 1);
// snprintf(result, strlen(s1) + strlen(s2) + 1, "%s%s", s1, s2);