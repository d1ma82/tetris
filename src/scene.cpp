#include <exception>
#include "scene.h"
#include "log.h"

enum {PIXEL, VERTEX, ELEMENT, BUF_COUNT};

const GLfloat vertex[20] {
                    // positions              texture coords
                     1.0f,  1.0f, 0.0f,        1.0f, 0.0f,     // top right
                     1.0f, -1.0f, 0.0f,        1.0f, 1.0f,     // bottom right
                    -1.0f, -1.0f, 0.0f,        0.0f, 1.0f,     // bottom left
                    -1.0f,  1.0f, 0.0f,        0.0f, 0.0f      // top left
};

const GLuint indices[6] {
                    0, 1, 3, 
                    1, 2, 3
};

static GLint err;

Scene::Scene(
    dims viewport, 
    const char* fragment_src, 
    const char* vertex_src): 
        viewport{viewport}, fsrc{fragment_src}, vsrc{vertex_src} {

    LOGI("GL_VERSION: %s, GL_SHADER_LANG_VERSION: %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION))

    LOGD("Create shaders")
    vertex_shader = create_shader(vsrc, GL_VERTEX_SHADER);
    if (vertex_shader==0) { LOGE("Create vertex shader fails") throw std::exception(); }
    fragment_shader = create_shader(fsrc, GL_FRAGMENT_SHADER);
    if (fragment_shader==0) { LOGE("Create fragment shader fails") throw std::exception(); }
    
    LOGD("Create program")
    id = create_programm(vertex_shader, fragment_shader);
    if (id==0) { LOGE("Create program fails") throw std::exception(); }

    LOGD("Create texture")
    texture = d2_texture(viewport.first, viewport.second, GL_RGB, nullptr);
    if (texture==0) {LOGE("Create texture fails") throw std::exception(); }

    LOGD("Bind buffers")
    bind_buffers();
}

void Scene::bind_buffers() {

    glGenVertexArrays(1, &vertex_array);
    glGenBuffers(BUF_COUNT, buffers);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffers[PIXEL]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, viewport.first*viewport.second*3, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    LOGI("VAO=%d; VBO=%d; EBO=%d; PBO=%d", vertex_array, buffers[VERTEX], buffers[ELEMENT], buffers[PIXEL])

    CALLGL(glBindVertexArray(vertex_array))
    CALLGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[ELEMENT]))
    CALLGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW))

    CALLGL(glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTEX]))
    CALLGL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW))

    CALLGL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (void*)0))
    CALLGL(glEnableVertexAttribArray(0))

    CALLGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT))))
    CALLGL(glEnableVertexAttribArray(1))

    CALLGL(glUseProgram(id))
    CALLGL(glUniform1i(glGetUniformLocation(id, "camera"), 0))
 
    CALLGL(glBindVertexArray(0))
    glViewport(0, 0, viewport.first, viewport.second);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Scene::draw() {
    
    CALLGL(glBindTexture(GL_TEXTURE_2D, texture))
    CALLGL(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffers[PIXEL]))
    CALLGL(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport.first, viewport.second, GL_RGB, GL_UNSIGNED_BYTE, 0))
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    CALLGL(glBindVertexArray(vertex_array))
    CALLGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0))
    CALLGL(glBindTexture(GL_TEXTURE_2D, 0))
    CALLGL(glBindVertexArray(0))
}

GLubyte* Scene::map_frame() {

    CALLGL(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffers[PIXEL]))
    GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    return ptr;
}

void Scene::unmap_frame() {
    CALLGL(glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER)) 
    CALLGL(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0))
}

Scene::~Scene() {

    LOGI("Destroy scene")
    if (texture>0) glDeleteTextures(1, &texture);
    
    if (fragment_shader>0) {
        if (id>0) glDetachShader(id, fragment_shader);
        glDeleteShader(fragment_shader);
    }

    if (vertex_shader>0) {
        if (id>0) glDetachShader(id, vertex_shader);
        glDeleteShader(vertex_shader);
    }

    if (id>0) glDeleteProgram(id);

    glDeleteBuffers(BUF_COUNT, buffers);
    glDeleteVertexArrays(1, &vertex_array);
}