#include <gst/gst.h>

int main(int argc, char *argv[]) {
    gst_init(&argc, &argv);

    // gchar *pipeline_desc =
    //     "udpsrc port=5000 caps=\"application/x-rtp, media=video, "
    //     "encoding-name=H264, payload=96\" ! "
    //     "rtph264depay ! avdec_h264 ! videoconvert ! autovideosink sync=false";

    if (argc != 2) {
        g_printerr("Usage: %s <UDP_port>\n", argv[0]);
    }
    gchar *udp_port = argv[1];
    gchar *pipeline_desc =
        g_strdup_printf(
        "udpsrc port=%s caps=\"application/x-rtp,media=video,"
        "encoding-name=JPEG,payload=26\" ! "
        "rtpjpegdepay ! jpegdec ! autovideosink sync=false", udp_port);

    GError *error = NULL;
    GstElement *pipeline = gst_parse_launch(pipeline_desc, &error);
    if (!pipeline) {
        g_printerr("Pipeline error: %s\n", error->message);
        g_error_free(error);
        return -1;
    }

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_print("Receiving stream on UDP port %s...\n", udp_port);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}
