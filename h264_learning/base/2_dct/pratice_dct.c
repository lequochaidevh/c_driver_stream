#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define WIDTH 176
#define HEIGHT 144
#define BLOCK_SIZE 8
#define BLOCK BLOCK_SIZE

#include <ctype.h>
#include <string.h>




// Trả về 1 nếu OK, 0 nếu lỗi. *out_data là malloc'ed (caller free).
int read_pgm(const char *path, unsigned char **out_data, int *out_w, int *out_h) {
    FILE *f = fopen(path, "rb");
    if (!f) return 0;

    char magic[3] = {0};
    if (fscanf(f, "%2s", magic) != 1) { fclose(f); return 0; }
    if (strcmp(magic, "P5") != 0) { fclose(f); return 0; }

    // helper: đọc số tiếp theo, bỏ comment (#...) và whitespace
    int read_int(FILE *fp, int *val) {
        int c;
        // bỏ whitespace và comment
        while ((c = fgetc(fp)) != EOF) {
            if (isspace(c)) continue;
            if (c == '#') { // comment: bỏ cả dòng
                while ((c = fgetc(fp)) != EOF && c != '\n');
                continue;
            }
            ungetc(c, fp);
            break;
        }
        if (fscanf(fp, "%d", val) != 1) return 0;
        return 1;
    };

    int w = 0, h = 0, maxv = 0;
    if (!read_int(f, &w) || !read_int(f, &h) || !read_int(f, &maxv)) {
        fclose(f); return 0;
    }

    // 1 byte per pixel expected for maxv <= 255
    if (maxv > 255) { fclose(f); return 0; }

    // consume one single byte (usually newline) before binary data if needed
    int c = fgetc(f);
    if (c == '\r') { // maybe \r\n
        int c2 = fgetc(f);
        if (c2 != '\n') ungetc(c2, f);
    } else if (c != '\n') {
        ungetc(c, f);
    }

    size_t len = (size_t)w * (size_t)h;
    unsigned char *buf = malloc(len);
    if (!buf) { fclose(f); return 0; }

    size_t got = fread(buf, 1, len, f);
    fclose(f);
    if (got != len) { free(buf); return 0; }

    *out_data = buf;
    *out_w = w;
    *out_h = h;
    return 1;
}

// Ma trận lượng tử hóa đơn giản
int Q[8][8] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}};

// DCT 8x8
void dct8x8(double input[BLOCK_SIZE][BLOCK_SIZE], double output[BLOCK_SIZE][BLOCK_SIZE])
{
    double cu, cv, sum;
    for (int u = 0; u < BLOCK_SIZE; u++)
    {
        for (int v = 0; v < BLOCK_SIZE; v++)
        {
            cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
            cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
            sum = 0.0;
            for (int x = 0; x < BLOCK_SIZE; x++)
            {
                for (int y = 0; y < BLOCK_SIZE; y++)
                {
                    sum += input[x][y] *
                           cos((2 * x + 1) * u * M_PI / 16.0) *
                           cos((2 * y + 1) * v * M_PI / 16.0);
                }
            }
            output[u][v] = 0.25 * cu * cv * sum;
        }
    }
}

// Lượng tử hóa
void quantize(double input[BLOCK_SIZE][BLOCK_SIZE], int output[BLOCK_SIZE][BLOCK_SIZE])
{
    for (int i = 0; i < BLOCK_SIZE; i++)
        for (int j = 0; j < BLOCK_SIZE; j++)
            output[i][j] = round(input[i][j] / Q[i][j]);
}

// DCT → Quantization → Inverse Quantization → IDCT → tái tạo ảnh.

// 🔄 Inverse DCT 8×8
void idct8x8(double in[BLOCK][BLOCK], double out[BLOCK][BLOCK])
{
    double cu, cv, sum;
    for (int x = 0; x < BLOCK; x++)
    {
        for (int y = 0; y < BLOCK; y++)
        {
            sum = 0.0;
            for (int u = 0; u < BLOCK; u++)
            {
                for (int v = 0; v < BLOCK; v++)
                {
                    cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
                    cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
                    sum += cu * cv * in[u][v] *
                           cos((2 * x + 1) * u * M_PI / 16.0) *
                           cos((2 * y + 1) * v * M_PI / 16.0);
                }
            }
            out[x][y] = 0.25 * sum;
        }
    }
}

void dequantize(int in[BLOCK][BLOCK], double out[BLOCK][BLOCK])
{
    for (int i = 0; i < BLOCK; i++)
        for (int j = 0; j < BLOCK; j++)
            out[i][j] = in[i][j] * Q[i][j];
}

