// read_yuv_info.c
// Y (luminance) and UV (chrominance).
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>

int view_pgm_gray() {
    const char *filename = "foreman_qcif.yuv"; //
    int width = 176;
    int height = 144;

    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Cannot open file");
        return 1;
    }

    // Calc size of frame
    long frame_size = width * height * 3 / 2;
    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    int num_frames = filesize / frame_size;
    rewind(f);

    printf("File: %s\n", filename);
    printf("Resolution: %dx%d\n", width, height);
    printf("Frame size: %ld bytes\n", frame_size);
    printf("Total frames: %d\n", num_frames);

    // Read first frame
    unsigned char *Y = malloc(width * height);
    unsigned char *U = malloc(width * height / 4);
    unsigned char *V = malloc(width * height / 4);
    if (!Y || !U || !V) {
        printf("Memory allocation failed\n");
        return 1;
    }

    size_t readY = fread(Y, 1, width * height, f);
    size_t readU = fread(U, 1, width * height / 4, f);
    size_t readV = fread(V, 1, width * height / 4, f);

    printf("Read Y: %zu bytes, U: %zu, V: %zu\n", readY, readU, readV);

    // Write frame Y to file PGM to check
    FILE *pgm = fopen("frame0_Y.pgm", "wb");
    fprintf(pgm, "P5\n%d %d\n255\n", width, height);
    fwrite(Y, 1, width * height, pgm);
    fclose(pgm);

    printf("Saved Y plane as frame0_Y.pgm (grayscale image)\n");

    free(Y); free(U); free(V);
    fclose(f);
    return 0;
}

// --- HÀM 1: Ghi mặt phẳng ra file PGM ---
void save_plane_to_pgm(const char *filename, unsigned char *data, int w, int h) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("Cannot create file");
        return;
    }
    fprintf(f, "P5\n%d %d\n255\n", w, h);
    fwrite(data, 1, w * h, f);
    fclose(f);
}

// --- HÀM 2: Lấy giá trị Y tại (x, y) ---
unsigned char get_pixel_Y(unsigned char *Y, int x, int y, int width, int height) {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        printf("⚠️  Out of range (%d,%d)\n", x, y);
        return 0;
    }
    return Y[y * width + x];
}

int minor() {
    const char *filename = "foreman_qcif.yuv"; //
    int width = 176, height = 144;

    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Cannot open file");
        return 1;
    }

    long frame_size = width * height * 3 / 2;
    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    int num_frames = filesize / frame_size;
    rewind(f);

    printf("File: %s\n", filename);
    printf("Resolution: %dx%d\n", width, height);
    printf("Total frames: %d\n", num_frames);

    int frame_index;
    printf("Enter frame index (0 - %d): ", num_frames - 1);
    scanf("%d", &frame_index);
    if (frame_index < 0 || frame_index >= num_frames) {
        printf("Invalid frame index!\n");
        return 1;
    }

    // Nhảy đến frame cần đọc
    fseek(f, (long)frame_index * frame_size, SEEK_SET);

    unsigned char *Y = malloc(width * height);
    unsigned char *U = malloc(width * height / 4);
    unsigned char *V = malloc(width * height / 4);

    fread(Y, 1, width * height, f);
    fread(U, 1, width * height / 4, f);
    fread(V, 1, width * height / 4, f);
    fclose(f);

    // --- GHI CÁC FILE PGM ---
    char filename_Y[64], filename_U[64], filename_V[64];
    sprintf(filename_Y, "frame%d_Y.pgm", frame_index);
    sprintf(filename_U, "frame%d_U.pgm", frame_index);
    sprintf(filename_V, "frame%d_V.pgm", frame_index);

    save_plane_to_pgm(filename_Y, Y, width, height);
    save_plane_to_pgm(filename_U, U, width / 2, height / 2);
    save_plane_to_pgm(filename_V, V, width / 2, height / 2);

    printf("Saved %s, %s, %s\n", filename_Y, filename_U, filename_V);

    // --- HIỂN THỊ GIÁ TRỊ PIXEL Y ---
    int x, y;
    printf("Enter pixel position (x y): ");
    scanf("%d %d", &x, &y);
    unsigned char value = get_pixel_Y(Y, x, y, width, height);
    printf("Y(%d,%d) = %u\n", x, y, value);

    free(Y); free(U); free(V);
    return 0;
}

static inline unsigned char clip(double x) {
    if (x < 0) return 0;
    if (x > 255) return 255;
    return (unsigned char)x;
}

