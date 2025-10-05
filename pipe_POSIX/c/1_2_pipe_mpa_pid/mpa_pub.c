#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

void notify_subscribers(const char *wakeup_dir) {
    FILE *wf = fopen(wakeup_dir, "r");
    if (!wf) {
        perror("open wakeup file");
        return;
    }

    pid_t pid;
    char fifo_path[256];

    while (fscanf(wf, "%d", &pid) == 1) {
        snprintf(fifo_path, sizeof(fifo_path), "./run/mpa/FIFOfile%d", pid);
        int fd = open(fifo_path, O_WRONLY | O_NONBLOCK);
        if (fd >= 0) {
            write(fd, "x", 1);
            close(fd);
            printf("[PUB] Notified %s\n", fifo_path);
        } else {
            // subscriber exit
            printf("[PUB] Skip PID %d (%s)\n", pid, strerror(errno));
        }
    }
    fclose(wf);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <wakeupfile>\n", argv[0]);
        return 1;
    }

    const char *wakeup_path = argv[1];

    int counter = 0;
    while (counter < 7) {
        printf("[PUB] Sending update %d...\n", counter++);
        notify_subscribers(wakeup_path);
        sleep(1);
    }
}