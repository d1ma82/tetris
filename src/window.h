#pragma once
#include <functional>
#include "filter.h"

const int PRESS = 1;

const int KEY_RIGHT = 262;
const int KEY_LEFT  = 263;
const int KEY_UP    = 265;
const int KEY_DOWN  = 264;
const int KEY_ESCAPE = 256;

using key_listener = std::function<void(int key, int action)>;


class Window {
protected:
    Window() {}
public:
    virtual bool should_close()=0;
    virtual void close()=0;
    virtual void draw()=0;
    virtual void attach_filters(std::vector<Filter*> filters)=0;
    virtual void set_key_event_listener(key_listener listener)=0;
    virtual ~Window()=default;
};