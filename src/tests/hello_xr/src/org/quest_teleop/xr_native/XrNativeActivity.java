package org.quest_teleop.xr_native;

import android.app.NativeActivity;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;

import org.freedesktop.gstreamer.GStreamer;

public class XrNativeActivity extends NativeActivity {
    private native void nativeNotifyGStreamerIsReady();
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        try {
            GStreamer.init(this);
        } catch (Exception e) {

            finish();
            return;
        }

        // Letting native side know GStreamer is ready.
        nativeNotifyGStreamerIsReady();
    }
  
    static {
        System.loadLibrary("gstreamer_android");
        System.loadLibrary("hello_xr");
    }

}
