#!/usr/bin/bash

mkdir -p ./build
cp asset/* build
gcc huffmanBase.c -o ./build/huffmanBase -lm

gnome-terminal -- bash -c "cd build/ && ./huffmanBase && xdg-open .; exec bash"

# sleep 30 && ./clean_all.sh