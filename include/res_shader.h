#include <string>
#include <GL/glew.h>
#include <iostream>
#include "scene.h"


class res_shader {
private:
    Scene* scene = nullptr;
    GLuint programID;         // OpenGL shader program ID
    int refCount = 0;             // Reference count

public:




    res_shader(const std::string& vertexSrc, const std::string& fragmentSrc, Scene& scene)
        : programID(0), refCount(0) {
        // Compile and link shaders
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vSource = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &vSource, nullptr);
        glCompileShader(vertexShader);

        if (!checkCompileErrors(vertexShader, "VERTEX")) {
            glDeleteShader(vertexShader);
            return;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fSource = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &fSource, nullptr);
        glCompileShader(fragmentShader);

        if (!checkCompileErrors(fragmentShader, "FRAGMENT")) {
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return;
        }

        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        if (!checkLinkErrors(programID)) {
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteProgram(programID);
            programID = 0;
            return;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~res_shader() {
        if (programID != 0) {
            glDeleteProgram(programID);
        }
    }

    GLuint getProgramID() const {
        return programID;
    }



private:
    bool checkCompileErrors(GLuint shader, const std::string& type) {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << type << " SHADER COMPILATION ERROR:\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }

    bool checkLinkErrors(GLuint program) {
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cerr << "SHADER PROGRAM LINKING ERROR:\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }
};
