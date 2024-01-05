#include "GLFW_wnd.h"
#include "log.h"

GLFW::GLFW(dims size, const char* title) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(size.first, size.second, "Tetris", nullptr, nullptr);
    if (window==nullptr) {
        LOGE("Failed to create GLFW window")
        throw std::exception();
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        LOGE("Failed to init GLAD")
        throw std::exception();
    }

    render = new Render(size);
}

void GLFW::set_key_event_listener(KeyListener* events) {
    
    this->events = events; 
    static KeyListener* ev=this->events;            // Do not know better way to put events in lambda

    glfwSetKeyCallback(window, 
            [] (GLFWwindow*, int key, int, int action, int) 
                            { ev->on_key_press(ev->context, key, action); });
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
