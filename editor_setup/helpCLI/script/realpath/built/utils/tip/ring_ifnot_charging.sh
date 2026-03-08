#!/bin/bash

echo "Monitoring power status... (Press Ctrl+C to stop)"

while true; do
    # Get the battery state (charging, discharging, or full)
    STATUS=$(upower -i $(upower -e | grep 'BAT') | grep "state" | awk '{print $2}')

    if [ "$STATUS" = "discharging" ]; then
        echo "WARNING: Laptop is DISCHARGING!"
        
        # Play a 1000Hz sine wave beep using GStreamer
        # num-buffers=100 provides approx 2 seconds of audio
        gst-launch-1.0 audiotestsrc volume=1 wave=sine ! audioconvert ! autoaudiosink
    else
        echo "Power Status: $STATUS"
    fi

    # Wait for 5 seconds before the next check
    sleep 5
done