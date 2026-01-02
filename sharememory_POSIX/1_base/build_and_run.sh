#!/usr/bin/bash

mkdir -p ./build/
gcc writer.c -o ./build/writer -lrt -pthread
gcc reader.c -o ./build/reader -lrt -pthread

gnome-terminal -- bash -c "cd build/ && ./writer; exec bash"

# sleep 0.5 # It ok
sleep 5 # issue: miss front data if not sync

gnome-terminal -- bash -c "cd build/ && ./reader; exec bash"

# sleep 30 && ./clean_all.sh

# rm /dev/shm/my_shm
