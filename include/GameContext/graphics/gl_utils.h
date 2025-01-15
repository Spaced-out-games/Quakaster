#pragma once
#include <GL/glew.h>

static void check_gl_error(const std::string& context) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error (" << context << "): " << err << std::endl;
        __debugbreak();
    }
}