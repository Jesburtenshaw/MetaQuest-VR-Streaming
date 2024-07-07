//
// Created by ksohe on 7/6/2024.
//

#include "decoder.h"
#include "pch.h"
#include "common.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <chrono>
#include "pipeline.h"

namespace quest_teleop {

Decoder::Decoder(int32_t videoWidth, int32_t videoHeight) : mVideoCodec(nullptr), m_videoWidth{videoWidth}, m_videoHeight{videoHeight} {
    should_return = false;
    DoWork();    
}

Decoder::~Decoder() {
    should_return = true;
    if (m_pub_thread->joinable()) {
        m_pub_thread->join();
    }
    if (m_dec_thread->joinable()) {
        m_dec_thread->join();
    }
    if (mVideoCodec) {
        std::lock_guard<std::mutex> guard(mSampleMutex);
        m_samples.clear();
        mMediaListMutex.lock();
        m_mediaFrames.clear();
        mMediaListMutex.lock();
        AMediaCodec_stop(mVideoCodec);
        AMediaCodec_delete(mVideoCodec);
    }
}

void Decoder::Publish() {
    while (!should_return) {
        AMediaCodecBufferInfo outputBufferInfo;
        ssize_t bufferIdx = AMediaCodec_dequeueOutputBuffer(mVideoCodec, &outputBufferInfo, 1);
        if (bufferIdx >= 0) {
            if (outputBufferInfo.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM) {
                Log::Write(Log::Level::Error, Fmt("video codec end"));
            }
            uint8_t *outputBuffer = AMediaCodec_getOutputBuffer(mVideoCodec, bufferIdx, nullptr);
            if (outputBuffer) {
                {
                    std::lock_guard<std::mutex> guard(mMediaListMutex);                    
                    m_mediaFrames.emplace_back(mVideoCodec);
                }
                MediaFrame& frame = m_mediaFrames.back();
        
                frame.width = m_videoWidth;
                frame.height = m_videoHeight;
                frame.pts = outputBufferInfo.presentationTimeUs / 1000;
                frame.number = 0;
                frame.data = outputBuffer + outputBufferInfo.offset;
                frame.size = outputBufferInfo.size;
                frame.bufferIndex = bufferIdx;
                frame.released = false;
                //Log::Write(Log::Level::Info, Fmt("Frame size is %d. Width and Height %dx%d", frame.size, frame.width, frame.height));
            }
        } else {
            //Log::Write(Log::Level::Error, Fmt("No decoded frame available, bufferIdx:%d", bufferIdx));
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    Log::Write(Log::Level::Error, Fmt("exit thread......"));
}

void Decoder::Decode() {
    while (!should_return) {
        bool sleep = false;
        {
            std::lock_guard<std::mutex> guard(mSampleMutex);
            if (m_samples.size() <= 1) {
                sleep = true;                
            }
        }
        if (sleep) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        //video
        ssize_t bufferIdx = AMediaCodec_dequeueInputBuffer(mVideoCodec, 1);
        if (bufferIdx >= 0) {
            size_t bufferSize = 0;
            uint8_t *buffer = AMediaCodec_getInputBuffer(mVideoCodec, bufferIdx, &bufferSize);
                        
            SampleRead& sample = m_samples.front();
            ssize_t size = sample.info.size;

            // copy data to buffer
            memcpy(buffer, sample.info.data, size);

            {
                std::lock_guard<std::mutex> guard(mSampleMutex);                
                m_samples.pop_front();
            }
            
            AMediaCodec_queueInputBuffer(mVideoCodec, bufferIdx, 0, size, 0, 0);            
        } 
    }
    
    Log::Write(Log::Level::Error, Fmt("exit thread......"));
}

void Decoder::DoWork() {
    const char *mime = "video/avc";
    auto format = AMediaFormat_new();
    
    AMediaFormat_setString(format, AMEDIAFORMAT_KEY_MIME, mime);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_WIDTH, m_videoWidth);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_HEIGHT, m_videoHeight);
    AMediaFormat_setInt64(format, AMEDIAFORMAT_KEY_DURATION, INT64_MAX);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_COLOR_FORMAT, 0x32315659);    

    mVideoCodec = AMediaCodec_createDecoderByType(mime);
    
    if (!mVideoCodec) {
        Log::Write(Log::Level::Error, Fmt("create mediacodec %s error", mime));
        AMediaFormat_delete(format);
        return;
    }
    
    media_status_t status = AMediaCodec_configure(mVideoCodec, format, nullptr, nullptr, 0);
    if (status != AMEDIA_OK) {
        Log::Write(Log::Level::Error, Fmt("AMediaCodec_configure error, status = %d", status));
        return;
    } else {
        Log::Write(Log::Level::Info, Fmt("video AMediaCodec_configure successfuly"));
    }
    
    AMediaFormat_delete(format);        
   
    status = AMediaCodec_start(mVideoCodec);
    if (status != AMEDIA_OK) {
        Log::Write(Log::Level::Error, Fmt("AMediaCodec_start error, status = %d", status));
        return;
    } else {
        Log::Write(Log::Level::Info, "video AMediaCodec_start successfully");
    }

    m_pub_thread = std::make_unique<std::thread>(&Decoder::Decode, this);
    m_dec_thread = std::make_unique<std::thread>(&Decoder::Publish, this);
}

void Decoder::stop() {
    should_return = true;
}

MediaFrame& Decoder::getFrame() {
    std::lock_guard<std::mutex> guard(mMediaListMutex);
    while (m_mediaFrames.size() > 2) {
        m_mediaFrames.pop_front();
    }
    return m_mediaFrames.front();
}
} // quest_teleop
