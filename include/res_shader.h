#include <GL/glew.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <glm/glm.hpp>

class shader_handle {
public:
    // Constructor is private, accessible only to res_shader
    shader_handle(GLuint programID, const std::string& uniformName)
        : programID(programID), uniformName(uniformName) {}

    // Overload the assignment operator to set uniforms
    template<typename T>
    shader_handle& operator=(const T& value) {
        GLint location = glGetUniformLocation(programID, uniformName.c_str());
        if (location == -1) {
            throw std::runtime_error("Uniform " + uniformName + " not found in shader.");
        }
        setUniformValue(location, value);
        return *this;
    }

private:
    GLuint programID;
    std::string uniformName;

    // Helper methods to set the uniform values based on type
    void setUniformValue(GLint location, int value) {
        glUniform1i(location, value);
    }

    void setUniformValue(GLint location, float value) {
        glUniform1f(location, value);
    }

    void setUniformValue(GLint location, const glm::vec3& value) {
        glUniform3fv(location, 1, &value[0]);
    }

    void setUniformValue(GLint location, const glm::mat4& value) {
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    }
};

class res_shader {
public:
    res_shader(const std::string& vertexPath, const std::string& fragmentPath) {
        // Load and compile the shaders
        GLuint vertexShader = loadShader(vertexPath, GL_VERTEX_SHADER);
        GLuint fragmentShader = loadShader(fragmentPath, GL_FRAGMENT_SHADER);

        // Link the program
        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        // Clean up shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Check for linking errors
        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(programID, 512, nullptr, infoLog);

            console_log(std::string("Error linking shader program: " + std::string(infoLog)), console_colors::DEFAULT_WARNING_SEVERE);
            // Debug-only runtime warning, basically. 
            //__debugbreak();


        }
    }

    ~res_shader() {
        glDeleteProgram(programID);
    }

    // Overload [] operator to return a shader_handle for uniform access
    shader_handle operator[](const std::string& uniformName) {
        return shader_handle(programID, uniformName); // Return a shader_handle tied to this shader
    }

    void use() const {
        glUseProgram(programID);
    }

    static void unbind() {
        glUseProgram(0);
    }

private:
    GLuint programID;

    static GLuint loadShader(const std::string& path, GLenum shaderType) {
        std::string shaderCode;
        // Load the shader code from file (not implemented here, but you can use std::ifstream)

        GLuint shaderID = glCreateShader(shaderType);
        const char* code = shaderCode.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);
        glCompileShader(shaderID);

        // Check for compilation errors
        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
            throw std::runtime_error("Error compiling shader: " + std::string(infoLog));
        }
        return shaderID;
    }
};
