#!/bin/bash
cd /home/devh/linux_std/c_driver_stream/pj_Test/frame2web
sudo modprobe v4l2loopback devices=1 video_nr=2 card_label="VirtualCam" exclusive_caps=1
gst-launch-1.0 v4l2src device=/dev/video0 ! videoconvert ! v4l2sink device=/dev/video2 sync=false &

npm init -y
npm install ws
node server.js &

# python3 -m http.server 8300 &
