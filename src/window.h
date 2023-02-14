#pragma oce

#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "types.h"

using key_listener = std::function<void(int key, int action)>;
using draw_listener = std::function<void()>;

class Window {
private:
    GLFWwindow* window {nullptr};
    key_listener key_press;
    draw_listener draw_callbak;
public:
    Window(dims size, const char* title);
    bool should_close() { return glfwWindowShouldClose(window); }
    void close() { glfwSetWindowShouldClose(window, 1); }
    void draw();
    void set_on_key_press_listener(key_listener cb) { key_press = cb; }
    void set_on_draw_listener(draw_listener cb) { draw_callbak = cb;}
    ~Window();
};