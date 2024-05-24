//
// Created by ksohe on 5/23/2024.
//

#include "static_gstreamer.h"

#include <gst/gst.h>
#include <gio/gio.h>

/* Call this function to load GIO modules */
static void
gst_android_load_gio_modules (void)
{
    GTlsBackend *backend;
    const gchar *ca_certs;

    GST_G_IO_MODULE_LOAD(openssl);

    ca_certs = g_getenv ("CA_CERTIFICATES");

    backend = g_tls_backend_get_default ();
    if (backend && ca_certs) {
        GTlsDatabase *db;
        GError *error = NULL;

        db = g_tls_file_database_new (ca_certs, &error);
        if (db) {
            g_tls_backend_set_default_database (backend, db);
            g_object_unref (db);
        } else {
            g_warning ("Failed to create a database from file: %s",
                       error ? error->message : "Unknown");
        }
    }
}
/* This is called by gst_init() */
void
gst_init_static_plugins (void)
{
    GST_PLUGIN_STATIC_REGISTER(coreelements);  GST_PLUGIN_STATIC_REGISTER(coretracers);  GST_PLUGIN_STATIC_REGISTER(adder);  GST_PLUGIN_STATIC_REGISTER(app);  GST_PLUGIN_STATIC_REGISTER(audioconvert);  GST_PLUGIN_STATIC_REGISTER(audiomixer);  GST_PLUGIN_STATIC_REGISTER(audiorate);  GST_PLUGIN_STATIC_REGISTER(audioresample);  GST_PLUGIN_STATIC_REGISTER(audiotestsrc);  GST_PLUGIN_STATIC_REGISTER(compositor);  GST_PLUGIN_STATIC_REGISTER(gio);  GST_PLUGIN_STATIC_REGISTER(overlaycomposition);  GST_PLUGIN_STATIC_REGISTER(pango);  GST_PLUGIN_STATIC_REGISTER(rawparse);  GST_PLUGIN_STATIC_REGISTER(typefindfunctions);  GST_PLUGIN_STATIC_REGISTER(videoconvertscale);  GST_PLUGIN_STATIC_REGISTER(videorate);  GST_PLUGIN_STATIC_REGISTER(videotestsrc);  GST_PLUGIN_STATIC_REGISTER(volume);  GST_PLUGIN_STATIC_REGISTER(autodetect);  GST_PLUGIN_STATIC_REGISTER(videofilter);  GST_PLUGIN_STATIC_REGISTER(insertbin);  GST_PLUGIN_STATIC_REGISTER(switchbin);  GST_PLUGIN_STATIC_REGISTER(fallbackswitch);  GST_PLUGIN_STATIC_REGISTER(livesync);  GST_PLUGIN_STATIC_REGISTER(rsinter);  GST_PLUGIN_STATIC_REGISTER(rstracers);  GST_PLUGIN_STATIC_REGISTER(threadshare);  GST_PLUGIN_STATIC_REGISTER(togglerecord);  GST_PLUGIN_STATIC_REGISTER(opengl);  GST_PLUGIN_STATIC_REGISTER(ipcpipeline);  GST_PLUGIN_STATIC_REGISTER(opensles);  GST_PLUGIN_STATIC_REGISTER(alpha);  GST_PLUGIN_STATIC_REGISTER(alphacolor);  GST_PLUGIN_STATIC_REGISTER(audiofx);  GST_PLUGIN_STATIC_REGISTER(cairo);  GST_PLUGIN_STATIC_REGISTER(cutter);  GST_PLUGIN_STATIC_REGISTER(debug);  GST_PLUGIN_STATIC_REGISTER(deinterlace);  GST_PLUGIN_STATIC_REGISTER(dtmf);  GST_PLUGIN_STATIC_REGISTER(effectv);  GST_PLUGIN_STATIC_REGISTER(equalizer);  GST_PLUGIN_STATIC_REGISTER(gdkpixbuf);  GST_PLUGIN_STATIC_REGISTER(imagefreeze);  GST_PLUGIN_STATIC_REGISTER(interleave);  GST_PLUGIN_STATIC_REGISTER(level);  GST_PLUGIN_STATIC_REGISTER(multifile);  GST_PLUGIN_STATIC_REGISTER(replaygain);  GST_PLUGIN_STATIC_REGISTER(shapewipe);  GST_PLUGIN_STATIC_REGISTER(smpte);  GST_PLUGIN_STATIC_REGISTER(spectrum);  GST_PLUGIN_STATIC_REGISTER(videobox);  GST_PLUGIN_STATIC_REGISTER(videocrop);  GST_PLUGIN_STATIC_REGISTER(videomixer);  GST_PLUGIN_STATIC_REGISTER(accurip);  GST_PLUGIN_STATIC_REGISTER(aes);  GST_PLUGIN_STATIC_REGISTER(aiff);  GST_PLUGIN_STATIC_REGISTER(audiobuffersplit);  GST_PLUGIN_STATIC_REGISTER(audiofxbad);  GST_PLUGIN_STATIC_REGISTER(audiolatency);  GST_PLUGIN_STATIC_REGISTER(audiomixmatrix);  GST_PLUGIN_STATIC_REGISTER(autoconvert);  GST_PLUGIN_STATIC_REGISTER(bayer);  GST_PLUGIN_STATIC_REGISTER(coloreffects);  GST_PLUGIN_STATIC_REGISTER(closedcaption);  GST_PLUGIN_STATIC_REGISTER(debugutilsbad);  GST_PLUGIN_STATIC_REGISTER(fieldanalysis);  GST_PLUGIN_STATIC_REGISTER(freeverb);  GST_PLUGIN_STATIC_REGISTER(frei0r);  GST_PLUGIN_STATIC_REGISTER(gaudieffects);  GST_PLUGIN_STATIC_REGISTER(geometrictransform);  GST_PLUGIN_STATIC_REGISTER(inter);  GST_PLUGIN_STATIC_REGISTER(interlace);  GST_PLUGIN_STATIC_REGISTER(isac);  GST_PLUGIN_STATIC_REGISTER(ivtc);  GST_PLUGIN_STATIC_REGISTER(legacyrawparse);  GST_PLUGIN_STATIC_REGISTER(proxy);  GST_PLUGIN_STATIC_REGISTER(removesilence);  GST_PLUGIN_STATIC_REGISTER(segmentclip);  GST_PLUGIN_STATIC_REGISTER(smooth);  GST_PLUGIN_STATIC_REGISTER(speed);  GST_PLUGIN_STATIC_REGISTER(soundtouch);  GST_PLUGIN_STATIC_REGISTER(timecode);  GST_PLUGIN_STATIC_REGISTER(videofiltersbad);  GST_PLUGIN_STATIC_REGISTER(videoframe_audiolevel);  GST_PLUGIN_STATIC_REGISTER(videosignal);  GST_PLUGIN_STATIC_REGISTER(webrtcdsp);  GST_PLUGIN_STATIC_REGISTER(ladspa);  GST_PLUGIN_STATIC_REGISTER(rsaudiofx);  GST_PLUGIN_STATIC_REGISTER(rsvideofx);
    gst_android_load_gio_modules ();
}
