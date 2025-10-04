#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <fifo_path> <output_file>\n", argv[0]);
        return 1;
    }

    const char *fifo_path = argv[1];
    const char *out_file = argv[2];

    int fifo_fd = open(fifo_path, O_RDONLY);
    if (fifo_fd < 0) { perror("open fifo"); return 1; }

    FILE *fout = fopen(out_file, "wb");
    if (!fout) { perror("open output"); return 1; }

    unsigned char buf[4096];
    ssize_t n;
    while ((n = read(fifo_fd, buf, sizeof(buf))) > 0) {
        fwrite(buf, 1, n, fout);
    }

    fclose(fout);
    close(fifo_fd);
    printf("PNG received via FIFO\n");
    return 0;
}
