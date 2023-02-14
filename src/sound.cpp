#include "sound.h"

Sound::Sound(const char* file) {
    
    alGenBuffers(1, &buffer);
    const AVChannelLayout out_layout = AV_CHANNEL_LAYOUT_STEREO;
        
    format = Format {avformat_alloc_context(), [](AVFormatContext* ctx) {avformat_close_input(&ctx); }};
    AVFormatContext* ctx = format.get();
    CALL(avformat_open_input(&ctx, file, nullptr, nullptr))
    CALL(avformat_find_stream_info(ctx, nullptr))
    av_dump_format(ctx, 0, file, 0);

    for (int i=0; i<ctx->nb_streams; i++) {

        if (ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio=i;
            break;
        }
    }
    codec_id  = ctx->streams[audio]->codecpar->codec_id;
    codec = avcodec_find_decoder(codec_id);

    LOGI("Codec: %s", avcodec_get_name(codec_id))

    codec_ctx = Codec { avcodec_alloc_context3(codec), [] (AVCodecContext* ctx) { avcodec_close(ctx); avcodec_free_context(&ctx); }};
    CALL(avcodec_parameters_to_context(codec_ctx.get(), format->streams[audio]->codecpar))
    CALL(avcodec_open2(codec_ctx.get(), codec, nullptr))

    CALL(swr_alloc_set_opts2(&swr_ctx, 
            &out_layout, 
            AV_SAMPLE_FMT_S16, 
            codec_ctx->sample_rate,
            (const AVChannelLayout *) &codec_ctx->ch_layout, 
            codec_ctx->sample_fmt, 
            codec_ctx->sample_rate, 0, nullptr)
    )
    swr_init(swr_ctx);
    decode();
    swr_close(swr_ctx);
}

bool Sound::read(Sample& smpl) {

    if (av_read_frame(format.get(), &pkt)>=0) {

        CALL(avcodec_send_packet(codec_ctx.get(), &pkt))
        CALL(avcodec_receive_frame(codec_ctx.get(), smpl.operator->()))
        if (err==AVERROR(EAGAIN)) {av_packet_unref(&pkt); read(smpl); }
        return err>=0;
    } 
    else return false;
}

void Sound::decode() {

    Sample smpl;
    int sz = 0;
    uint8_t* data = nullptr;
    std::basic_stringstream<uint8_t> stream;

    while (read(smpl)) {

       // int bytes = av_get_bytes_per_sample(codec_ctx->sample_fmt);
        //LOGI("Smpl: %d, %d, %d, %s", codec_ctx->sample_rate, smpl->ch_layout.nb_channels, smpl->nb_samples, av_get_sample_fmt_name(codec_ctx->sample_fmt))
        if (!data) CALL(av_samples_alloc(&data, &sz, smpl->ch_layout.nb_channels, smpl->nb_samples, codec_ctx->sample_fmt, 0))
        swr_convert(swr_ctx, &data, smpl->nb_samples, (const uint8_t**) smpl->extended_data, smpl->nb_samples);        
        stream.write(data, sz);
        av_packet_unref(&pkt);
    }
    av_freep(&data);
    stream.flush();
    stream.seekg(0);

    auto p = stream.rdbuf()->str();
    const uint8_t* mem = p.c_str();
    alBufferData(buffer, AL_FORMAT_STEREO16, mem, stream.tellp(), codec_ctx->sample_rate);
}

Sound::~Sound() {
    alDeleteBuffers(1, &buffer);
}