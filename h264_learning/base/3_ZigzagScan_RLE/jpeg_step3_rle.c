#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define WIDTH 176
#define HEIGHT 144
#define BLOCK 8

// Ma trận lượng tử hóa tiêu chuẩn (dạng đơn giản)
int Q[8][8] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}
};

// === DCT 8×8 ===
void dct8x8(double in[BLOCK][BLOCK], double out[BLOCK][BLOCK]) {
    for (int u = 0; u < BLOCK; u++)
        for (int v = 0; v < BLOCK; v++) {
            double cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
            double cv = (v == 0) ? 1.0 / sqrt(2.0) : 1.0;
            double sum = 0.0;
            for (int x = 0; x < BLOCK; x++)
                for (int y = 0; y < BLOCK; y++)
                    sum += in[x][y] *
                           cos((2 * x + 1) * u * M_PI / 16.0) *
                           cos((2 * y + 1) * v * M_PI / 16.0);
            out[u][v] = 0.25 * cu * cv * sum;
        }
}

// === IDCT 8×8 ===
void idct8x8(double in[BLOCK][BLOCK], double out[BLOCK][BLOCK]) {
    for (int x = 0; x < BLOCK; x++)
        for (int y = 0; y < BLOCK; y++) {
            double sum = 0.0;
            for (int u = 0; u < BLOCK; u++)
                for (int v = 0; v < BLOCK; v++) {
                    double cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
                    double cv = (v == 0) ? 1.0 / sqrt(2.0) : 1.0;
                    sum += cu * cv * in[u][v] *
                           cos((2 * x + 1) * u * M_PI / 16.0) *
                           cos((2 * y + 1) * v * M_PI / 16.0);
                }
            out[x][y] = 0.25 * sum;
        }
}

// === Lượng tử hóa / Giải lượng tử hóa ===
void quantize(double in[BLOCK][BLOCK], int out[BLOCK][BLOCK]) {
    for (int i = 0; i < BLOCK; i++)
        for (int j = 0; j < BLOCK; j++)
            out[i][j] = round(in[i][j] / Q[i][j]);
}

void dequantize(int in[BLOCK][BLOCK], double out[BLOCK][BLOCK]) {
    for (int i = 0; i < BLOCK; i++)
        for (int j = 0; j < BLOCK; j++)
            out[i][j] = in[i][j] * Q[i][j];
}

// === Zigzag Scan ===
int zigzag_index[64][2] = {
    {0,0},{0,1},{1,0},{2,0},{1,1},{0,2},{0,3},{1,2},
    {2,1},{3,0},{4,0},{3,1},{2,2},{1,3},{0,4},{0,5},
    {1,4},{2,3},{3,2},{4,1},{5,0},{6,0},{5,1},{4,2},
    {3,3},{2,4},{1,5},{0,6},{0,7},{1,6},{2,5},{3,4},
    {4,3},{5,2},{6,1},{7,0},{7,1},{6,2},{5,3},{4,4},
    {3,5},{2,6},{1,7},{2,7},{3,6},{4,5},{5,4},{6,3},
    {7,2},{7,3},{6,4},{5,5},{4,6},{3,7},{4,7},{5,6},
    {6,5},{7,4},{7,5},{6,6},{5,7},{6,7},{7,6},{7,7}
};

void zigzagScan(int block[8][8], int output[64]) {
    for (int i = 0; i < 64; i++) {
        int x = zigzag_index[i][0];
        int y = zigzag_index[i][1];
        output[i] = block[x][y];
    }
}

// === Run-Length Encoding ===
void runLengthEncode(int zz[64], FILE *csv, int bx, int by) {
    fprintf(csv, "Block (%d,%d)\nrun,value\n", bx, by);
    int run = 0;
    for (int i = 1; i < 64; i++) { // bỏ qua DC
        if (zz[i] == 0)
            run++;
        else {
            fprintf(csv, "%d,%d\n", run, zz[i]);
            run = 0;
        }
    }
    fprintf(csv, "EOB,0\n\n");
}

