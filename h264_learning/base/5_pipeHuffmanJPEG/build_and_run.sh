#!/usr/bin/bash

mkdir -p ./build
cp asset/* build
gcc compressHuffman.c -o ./build/compressHuffman -lm
gcc decode.c -lm -O2 -o ./build/decoder_from_huff
gnome-terminal -- bash -c "cd build/ && ./compressHuffman && sleep 1 && ./decoder_from_huff && xdg-open .; exec bash"

# sleep 30 && ./clean_all.sh