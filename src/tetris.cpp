#include <algorithm>
#include "tetris.h"
#include "log.h"
#include <string.h>

const int X = 0;
const int Y = 1;

static const int SQ_PER_LINE = 20;
static int gen = 0;     // Mino id generator


Tetris::Tetris(dims viewport, TetrisEventListener list, Factory* factory):

        viewport{viewport}, events{list}, 
            factory{factory}, min_y{viewport.second}
{
    sz      = viewport.first/SQ_PER_LINE;
    LOGI("SQ Per Line %d Square size %d", SQ_PER_LINE, sz)
}

int Tetris::create_mino() {
    
    std::vector<Brick> mino;
    factory->create_mino(SQ_PER_LINE, sz, mino);
    minos.emplace(++gen, mino);
    LOGD("Create mino id=%d type=%d; x=%d; y=%d; rgb=%d,%d,%d", 
        gen, minos[gen][0].type, minos[gen][0].tl[0], minos[gen][0].tl[1], minos[gen][0].color[0], minos[gen][0].color[1], minos[gen][0].color[2])
    return gen;
}

bool Tetris::move_down() {

    if (current==0 or game_over_flag or close_flag)  return false;
    
    for (const auto& current_brick: minos[current]) {

        if (current_brick.tl[Y]+sz >= viewport.second) return false;       // Если коснулись дна

        for (const auto& el: minos) {
            if (current==el.first or next==el.first) continue;

            for (const auto& brick: el.second)
                if (brick.enabled and current_brick.tl[Y]+sz >= brick.tl[Y]) {
                    if (current_brick.tl[X]==brick.tl[X] and 
                        current_brick.tl[X]+sz==brick.tl[X]+sz and
                        current_brick.tl[Y]+sz==brick.tl[Y]) return false;
            }
        }
    }

    for (auto& brick: minos[current]) brick.tl[Y]++;
    return true; 
}

bool Tetris::collision(const Brick& current_brick, const Brick& brick, bool left) {

    if (current_brick.tl[Y] < brick.tl[Y]) {
        if (current_brick.tl[Y]+sz > brick.tl[Y] and
            current_brick.tl[Y]+sz < brick.tl[Y]+sz and
                current_brick.tl[X]+(left? 0: sz) == brick.tl[X]+(left? sz: 0)) return true;

    } else if (current_brick.tl[Y] > brick.tl[Y]) {
            if (current_brick.tl[Y] < brick.tl[Y]+sz and
                    current_brick.tl[X]+(left? 0: sz) == brick.tl[X]+(left? sz: 0)) return true;
                    
    } else {
            if (current_brick.tl[Y]==brick.tl[Y] and 
                    current_brick.tl[Y]+sz==brick.tl[Y]+sz) return true;
    }
    return false;
}

void Tetris::move_left() {

    if (current==0 or game_over_flag or close_flag) return;

    for (const auto& current_brick: minos[current]) {

        if (current_brick.tl[X] == 0) return;        // Если коснулись левой стенки

        for (const auto& el: minos) {
            if (current==el.first or next==el.first) continue;

            for (const auto& brick: el.second)

                if (brick.enabled and collision(current_brick, brick, true)) return;
        }
    }

    for (auto& brick: minos[current]) brick.tl[X]-=sz;
    events.on_left((minos[current])[0].tl[X]);
}

void Tetris::move_right() {

    if (current==0 or game_over_flag or close_flag) return;
    
    for (const auto& current_brick: minos[current]) {

        if (current_brick.tl[X] == viewport.first-sz) return;        // Если коснулись правой стенки

        for (const auto& el: minos) {

            if (current==el.first or next==el.first) continue;

            for (const auto& brick: el.second)
                if (brick.enabled and collision(current_brick, brick, false)) return; 
        }
    }
  
    for (auto& brick: minos[current]) brick.tl[X]+=sz;
    events.on_right((minos[current])[0].tl[X]);
}

void Tetris::rotate() {

    if (current==0 or game_over_flag or close_flag) return;
    std::vector<Brick> mino;
    factory->rotate(minos[current], sz, mino);

    for (const auto& current_brick: mino) {

        if (current_brick.tl[X] > viewport.first-sz or current_brick.tl[X] < 0) return;

        for (const auto& el: minos) {
            if (current==el.first or next==el.first) continue;

                for (const auto& brick: el.second) {

                    if (not brick.enabled) continue;

                    if  (brick.tl[X] <= current_brick.tl[X] and current_brick.tl[X] <= brick.tl[X]+sz and 
                         brick.tl[Y] <= current_brick.tl[Y] and current_brick.tl[Y] <= brick.tl[Y]+sz) return;

                    if  (brick.tl[X] <= current_brick.tl[X]+sz and current_brick.tl[X]+sz <= brick.tl[X]+sz and
                         brick.tl[Y] <= current_brick.tl[Y]+sz and current_brick.tl[Y]+sz <= brick.tl[Y]+sz) return;
                }
        }
    }

    minos[current] = mino;
    events.on_rotate(current);
}

void Tetris::calc_min_line_from_bottom() {
        
    for (auto& brick: minos[current]) 
        min_y = std::min(min_y, brick.tl[Y]);

    LOGD("min y=%d", min_y)
}

void Tetris::delete_lines(int begin) {

    std::vector<Brick*> deleted_bricks;
    deleted_bricks.reserve(SQ_PER_LINE);

    for (auto& el: minos) {
        if (el.first==next) continue;

        for (auto& brick: el.second) {
            if (brick.enabled and brick.tl[Y]==begin) deleted_bricks.push_back(&brick);
        }
    }

    if (deleted_bricks.size()==SQ_PER_LINE) {

        score++;

        for (auto& brick: deleted_bricks) brick->enabled=false;

        for (auto& el: minos) {
            if (el.first==next) continue;

            for (auto& brick: el.second) {
                if (brick.enabled and brick.tl[Y] < begin) brick.tl[Y]+=sz;
            }
        }
        delete_lines(begin);
    } else {
        if (begin-sz > min_y) delete_lines(begin-sz);
    }    
}

// Function to draw a filled rectangle in a 1D RGB array
static void draw_filled_rect(uint8_t* rgb_array, dims size, int x, int y, dims rect_size, Color color) {

    for (int i = y; i < y + rect_size.second; ++i) {
        for (int j = x; j < x + rect_size.first; ++j) {

            // Calculate the index in the 1D array
            int index = (i * size.first + j) * 3;

            // Set the RGB values for the current pixel
            rgb_array[index] = color[0];
            rgb_array[index + 1] = color[1];
            rgb_array[index + 2] = color[2];
        }
    }
}

void Tetris::apply(uint8_t* scene) {
    
    if (game_over_flag or close_flag) return;

    if (current==0) {
         current= next==0? create_mino(): next;
         next = create_mino();
    }

    if (not move_down()) {
        
        calc_min_line_from_bottom();
        delete_lines(viewport.second-sz);
                
        if (score > prev_score) {

            int lines = score-prev_score;
            prev_score=score;
            LOGI("Score %d", score)
            events.on_delete(lines);
        }
        events.on_ground(current);
        current=0;
    }

    game_over();
    if (game_over_flag) events.on_game_over(score);
    
    memset(scene, 0, viewport.first*viewport.second*3);
    for (auto& mino: minos)
        for (auto& brick: mino.second)   
            if (brick.enabled) draw_filled_rect(scene, viewport, brick.tl[X], brick.tl[Y], {sz, sz}, brick.color);
}
