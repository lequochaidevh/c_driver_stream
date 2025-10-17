#!/usr/bin/bash

mkdir -p ./build
cp asset/* build
gcc jpeg_step3_rle.c -o ./build/jpeg_step3_rle -lm

gnome-terminal -- bash -c "cd build/ && ./jpeg_step3_rle && xdg-open .; exec bash"

# sleep 30 && ./clean_all.sh