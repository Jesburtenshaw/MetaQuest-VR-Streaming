//
// Created by ksohe on 7/6/2024.
//

#ifndef HELLO_XR_DECODER_H
#define HELLO_XR_DECODER_H

#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <list>
#include <deque>
#include <memory>
#include <media/NdkMediaExtractor.h>
#include <gst/gst.h>
#include <gio/gio.h>
#include <gst/gstbus.h>
#include <gst/app/gstappsink.h>


namespace quest_teleop {
enum class MediaType{
    Video,
    Audio
};

struct SampleRead {
    // delete copy constructor
    SampleRead(const SampleRead &) = delete;
    
    // delete copy assignment
    SampleRead &operator=(const SampleRead &) = delete;
    
    SampleRead() {
//        images[0] = cv::Mat(cv::Size(10, 10), CV_8UC3,
//        cv::Scalar(0, 0, 200));
//        images[1] = cv::Mat(cv::Size(10, 10), CV_8UC3,
//        cv::Scalar(0, 0, 200));
    }
    
    ~SampleRead() {
        if (buffer && mapped) {
            gst_buffer_unmap(buffer, &info);
        }
        if (sample) {
            gst_sample_unref(sample);
        }
    }
    
    //cv::Mat images[2];
    GstMapInfo info;
    GstBuffer *buffer = nullptr;
    GstSample *sample = nullptr;
    bool mapped = false;
};

struct MediaFrame {
private:
    AMediaCodec* mVideoCodec;

    // delete copy constructor
    MediaFrame(const MediaFrame &) = delete;
    // delete copy assignment
    MediaFrame &operator=(const MediaFrame &) = delete;
    // delete move constructor
    MediaFrame(MediaFrame &&) = delete;
    // delete move assignment
    MediaFrame &operator=(MediaFrame &&) = delete;
public:
    
    MediaFrame(AMediaCodec* videoCodec) : released{true}, mVideoCodec{videoCodec}, pts(0), number(0), data(nullptr), size(0) {
    }
    
    ~MediaFrame() {
        Release();
    }
    
    void Release() {
        if (released) return;
        
        AMediaCodec_releaseOutputBuffer(mVideoCodec, bufferIndex, false);
        released = true;
    }
    bool released;
    uint64_t pts;
    int32_t width;
    int32_t height;
    uint32_t number;
    uint8_t* data;
    uint32_t size;
    ssize_t bufferIndex;    
};

class Decoder {

public:
    Decoder(int32_t videoWidth, int32_t videoHeight);
    
    ~Decoder();
    
    void DoWork();
    void Decode();
    void Publish();
    
    void stop();
    
    bool is_frame_available() {
        std::lock_guard<std::mutex> guard(mMediaListMutex);
        return m_mediaFrames.size() > 1;
    }
    
    MediaFrame& getFrame();

    SampleRead & get_new_sample() {
        std::lock_guard<std::mutex> guard(mSampleMutex);
        m_samples.emplace_back();
        return m_samples.back();
    }
    void delete_newly_created_sample() {
        std::lock_guard<std::mutex> guard(mSampleMutex);
        m_samples.pop_back();    
    }

private: 
    
    AMediaCodec*     mVideoCodec;
    int32_t m_videoWidth;
    int32_t m_videoHeight; 
    std::mutex       mMediaListMutex;
    std::mutex       mSampleMutex;
    std::deque <SampleRead> m_samples;
    std::deque <MediaFrame> m_mediaFrames;
    std::atomic<bool> should_return;
    std::unique_ptr<std::thread> m_pub_thread;
    std::unique_ptr<std::thread> m_dec_thread;
};
} // quest_teleop

#endif //HELLO_XR_DECODER_H
