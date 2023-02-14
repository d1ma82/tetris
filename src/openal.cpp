#include <cstring>
#include "openal.h"
#include "log.h"

void dump() {

    LOGI("***OpenAL***")
    const ALCchar* devices = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
    const ALCchar* ptr = devices;

    while (*ptr) {
        LOGI(" %s\n", ptr)
        ptr += strlen(ptr)+1;
    }
}

void ALError(
    ALenum err, 
    int line, 
    const char* file, 
    const char* proc
) {
    LOGI("[FAIL AL ] %s:%d, %s", file, line, proc)

    switch (err) {

        case ALC_INVALID_DEVICE: LOGE("No device") break;
        case ALC_INVALID_CONTEXT: LOGE("invalid context ID") break;
        case ALC_INVALID_ENUM: LOGE("Bad enum") break;
        case ALC_INVALID_VALUE: LOGE("Bad value") break;
        case ALC_OUT_OF_MEMORY: LOGE("Out of memory") break;
    }
}