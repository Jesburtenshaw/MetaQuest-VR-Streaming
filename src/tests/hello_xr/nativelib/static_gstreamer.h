//
// Created by ksohe on 5/23/2024.
//

#ifndef HELLO_XR_STATIC_GSTREAMER_H
#define HELLO_XR_STATIC_GSTREAMER_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <gst/gst.h>
#include <gio/gio.h>

#define GST_G_IO_MODULE_DECLARE(name) \
extern void G_PASTE(g_io_, G_PASTE(name, _load)) (gpointer data)

#define GST_G_IO_MODULE_LOAD(name) \
G_PASTE(g_io_, G_PASTE(name, _load)) (NULL)

/* Declaration of static plugins */
GST_PLUGIN_STATIC_DECLARE(coreelements);  GST_PLUGIN_STATIC_DECLARE(coretracers);  GST_PLUGIN_STATIC_DECLARE(adder);  GST_PLUGIN_STATIC_DECLARE(app);  GST_PLUGIN_STATIC_DECLARE(audioconvert);  GST_PLUGIN_STATIC_DECLARE(audiomixer);  GST_PLUGIN_STATIC_DECLARE(audiorate);  GST_PLUGIN_STATIC_DECLARE(audioresample);  GST_PLUGIN_STATIC_DECLARE(audiotestsrc);  GST_PLUGIN_STATIC_DECLARE(compositor);  GST_PLUGIN_STATIC_DECLARE(gio);  GST_PLUGIN_STATIC_DECLARE(overlaycomposition);  GST_PLUGIN_STATIC_DECLARE(pango);  GST_PLUGIN_STATIC_DECLARE(rawparse);  GST_PLUGIN_STATIC_DECLARE(typefindfunctions);  GST_PLUGIN_STATIC_DECLARE(videoconvertscale);  GST_PLUGIN_STATIC_DECLARE(videorate);  GST_PLUGIN_STATIC_DECLARE(videotestsrc);  GST_PLUGIN_STATIC_DECLARE(volume);  GST_PLUGIN_STATIC_DECLARE(autodetect);  GST_PLUGIN_STATIC_DECLARE(videofilter);  GST_PLUGIN_STATIC_DECLARE(insertbin);  GST_PLUGIN_STATIC_DECLARE(switchbin);  GST_PLUGIN_STATIC_DECLARE(fallbackswitch);  GST_PLUGIN_STATIC_DECLARE(livesync);  GST_PLUGIN_STATIC_DECLARE(rsinter);  GST_PLUGIN_STATIC_DECLARE(rstracers);  GST_PLUGIN_STATIC_DECLARE(threadshare);  GST_PLUGIN_STATIC_DECLARE(togglerecord);  GST_PLUGIN_STATIC_DECLARE(opengl);  GST_PLUGIN_STATIC_DECLARE(ipcpipeline);  GST_PLUGIN_STATIC_DECLARE(opensles);  GST_PLUGIN_STATIC_DECLARE(alpha);  GST_PLUGIN_STATIC_DECLARE(alphacolor);  GST_PLUGIN_STATIC_DECLARE(audiofx);  GST_PLUGIN_STATIC_DECLARE(cairo);  GST_PLUGIN_STATIC_DECLARE(cutter);  GST_PLUGIN_STATIC_DECLARE(debug);  GST_PLUGIN_STATIC_DECLARE(deinterlace);  GST_PLUGIN_STATIC_DECLARE(dtmf);  GST_PLUGIN_STATIC_DECLARE(effectv);  GST_PLUGIN_STATIC_DECLARE(equalizer);  GST_PLUGIN_STATIC_DECLARE(gdkpixbuf);  GST_PLUGIN_STATIC_DECLARE(imagefreeze);  GST_PLUGIN_STATIC_DECLARE(interleave);  GST_PLUGIN_STATIC_DECLARE(level);  GST_PLUGIN_STATIC_DECLARE(multifile);  GST_PLUGIN_STATIC_DECLARE(replaygain);  GST_PLUGIN_STATIC_DECLARE(shapewipe);  GST_PLUGIN_STATIC_DECLARE(smpte);  GST_PLUGIN_STATIC_DECLARE(spectrum);  GST_PLUGIN_STATIC_DECLARE(videobox);  GST_PLUGIN_STATIC_DECLARE(videocrop);  GST_PLUGIN_STATIC_DECLARE(videomixer);  GST_PLUGIN_STATIC_DECLARE(accurip);  GST_PLUGIN_STATIC_DECLARE(aes);  GST_PLUGIN_STATIC_DECLARE(aiff);  GST_PLUGIN_STATIC_DECLARE(audiobuffersplit);  GST_PLUGIN_STATIC_DECLARE(audiofxbad);  GST_PLUGIN_STATIC_DECLARE(audiolatency);  GST_PLUGIN_STATIC_DECLARE(audiomixmatrix);  GST_PLUGIN_STATIC_DECLARE(autoconvert);  GST_PLUGIN_STATIC_DECLARE(bayer);  GST_PLUGIN_STATIC_DECLARE(coloreffects);  GST_PLUGIN_STATIC_DECLARE(closedcaption);  GST_PLUGIN_STATIC_DECLARE(debugutilsbad);  GST_PLUGIN_STATIC_DECLARE(fieldanalysis);  GST_PLUGIN_STATIC_DECLARE(freeverb);  GST_PLUGIN_STATIC_DECLARE(frei0r);  GST_PLUGIN_STATIC_DECLARE(gaudieffects);  GST_PLUGIN_STATIC_DECLARE(geometrictransform);  GST_PLUGIN_STATIC_DECLARE(inter);  GST_PLUGIN_STATIC_DECLARE(interlace);  GST_PLUGIN_STATIC_DECLARE(isac);  GST_PLUGIN_STATIC_DECLARE(ivtc);  GST_PLUGIN_STATIC_DECLARE(legacyrawparse);  GST_PLUGIN_STATIC_DECLARE(proxy);  GST_PLUGIN_STATIC_DECLARE(removesilence);  GST_PLUGIN_STATIC_DECLARE(segmentclip);  GST_PLUGIN_STATIC_DECLARE(smooth);  GST_PLUGIN_STATIC_DECLARE(speed);  GST_PLUGIN_STATIC_DECLARE(soundtouch);  GST_PLUGIN_STATIC_DECLARE(timecode);  GST_PLUGIN_STATIC_DECLARE(videofiltersbad);  GST_PLUGIN_STATIC_DECLARE(videoframe_audiolevel);  GST_PLUGIN_STATIC_DECLARE(videosignal);  GST_PLUGIN_STATIC_DECLARE(webrtcdsp);  GST_PLUGIN_STATIC_DECLARE(ladspa);  GST_PLUGIN_STATIC_DECLARE(rsaudiofx);  GST_PLUGIN_STATIC_DECLARE(rsvideofx);
/* Declaration of static gio modules */
GST_G_IO_MODULE_DECLARE(openssl);

/* Call this function to load GIO modules */
static void
gst_android_load_gio_modules (void);

/* This is called by gst_init() */

void gst_init_static_plugins (void);
#ifdef __cplusplus
}
#endif
#endif //HELLO_XR_STATIC_GSTREAMER_H
