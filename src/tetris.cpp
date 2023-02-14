#include <random>
#include <chrono>
#include <algorithm>

#include <opencv2/imgproc.hpp>
#include "tetris.h"
#include "log.h"

static const int SQ_PER_LINE = 20;
static const int SQ_PER_COL  = 26;
static int sz;                  // Size of square
static const uint32_t seed = std::chrono::system_clock::now().time_since_epoch().count();
static cv::RNG rng(0xFFFFFFFF);

static auto gen_type = std::bind(std::uniform_int_distribution(0, Type::COUNT-1), std::default_random_engine{seed});
static auto gen_pos  = std::bind(std::uniform_int_distribution(4, SQ_PER_LINE-4), std::default_random_engine{seed});

Tetris::Tetris(const dims& viewport, Tetrislisteners list): viewport{viewport}, listeners{list} {

    img     = cv::Mat::zeros(viewport.second, viewport.first, CV_8UC3);
    zeros   = cv::Mat::zeros(viewport.second, viewport.first, CV_8UC3);
    sz      = viewport.first/SQ_PER_LINE;
    LOGI("Seed: %d", seed)
}

static cv::Scalar random_color(cv::RNG& rng) {

	auto icolor = (unsigned) rng;
	return cv::Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}

void Tetris::create_mino(Type type, Orientation orientation, cv::Scalar color, int pos_x, int pos_y, std::array<Mino, 4>& mino) noexcept {

    switch (type) {
        case LINE: 
            switch (orientation) {
                case O1:
                    mino[0] = {type, O1, color, {pos_x, pos_y}};
                    mino[1] = {type, O1, color, {pos_x+sz, pos_y}};
                    mino[2] = {type, O1, color, {pos_x+2*sz, pos_y}};
                    mino[3] = {type, O1, color, {pos_x+3*sz, pos_y}};
                    break;
                case O2:
                    mino[0] = {type, O2, color, {pos_x, pos_y}};
                    mino[1] = {type, O2, color, {pos_x, pos_y+sz}};
                    mino[2] = {type, O2, color, {pos_x, pos_y+2*sz}};
                    mino[3] = {type, O2, color, {pos_x, pos_y+3*sz}};
                default: break;
            }
            break;
        case Z:
            switch (orientation) {
                case O1:
                    mino[0] = {type, O1, color, {pos_x, pos_y}};
                    mino[1] = {type, O1, color, {pos_x+sz, pos_y}};
                    mino[2] = {type, O1, color, {pos_x+sz, pos_y+sz}};
                    mino[3] = {type, O1, color, {pos_x+2*sz, pos_y+sz}};
                    break;
                case O2:
                    mino[0] = {type, O2, color, {pos_x, pos_y}};
                    mino[1] = {type, O2, color, {pos_x, pos_y+sz}};
                    mino[2] = {type, O2, color, {pos_x-sz, pos_y+sz}};
                    mino[3] = {type, O2, color, {pos_x-sz, pos_y+2*sz}};
                default: break;
            }
            break;    
        case RZ:
            switch (orientation) {
                case O1:
                    mino[0] = {type, O1, color, {pos_x, pos_y}};
                    mino[1] = {type, O1, color, {pos_x-sz, pos_y}};
                    mino[2] = {type, O1, color, {pos_x-sz, pos_y+sz}};
                    mino[3] = {type, O1, color, {pos_x-2*sz, pos_y+sz}};
                    break;
                case O2:
                    mino[0] = {type, O2, color, {pos_x, pos_y}};
                    mino[1] = {type, O2, color, {pos_x, pos_y+sz}};
                    mino[2] = {type, O2, color, {pos_x+sz, pos_y+sz}};
                    mino[3] = {type, O2, color, {pos_x+sz, pos_y+2*sz}};
                default: break;
            }
            break;
        case L:
            switch (orientation) {
                case O1:
                    mino[0] = {type, O1, color, {pos_x, pos_y}};
                    mino[1] = {type, O1, color, {pos_x+sz, pos_y}};             //            |
                    mino[2] = {type, O1, color, {pos_x+2*sz, pos_y}};           //      ______|
                    mino[3] = {type, O1, color, {pos_x+2*sz, pos_y-sz}};
                    break;
                case O2:
                    mino[0] = {type, O2, color, {pos_x, pos_y}};             
                    mino[1] = {type, O2, color, {pos_x, pos_y+sz}};          //      |    
                    mino[2] = {type, O2, color, {pos_x, pos_y+2*sz}};        //      |__
                    mino[3] = {type, O2, color, {pos_x+sz, pos_y+2*sz}};
                    break;
                case O3:
                    mino[0] = {type, O3, color, {pos_x, pos_y}};             //       ______  
                    mino[1] = {type, O3, color, {pos_x, pos_y-sz}};          //      |    
                    mino[2] = {type, O3, color, {pos_x+sz, pos_y-sz}};       
                    mino[3] = {type, O3, color, {pos_x+2*sz, pos_y-sz}};
                    break;
                case O4:
                    mino[0] = {type, O4, color, {pos_x, pos_y}};             //      __  
                    mino[1] = {type, O4, color, {pos_x+sz, pos_y}};          //        |    
                    mino[2] = {type, O4, color, {pos_x+sz, pos_y+sz}};       //        | 
                    mino[3] = {type, O4, color, {pos_x+sz, pos_y+2*sz}};
            }
            break;
        case RL:
            switch (orientation) {
                case O1:
                    mino[0] = {type, O1, color, {pos_x, pos_y}};
                    mino[1] = {type, O1, color, {pos_x, pos_y+sz}};         //         |
                    mino[2] = {type, O1, color, {pos_x+sz, pos_y+sz}};      //         |_______ 
                    mino[3] = {type, O1, color, {pos_x+2*sz, pos_y+sz}};
                    break;
                case O2:
                    mino[0] = {type, O2, color, {pos_x, pos_y}};
                    mino[1] = {type, O2, color, {pos_x-sz, pos_y}};         //         __
                    mino[2] = {type, O2, color, {pos_x-sz, pos_y+sz}};      //        | 
                    mino[3] = {type, O2, color, {pos_x-sz, pos_y+2*sz}};    //        |  
                    break;
                case O3:
                    mino[0] = {type, O3, color, {pos_x, pos_y}};
                    mino[1] = {type, O3, color, {pos_x+sz, pos_y}};         //          ____
                    mino[2] = {type, O3, color, {pos_x+2*sz, pos_y}};       //              |    
                    mino[3] = {type, O3, color, {pos_x+2*sz, pos_y+sz}};            
                    break;
                case O4:
                    mino[0] = {type, O4, color, {pos_x, pos_y}};
                    mino[1] = {type, O4, color, {pos_x, pos_y+sz}};         //          |
                    mino[2] = {type, O4, color, {pos_x, pos_y+2*sz}};       //        __|    
                    mino[3] = {type, O4, color, {pos_x-sz, pos_y+2*sz}};    //          
            }
            break;
        case T:
            switch (orientation) {
                case O1:
                    mino[0] = {type, O1, color, {pos_x, pos_y}};
                    mino[1] = {type, O1, color, {pos_x+sz, pos_y}};      
                    mino[2] = {type, O1, color, {pos_x+2*sz, pos_y}};    //      __|__
                    mino[3] = {type, O1, color, {pos_x+sz, pos_y-sz}};
                    break;
                case O2:
                    mino[0] = {type, O2, color, {pos_x, pos_y}};
                    mino[1] = {type, O2, color, {pos_x, pos_y+sz}};      
                    mino[2] = {type, O2, color, {pos_x, pos_y+2*sz}};    //      |_
                    mino[3] = {type, O2, color, {pos_x+sz, pos_y+sz}};  //       |   
                    break;
                case O3:
                    mino[0] = {type, O3, color, {pos_x, pos_y}};
                    mino[1] = {type, O3, color, {pos_x+sz, pos_y}};      //     _____
                    mino[2] = {type, O3, color, {pos_x+2*sz, pos_y}};    //       |
                    mino[3] = {type, O3, color, {pos_x+sz, pos_y+sz}};
                    break;
                case O4:
                    mino[0] = {type, O4, color, {pos_x, pos_y}};
                    mino[1] = {type, O4, color, {pos_x, pos_y+sz}};      //    _|
                    mino[2] = {type, O4, color, {pos_x, pos_y+2*sz}};    //     |  
                    mino[3] = {type, O4, color, {pos_x-sz, pos_y+sz}};
            }
            break;
        case SQUARE: default:
            mino[0] = {type, O1, color, {pos_x, pos_y}};
            mino[1] = {type, O1, color, {pos_x+sz, pos_y}};
            mino[2] = {type, O1, color, {pos_x, pos_y+sz}};
            mino[3] = {type, O1, color, {pos_x+sz, pos_y+sz}};
    }
}