int yyuv420_to_rgb_pixel() {
    const char *filename = "foreman_qcif.yuv";
    int width = 176, height = 144;
    int frame_index = 0;

    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Cannot open file");
        return 1;
    }

    long frame_size = width * height * 3 / 2;
    fseek(f, frame_index * frame_size, SEEK_SET);

    unsigned char *Y = malloc(width * height);
    unsigned char *U = malloc(width * height / 4);
    unsigned char *V = malloc(width * height / 4);

    fread(Y, 1, width * height, f);
    fread(U, 1, width * height / 4, f);
    fread(V, 1, width * height / 4, f);
    fclose(f);

    // --- YUV -> RGB ---
    unsigned char *RGB = malloc(width * height * 3);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int Y_index = y * width + x;
            int U_index = (y / 2) * (width / 2) + (x / 2);
            int V_index = U_index;

            double Yval = Y[Y_index];
            double Uval = U[U_index];
            double Vval = V[V_index];

            double R = Yval + 1.402 * (Vval - 128);
            double G = Yval - 0.344136 * (Uval - 128) - 0.714136 * (Vval - 128);
            double B = Yval + 1.772 * (Uval - 128);

            RGB[(Y_index * 3) + 0] = clip(R);
            RGB[(Y_index * 3) + 1] = clip(G);
            RGB[(Y_index * 3) + 2] = clip(B);
        }
    }

    // --- Ghi file PPM ---
    FILE *ppm = fopen("frame0_RGB.ppm", "wb");
    fprintf(ppm, "P6\n%d %d\n255\n", width, height);
    fwrite(RGB, 1, width * height * 3, ppm);
    fclose(ppm);

    printf("Saved color frame as frame0_RGB.ppm\n");

    free(Y); free(U); free(V); free(RGB);
    return 0;
}

int read10frame_YUV2RBG() {
    const char *filename = "foreman_qcif.yuv"; //
    int width = 176, height = 144;
    int num_frames_to_export = 10;

    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Cannot open file");
        return 1;
    }

    long frame_size = width * height * 3 / 2;

    unsigned char *Y = malloc(width * height);
    unsigned char *U = malloc(width * height / 4);
    unsigned char *V = malloc(width * height / 4);
    unsigned char *RGB = malloc(width * height * 3);

    if (!Y || !U || !V || !RGB) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int frame_idx = 0; frame_idx < num_frames_to_export; frame_idx++) {
        // Đọc frame
        fseek(f, frame_idx * frame_size, SEEK_SET);
        size_t readY = fread(Y, 1, width * height, f);
        size_t readU = fread(U, 1, width * height / 4, f);
        size_t readV = fread(V, 1, width * height / 4, f);

        if (readY != width * height) {
            printf("⚠️  End of file reached at frame %d\n", frame_idx);
            break;
        }

        // --- Chuyển YUV -> RGB ---

        // for (int y = 0; y < height; y+=4) {
        //     for (int x = 0; x < width; x+=4) {

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int Y_index = y * width + x;
                int U_index = (y / 2) * (width / 2) + (x / 2);
                int V_index = U_index;

                double Yval = Y[Y_index];
                double Uval = U[U_index];
                double Vval = V[V_index];

                double R = Yval + 1.402 * (Vval - 128);
                double G = Yval - 0.344136 * (Uval - 128) - 0.714136 * (Vval - 128);
                double B = Yval + 1.772 * (Uval - 128);
                
                unsigned char clipR = clip(R);
                unsigned char clipG = clip(G);
                unsigned char clipB = clip(B);

                RGB[Y_index * 3 + 0] = clipR;
                RGB[Y_index * 3 + 1] = clipG;
                RGB[Y_index * 3 + 2] = clipB;

                // RGB[((y+1) * width + x+1) * 3 + 0] = clipR;
                // RGB[((y+1) * width + x+1) * 3 + 1] = clipG;
                // RGB[((y+1) * width + x+1) * 3 + 2] = clipB;

                // RGB[((y+2) * width + x+2) * 3 + 0] = clipR;
                // RGB[((y+2) * width + x+2) * 3 + 1] = clipG;
                // RGB[((y+2) * width + x+2) * 3 + 2] = clipB;

                // RGB[((y+3) * width + x+3) * 3 + 0] = clipR;
                // RGB[((y+3) * width + x+3) * 3 + 1] = clipG;
                // RGB[((y+3) * width + x+3) * 3 + 2] = clipB;

            }
        }

        // --- Ghi file PPM ---
        char out_name[64];
        sprintf(out_name, "frame%d_RGB.ppm", frame_idx);

        FILE *ppm = fopen(out_name, "wb");
        if (!ppm) {
            perror("Cannot create PPM file");
            break;
        }
        fprintf(ppm, "P6\n%d %d\n255\n", width, height);
        fwrite(RGB, 1, width * height * 3, ppm);
        fclose(ppm);

        printf("Saved %s\n", out_name);
    }

    free(Y); free(U); free(V); free(RGB);
    fclose(f);
    printf("\n Done! Exported up to %d frames.\n", num_frames_to_export);
    // create video ffmpeg -framerate 25 -i frame%d_RGB.ppm -c:v libx264 -pix_fmt yuv420p preview.mp4

    return 0;
}

int main() {
    int width1 = 640, height1 = 480;
    FILE *f = fopen("foreman_qcif.yuv", "rb");
    if (!f) return printf("Cannot open file\n"), 1;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    long frame_size = width1 * height1 * 3 / 2; // 4:2:0
    printf("Frame size: %ld bytes\n", frame_size);
    printf("Total frames: %ld\n", size / frame_size);

    view_pgm_gray();
    // minor();
    // yuv420_to_rgb_pixel();
    read10frame_YUV2RBG();
    return 0;
}
