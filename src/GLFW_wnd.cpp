#include "GLFW_wnd.h"
#include "log.h"

GLFW::GLFW(dims size, const char* title) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(size.first, size.second, "Tetris", nullptr, nullptr);
    if (window==nullptr) {
        LOGE("Failed to create GLFW window")
        abort();
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        LOGE("Failed to init GLAD")
        abort();
    }

    render = new Render(size);
}

void GLFW::set_key_event_listener(KeyEvents* events) {
    
    this->events = events; 
    static KeyEvents* ev=this->events;            // Do not know better way to put events in lambda

    glfwSetKeyCallback(window, 
            [] (GLFWwindow*, int key, int, int action, int) 
                            { ev->on_key_press(key, action); });
}

void GLFW::draw() {
    
    render->render();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

GLFW::~GLFW() {

    LOGI("Destroy window")
    delete render;
    glfwTerminate();
}