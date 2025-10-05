#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>

void register_pid(const char *wakeup_path, const char *fifo_path) {
    FILE *wf = fopen(wakeup_path, "a");
    if (!wf) {
        perror("open wakeup");
        exit(1);
    }
    fprintf(wf, "%d\n", getpid());
    fclose(wf);

    // create FIFO for this subscriber
    if (mkfifo(fifo_path, 0666) < 0 && access(fifo_path, F_OK) != 0) {
        perror("mkfifo");
        exit(1);
    }
    printf("[SUB %d] Registered FIFO: %s\n", getpid(), fifo_path);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <wakeupfile>\n", argv[0]);
        return 1;
    }

    const char *wakeup_path = argv[1];

    char fifo_path[256];
    snprintf(fifo_path, sizeof(fifo_path), "./run/mpa/FIFOfile%d", getpid());

    register_pid(wakeup_path, fifo_path);

    int fd = open(fifo_path, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("open fifo");
        return 1;
    }

    struct pollfd pfd = { .fd = fd, .events = POLLIN };
    printf("[SUB %d] Waiting for publisher...\n", getpid());

    int downcount = 5;
    while (downcount) {
        int ret = poll(&pfd, 1, -1);
        if (ret > 0 && (pfd.revents & POLLIN)) {
            char buf[8];
            read(fd, buf, sizeof(buf));
            printf("[SUB %d] Got wakeup!\n", getpid());
            downcount--;
        }
    }
}
