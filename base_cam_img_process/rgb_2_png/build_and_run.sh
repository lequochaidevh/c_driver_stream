#!/usr/bin/bash
# sudo apt-get install libpng-dev

mkdir -p ./build
cp frame.raw ./build
# gcc rgb2png.c -o ./build/rgb2png -lz
gcc rgb2pngNocompress.c -o ./build/rgb2png

gnome-terminal -- bash -c "cd build/ && ./rgb2png && feh output.png; exec bash"

sleep 30 && ./clean_all.sh