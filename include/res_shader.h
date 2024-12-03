#include <GL/glew.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
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
            console_log("Uniform " + uniformName + " not found in shader.", console_colors::DEFAULT_ERROR_CRITICAL);
            __debugbreak();

            //throw std::runtime_error("Uniform " + uniformName + " not found in shader.");
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
            #ifdef _DEBUG
            console_log(std::string("Error linking shader program: " + std::string(infoLog)), console_colors::DEFAULT_WARNING_SEVERE);
            __debugbreak();

            #endif
            // Debug-only runtime warning, basically. 


        }
    }

    ~res_shader() {
        #ifdef _DEBUG
        console_log(std::string("Deleted shader with ID: ") + std::to_string((int)programID), console_colors::DEFAULT_WARNING);

        #endif
        
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

    static std::string loadShaderCodeFromFile(const std::string& path) {
        std::ifstream shaderFile(path);
        if (!shaderFile.is_open()) {
            console_log(std::string("Could not open shader at: " + path), console_colors::DEFAULT_WARNING_SEVERE);
        }

        std::stringstream buffer;
        buffer << shaderFile.rdbuf();
        return buffer.str();
    }

    static GLuint loadShader(const std::string& path, GLenum shaderType) {
        // Load the shader code from file
        std::string shaderCode = loadShaderCodeFromFile(path);

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

            // Use the ShaderDebugInfo to output the error message with additional context

        }

        return shaderID;
    }
};
