#pragma once
#include <opencv2/core.hpp>
#include "types.h"

class Filter {
protected:
    Filter() {}
public:
    
    virtual cv::Mat frame() const noexcept =0;
    virtual void apply() =0;
    virtual ~Filter()=default;
};


class Input : public Filter {
protected:
    uint32_t texture{0};
    Input() {}
public:
    virtual ~Input()=default;
};

class Output: public Filter {
protected:
    Output() {};
public:
    uint32_t texture{0};
    virtual ~Output() {}
};