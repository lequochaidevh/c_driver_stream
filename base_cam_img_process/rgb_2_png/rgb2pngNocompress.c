#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
uint32_t crc_table[256];
int crc_table_computed = 0;

void make_crc_table(void) {
    uint32_t c;
    for (int n = 0; n < 256; n++) {
        c = (uint32_t) n;
        for (int k = 0; k < 8; k++) {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[n] = c;
    }
    crc_table_computed = 1;
}

uint32_t update_crc(uint32_t crc, uint8_t *buf, int len) {
    uint32_t c = crc;
    if (!crc_table_computed) make_crc_table();
    for (int n = 0; n < len; n++) {
        c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
    }
    return c;
}

uint32_t crc(uint8_t *buf, int len) {
    return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}

void write_chunk(FILE *fp, const char *type, const uint8_t *data, int length) {
    uint32_t len_be = htonl(length);
    fwrite(&len_be, 4, 1, fp);
    fwrite(type, 1, 4, fp);
    if (length > 0 && data != NULL) {
        fwrite(data, 1, length, fp);
    }
    uint8_t *crc_buf = malloc(length + 4);
    memcpy(crc_buf, type, 4);
    if (length > 0 && data != NULL) {
        memcpy(crc_buf + 4, data, length);
    }
    uint32_t crc_val = crc(crc_buf, length + 4);
    free(crc_buf);
    uint32_t crc_be = htonl(crc_val);
    fwrite(&crc_be, 4, 1, fp);
}

void save_png_minimal(const char *filename, uint8_t *rgb, int width, int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return;
    }

    // PNG signature
    uint8_t pngsig[8] = {137,80,78,71,13,10,26,10};
    fwrite(pngsig, 1, 8, fp);

    // IHDR chunk data
    uint8_t ihdr[13];
    ihdr[0] = (width >> 24) & 0xFF;
    ihdr[1] = (width >> 16) & 0xFF;
    ihdr[2] = (width >> 8) & 0xFF;
    ihdr[3] = width & 0xFF;
    ihdr[4] = (height >> 24) & 0xFF;
    ihdr[5] = (height >> 16) & 0xFF;
    ihdr[6] = (height >> 8) & 0xFF;
    ihdr[7] = height & 0xFF;
    ihdr[8] = 8; // bit depth
    ihdr[9] = 2; // color type RGB
    ihdr[10] = 0; // compression method
    ihdr[11] = 0; // filter method
    ihdr[12] = 0; // interlace method

    write_chunk(fp, "IHDR", ihdr, 13);

    int row_bytes = width * 3;
    int raw_data_size = (row_bytes + 1) * height;
    uint8_t *raw_data = malloc(raw_data_size);
    if (!raw_data) {
        fclose(fp);
        fprintf(stderr, "Memory alloc failed\n");
        return;
    }

    for (int y = 0; y < height; y++) {
        raw_data[y*(row_bytes+1)] = 0; // filter type 0
        memcpy(&raw_data[y*(row_bytes+1) + 1], &rgb[y*row_bytes], row_bytes);
    }

    // zlib no compression header and block
    int zlib_overhead = 7;
    int idat_size = raw_data_size + zlib_overhead;
    uint8_t *idat = malloc(idat_size);
    if (!idat) {
        free(raw_data);
        fclose(fp);
        fprintf(stderr, "Memory alloc failed\n");
        return;
    }

    idat[0] = 0x78; // CMF
    idat[1] = 0x01; // FLG (no compression)
    idat[2] = 0x01; // final block, no compression
    idat[3] = raw_data_size & 0xFF;
    idat[4] = (raw_data_size >> 8) & 0xFF;
    idat[5] = (~raw_data_size) & 0xFF;
    idat[6] = ((~raw_data_size) >> 8) & 0xFF;
    memcpy(&idat[7], raw_data, raw_data_size);

    write_chunk(fp, "IDAT", idat, idat_size);
    write_chunk(fp, "IEND", NULL, 0);

    fclose(fp);
    free(raw_data);
    free(idat);
}

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Tạo các block nén zlib dạng no compression, chia raw_data thành các block ≤ 65535 bytes
uint8_t* create_zlib_no_compression_blocks(uint8_t* raw_data, int raw_data_size, int* out_size) {
    const int max_block_size = 65535;
    int total_size = 2; // 2 bytes header zlib (CMF + FLG)
    int pos = 0;

    // Tính tổng kích thước buffer mới
    while (pos < raw_data_size) {
        int block_len = raw_data_size - pos;
        if (block_len > max_block_size) block_len = max_block_size;
        // mỗi block có 5 bytes overhead: 1 byte header block + 2 bytes LEN + 2 bytes NLEN
        total_size += 5 + block_len;
        pos += block_len;
    }

    uint8_t *out = malloc(total_size);
    if (!out) return NULL;

    out[0] = 0x78;  // CMF: Compression Method/Flags (deflate, 32K window)
    out[1] = 0x01;  // FLG: no compression, fastest

    pos = 0;
    int out_pos = 2;
    while (pos < raw_data_size) {
        int block_len = raw_data_size - pos;
        if (block_len > max_block_size) block_len = max_block_size;

        // Final block flag = 1 nếu là block cuối, 0 nếu không
        uint8_t final_flag = (pos + block_len >= raw_data_size) ? 1 : 0;

        out[out_pos++] = final_flag; // 1 bit final block + 2 bits block type = 00 (no compression)
        // LEN (2 bytes)
        out[out_pos++] = block_len & 0xFF;
        out[out_pos++] = (block_len >> 8) & 0xFF;
        // NLEN (2 bytes) - ones complement của LEN
        uint16_t nlen = ~block_len;
        out[out_pos++] = nlen & 0xFF;
        out[out_pos++] = (nlen >> 8) & 0xFF;

        // copy dữ liệu raw block_len bytes
        memcpy(&out[out_pos], &raw_data[pos], block_len);
        out_pos += block_len;

        pos += block_len;
    }

    *out_size = total_size;
    return out;
}

