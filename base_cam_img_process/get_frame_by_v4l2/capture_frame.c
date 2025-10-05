#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>              // open
#include <unistd.h>             // close
#include <sys/ioctl.h>          // ioctl
#include <linux/videodev2.h>    // v4l2
#include <sys/mman.h>   //// mmap, munmap, PROT_*, MAP_*
#include <time.h>

#ifndef CLOCK_MONOTONIC
# define CLOCK_MONOTONIC		1
#endif
struct timespec start, end;
#define DEVICE "/dev/video0"

void yuyv_to_rgb000(unsigned char* yuyv, unsigned char* rgb, int width, int height) {
    int i, j;
    int index = 0;

    for (i = 0; i < width * height * 2; i += 4) {
        int y0 = yuyv[i + 0];
        int u  = yuyv[i + 1] - 128;
        int y1 = yuyv[i + 2];
        int v  = yuyv[i + 3] - 128;

        int r, g, b;

        // Pixel 1
        r = y0 + 1.402 * v;
        g = y0 - 0.344136 * u - 0.714136 * v;
        b = y0 + 1.772 * u;

        rgb[index++] = r < 0 ? 0 : r > 255 ? 255 : r;
        rgb[index++] = g < 0 ? 0 : g > 255 ? 255 : g;
        rgb[index++] = b < 0 ? 0 : b > 255 ? 255 : b;

        // Pixel 2
        r = y1 + 1.402 * v;
        g = y1 - 0.344136 * u - 0.714136 * v;
        b = y1 + 1.772 * u;

        rgb[index++] = r < 0 ? 0 : r > 255 ? 255 : r;
        rgb[index++] = g < 0 ? 0 : g > 255 ? 255 : g;
        rgb[index++] = b < 0 ? 0 : b > 255 ? 255 : b;
    }
}

void brighten_rgb(unsigned char* rgb, int width, int height, int amount) {
    int size = width * height * 3;
    for (int i = 0; i < size; i++) {
        int val = rgb[i] + amount;
        rgb[i] = val > 255 ? 255 : val;
    }
}

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

#include <png.h>

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

    png_set_IHDR(png, info, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
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

    // Request buffer
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

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

    // Mmap buffer
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

    // Start streaming
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) < 0) {
        perror("VIDIOC_STREAMON");
        close(fd);
        return 1;
    }

    // Dequeue buffer (grab frame)
    if (ioctl(fd, VIDIOC_DQBUF, &buf) < 0) {
        perror("VIDIOC_DQBUF");
        close(fd);
        return 1;
    }

    // Save frame to file
    FILE* out = fopen("frame.raw", "wb");
    fwrite(buffer_start, buf.bytesused, 1, out);
    fclose(out);

    printf("Frame captured and saved to frame.raw (%d bytes)\n", buf.bytesused);
    // Convert YUYV → RGB before cleanup
    unsigned char* rgb_buffer = malloc(WIDTH * HEIGHT * 3);  // RGB24
    yuyv_to_rgb(buffer_start, rgb_buffer, WIDTH, HEIGHT);
    //brighten_rgb(rgb_buffer, WIDTH, HEIGHT, 20);
    save_png("frame.png", rgb_buffer, WIDTH, HEIGHT);
    free(rgb_buffer);

    // Stop streaming
    ioctl(fd, VIDIOC_STREAMOFF, &type);

    // Cleanup
    munmap(buffer_start, buf.length);
    close(fd);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                        (end.tv_nsec - start.tv_nsec) / 1e6;

    printf("Time to capture 1 frame: %.2f ms\n", elapsed_ms);

    return 0;
}