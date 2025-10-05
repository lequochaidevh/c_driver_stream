#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define MAX_SUB 100
#define DATAFILE "./run/mpa/datafile"

typedef struct {
    pid_t pid;
    char control_fifo[256];
} subscriber_t;

subscriber_t subs[MAX_SUB];
int sub_count = 0;

const char *request_fifo = "./run/mpa/request_FIFO";

int read_requests(int old_fd) {
    int fd;
    if (!old_fd) {
        fd = open(request_fifo, O_RDONLY | O_NONBLOCK);
    } else { fd = old_fd; }

    if (fd < 0) { perror("open request_fifo"); return -1; }
    // sleep(1);
    char buf[1024];
    ssize_t n = -1;
    while (1) {
        if((n = read(fd, buf, sizeof(buf)-1)) >= 0) break;
        printf("[n] = %ld", n);
        //usleep(100000); // sleep 0.2s
    }    
    buf[n] = '\0';
    if (strncmp(buf, "UNREG", 5) == 0) {
        pid_t pid;
        sscanf(buf+6, "%d", &pid);
        // xóa sub khỏi danh sách
        for (int i = 0; i < sub_count; i++) {
            if (subs[i].pid == pid) {
                printf("[PUB] Unregister PID %d\n", pid);
                for (int j = i; j < sub_count-1; j++)
                    subs[j] = subs[j+1];
                sub_count--;
                break;
            }
        }
    } else {
        pid_t pid;
        char fifo[256];
        if (sscanf(buf, "%d %255s", &pid, fifo) == 2) {
            if (sub_count < MAX_SUB) {
                subs[sub_count].pid = pid;
                strncpy(subs[sub_count].control_fifo, fifo, 256);
                sub_count++;
                printf("[PUB] Register PID %d FIFO %s\n", pid, fifo);

                int ack_fd = open(fifo, O_WRONLY);
                if (ack_fd >= 0) {
                    write(ack_fd, "OK\n", 3);
                    close(ack_fd);
                }
            }
        }
    }
    
    // close(fd);
    return fd;
}

void notify_all() {
    for (int i = 0; i < sub_count; i++) {
        int fd = open(subs[i].control_fifo, O_WRONLY);
        if (fd >= 0) {
            write(fd, "x", 1);
            close(fd);
        } else {
            printf("[PUB] Skip PID %d (%s)\n", subs[i].pid, strerror(errno));
        }
    }
}

int main() {
    // tạo request FIFO
    mkfifo(request_fifo, 0666);
    int init_request_fd = 0;
    init_request_fd = read_requests(init_request_fd);
    int count = 0;
    while (1) {
        read_requests(init_request_fd);

        if (sub_count == 0 && count > 0) {
            printf("[PUB] No more subscribers, exiting in 2s...\n");
            sleep(2);
            break;
        }

        // Ghi data
        FILE *f = fopen(DATAFILE, "w");
        if (f) {
            fprintf(f, "Update %d: %d\n", count, rand()%100);
            fclose(f);
        }

        printf("[PUB] Update %d, triggering subs...\n", count);
        notify_all();
        
        count++;

        sleep(1);
        // close(rfd);
    }

    unlink(request_fifo);
    printf("[PUB] Done.\n");
    return 0;
}
