#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>              // open
#include <unistd.h>             // close
#include <sys/ioctl.h>          // ioctl
#include <linux/videodev2.h>    // v4l2
#include <sys/mman.h>           // mmap, munmap, PROT_*, MAP_*
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

struct timespec start, end;

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
    if (!png) {
        fclose(fp);
        fprintf(stderr, "png_create_write_struct failed\n");
        return;
    }
    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        fprintf(stderr, "png_create_info_struct failed\n");
        return;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        fprintf(stderr, "Error during png creation\n");
        return;
    }

    png_init_io(png, fp);

    png_set_IHDR(png, info, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png, info);

    png_bytep* row_pointers = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = rgb_data + y * width * 3;
    }

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    free(row_pointers);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

int main() {
    clock_gettime(CLOCK_MONOTONIC, &start);

    int fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    // Query capabilities
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

    printf("Final format: %dx%d, pixelformat: %c%c%c%c\n",
       fmt.fmt.pix.width,
       fmt.fmt.pix.height,
       fmt.fmt.pix.pixelformat & 0xFF,
       (fmt.fmt.pix.pixelformat >> 8) & 0xFF,
       (fmt.fmt.pix.pixelformat >> 16) & 0xFF,
       (fmt.fmt.pix.pixelformat >> 24) & 0xFF);

    unsigned int WIDTH = fmt.fmt.pix.width;
    unsigned int HEIGHT = fmt.fmt.pix.height;

    // Request buffers
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));
    req.count = 4;  // request 4 buffers
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0) {
        perror("VIDIOC_REQBUFS");
        close(fd);
        return 1;
    }

    if (req.count < 2) {
        fprintf(stderr, "Insufficient buffer memory\n");
        close(fd);
        return 1;
    }

    // Mmap buffers
    struct buffer* buffers = calloc(req.count, sizeof(struct buffer));
    if (!buffers) {
        perror("calloc");
        close(fd);
        return 1;
    }

    for (unsigned int i = 0; i < req.count; ++i) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
            perror("VIDIOC_QUERYBUF");
            free(buffers);
            close(fd);
            return 1;
        }

        buffers[i].length = buf.length;
        buffers[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

        if (buffers[i].start == MAP_FAILED) {
            perror("mmap");
            // Unmap previously mapped buffers
            for (unsigned int j = 0; j < i; j++) {
                munmap(buffers[j].start, buffers[j].length);
            }
            free(buffers);
            close(fd);
            return 1;
        }
    }

    // Queue all buffers
    for (unsigned int i = 0; i < req.count; ++i) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
            perror("VIDIOC_QBUF");
            for (unsigned int j = 0; j < req.count; ++j)
                munmap(buffers[j].start, buffers[j].length);
            free(buffers);
            close(fd);
            return 1;
        }
    }

    // Start streaming
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) < 0) {
        perror("VIDIOC_STREAMON");
        for (unsigned int j = 0; j < req.count; ++j)
            munmap(buffers[j].start, buffers[j].length);
        free(buffers);
        close(fd);
        return 1;
    }

    struct timespec t1, t2;

    // Capture 1 frame
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    // Dequeue buffer
    if (ioctl(fd, VIDIOC_DQBUF, &buf) < 0) {
        perror("VIDIOC_DQBUF");
        goto cleanup;
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    // Process frame (convert to RGB and save PNG)
    // 110ms
    unsigned char* rgb_buffer = malloc(WIDTH * HEIGHT * 3);
    if (!rgb_buffer) {
        perror("malloc");
        goto requeue;
    }

    yuyv_to_rgb((unsigned char*)buffers[buf.index].start, rgb_buffer, WIDTH, HEIGHT);

    save_png("frame.png", rgb_buffer, WIDTH, HEIGHT);
    printf("Saved frame.png\n");

    free(rgb_buffer);
    // 110ms
requeue:
    // Re-queue buffer for future use
    if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
        perror("VIDIOC_QBUF");
        goto cleanup;
    }

    clock_gettime(CLOCK_MONOTONIC, &t2);

    double elapsed_ms = (t2.tv_sec - t1.tv_sec) * 1000.0 +
                        (t2.tv_nsec - t1.tv_nsec) / 1e6;
    printf("Time to capture and save 1 frame: %.2f ms\n", elapsed_ms);

cleanup:
    // Stop streaming
    ioctl(fd, VIDIOC_STREAMOFF, &type);

    // Unmap buffers
    for (unsigned int i = 0; i < req.count; ++i) {
        munmap(buffers[i].start, buffers[i].length);
    }
    free(buffers);

    close(fd);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double total_elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                              (end.tv_nsec - start.tv_nsec) / 1e6;
    printf("Total time: %.2f ms\n", total_elapsed_ms);

    return 0;
}