// === Ghép lại frame ===
void reconstructFrame(unsigned char *recon, double blocks[][BLOCK][BLOCK],
                      int width, int height) {
    int bxCount = width / BLOCK, byCount = height / BLOCK;
    for (int by = 0; by < byCount; by++)
        for (int bx = 0; bx < bxCount; bx++) {
            double *bptr = (double *)blocks[by * bxCount + bx];
            for (int i = 0; i < BLOCK; i++)
                for (int j = 0; j < BLOCK; j++) {
                    int x = bx * BLOCK + j;
                    int y = by * BLOCK + i;
                    int val = (int)(bptr[i * BLOCK + j] + 128.0);
                    if (val < 0) val = 0;
                    if (val > 255) val = 255;
                    recon[y * width + x] = (unsigned char)val;
                }
        }
}

// === Heatmap DC ===
void generateDCHeatmapPGM(int dcmap[][WIDTH / BLOCK], int bxCount, int byCount) {
    FILE *f = fopen("dc_heatmap.pgm", "wb");
    fprintf(f, "P5\n%d %d\n255\n", bxCount, byCount);
    for (int y = 0; y < byCount; y++)
        for (int x = 0; x < bxCount; x++) {
            int val = dcmap[y][x] + 128;
            if (val < 0) val = 0;
            if (val > 255) val = 255;
            fputc(val, f);
        }
    fclose(f);
}

int main() {
    FILE *fy = fopen("frame0_Y.pgm", "rb");
    if (!fy) { perror("frameY"); return 1; }

    unsigned char *Y = malloc(WIDTH * HEIGHT);
    fread(Y, 1, WIDTH * HEIGHT, fy);
    fclose(fy);

    int bxCount = WIDTH / BLOCK, byCount = HEIGHT / BLOCK;
    double block[BLOCK][BLOCK], dct[BLOCK][BLOCK], idct[BLOCK][BLOCK];
    int qblock[BLOCK][BLOCK], dcmap[HEIGHT / BLOCK][WIDTH / BLOCK];
    double allBlocks[bxCount * byCount][BLOCK][BLOCK];

    FILE *rle_csv = fopen("zigzag_rle.csv", "w");

    for (int by = 0; by < byCount; by++)
        for (int bx = 0; bx < bxCount; bx++) {
            // Cắt block 8x8
            for (int i = 0; i < BLOCK; i++)
                for (int j = 0; j < BLOCK; j++)
                    block[i][j] = (double)Y[(by * BLOCK + i) * WIDTH + (bx * BLOCK + j)] - 128.0;

            // DCT + Quantize
            dct8x8(block, dct);
            quantize(dct, qblock);
            dcmap[by][bx] = qblock[0][0];

            // Zigzag + RLE
            int zz[64];
            zigzagScan(qblock, zz);
            runLengthEncode(zz, rle_csv, bx, by);

            // Giải lượng tử + IDCT
            double deq[BLOCK][BLOCK];
            dequantize(qblock, deq);
            idct8x8(deq, idct);

            // Lưu lại để ghép frame
            for (int i = 0; i < BLOCK; i++)
                for (int j = 0; j < BLOCK; j++)
                    allBlocks[by * bxCount + bx][i][j] = idct[i][j];
        }

    fclose(rle_csv);

    // Ghép frame tái tạo
    unsigned char *recon = malloc(WIDTH * HEIGHT);
    reconstructFrame(recon, allBlocks, WIDTH, HEIGHT);

    FILE *fout = fopen("reconstructed.pgm", "wb");
    fprintf(fout, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(recon, 1, WIDTH * HEIGHT, fout);
    fclose(fout);

    generateDCHeatmapPGM(dcmap, bxCount, byCount);

    free(Y);
    free(recon);
    printf("✅ Output:\n - reconstructed.pgm\n - dc_heatmap.pgm\n - zigzag_rle.csv\n");
    return 0;
}
