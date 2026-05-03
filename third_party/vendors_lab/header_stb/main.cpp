#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

// Load file to buffer
std::vector<unsigned char> loadFile(const char* path) {
    std::ifstream file(path, std::ios::binary);
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)),
                                       std::istreambuf_iterator<char>());
}

// Overlay charactor to image RGB
void drawText(
    unsigned char* image, int imgW, int imgH,
    const char* fontPath,
    const std::string& text,
    int posX, int posY,
    float fontSize
) {
    auto fontBuffer = loadFile(fontPath);
    if (fontBuffer.empty()) {
        std::cerr << "Failed to load font!\n";
        return;
    }

    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, fontBuffer.data(), 0)) {
        std::cerr << "Init font failed!\n";
        return;
    }


    float scale = stbtt_ScaleForPixelHeight(&font, fontSize);

    int x = posX;

    for (char c : text) {
        int ax, lsb;
        stbtt_GetCodepointHMetrics(&font, c, &ax, &lsb);

        int w, h, xoff, yoff;
        unsigned char* bitmap = stbtt_GetCodepointBitmap(
            &font, 0, scale, c, &w, &h, &xoff, &yoff
        );

        // Blend bitmap to image
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                int imgX = x + i + xoff;
                int imgY = posY + j + yoff;

                if (imgX < 0 || imgY < 0 || imgX >= imgW || imgY >= imgH)
                    continue;

                unsigned char alpha = bitmap[j * w + i];

                int idx = (imgY * imgW + imgX) * 3;

                // charactor color (white)
                unsigned char r = 255, g = 255, b = 255;

                image[idx + 0] = (image[idx + 0] * (255 - alpha) + r * alpha) / 255;
                image[idx + 1] = (image[idx + 1] * (255 - alpha) + g * alpha) / 255;
                image[idx + 2] = (image[idx + 2] * (255 - alpha) + b * alpha) / 255;
            }
        }

        stbtt_FreeBitmap(bitmap, nullptr);

        x += (int)(ax * scale);
    }
}

int main() {
    const int W = 512, H = 512;

    // Black background
    std::vector<unsigned char> image(W * H * 3, 0);

    drawText(image.data(), W, H,
             "DejaVuSans.ttf",
             "Hello stb_truetype!",
             50, 200,
             48.0f);

    // Export PPM basic file 
    std::ofstream out("output.ppm", std::ios::binary);
    out << "P6\n" << W << " " << H << "\n255\n";
    out.write((char*)image.data(), image.size());
}