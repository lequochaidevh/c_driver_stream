#!/usr/bin/bash


mkdir -p ./build/run/mpa

mkdir -p ./build/run/mpa/sub1
mkdir -p ./build/run/mpa/sub2

gcc mpa_pub_full.c -o ./build/mpa_pub
gcc mpa_sub_full.c -o ./build/mpa_sub

# chạy publisher
gnome-terminal -- bash -c "cd build/ && ./mpa_pub; exec bash"

# chạy subscriber
gnome-terminal -- bash -c "cd build/ && ./mpa_sub ./run/mpa/sub1; exec bash"
sleep 0.5
gnome-terminal -- bash -c "cd build/ && ./mpa_sub ./run/mpa/sub2; exec bash"

sleep 30 && ./clean_all.sh