#!/usr/bin/bash

mkdir -p build/client
echo "test send and recive file" > build/client/send_me.txt

cp -rf img_test.png build/client
cp -rf test_video.mp4 build/client

gcc server_receive.c -o ./build/server -lpthread
gcc client_send.c -o ./build/client/client -lpthread

gnome-terminal -- bash -c "cd build/ && ./server; exec bash"
sleep 2 # to watch server open and client conect
gnome-terminal -- bash -c "cd build/client/ && ./client; exec bash"
