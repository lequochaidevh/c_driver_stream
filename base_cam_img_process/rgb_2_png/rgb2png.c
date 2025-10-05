#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <zlib.h>
#include <arpa/inet.h>  //  htonl()


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

uint32_t crc(const unsigned char *buf, size_t len) {
    return crc32(0L, buf, len);
}

void write_chunk(FILE *fp, const char *type, const unsigned char *data, size_t len) {
    uint32_t length = htonl(len);
    fwrite(&length, 4, 1, fp);
    fwrite(type, 1, 4, fp);
    fwrite(data, 1, len, fp);

    unsigned char *crc_buf = malloc(len + 4);
    memcpy(crc_buf, type, 4);
    memcpy(crc_buf + 4, data, len);
    uint32_t crc_val = htonl(crc(crc_buf, len + 4));
    fwrite(&crc_val, 4, 1, fp);
    free(crc_buf);
}

void save_png(const char *filename, unsigned char *rgb, int width, int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return;
    }

    // PNG Signature
    unsigned char sig[8] = {137,80,78,71,13,10,26,10};
    fwrite(sig, 1, 8, fp);

    // IHDR
    unsigned char ihdr[13];
    ihdr[0] = (width >> 24) & 0xff;
    ihdr[1] = (width >> 16) & 0xff;
    ihdr[2] = (width >> 8) & 0xff;
    ihdr[3] = width & 0xff;
    ihdr[4] = (height >> 24) & 0xff;
    ihdr[5] = (height >> 16) & 0xff;
    ihdr[6] = (height >> 8) & 0xff;
    ihdr[7] = height & 0xff;
    ihdr[8] = 8;  // Bit depth
    ihdr[9] = 2;  // Color type: RGB
    ihdr[10] = 0; // Compression
    ihdr[11] = 0; // Filter
    ihdr[12] = 0; // Interlace
    write_chunk(fp, "IHDR", ihdr, 13);

    // Prepare image data with filter byte at start of each row
    size_t row_size = width * 3;
    size_t raw_size = height * (1 + row_size);
    unsigned char *raw = malloc(raw_size);
    for (int y = 0; y < height; y++) {
        raw[y * (row_size + 1)] = 0; // No filter
        memcpy(raw + y * (row_size + 1) + 1, rgb + y * row_size, row_size);
    }

    // Compress image data
    uLongf comp_size = compressBound(raw_size);
    unsigned char *compressed = malloc(comp_size);
    compress2(compressed, &comp_size, raw, raw_size, Z_BEST_COMPRESSION);
    write_chunk(fp, "IDAT", compressed, comp_size);

    free(compressed);
    free(raw);

    // IEND
    write_chunk(fp, "IEND", NULL, 0);
    fclose(fp);
}

#define WIDTH 640
#define HEIGHT 480

int main() {
    FILE* in = fopen("frame.raw", "rb");
    if (!in) {
        perror("fopen frame.raw");
        return 1;
    }

    size_t yuyv_size = WIDTH * HEIGHT * 2;
    unsigned char* yuyv_buffer = malloc(yuyv_size);
    fread(yuyv_buffer, 1, yuyv_size, in);
    fclose(in);

    unsigned char* rgb_buffer = malloc(WIDTH * HEIGHT * 3);
    yuyv_to_rgb(yuyv_buffer, rgb_buffer, WIDTH, HEIGHT);

    save_png("output.png", rgb_buffer, WIDTH, HEIGHT);

    free(yuyv_buffer);
    free(rgb_buffer);
    printf("Saved to output.png\n");

    return 0;
}