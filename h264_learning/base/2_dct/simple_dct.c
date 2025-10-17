// simple_dct.c
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define N 8
// Discrete Cosine Transform
// Hàm tính DCT cho khối 8x8
void dct8x8(double input[N][N], double output[N][N]) {
    double cu, cv, sum;
    for (int u = 0; u < N; u++) {
        for (int v = 0; v < N; v++) {
            cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
            cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
            sum = 0.0;
            for (int x = 0; x < N; x++) {
                for (int y = 0; y < N; y++) {
                    sum += input[x][y] *
                           cos((2 * x + 1) * u * M_PI / 16.0) *
                           cos((2 * y + 1) * v * M_PI / 16.0);
                }
            }
            output[u][v] = 0.25 * cu * cv * sum;
        }
    }
}

// Ma trận lượng tử hóa
int Q[8][8] = {
 {16,11,10,16,24,40,51,61},
 {12,12,14,19,26,58,60,55},
 {14,13,16,24,40,57,69,56},
 {14,17,22,29,51,87,80,62},
 {18,22,37,56,68,109,103,77},
 {24,35,55,64,81,104,113,92},
 {49,64,78,87,103,121,120,101},
 {72,92,95,98,112,100,103,99}
};

// Hàm lượng tử hóa
void quantize(double input[N][N], int output[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            output[i][j] = round(input[i][j] / Q[i][j]);
}

int dtc_main() {
    double block[8][8] = {
        {52,55,61,66,70,61,64,73},
        {63,59,66,90,109,85,69,72},
        {62,59,68,113,144,104,66,73},
        {63,58,71,122,154,106,70,69},
        {67,61,68,104,126,88,68,70},
        {79,65,60,70,77,68,58,75},
        {85,71,64,59,55,61,65,83},
        {87,79,69,68,65,76,78,94}
    };

    double dctBlock[8][8];
    int quantized[8][8];

    dct8x8(block, dctBlock);
    quantize(dctBlock, quantized);

    printf("🔹 Hệ số DCT:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%8.2f ", dctBlock[i][j]);
        printf("\n");
    }

    printf("\n🔸 Sau lượng tử hóa:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%4d ", quantized[i][j]);
        printf("\n");
    }

    return 0;
}

// Intra/Inter
// motion vector between two frame
// motion_sad.c
int SAD(unsigned char *a, unsigned char *b, int w, int h) {
    int sum = 0;
    for (int i = 0; i < w*h; i++)
        sum += abs(a[i] - b[i]);
    return sum;
}


int main() {

    dtc_main();
}