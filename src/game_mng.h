#include <memory>
#include "tetris.h"
#include "window.h"

namespace manager {

static std::unique_ptr<Tetris> game = nullptr;

static void on_key(int key, int action) {

    if (action == PRESS) {
        switch(key) {
            case KEY_LEFT:   game->move_left(); break;
            case KEY_RIGHT:  game->move_right(); break;
            case KEY_DOWN:   break;
            case KEY_UP:     game->rotate(); break;
            case KEY_ESCAPE: game->close(); break;
        }
    }
}

static KeyEvents key_listener(on_key);
}