void Tetris::create_mino() noexcept {

    // Проверить если текущего объекта не существует, тогда
    // Определить рандомно тип объекта
    // Определить рандомную позицию по горизонтали,
    // так чтоб вновь созданный объект не выходил за рамки.
    if (current) return;
    mino_id++;
   
    Type type           = (Type) gen_type();
    cv::Scalar color    = random_color(rng);
    int pos_x           = gen_pos()*sz;
    int pos_y           = 0;

    std::array<Mino, 4> mino;
    create_mino(type, O1, color, pos_x, pos_y, mino);
    minos.emplace(mino_id, mino);
    current = mino_id;
    LOGI("New mino type %d, pos %d size %d", type, pos_x, sz)
}

bool Tetris::move_down() noexcept {

    if (!current || game_over_flag) return false;

    const std::array<Mino, 4>& mino = minos[current];
    
    for (const auto& cur_el: mino) {

        if (cur_el.TL.y+sz > viewport.second) return false;        // Если коснулись дна

        for (const auto& el: minos) {

            if (current==el.first) continue;

            for (const auto& next: el.second)
                if (next.enabled && cur_el.TL.y+sz > next.TL.y) {

                    const cv::Rect r(next.TL.x, next.TL.y, sz, sz);
                    if (r.contains({cur_el.TL.x, cur_el.TL.y+sz}) ||
                        r.contains({cur_el.TL.x+sz-1, cur_el.TL.y+sz})) return false;
            }
        }
    }

    for (auto& el: minos[current]) el.TL.y++;
    return true; 
}

