#!/usr/bin/bash

mkdir -p build/

gcc gst_server.c -o build/gst_server $(pkg-config --cflags --libs gstreamer-1.0)
gcc gst_client.c -o build/gst_client $(pkg-config --cflags --libs gstreamer-1.0)

gnome-terminal -- bash -c "cd build/ && ./gst_server 5100 5101 5102 5103 5104; exec bash"
gnome-terminal -- bash -c "cd build/ && ./gst_client 5100; exec bash"
gnome-terminal -- bash -c "cd build/ && ./gst_client 5101; exec bash"
gnome-terminal -- bash -c "cd build/ && ./gst_client 5102; exec bash"
gnome-terminal -- bash -c "cd build/ && ./gst_client 5103; exec bash"
gnome-terminal -- bash -c "cd build/ && ./gst_client 5104; exec bash"

# sudo cpupower frequency-set -g performance
# Use: chrt -f 90 (real-time FIFO scheduling)

# sudo chrt -f 90 ./gst_server
# sudo taskset -c 3 chrt -f 90