gst-launch-1.0 v4l2src device=/dev/video0 ! image/jpeg,width=640,height=480,framerate=30/1 ! jpegparse ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000

gst-launch-1.0 udpsrc port=5000 caps="application/x-rtp,media=video,encoding-name=JPEG,payload=26" ! rtpjpegdepay ! jpegdec ! autovideosink sync=false

gst-launch-1.0 v4l2src ! jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000
--------------------
gst-launch-1.0 v4l2src device=/dev/video0 ! autovideosink


---stream--------------------
gst-launch-1.0 v4l2src device=/dev/video0 ! image/jpeg,width=640,height=480 ! jpegparse ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000
---stream--------------------
gst-launch-1.0 udpsrc port=5000 \
caps="application/x-rtp,media=video,encoding-name=JPEG,payload=26" ! \
rtpjpegdepay ! jpegdec ! queue ! videoconvert ! ximagesink sync=false

gst-launch-1.0 udpsrc port=5000 caps="application/x-rtp,media=video,encoding-name=JPEG,payload=26" ! rtpjpegdepay ! jpegdec ! autovideosink

gst-launch-1.0 udpsrc port=5000 caps="application/x-rtp,media=video,encoding-name=JPEG,payload=26" ! rtpjpegdepay ! jpegdec ! glimagesink

---filesink--------------------
gst-launch-1.0 udpsrc port=5000 \
caps="application/x-rtp,media=video,encoding-name=JPEG,payload=26" ! \
rtpjpegdepay ! matroskamux ! filesink location=output.mkv sync=false

gst-launch-1.0 udpsrc port=5000 \
caps="application/x-rtp,media=video,encoding-name=JPEG,payload=26" ! \
rtpjpegdepay ! jpegdec ! videoconvert ! x264enc tune=zerolatency bitrate=2000 speed-preset=ultrafast ! \
matroskamux ! filesink location=output.mkv sync=false




---filesink
gst-launch-1.0 filesrc location=output.mkv ! matroskademux ! decodebin ! autovideosink



// 
gst-launch-1.0 v4l2src device=/dev/video0 ! image/jpeg,width=1280,height=720,framerate=30/1 ! matroskamux ! filesink location=record_mjpeg.mkv

//encode low cpu
gst-launch-1.0 -e v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! videoconvert ! avenc_mpeg4 bitrate=2000000 ! mp4mux ! filesink location=output_mpeg4.mp4
gst-launch-1.0 -e v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! videoconvert ! avenc_mpeg4 bitrate=2000000 ! matroskamux ! filesink location=output_mpeg4.mkv




gst-launch-1.0 -e v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=1280,height=720,framerate=30/1 ! videoconvert ! avenc_mpeg4 bitrate=2000000 ! jpegparse ! rtpjpegpay ! udpsink host=127.0.0.1 port=5000

gst-launch-1.0 udpsrc port=5000 \
caps="application/x-rtp,media=video,encoding-name=JPEG,payload=26" ! \
rtpjpegdepay ! jpegdec ! queue ! videoconvert ! ximagesink sync=false


gst-launch-1.0 -e v4l2src device=/dev/video0 ! \
video/x-raw,format=YUY2,width=1280,height=720,framerate=30/1 ! \
videoconvert ! avenc_mpeg4 bitrate=2000000 ! \
rtpmp4vpay ! udpsink host=127.0.0.1 port=5000



gst-launch-1.0 v4l2src device=/dev/video0 ! image/jpeg,width=1280,height=720 ! jpegparse ! rtpjpegpay ! tee name=split_stream \
split_stream. ! queue ! udpsink host=127.0.0.1 port=5000 \
split_stream. ! queue ! udpsink host=127.0.0.1 port=5001

gst-launch-1.0 udpsrc port=5000 caps="application/x-rtp,media=video,encoding-name=JPEG,payload=26" ! rtpjpegdepay ! jpegdec ! autovideosink sync=false
gst-launch-1.0 udpsrc port=5001 caps="application/x-rtp,media=video,encoding-name=JPEG,payload=26" ! rtpjpegdepay ! jpegdec ! autovideosink sync=false





chua chay dc
st-launch-1.0 -e v4l2src device=/dev/video0 ! \
>   video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! \
>   videoconvert ! avenc_mpeg4 bitrate=2000000 ! \
>   rtpmp4vpay ! udpsink host=127.0.0.1 port=5000
gst-launch-1.0 udpsrc port=5000 \
caps="application/x-rtp,media=video,encoding-name=MP4V-ES,payload=96" ! \
rtpmp4vdepay ! avdec_mpeg4 ! videoconvert ! autovideosink sync=false