void Tetris::move_left() noexcept {

    if (!current || game_over_flag) return;
    
    const std::array<Mino, 4>& mino = minos[current];

    for (const auto& cur_el: mino) {

        if (cur_el.TL.x-sz < 0) return;        // Если коснулись левой стенки

        for (const auto& el: minos) {

            if (current==el.first) continue;

            for (const auto& next: el.second)
                if (next.enabled && cur_el.TL.x-sz < next.TL.x+sz) {

                    const cv::Rect r(next.TL.x, next.TL.y, sz, sz);
                    if (r.contains({cur_el.TL.x-sz, cur_el.TL.y}) ||
                        r.contains({cur_el.TL.x-sz, cur_el.TL.y+sz})) return;
            }
        }
    }
  
    for (auto& el: minos[current]) el.TL.x-=sz;
    listeners.on_left(minos[current][0].type);
}

void Tetris::move_right() noexcept {

    if (!current || game_over_flag) return;
    
    const std::array<Mino, 4>& mino = minos[current];

    for (const auto& cur_el: mino) {

        if (cur_el.TL.x+sz >= viewport.first) return;        // Если коснулись правой стенки

        for (const auto& el: minos) {

            if (current==el.first) continue;

            for (const auto& next: el.second)
                if (next.enabled && cur_el.TL.x+sz >= next.TL.x) {

                    const cv::Rect r(next.TL.x, next.TL.y, sz, sz);
                    if (r.contains({cur_el.TL.x+sz, cur_el.TL.y}) ||
                        r.contains({cur_el.TL.x+sz, cur_el.TL.y+sz})) return;
            }
        }
    }
  
    for (auto& el: minos[current]) el.TL.x+=sz;
    listeners.on_right(minos[current][0].type);
}

bool Tetris::can_rotate(const std::array<Mino, 4>& mino) noexcept {

    for (const auto& cur_el: mino) {

        if (cur_el.TL.x+sz >= viewport.first) return false;      
        if (cur_el.TL.x-sz < 0) return false;

        for (const auto& el: minos) {

            if (current==el.first) continue;

            for (const auto& next: el.second) {

                if (!next.enabled) continue;

                const cv::Rect r(next.TL.x, next.TL.y, sz, sz);
                if (r.contains({cur_el.TL.x+sz, cur_el.TL.y}) ||
                    r.contains({cur_el.TL.x+sz, cur_el.TL.y+sz}) ||
                    r.contains({cur_el.TL.x-sz, cur_el.TL.y}) ||
                    r.contains({cur_el.TL.x-sz, cur_el.TL.y+sz})) return false;
            }
        }
    }
    return true;
}

