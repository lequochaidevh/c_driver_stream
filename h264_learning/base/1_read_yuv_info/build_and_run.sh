#!/usr/bin/bash
# sudo apt-get install libpng-dev

mkdir -p ./build
cp foreman_qcif.yuv ./build

gcc read_yuv_info.c -o ./build/read_yuv_info

gnome-terminal -- bash -c "cd build/ && ./read_yuv_info && xdg-open . ; exec bash"

sleep 30 && ./clean_all.sh