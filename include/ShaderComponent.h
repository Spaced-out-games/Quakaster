#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

class ShaderPool {
    std::unordered_map<std::string, GLuint> programs;

    GLuint compileShader(const std::string& source, GLenum shaderType) {
        GLuint shader = glCreateShader(shaderType);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        // Error checking
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader Compilation Failed: " << infoLog << std::endl;
        }

        return shader;
    }

    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader) {
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        // Error checking
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            std::cerr << "Shader Linking Failed: " << infoLog << std::endl;
        }

        return program;
    }

public:
    GLuint loadShader(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string key = vertexPath + ":" + fragmentPath;
        if (programs.find(key) != programs.end())
            return programs[key];

        // Read shader files
        auto readFile = [](const std::string& path) {
            std::ifstream file(path);
            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        };

        std::string vertexSource = readFile(vertexPath);
        std::string fragmentSource = readFile(fragmentPath);

        GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
        GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

        GLuint program = linkProgram(vertexShader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        programs[key] = program;
        return program;
    }
};


struct ShaderComponent {
    GLuint programID = 0;

    ShaderComponent() : programID(0) {}
    //ShaderComponent(GLuint programID) : programID(programID) {}
    ShaderComponent(const std::string& vertexPath, const std::string& fragmentPath, ShaderPool& pool)
    {
        load(vertexPath, fragmentPath, pool);
    }

    void load(const std::string& vertexPath, const std::string& fragmentPath, ShaderPool& pool) {
        programID = pool.loadShader(vertexPath, fragmentPath);
    }

    void use() const {
        if (programID)
            glUseProgram(programID);
    }
};
