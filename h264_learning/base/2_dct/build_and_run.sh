#!/usr/bin/bash

mkdir -p ./build
cp *.yuv *.pgm build
gcc pratice_dct.c -o ./build/pratice_dct -lm

gnome-terminal -- bash -c "cd build/ && ./pratice_dct && xdg-open .; exec bash"

sleep 30 && ./clean_all.sh