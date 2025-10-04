#!/usr/bin/bash


mkdir -p ./build/run/mpa

# gcc pipe_basic.c -o ./build/pipe -lpthread

# gnome-terminal -- bash -c "./build/pipe; exec bash"


gcc mpa_pub.c -o ./build/mpa_pub && gcc mpa_sub.c -o ./build/mpa_sub
gnome-terminal -- bash -c "cd build && ./mpa_pub ./run/mpa/hellofile hellopipeFIFO; exec bash"

sleep 20 && ./clean_all.sh