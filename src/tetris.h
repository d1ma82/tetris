#pragma once

#include <map>
#include <functional>
#include "filter.h"
#include "opengl.h"
#include "types.h"
#include "mino_factory.h"

namespace tetris {

using event = std::function<void(int)>;

struct Events {

    event on_left, on_right, on_rotate, on_ground, on_delete, on_game_over, on_close;
};

}

class Tetris: public Filter {
private:
    int                     sz;
    int                     min_y;
    int                     score{0}, prev_score{0};
    dims                    viewport;
    int                     current {0}, next {0};
    bool                    game_over_flag {false};
    bool                    close_flag {false};
    Factory*                factory;
    std::map<int, Mino>     minos;
    tetris::Events events;

    int create_mino();
    // Calc min y pos from the bottom, for current mino, and store in a class attrib
    void calc_min_line_from_bottom();
    void delete_lines(int begin);
    void game_over() { game_over_flag = min_y<=sz*2; }
    bool collision(const Brick& current_brick, const Brick& brick, bool left);
public:
    Tetris(dims viewport, tetris::Events list, Factory* factory);
    
    bool move_down();
    void move_left();
    void move_right();
    void rotate();
    void close() { close_flag=true; events.on_close(0);}
    void apply(uint8_t* scene);
    ~Tetris() {}
};
