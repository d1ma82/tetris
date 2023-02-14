#pragma once

#include <map>
#include "filter.h"
#include "opengl.h"

enum Type { LINE, Z, RZ, L, RL, T, SQUARE, COUNT };
enum Orientation { O1=1, O2, O3, O4 };

struct Mino {
    Type        type;
    Orientation orientation;        // Orientation of whole mino
    cv::Scalar  color;
    cv::Point   TL;                 // Top left point
    bool        enabled {true};     // if not enabled means deleted
};

using listener = std::function<void(Type type)>;

struct Tetrislisteners {

    listener on_left, on_right, on_rotate, on_ground, on_delete;
};

class Tetris: public Filter {
private:
    uint32_t    mino_id    {0};
    uint32_t    score      {0};
    const dims& viewport;
    cv::Mat     img, zeros;
    uint32_t    current {0}, previous {0}, next {0};
    bool        game_over_flag {false};
    std::map<uint32_t, std::array<Mino, 4>> minos;
    Tetrislisteners listeners;
    // Создает новый предмет
    // если текущий предмет упал или не создан
    uint32_t create_mino() noexcept;
    void create_mino(Type type, Orientation orientation, cv::Scalar color, int pos_x, int pos_y, std::array<Mino, 4>& mino) noexcept;
    // Возвращает номер линии, с наивысшей точкой
    uint32_t get_min_line();

    void delete_lines(uint32_t from_line, uint32_t& ret);
    bool game_over();
    bool can_rotate(const std::array<Mino, 4>& mino) noexcept;
public:
    Tetris(const dims& viewport, Tetrislisteners list);
    bool move_down() noexcept;
    void move_left() noexcept;
    void move_right() noexcept;
    void rotate() noexcept;
    cv::Mat frame() const noexcept { return img; }
    void apply();
    ~Tetris() {}
};
