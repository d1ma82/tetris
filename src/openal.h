#pragma once

#include <stdint.h>

#include <al.h>
#include <alc.h>


#define CALLAL(proc) proc; \
        err = alGetError();\
        if (err != AL_NO_ERROR) ALError(err, __LINE__, __FILE__, #proc);

void ALError(ALenum, int, const char*, const char*);
void dump();