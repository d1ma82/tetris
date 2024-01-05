#include <vector>
#include <fstream>
#include <string>

#include "opengl.h"
#include "log.h"

static GLenum err;

void GLError(
    GLenum err, 
    int line,
    const char* file,
    const char* proc
) {
    LOGI("[FAIL GL] %s:%d, %s", file, line, proc)

    switch(err) {

        case GL_INVALID_ENUM:
            LOGE("GLenum argument out of range");
            break;

        case GL_INVALID_VALUE:
            LOGE("numeric argument out of range");
            break;

        case GL_INVALID_OPERATION:
            LOGE("operation illegal in current state");
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            LOGE("framebuffer object is not complete");
            break;

        case GL_OUT_OF_MEMORY:
            LOGE("not enough memory left to execute command");
            break;

        default:
            LOGE("unlisted error, %d", err);
            break;
    }
}

GLuint create_shader(
    const char* file, 
    GLenum type
) {
    GLuint shader       = glCreateShader(type);
    std::ifstream       src(file);
    std::string         content;

    if (!src.is_open()) {
        LOGE("Could not open file %s", file)
        abort();
    }
    src.seekg(0, std::ios::end);
    content.reserve(src.tellg());
    src.seekg(0, std::ios::beg);

    content.assign(std::istreambuf_iterator<char>(src), std::istreambuf_iterator<char>());
    const char* cstr = content.c_str();

    glShaderSource(shader, 1, &cstr, nullptr);
    glCompileShader(shader);
    src.close();

    GLint is_compiled=0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);

    if (is_compiled==GL_FALSE) {
        
        GLint length=0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<GLchar> log(length);
        glGetShaderInfoLog(shader, length, &length, log.data());
        LOGE("Could not compile shader %s - %s", cstr, log.data());        
        glDeleteShader(shader);
        shader=0;
    }
    return shader;
}

GLuint create_programm(
    GLuint vertex_shader, 
    GLuint fragment_shader
) {
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vertex_shader);
    glAttachShader(prog, fragment_shader);
    glLinkProgram(prog);
    GLint is_linked = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &is_linked);

    if (is_linked == GL_FALSE) {

        LOGE("Could not link program");
        glDeleteProgram(prog);
        return 0;
    }
    return prog;
}


GLuint ext_texture() {

    GLuint result=0;
    #ifdef ANDROID
    glGenTextures(1, &result);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, result);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    #endif
    return result;
}


GLuint d2_texture() {

    GLuint result=0;
    glGenTextures(1, &result);
    glBindTexture(GL_TEXTURE_2D, result);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return result;
}

GLuint d2_texture(GLsizei width, GLsizei height, GLenum format, GLubyte* data) {
    GLuint result=0;
    glGenTextures(1, &result);
    glBindTexture(GL_TEXTURE_2D, result);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, (GLvoid*)data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return result;    
}