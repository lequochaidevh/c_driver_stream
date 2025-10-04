#!/usr/bin/bash

mkdir build
gcc server_multi_adv.c helper.c -o ./build/server -lpthread
gcc chat_client.c helper.c -o ./build/client -lpthread

gnome-terminal -- bash -c "./build/server; exec bash"
gnome-terminal -- bash -c "./build/client; exec bash"
gnome-terminal -- bash -c "./build/client; exec bash"
gnome-terminal -- bash -c "./build/client; exec bash"
