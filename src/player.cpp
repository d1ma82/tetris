#include "player.h"
#include "log.h"

Player::Player() {

    dump();
    device = alcOpenDevice(nullptr);
    if (!device) {
        LOGE("could not open device")
        return;
    }
    LOGI("Open device: %s", alcGetString(device, ALC_ALL_DEVICES_SPECIFIER))

    context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);
    alcProcessContext(context);
    //alGenBuffers(1, &buffer);
    alGenSources(1, &source);
}

void Player::play(ALuint data) {

    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING) alSourceStop(source);

    alSourcei(source, AL_BUFFER, data);
    alSourcePlay(source);
}

Player::~Player() {
    
    alDeleteSources(1, &source);
    alcMakeContextCurrent(nullptr);
    alcCloseDevice(device);
}