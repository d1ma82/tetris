#pragma once

#include "opengl.h"
#include "types.h"

class Scene {
private:
    const char* fsrc;
    const char* vsrc;
    dims viewport;
    GLuint texture = 0;
    GLuint fragment_shader = 0;
    GLuint vertex_shader = 0;
    GLuint id = 0;
    GLuint vertex_array = 0;
    GLuint buffers[3];
    
    void bind_buffers();
public:
    Scene(dims viewport, const char* fragment_src, const char* vertex_src);
    ~Scene();
    void draw();
    GLubyte* map_frame();
    void unmap_frame();
};
