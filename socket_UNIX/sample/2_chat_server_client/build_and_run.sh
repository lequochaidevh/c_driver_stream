#!/usr/bin/bash

mkdir build
gcc chat_server.c helper.c -o ./build/server -lpthread
gcc chat_client.c helper.c -o ./build/client -lpthread

gnome-terminal -- bash -c "./build/server; exec bash"
./build/client