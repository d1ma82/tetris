#pragma once
#include <functional>
#include "filter.h"

const int PRESS = 1;

const int KEY_RIGHT = 262;
const int KEY_LEFT  = 263;
const int KEY_UP    = 265;
const int KEY_DOWN  = 264;
const int KEY_ESCAPE = 256;

using key_event = std::function<void(int key, int action)>;

struct KeyEvents {
    key_event on_key_press;

    KeyEvents(key_event func): on_key_press{func} {}
};

class Window {
protected:
    Window() {}
public:
    virtual bool should_close()=0;
    virtual void close()=0;
    virtual void draw()=0;
    virtual void attach_filters(std::vector<Filter*> filters)=0;
    virtual void set_key_event_listener(KeyEvents* events)=0;
    virtual ~Window()=default;
};