void save_png(const char *filename, uint8_t *rgb, int width, int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return;
    }

    // PNG signature
    uint8_t pngsig[8] = {137,80,78,71,13,10,26,10};
    fwrite(pngsig, 1, 8, fp);

    // IHDR chunk
    uint8_t ihdr[13];
    ihdr[0] = (width >> 24) & 0xFF;
    ihdr[1] = (width >> 16) & 0xFF;
    ihdr[2] = (width >> 8) & 0xFF;
    ihdr[3] = width & 0xFF;
    ihdr[4] = (height >> 24) & 0xFF;
    ihdr[5] = (height >> 16) & 0xFF;
    ihdr[6] = (height >> 8) & 0xFF;
    ihdr[7] = height & 0xFF;
    ihdr[8] = 8;      // bit depth
    ihdr[9] = 2;      // color type = truecolor RGB
    ihdr[10] = 0;     // compression method
    ihdr[11] = 0;     // filter method
    ihdr[12] = 0;     // interlace method
    write_chunk(fp, "IHDR", ihdr, 13);

    int row_bytes = 3 * width;
    int raw_data_size = (row_bytes + 1) * height;
    uint8_t *raw_data = malloc(raw_data_size);
    if (!raw_data) {
        fclose(fp);
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    for (int y = 0; y < height; y++) {
        raw_data[y*(row_bytes+1)] = 0; // filter type 0 (no filter)
        memcpy(&raw_data[y*(row_bytes+1) + 1], &rgb[y*row_bytes], row_bytes);
    }

    int idat_data_size = 0;
    uint8_t* idat_data = create_zlib_no_compression_blocks(raw_data, raw_data_size, &idat_data_size);
    if (!idat_data) {
        free(raw_data);
        fclose(fp);
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    write_chunk(fp, "IDAT", idat_data, idat_data_size);

    write_chunk(fp, "IEND", NULL, 0);

    fclose(fp);
    free(raw_data);
    free(idat_data);
}


void save_png1(const char *filename, uint8_t *rgb, int width, int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return;
    }

    // PNG signature
    uint8_t pngsig[8] = {137,80,78,71,13,10,26,10};
    fwrite(pngsig, 1, 8, fp);

    // IHDR chunk
    uint8_t ihdr[13];
    ihdr[0] = (width >> 24) & 0xFF;
    ihdr[1] = (width >> 16) & 0xFF;
    ihdr[2] = (width >> 8) & 0xFF;
    ihdr[3] = width & 0xFF;
    ihdr[4] = (height >> 24) & 0xFF;
    ihdr[5] = (height >> 16) & 0xFF;
    ihdr[6] = (height >> 8) & 0xFF;
    ihdr[7] = height & 0xFF;
    ihdr[8] = 8;      // bit depth
    ihdr[9] = 2;      // color type = truecolor RGB
    ihdr[10] = 0;     // compression method
    ihdr[11] = 0;     // filter method
    ihdr[12] = 0;     // interlace method
    write_chunk(fp, "IHDR", ihdr, 13);

    // Tạo buffer chứa dữ liệu IDAT (zlib no compression block)
    // Mỗi scanline bắt đầu bằng 1 byte filter = 0
    int row_bytes = 3 * width;
    int raw_data_size = (row_bytes + 1) * height; // mỗi dòng thêm 1 byte filter
    uint8_t *raw_data = malloc(raw_data_size);
    if (!raw_data) {
        fclose(fp);
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    for (int y = 0; y < height; y++) {
        raw_data[y*(row_bytes+1)] = 0; // filter type 0 (no filter)
        memcpy(&raw_data[y*(row_bytes+1) + 1], &rgb[y*row_bytes], row_bytes);
    }

    // Tạo buffer nén zlib (không nén) với header zlib 2 bytes + block no compression
    // Zlib header 0x78 0x01 (no compression, fastest)
    // Block format: 1 byte final block flag + block type (0 = no compression)
    // 2 bytes LEN + 2 bytes NLEN (ones complement of LEN)
    // followed by LEN bytes data

    int zlib_overhead = 7;
    int idat_data_size = raw_data_size + zlib_overhead;
    uint8_t *idat_data = malloc(idat_data_size);
    if (!idat_data) {
        fclose(fp);
        free(raw_data);
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    idat_data[0] = 0x78; // zlib CMF
    idat_data[1] = 0x01; // zlib FLG (no compression, check bits)

    // One single no compression block with full raw data
    idat_data[2] = 0x01; // final block, no compression
    idat_data[3] = raw_data_size & 0xFF;           // LEN low byte
    idat_data[4] = (raw_data_size >> 8) & 0xFF;    // LEN high byte
    idat_data[5] = (~raw_data_size) & 0xFF;        // NLEN low byte
    idat_data[6] = ((~raw_data_size) >> 8) & 0xFF; // NLEN high byte

    memcpy(&idat_data[7], raw_data, raw_data_size);

    // Viết chunk IDAT
    write_chunk(fp, "IDAT", idat_data, idat_data_size);

    // Viết chunk IEND
    write_chunk(fp, "IEND", NULL, 0);

    fclose(fp);
    free(raw_data);
    free(idat_data);
}

#define WIDTH 640
#define HEIGHT 480

int main() {
    make_crc_table();


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

    save_png1("output1.png", rgb_buffer, WIDTH, HEIGHT);
    save_png("output.png", rgb_buffer, WIDTH, HEIGHT);

    free(yuyv_buffer);
    free(rgb_buffer);
    printf("Saved to output.png\n");

    // int width = 4;
    // int height = 4;
    // uint8_t rgb[4*4*3] = {
    //     // 4x4 pixels RGB
    //     255,0,0,    0,255,0,    0,0,255,    255,255,0,
    //     255,0,255,  0,255,255,  192,192,192,128,128,128,
    //     0,0,0,      255,255,255,128,0,0,    0,128,0,
    //     0,0,128,    128,128,0,  0,128,128,  128,0,128
    // };

    // save_png_minimal("test.png", rgb, width, height);
    // printf("test.png generated\n");
    return 0;
}
