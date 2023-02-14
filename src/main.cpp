#include <iostream>
#include <memory>

#include "log.h"
#include "types.h"
#include "window.h"
#include "tetris.h"
#include "render.h"
#include "player.h"
#include "sound.h"

static const int WIDTH = 480;
static const int HEIGHT = 640;
static std::unique_ptr<Window> game_window;
static std::unique_ptr<Render> render;
static Tetris* tetris;
static std::vector<Filter*> filters;
static std::unique_ptr<Sound> snd_move, snd_ground;
static std::unique_ptr<Player> player;

static void init() {

    player      = std::make_unique<Player>();
    snd_move    = std::make_unique<Sound>("../media/move.wav");
    snd_ground  = std::make_unique<Sound>("../media/ground.wav");

    game_window = std::make_unique<Window>(dims{WIDTH, HEIGHT}, "Tetris");
    game_window->set_on_key_press_listener (
        [] (int key, int action) {

            if (action==GLFW_PRESS) {

                switch (key) {
                    case GLFW_KEY_RIGHT:    tetris->move_right(); break;
                    case GLFW_KEY_LEFT:     tetris->move_left();  break; 
                    case GLFW_KEY_UP:       tetris->rotate();     break; 
                    case GLFW_KEY_DOWN:     tetris->move_down();  break; // ?  
                    case GLFW_KEY_ESCAPE:   game_window->close();
                }
            }
        }
    );

    game_window->set_on_draw_listener([] () { render->render(); });

    render = std::make_unique<Render>(dims{WIDTH, HEIGHT});
    Tetrislisteners list = {
        .on_left    = [] (Type) { player->play(snd_move->buf()); },
        .on_right   = [] (Type) { player->play(snd_move->buf()); },
        .on_rotate  = [] (Type) { player->play(snd_move->buf()); },
        .on_ground  = [] (Type) { player->play(snd_ground->buf()); }
    };

    filters.push_back(new Tetris(dims{WIDTH, HEIGHT}, list));
    render->attach_filters(filters);
    tetris = (Tetris*) filters[0];

    //player = std::make_unique<Player>();
}

static void loop() {

    while (! game_window->should_close()) {

        game_window->draw();
    }
}

static void clear() {

    //player.reset(nullptr);
    render->wait_finish();
    for (auto f: filters) delete f;
    render.reset(nullptr);
    game_window.reset(nullptr);
    snd_ground.reset(nullptr);
    snd_move.reset(nullptr);
    player.reset(nullptr);
}

int main(int argc, char** argv) {

    int result = 0;
    try{
        init();
        loop();
        clear();

    } catch (std::exception& e) {
        LOGE("%s", e.what())
        result = errno;
    }
    catch(...) {
        LOGE("Unknown error, %d", errno)
        result = errno;
    }
    LOGI("End.")
    return result;
}