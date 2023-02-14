#pragma once

#include <vector>
#include "opengl.h"
#include "filter.h"
#include "log.h"

class Render {
    dims          viewport;
    Program       output;
    GLenum        err;
    bool          finish    {true};         //  |
    bool          stop      {false};        //  |   Flags to sync GLThread
    std::vector<Filter*> filters;

    const GLfloat vertex[20] {
        // positions              texture coords
         1.0f,  1.0f, 0.0f,        1.0f, 0.0f,     // top right
         1.0f, -1.0f, 0.0f,        1.0f, 1.0f,     // bottom right
        -1.0f, -1.0f, 0.0f,        0.0f, 1.0f,     // bottom left
        -1.0f,  1.0f, 0.0f,        0.0f, 0.0f      // top left             
    };

    const GLuint indices[6] {
        0, 1, 3,    // first triangle
        1, 2, 3     // second
    };

    enum { VERTEX_ATTRIB, TEXTURE_ATTRIB };

    void toogle_finish() { 
        if (!stop) finish=!finish;
    }

public:
    Render(dims viewport_input): viewport{viewport_input} {
        
        LOGI("GL_VERSION: %s, GL_SHADER_LANG_VERSION: %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION))
        LOGI("Configure output")
        output.fragment_shader =   create_shader("../shader/out.frag", GL_FRAGMENT_SHADER);
        output.texture         =   d2_texture(viewport.first, viewport.second, nullptr);
        output.vertex_shader   =   create_shader("../shader/out.vert", GL_VERTEX_SHADER);
        output.id              =   create_programm(output.vertex_shader, output.fragment_shader);

        CALLGL(glGenVertexArrays(1, &output.VAO))
        CALLGL(glGenBuffers(1, &output.array[VERTEX]))
        CALLGL(glGenBuffers(1, &output.array[ELEMENT]))

        CALLGL(glBindVertexArray(output.VAO))

        CALLGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, output.array[ELEMENT]))
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, output.array[VERTEX]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

        glVertexAttribPointer(VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(TEXTURE_ATTRIB, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        CALLGL(glUseProgram(output.id))
        glUniform1i(glGetUniformLocation(output.id, "camera"), 0);
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(0);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }

    void render() { 

        toogle_finish();
        if (finish) return;
        
        for (auto filter: filters) filter->apply();

        glViewport(0, 0, viewport.first, viewport.second);
        glClear(GL_COLOR_BUFFER_BIT);

        CALLGL(glUseProgram(output.id))

        CALLGL(glBindVertexArray(output.VAO))
        CALLGL(glBindTexture(GL_TEXTURE_2D, output.texture))
        CALLGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewport.first, viewport.second, 0, GL_BGR, GL_UNSIGNED_BYTE, (*filters.rbegin())->frame().data ))
        CALLGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0))
        CALLGL(glBindTexture(GL_TEXTURE_2D, 0))
        glBindVertexArray(0);
        toogle_finish();
    }

    void wait_finish() { while (!finish) {}; stop=true; }
    void attach_filters(const std::vector<Filter*>& filters_in) { filters=filters_in; }
    ~Render() { }
};