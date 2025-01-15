#pragma once
#include <GL/glew.h>

static void check_gl_error(const std::string& context) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error (" << context << "): " << err << std::endl;
        __debugbreak();
    }
}
void show_uniforms(GLuint programID)
{
    GLint numUniforms;
    glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numUniforms);

    for (GLint i = 0; i < numUniforms; ++i) {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(programID, i, sizeof(name), &length, &size, &type, name);
        printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
    }
}