void Tetris::rotate() noexcept {

    if (!current || game_over_flag) return;

    const std::array<Mino, 4>& mino = minos[current];
    std::array<Mino, 4> new_mino;

    switch (mino[0].type) {
        case LINE: case Z: case RZ:

            switch (mino[0].orientation) {
                case O1: 
                    create_mino(mino[0].type, O2, mino[0].color, mino[0].TL.x, mino[0].TL.y, new_mino);
                    break;
                case O2:
                    create_mino(mino[0].type, O1, mino[0].color, mino[0].TL.x, mino[0].TL.y, new_mino); 
                default: break;
            }
            break;
        case L: case RL: case T:

            switch (mino[0].orientation) {
                case O1: 
                    create_mino(mino[0].type, O2, mino[0].color, mino[0].TL.x, mino[0].TL.y, new_mino);
                    break;
                case O2:
                    create_mino(mino[0].type, O3, mino[0].color, mino[0].TL.x, mino[0].TL.y, new_mino);
                    break;
                case O3:
                    create_mino(mino[0].type, O4, mino[0].color, mino[0].TL.x, mino[0].TL.y, new_mino);
                    break;
                case O4:
                    create_mino(mino[0].type, O1, mino[0].color, mino[0].TL.x, mino[0].TL.y, new_mino);
            }
            break;            
        case SQUARE: default: return;
    }

    if (can_rotate(new_mino)) {
        
         minos[current] = new_mino;
         listeners.on_rotate(minos[current][0].type);
    }
}

uint32_t Tetris::get_min_line() {
        
    auto min_y = [] (const std::array<Mino, 4>& m1, const std::array<Mino, 4>& m2) { 
            
        auto a = std::min_element(m1.begin(), m1.end(), [] (const auto& m1, const auto& m2) { return m1.TL.y<m2.TL.y;} );
        auto b = std::min_element(m2.begin(), m2.end(), [] (const auto& m1, const auto& m2) { return m1.TL.y<m2.TL.y;} ); 
        return a->TL.y<b->TL.y; 
    };
    
    auto min_y_mino = std::min_element(minos.begin(), minos.end(), 
            [&] (const auto& m1, const auto& m2) { return min_y(m1.second, m2.second); });

    return std::min_element(
            min_y_mino->second.begin(), min_y_mino->second.end(), 
                            [] (const auto& m1, const auto& m2) { return m1.TL.y<m2.TL.y;} )->TL.y/sz;
}

void Tetris::delete_lines(uint32_t from_line, uint32_t& ret) {

    if (from_line==get_min_line()) return;
    
    std::vector<Mino*> to_delete;
    to_delete.reserve(sizeof(Mino*)*SQ_PER_LINE);

    for (auto& itm: minos)
        std::for_each(itm.second.begin(), itm.second.end(), 
            [&] (auto& item) { if (item.enabled && item.TL.y/sz==from_line) to_delete.push_back(&item); });

    //LOGI("To delete %d", (int)to_delete.size())
    if (to_delete.size()==SQ_PER_LINE) {

        std::for_each(to_delete.begin(), to_delete.end(), [] (auto item) { item->enabled=false; });

        for (auto& itm: minos)
            std::for_each(itm.second.begin(), itm.second.end(), 
                [=] (auto& item) { if (item.enabled && item.TL.y/sz<from_line) item.TL.y+=sz; });
        
        delete_lines(from_line, ++ret);
    } else {
        delete_lines(from_line-1, ret);
    }
}

bool Tetris::game_over() {

    return previous > 0 && 
        std::min_element(minos[previous].begin(), minos[previous].end(), 
                           [] (const auto& m1, const auto& m2) { return m1.TL.y<m2.TL.y; } )->TL.y/sz <= 3;
}

void Tetris::apply() {
    
    char buf[256];

    create_mino();
    if (!game_over_flag && !move_down()) {
        
        previous = current;
        current = 0;
        uint32_t deleted = 0;
        delete_lines(SQ_PER_COL, deleted);
        game_over_flag = game_over();
        Type type = minos[previous][0].type;
        
        if (deleted>0) {        // опускаем верхние блоки 

            score += deleted;
            listeners.on_delete(type);

            for (auto& itm: minos) {

                int cnt=0;
                std::for_each(itm.second.begin(), itm.second.end(), [&] (const auto& item) { if (!item.enabled) cnt++; });
                if (cnt==4) minos.erase(itm.first);
            }
        } else {
            listeners.on_ground(type);
        }
    }

    zeros.copyTo(img);
    for (auto& mino: minos)
        for (auto& el: mino.second)   
            if (el.enabled) cv::rectangle(img, cv::Rect(el.TL.x, el.TL.y, sz, sz), el.color, -1); 
    
    snprintf(buf, 256, "Score: %d%c", score, '\0');
    cv::putText(img, buf, {0,30}, cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0,255,0));

    if (game_over_flag) {
        
        snprintf(buf, 256, "Game over%c", '\0');
        cv::putText(img, buf, {viewport.first/4,viewport.second/2}, cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0,0,255), 3);
    }
}