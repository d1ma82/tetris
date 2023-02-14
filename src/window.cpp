#include "window.h"
#include "log.h"

static Window* _window {nullptr};

Window::Window(dims size, const char* title) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(size.first, size.second, "Tetris", nullptr, nullptr);
    
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        LOGE("Failed to init GLAD")
        abort();
    }
    glfwSetKeyCallback(window, [] (GLFWwindow*, int key, int, int action, int) { _window->key_press(key, action); });
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);  // ?
    _window = this;
}

void Window::draw() {

    draw_callbak();    
    glfwSwapBuffers(window);
    glfwPollEvents();
}

Window::~Window() {

    _window=nullptr;
    glfwTerminate();
}