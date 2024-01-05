#pragma once

#include <glad/glad.h>

#ifdef NDEBUG
        #define CALLGL(proc) proc;

#else
        #define CALLGL(proc) proc; \
                err = glGetError();\
                if (err != GL_NO_ERROR) GLError(err, __LINE__, __FILE__, #proc);
#endif

GLuint create_shader(const char* path, GLenum type);
GLuint create_programm(GLuint vertex_shader, GLuint fragment_shader);
GLuint ext_texture();
GLuint d2_texture();
GLuint d2_texture(GLsizei width, GLsizei height, GLenum format, GLubyte* data);
void GLError(GLenum err, int line, const char* file, const char* proc);