// 📊 Ghi DCT block ra file CSV
void saveBlockDCTtoCSV(double dct[BLOCK][BLOCK], FILE *f, int bx, int by)
{
    fprintf(f, "Block (%d,%d)\n", bx, by);
    for (int i = 0; i < BLOCK; i++)
    {
        for (int j = 0; j < BLOCK; j++)
            fprintf(f, "%.2f,", dct[i][j]);
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
}

// 🔁 Ghép frame sau IDCT
void reconstructFrame(unsigned char *recon, double blocks[][BLOCK][BLOCK],
                      int width, int height)
{
    int bxCount = width / BLOCK;
    int byCount = height / BLOCK;
    for (int by = 0; by < byCount; by++)
    {
        for (int bx = 0; bx < bxCount; bx++)
        {
            double *bptr = (double *)blocks[by * bxCount + bx];
            
            for (int i = 0; i < BLOCK; i++)
                for (int j = 0; j < BLOCK; j++)
                {
                    int x = bx * BLOCK + j;
                    int y = by * BLOCK + i;
                    int val = (int)(bptr[i * BLOCK + j] + 128.0);
                    if (val < 0)
                        val = 0;
                    if (val > 255)
                        val = 255;
                    recon[y * width + x] = (unsigned char)val;
                }
        }
    }
}

// 🧯 Heatmap DC → ảnh PGM
void generateDCHeatmapPGM(int dc_values[][WIDTH / BLOCK], int bxCount, int byCount)
{
    FILE *f = fopen("dc_heatmap.pgm", "wb");
    fprintf(f, "P5\n%d %d\n255\n", bxCount, byCount);
    for (int y = 0; y < byCount; y++)
        for (int x = 0; x < bxCount; x++)
        {
            int val = dc_values[y][x] + 128;

            if (val < 0)
                val = 0;
            if (val > 255)
                val = 255;
            fputc((unsigned char)val, f);
        }
    fclose(f);
}

// Đọc frame Y từ file YUV420
unsigned char *readYFrame(const char *filename, int width, int height, int frame_index)
{
    FILE *f = fopen(filename, "rb");
    if (!f)
    {
        perror("open yuv");
        return NULL;
    }

    int frame_size = width * height * 3 / 2;
    fseek(f, frame_index * frame_size, SEEK_SET);

    unsigned char *Y = malloc(width * height);
    fread(Y, 1, width * height, f);
    fclose(f);
    return Y;
}

// 🧪 MAIN DEMO
int adv_main()
{
    FILE *fy = fopen("frame0_Y.pgm", "rb");
    if (!fy)
    {
        perror("frameY");
        return 1;
    }

    unsigned char *Y = malloc(WIDTH * HEIGHT);
    fread(Y, 1, WIDTH * HEIGHT, fy);
    fclose(fy);

    int bxCount = WIDTH / BLOCK, byCount = HEIGHT / BLOCK;
    double block[BLOCK][BLOCK], dct[BLOCK][BLOCK], idct[BLOCK][BLOCK];
    int qblock[BLOCK][BLOCK];
    double allBlocks[bxCount * byCount][BLOCK][BLOCK];
    int dcmap[HEIGHT / BLOCK][WIDTH / BLOCK];

    FILE *csv = fopen("dct_blocks.csv", "w");

    for (int by = 0; by < byCount; by++)
    {
        for (int bx = 0; bx < bxCount; bx++)
        {
            // Lấy block 8x8 từ frame Y
            for (int i = 0; i < BLOCK; i++)
                for (int j = 0; j < BLOCK; j++)
                    block[i][j] = (double)Y[(by * BLOCK + i) * WIDTH + (bx * BLOCK + j)] - 128.0;

            // DCT + Quantize
            dct8x8(block, dct);
            quantize(dct, qblock);
            saveBlockDCTtoCSV(dct, csv, bx, by);
            dcmap[by][bx] = qblock[0][0];

            // Dequantize + IDCT
            double deq[BLOCK][BLOCK];
            dequantize(qblock, deq);
            idct8x8(deq, idct);

            // Lưu block IDCT lại để ghép frame
            for (int i = 0; i < BLOCK; i++)
                for (int j = 0; j < BLOCK; j++)
                    allBlocks[by * bxCount + bx][i][j] = idct[i][j];
            
        }
    }
    fclose(csv);

    // Ghép lại frame
    unsigned char *recon = malloc(WIDTH * HEIGHT);
    reconstructFrame(recon, allBlocks, WIDTH, HEIGHT);

    FILE *fout = fopen("reconstructed.pgm", "wb");
    fprintf(fout, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(recon, 1, WIDTH * HEIGHT, fout);
    fclose(fout);

    // Xuất heatmap DC
    generateDCHeatmapPGM(dcmap, bxCount, byCount);

    free(Y);
    free(recon);

    printf("✅ : dct_blocks.csv, reconstructed.pgm, dc_heatmap.pgm\n");
    return 0;
}


int adv_main2()
{
    unsigned char *Y = NULL;
    int w = 0, h = 0;
    if (!read_pgm("frame0_Y.pgm", &Y, &w, &h)) {
        fprintf(stderr, "Failed to read PGM\n"); return 1;
    }

    if (w != WIDTH || h != HEIGHT) {
        fprintf(stderr, "PGM size mismatch: file %dx%d vs expected %dx%d\n", w, h, WIDTH, HEIGHT);
        free(Y); return 1;
    }

    printf("Y[0..7]: ");
    for (int i=0;i<8;i++) printf("%u ", Y[i]);
    printf("\n");

    int bxCount = WIDTH / BLOCK, byCount = HEIGHT / BLOCK;
    double block[BLOCK][BLOCK], dct[BLOCK][BLOCK], idct[BLOCK][BLOCK];
    int qblock[BLOCK][BLOCK];
    double allBlocks[bxCount * byCount][BLOCK][BLOCK];
    int dcmap[HEIGHT / BLOCK][WIDTH / BLOCK];

    FILE *csv = fopen("dct_blocks.csv", "w");

    for (int by = 0; by < byCount; by++)
    {
        for (int bx = 0; bx < bxCount; bx++)
        {
            // Lấy block 8x8 từ frame Y
            for (int i = 0; i < BLOCK; i++)
                for (int j = 0; j < BLOCK; j++)
                    block[i][j] = (double)Y[(by * BLOCK + i) * WIDTH + (bx * BLOCK + j)] - 128.0;

            // DCT + Quantize
            dct8x8(block, dct);
            quantize(dct, qblock);
            saveBlockDCTtoCSV(dct, csv, bx, by);
            dcmap[by][bx] = qblock[0][0];

            // Dequantize + IDCT
            double deq[BLOCK][BLOCK];
            dequantize(qblock, deq);
            idct8x8(deq, idct);

            // Lưu block IDCT lại để ghép frame
            for (int i = 0; i < BLOCK; i++)
                for (int j = 0; j < BLOCK; j++)
                    allBlocks[by * bxCount + bx][i][j] = idct[i][j];
            
        }
    }
    fclose(csv);

    // Ghép lại frame
    unsigned char *recon = malloc(WIDTH * HEIGHT);
    reconstructFrame(recon, allBlocks, WIDTH, HEIGHT);

    FILE *fout = fopen("reconstructed.pgm", "wb");
    fprintf(fout, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(recon, 1, WIDTH * HEIGHT, fout);
    fclose(fout);

    // Xuất heatmap DC
    generateDCHeatmapPGM(dcmap, bxCount, byCount);

    free(Y);
    free(recon);

    printf("✅ : dct_blocks.csv, reconstructed.pgm, dc_heatmap.pgm\n");
    return 0;
}

int main()
{
    // const char *filename = "foreman_qcif.yuv";
    // unsigned char *Y = readYFrame(filename, WIDTH, HEIGHT, 0);
    // if (!Y)
    //     return 1;

    // double block[BLOCK_SIZE][BLOCK_SIZE];
    // double dct[BLOCK_SIZE][BLOCK_SIZE];
    // int qblock[BLOCK_SIZE][BLOCK_SIZE];

    // int block_count_x = WIDTH / BLOCK_SIZE;
    // int block_count_y = HEIGHT / BLOCK_SIZE;

    // printf("🔹 Tổng số block: %d x %d = %d\n\n",
    //        block_count_x, block_count_y, block_count_x * block_count_y);

    // for (int by = 0; by < block_count_y; by++)
    // {
    //     for (int bx = 0; bx < block_count_x; bx++)
    //     {

    //         // 1️⃣ Trích block 8×8 từ Y
    //         for (int i = 0; i < BLOCK_SIZE; i++)
    //             for (int j = 0; j < BLOCK_SIZE; j++)
    //                 block[i][j] = (double)Y[(by * BLOCK_SIZE + i) * WIDTH + (bx * BLOCK_SIZE + j)] - 128.0;

    //         // 2️⃣ DCT + lượng tử hóa
    //         dct8x8(block, dct);
    //         quantize(dct, qblock);

    //         // 3️⃣ In hệ số DC đầu tiên (đại diện cho độ sáng trung bình block)
    //         printf("%4d ", qblock[0][0]);
    //     }
    //     printf("\n");
    // }

    // free(Y);

    adv_main2();
    return 0;
}
