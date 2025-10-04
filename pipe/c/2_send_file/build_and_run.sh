#!/usr/bin/bash

mkdir -p ./build/run/mpa
cp img_test.png build

gcc mpa_pub_png.c -o ./build/mpa_pub && gcc mpa_sub_png.c -o ./build/mpa_sub
gnome-terminal -- bash -c "cd build && ./mpa_pub img_test.png ./run/mpa/FIFOfile; exec bash"
gnome-terminal -- bash -c "cd build && ./mpa_sub ./run/mpa/FIFOfile img_reciev.png; exec bash"




sleep 20 && ./clean_all.sh