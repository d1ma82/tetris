#pragma once

#include "openal.h"

class Player {
private:
    ALCdevice* device        {nullptr};
    ALCcontext* context      {nullptr};
    ALuint source            {0};
    ALint state;
public:
    Player();
    ~Player();
    void play(ALuint data);
};

