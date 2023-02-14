#pragma once

#include "ffmpeg.h"
#include "openal.h"

class Sound {
    ALuint              buffer      {0};
    AVPacket            pkt         {};
    Format              format      {nullptr};
    enum AVCodecID      codec_id    {};
    const AVCodec*      codec       {nullptr};
    Codec               codec_ctx   {nullptr};
    int                 audio       {-1};
    int                 err         {0};
    SwrContext*         swr_ctx     {nullptr};
    
    bool read(Sample& smpl);
    void decode();
public:
    Sound(const char* file);
    ALuint  buf() const { return buffer; }
    ~Sound();
};