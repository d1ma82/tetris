#pragma once
#include <array>
#include <vector>
#include <memory>


typedef std::array<int, 2> Point;       // x,y coordinates
typedef std::array<uint8_t, 3> Color;   // rgb values

struct Brick {
    
    int type = 0;
    int orientation = 0;
    Point tl     {0,0};                       //  Top left point
    Color color  {0,0,0};
    bool enabled = true;

    Brick() {}

    Brick(
        int type, 
        int orientation, 
        Color color, 
        Point tl): type{type}, color{color}, orientation{orientation}, tl{tl} {
    }
};

typedef std::vector<Brick> Mino;

class Factory {
protected:
    Factory() {}
public:
    virtual void create_mino(int sq_per_line, int sz, Mino& mino)=0;
    virtual void rotate(const Mino& input, int sz, Mino& output)=0;
    virtual ~Factory()=default;
};