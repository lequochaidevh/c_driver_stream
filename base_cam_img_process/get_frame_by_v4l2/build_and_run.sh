#!/usr/bin/bash
# sudo apt-get install libpng-dev

mkdir -p ./build

gcc capture_frame.c -o ./build/capture -lpng

gnome-terminal -- bash -c "cd build/ && ./capture && feh frame.png; exec bash"

# ffmpeg -f rawvideo -pix_fmt yuyv422 -s 640x480 -i frame.raw frame.png

sleep 30 && ./clean_all.sh