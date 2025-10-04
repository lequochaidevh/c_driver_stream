#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <png_file> <fifo_path>\n", argv[0]);
        return 1;
    }

    const char *png_file = argv[1];
    const char *fifo_path = argv[2];

    mkfifo(fifo_path, 0666);

    int fifo_fd = open(fifo_path, O_WRONLY);
    if (fifo_fd < 0) { perror("open fifo"); return 1; }

    FILE *f = fopen(png_file, "rb");
    if (!f) { perror("open png"); return 1; }

    unsigned char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
        write(fifo_fd, buf, n);
    }

    fclose(f);
    close(fifo_fd);
    printf("PNG sent via FIFO\n");
    return 0;
}