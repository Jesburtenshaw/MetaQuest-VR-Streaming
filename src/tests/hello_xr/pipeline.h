#ifndef HELLO_XR_PIPELINE_H
#define HELLO_XR_PIPELINE_H

#include "pch.h"
#include <opencv2/core.hpp>
#include <gst/gst.h>
#include <gio/gio.h>
#include <gst/gstbus.h>
#include <gst/app/gstappsink.h>
#include <deque>
#include <atomic>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <memory>
#include <vector>


#include "nativelib/static_gstreamer.h"

namespace quest_teleop {
    enum class StreamType {
        Mono,
        Stereo
    };
    
    enum class CodecType {
        H264,
        H265,
        AV1
    };

    struct StreamConfig {
        StreamType type;
        CodecType codec;
        int port;
        XrVector3f position;
        XrVector3f scale;
        std::string name;
    };
    
    struct SampleRead {
        // delete copy constructor
        SampleRead(const SampleRead &) = delete;

        // delete copy assignment
        SampleRead &operator=(const SampleRead &) = delete;

        SampleRead() {
            image = cv::Mat(cv::Size(10, 10), CV_8UC4,
                            cv::Scalar(0, 0, 200, 50));
        }

        ~SampleRead() {
            if (buffer && mapped) {
                gst_buffer_unmap(buffer, &info);
            }
            if (sample) {
                gst_sample_unref(sample);
            }
        }

        cv::Mat image;
        GstMapInfo info;
        GstBuffer *buffer = nullptr;
        GstSample *sample = nullptr;
        bool mapped = false;
    };
    
    class Pipeline {
        //delete copy constructor
        Pipeline(const Pipeline &) = delete;

        // delete copy assignment
        Pipeline &operator=(const Pipeline &) = delete;

    public:
        static void InitializeGStreamer();

        Pipeline(const StreamConfig &streamConfig);

        ~Pipeline();

        void getPadProperty(GstElement *element, const gchar *pad_name, const gchar *property_name,
                            int *value);

        void SetVideoSize();       

        void SampleReader();

        cv::Mat &GetImage();

    private:
        std::deque <SampleRead> m_samples;
        static bool is_initialized;
        std::string pipeline_;
        GstElement *m_pipeline;
        GstBus *m_bus;
        GstMessage *m_msg;
        GstAppSink *m_appSink;
        GMainContext *m_dataContext;
        std::atomic<bool> m_exit{false};
        std::unique_ptr <std::thread> m_thread;
        std::mutex m_mutex;
        static const int kMaxSamples = 10;

        std::string origin;

        GstElement *m_vc_factory;

        int m_width;
        int m_height;   
        StreamConfig m_streamConfig;
    };

    bool Pipeline::is_initialized = false;

}   // namespace quest_teleop

#endif //HELLO_XR_PIPELINE_H
