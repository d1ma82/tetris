#pragma once

typedef unsigned char uint8_t;

class Filter {
protected:
    Filter() {}
public:
    
    virtual void apply(uint8_t* scene) = 0;     // pointer to scene memory location, which filter must change
    virtual ~Filter() = default;
};