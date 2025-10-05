#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <time.h>
#include <png.h>


#ifndef CLOCK_MONOTONIC
# define CLOCK_MONOTONIC		1
#endif

#define DEVICE "/dev/video0"

struct buffer {
    void* start;
    size_t length;
};

int clamp(int val) {
    if (val < 0) return 0;
    if (val > 255) return 255;
    return val;
}

void yuyv_to_rgb(unsigned char* yuyv, unsigned char* rgb, int width, int height) {
    int rgb_index = 0;
    for (int i = 0; i < width * height * 2; i += 4) {
        int y0 = yuyv[i + 0];
        int u  = yuyv[i + 1] - 128;
        int y1 = yuyv[i + 2];
        int v  = yuyv[i + 3] - 128;

        // Pixel 1
        int r = y0 + (1.370705 * v);
        int g = y0 - (0.698001 * v) - (0.337633 * u);
        int b = y0 + (1.732446 * u);

        rgb[rgb_index++] = clamp(r);
        rgb[rgb_index++] = clamp(g);
        rgb[rgb_index++] = clamp(b);

        // Pixel 2
        r = y1 + (1.370705 * v);
        g = y1 - (0.698001 * v) - (0.337633 * u);
        b = y1 + (1.732446 * u);

        rgb[rgb_index++] = clamp(r);
        rgb[rgb_index++] = clamp(g);
        rgb[rgb_index++] = clamp(b);
    }
}

void save_png(const char* filename, unsigned char* rgb_data, int width, int height) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    if (!png || !info) {
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return;
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, width, height, 8,
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png, info);

    png_bytep row_pointers[height];
    for (int y = 0; y < height; y++) {
        row_pointers[y] = rgb_data + y * width * 3;
    }

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

int main() {
    struct timespec start, end, t1, t2;

    clock_gettime(CLOCK_MONOTONIC, &start);

    int fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    // Get capabilities
    struct v4l2_capability cap;
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) {
        perror("VIDIOC_QUERYCAP");
        close(fd);
        return 1;
    }

    // Set format
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 640;
    fmt.fmt.pix.height = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) {
        perror("VIDIOC_S_FMT");
        close(fd);
        return 1;
    }

    unsigned int WIDTH = fmt.fmt.pix.width;
    unsigned int HEIGHT = fmt.fmt.pix.height;

    // Request buffer
    struct v4l2_requestbuffers req = {
        .count = 1,
        .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
        .memory = V4L2_MEMORY_MMAP
    };

    if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0) {
        perror("VIDIOC_REQBUFS");
        close(fd);
        return 1;
    }

    // Query buffer
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;

    if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
        perror("VIDIOC_QUERYBUF");
        close(fd);
        return 1;
    }

    // mmap
    void* buffer_start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    if (buffer_start == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // Queue buffer
    if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
        perror("VIDIOC_QBUF");
        close(fd);
        return 1;
    }

    // Start stream
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) < 0) {
        perror("VIDIOC_STREAMON");
        close(fd);
        return 1;
    }

    // Bỏ qua vài frame đầu nếu cần
    for (int i = 0; i < 3; i++) {
        ioctl(fd, VIDIOC_QBUF, &buf);
        ioctl(fd, VIDIOC_DQBUF, &buf);
    }

    // ==== FRAME 1 ====
    ioctl(fd, VIDIOC_QBUF, &buf);
    ioctl(fd, VIDIOC_DQBUF, &buf);
    clock_gettime(CLOCK_MONOTONIC, &t1);

    unsigned char* frame1 = malloc(buf.bytesused);
    memcpy(frame1, buffer_start, buf.bytesused);

    // ==== FRAME 2 ====
    ioctl(fd, VIDIOC_QBUF, &buf);
    ioctl(fd, VIDIOC_DQBUF, &buf);
    clock_gettime(CLOCK_MONOTONIC, &t2);

    unsigned char* frame2 = malloc(buf.bytesused);
    memcpy(frame2, buffer_start, buf.bytesused);

    // Convert to RGB
    unsigned char* rgb1 = malloc(WIDTH * HEIGHT * 3);
    unsigned char* rgb2 = malloc(WIDTH * HEIGHT * 3);
    yuyv_to_rgb(frame1, rgb1, WIDTH, HEIGHT);
    yuyv_to_rgb(frame2, rgb2, WIDTH, HEIGHT);

    save_png("frame1.png", rgb1, WIDTH, HEIGHT);
    save_png("frame2.png", rgb2, WIDTH, HEIGHT);
    printf("Saved frame1.png and frame2.png\n");

    // Time diff
    double diff_ms = (t2.tv_sec - t1.tv_sec) * 1000.0 +
                     (t2.tv_nsec - t1.tv_nsec) / 1e6;
    printf("Time between 2 captured frames: %.2f ms\n", diff_ms);

    // Cleanup
    ioctl(fd, VIDIOC_STREAMOFF, &type);
    munmap(buffer_start, buf.length);
    close(fd);

    free(frame1);
    free(frame2);
    free(rgb1);
    free(rgb2);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double total_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                      (end.tv_nsec - start.tv_nsec) / 1e6;
    printf("Total time: %.2f ms\n", total_ms);

    return 0;
}
