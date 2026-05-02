### Build in u20 1.16 but meet some warning:
```sh
cd gst-rtsp-server/ 
git checkout 1.16
gcc examples/test-launch.c -o test-launch $(pkg-config --cflags --libs gstreamer-1.0 gstreamer-rtsp-server-1.0)
```

### Run testsrc
```sh
./test-launch "( videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96 )"
```

### View stream with rtsp-server at loopback
```sh
ffplay rtsp://127.0.0.1:8554/test
```