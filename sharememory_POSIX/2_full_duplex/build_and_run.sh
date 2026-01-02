#!/usr/bin/bash

mkdir -p ./build/
gcc process_a.c ipc.c -o ./build/process_a -lrt -pthread
gcc process_b.c ipc.c -o ./build/process_b -lrt -pthread


gnome-terminal -- bash -c "cd build/ && ./process_a; exec bash"

sleep 5
gnome-terminal -- bash -c "cd build/ && ./process_b; exec bash"

# sleep 30 && ./clean_all.sh

# rm /dev/shm/my_shm
