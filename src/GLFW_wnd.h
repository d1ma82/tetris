#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.h"
#include "render.h"
#include "types.h"

class GLFW: public Window {
private:
    GLFWwindow* window {nullptr};
    KeyListener* events {nullptr};
    Render* render;
public:
    GLFW(dims size, const char* title);
    bool should_close() { return glfwWindowShouldClose(window); }
    void close() { glfwSetWindowShouldClose(window, 1); }
    void draw();
    void attach_filters(std::vector<Filter*> filters) { render->attach_filters(filters); }
    void set_key_event_listener(KeyListener* events);
    ~GLFW();
};