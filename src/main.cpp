#include <iostream>
#include <memory>

#include "log.h"
#include "types.h"
#include "GLFW_wnd.h"
#include "default_factory.h"
#include "tetris.h"

static const int WIDTH = 480;
static const int HEIGHT = 640;
static GLFW* game_window {nullptr};
static Tetris* game {nullptr};
static std::vector<Filter*> filters;
static DefaultFactory factory;

static void init() {

    LOGI("Create window")
    game_window = new GLFW(dims{WIDTH, HEIGHT}, "Tetris");

    TetrisEventListener events = TetrisEventListener();
    events.on_ground = [] (int mino) {LOGI("On the ground %d", mino)};
    events.on_delete = [] (int lines) {LOGI("Delete %d line(s)", lines)};
    events.on_game_over = [] (int score) {LOGI("Game Over. Total %d", score)};
    events.on_left = [] (int mino) {LOGI("On left %d", mino)};
    events.on_right = [] (int mino) {LOGI("On right %d", mino)};
    events.on_rotate = [] (int mino) {LOGI("On rotete %d", mino)};
    events.on_close = [] (int result) {game_window->close();};

    game = new Tetris(dims{WIDTH, HEIGHT}, events, &factory);
    filters.push_back(game);
    game_window->set_key_event_listener(game->key_listener());
    game_window->attach_filters(filters);
}

static void loop() {

    LOGI("Start main loop.")
    while (! game_window->should_close()) {

        game_window->draw();
    }
}

static void clear() {

    LOGI("Clear resources")
    delete game;
    delete game_window;
}

int main(int argc, char** argv) {

    int result = 0;
    try{
        init();
        loop();

    } catch (std::exception& e) {
        LOGE("%s", e.what())
        result = errno;
    }
    catch(...) {
        LOGE("Unknown error, %d", errno)
        result = errno;
    }
    clear();
    LOGI("End.")
    return result;
}