#!/usr/bin/bash

mkdir build
gcc server_tcp.c -o ./build/server
gcc client_tcp.c -o ./build/client

gnome-terminal -- bash -c "./build/server; exec bash"
read -t 1 -p "I will wait for 2 seconds..."
./build/client 127.0.0.1 8080