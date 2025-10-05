#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>
#include <sys/stat.h>

const char *request_fifo = "./run/mpa/request_FIFO";
#define MAX_RETRIES 5

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <sub_dir>\n", argv[0]);
        return 1;
    }

    char *sub_dir = argv[1];
    char control_fifo[256];
    snprintf(control_fifo, sizeof(control_fifo), "%s/control_fifo", sub_dir);
    mkfifo(control_fifo, 0666);

    pid_t pid = getpid();

    // wait pub created request_FIFO.
    while (access(request_fifo, F_OK) != 0) {
        usleep(100000); // 0.1s
    }

    // Send request register + wait ACK
    int registered = 0;
    for (int retry = 0; retry < MAX_RETRIES && !registered; retry++) {
        printf("[SUB %d] registered %d time...\n", pid, retry+1);
        int req_fd = open(request_fifo, O_WRONLY);
        if (req_fd < 0) { perror("open request_fifo"); usleep(200000); continue; }

        char buf[512];
        snprintf(buf, sizeof(buf), "%d %s\n", pid, control_fifo);
        write(req_fd, buf, strlen(buf));
        close(req_fd);

        // wait ACK from publisher
        int ack_fd;
        while ((ack_fd = open(control_fifo, O_RDONLY | O_NONBLOCK)) < 0)
        if (ack_fd < 0) { perror("open control_fifo"); usleep(200000); continue; }

        char ack[16];
        int waited = 0;
        while (waited < 1000) { // timeout 5000ms
            ssize_t n = read(ack_fd, ack, sizeof(ack)-1);
            if (n > 0) {
                ack[n] = '\0';
                if (strncmp(ack, "OK", 2) == 0) {
                    registered = 1;
                    break;
                }
            }
            usleep(100000);
            waited += 100;
        }
        close(ack_fd);

        if (!registered) printf("[SUB %d] No ACK, retrying...\n", pid);
    }

    if (!registered) {
        fprintf(stderr, "[SUB %d] Registration failed\n", pid);
        return 1;
    }

    // open control FIFO to poll
    int fd = open(control_fifo, O_RDONLY | O_NONBLOCK);
    struct pollfd pfd = { .fd = fd, .events = POLLIN };

    printf("[SUB %d] Ready and waiting updates...\n", pid);

    int updates = 0;
    while (updates < 10) {
        int ret = poll(&pfd, 1, 1000);
        if (ret > 0 && (pfd.revents & POLLIN)) {
            char tmp[8];
            read(fd, tmp, sizeof(tmp));

            // read data
            FILE *f = fopen("./run/mpa/datafile", "r");
            if (f) {
                char line[256];
                fgets(line, sizeof(line), f);
                fclose(f);
                printf("[SUB %d] Got: %s", pid, line);
            }
            updates++;
        }
    }

    // send request to unregister
    int exit_req_fd = open(request_fifo, O_WRONLY);
    char buffexit[255];
    snprintf(buffexit, sizeof(buffexit), "UNREG %d\n", pid);
    write(exit_req_fd, buffexit, strlen(buffexit));
    close(exit_req_fd);

    unlink(control_fifo);
    printf("[SUB %d] Exiting\n", pid);
    return 0;
}
