from PIL import Image
import sys

# Ký tự ASCII từ đậm → nhạt (không có khoảng trắng)
ASCII_CHARS = "@#%8&WM$B0QOZ[]{}()/?!|*+=-;:,."

def pixel_to_ascii(pixel):
    r, g, b = pixel
    brightness = int((r + g + b) / 3)
    index = int((brightness / 255) * (len(ASCII_CHARS) - 1))
    char = ASCII_CHARS[index]

    # ANSI 24-bit color
    return f"\033[38;2;{r};{g};{b}m{char}\033[0m"

def image_to_ascii(path, new_width=120):
    img = Image.open(path).convert("RGB")

    # Tỉ lệ ngang/dọc (font monospace cao hơn)
    width = 1920
    height = 1090
    # width, height = img.size
    aspect_ratio = height/width * 0.5
    new_height = int(new_width * aspect_ratio)
    img = img.resize((new_width, new_height))

    ascii_art = []
    for y in range(new_height):
        line = "".join(pixel_to_ascii(img.getpixel((x, y))) for x in range(new_width))
        ascii_art.append(line)

    return "\n".join(ascii_art)

if __name__ == "__main__":
    path = "resized.png"
    print(image_to_ascii(path))
