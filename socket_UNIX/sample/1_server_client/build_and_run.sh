#!/usr/bin/bash

mkdir build
gcc server.c -o ./build/server
gcc client.c -o ./build/client

gnome-terminal -- bash -c "./build/server; exec bash"
read -t 2 -p "I will wait for 2 seconds..."
./build/client