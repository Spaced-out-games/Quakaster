#pragma once
#include <GL/glew.h>
#include <include/thirdparty/entt.hpp>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <include/GameContext/components/camera.h>

class res_shader;

extern std::string root_directory;

class shader_handle {
public:
    // Constructor is private, accessible only to res_shader
    shader_handle(GLuint programID, GLint location, std::string name = "N/a")
        : programID(programID), uniformLocation(location), name(name) {}

    // Overload the assignment operator to set uniforms
    template<typename T>
    shader_handle& operator=(const T& value) {
        if (uniformLocation == -1) {
            std::cout << "Uniform location is invalid (ID: " << name << ")";
             __debugbreak();
        }
        else
        {
            set_uniform(uniformLocation, value);
        }
        return *this;
    }



    // Helper methods to set the uniform values based on type
    void set_uniform(GLint location, int value) {
        glUniform1i(location, value);
    }

    void set_uniform(GLint location, float value) {
        glUniform1f(location, value);
    }

    void set_uniform(GLint location, const glm::vec3& value) {
        glUniform3fv(location, 1, &value[0]);
    }

    void set_uniform(GLint location, const glm::mat4& value) {
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    }

private:
    #ifdef _DEBUG
    std::string name;
    #endif
    GLuint programID;
    GLint uniformLocation; // Store uniform location instead of name
};


class res_shader {
public:

    // Helper methods to set the uniform values based on type
    void set_uniform(int value) {
        glUniform1i(program_ID, value);
    }

    void set_uniform(float value) {
        glUniform1f(program_ID, value);
    }

    void set_uniform(const glm::vec3& value) {
        glUniform3fv(program_ID, 1, &value[0]);
    }

    void set_uniform(const glm::mat4& value) {
        glUniformMatrix4fv(program_ID, 1, GL_FALSE, &value[0][0]);
    }
    res_shader()
    {
        //std::cout << "";
    }
    res_shader(const std::string& vertex_path, const std::string& fragment_path) {
        std::cout << "";
        compile_and_link(vertex_path, fragment_path);
    }

    inline void init(const std::string& vertex_path, const std::string& fragment_path) {
        compile_and_link(vertex_path, fragment_path);
    }

    inline void compile_and_link(const std::string& vertex_path, const std::string& fragment_path) {
        std::string error_message;


        GLuint vertex_shader = load_shader(vertex_path, GL_VERTEX_SHADER, error_message);
        if (vertex_shader == 0) {
            std::cout << "Vertex Shader Error: " << error_message;
            __debugbreak();
        }

        GLuint fragment_shader = load_shader(fragment_path, GL_FRAGMENT_SHADER, error_message);
        if (fragment_shader == 0) {
            glDeleteShader(vertex_shader);
            //throw std::runtime_error("Fragment Shader Error: " + error_message);

            std::cout << "Fragment Shader Error: " << error_message;
            __debugbreak();

        }

        program_ID = glCreateProgram();
        glAttachShader(program_ID, vertex_shader);
        glAttachShader(program_ID, fragment_shader);
        glLinkProgram(program_ID);

        GLint success;
        glGetProgramiv(program_ID, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar log[512];
            glGetProgramInfoLog(program_ID, 512, nullptr, log);
            glDeleteProgram(program_ID);
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
            std::cout << "Shader Linking Error: " << std::string(log);
            __debugbreak();
        }

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    static GLint current_shader_id()
    {
        GLint program_ID = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &program_ID);
        return program_ID;
    }

    // Overload [] operator to return a shader_handle for uniform access
    shader_handle operator[](const std::string& uniformName) {
        GLint location = glGetUniformLocation(program_ID, uniformName.c_str());
        if (location == -1) {
            std::cout << "ERROR: could not find uniform with name '" << uniformName << "'.\n";
            __debugbreak();
        }
        return shader_handle(program_ID, location, uniformName); // Return a shader_handle with location
    }

    ~res_shader() {
        if (program_ID != 0) {
            glDeleteProgram(program_ID);
        }
    }

    void bind() {
        glUseProgram(program_ID);
        if (!Camera::target_camera) return;
        this->operator[]("u_view") = glm::inverse(Camera::target_camera->get_matrix());
        this->operator[]("u_proj") = Camera::target_camera->get_projection_matrix();

        //Camera::target_camera->set_shader_uniforms(*this, entity, registry);
    }
    void unbind() const { glUseProgram(0); }

    GLuint get_program_ID() const { return program_ID; }

    static std::string load_source(const std::string& source_path) {
        std::ifstream shader_file(std::string(root_directory + source_path));
        if (!shader_file.is_open()) {
            return "";
        }

        std::stringstream buffer;
        buffer << shader_file.rdbuf();
        return buffer.str();
    }

    static GLuint load_shader(const std::string source_path, GLenum shader_type, std::string& error_message) {

        std::string code = load_source(source_path);

        if (code.empty()) {
            error_message = "Error: Could not load shader source from: " + source_path;
            return 0;
        }

        GLuint shader_ID;
        error_message = compile_source(shader_ID, code, shader_type);
        std::cout << error_message;
        return shader_ID;
    }

    static std::string compile_source(GLuint& shader_ID, const std::string& source, GLenum shader_type) {
        shader_ID = glCreateShader(shader_type);
        const char* char_source = source.c_str();
        glShaderSource(shader_ID, 1, &char_source, nullptr);
        glCompileShader(shader_ID);

        GLint success;
        glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &success);

        if (!success) {
            GLchar log[512];
            glGetShaderInfoLog(shader_ID, 512, nullptr, log);
            std::stringstream ss;

            ss << "COMPILATION_ERROR (" << (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "): " << log << '\n';
            return ss.str();
        }

        return "";
    }


    

    static std::shared_ptr<res_shader> load(const std::string& name, const std::string& vertex_path, const std::string& fragment_path)
    {
        auto handle = res_shader::shader_cache.load(
            entt::hashed_string{ name.c_str()},
            vertex_path.c_str(),
            fragment_path.c_str()
        );
        return handle.first->second.handle();
    }
//private:
    static inline entt::resource_cache<res_shader> shader_cache;
    GLuint program_ID = 0;
    struct shader_loader {
        std::shared_ptr<res_shader> operator()(const std::string& vertex_path, const std::string& fragment_path) const {
            return std::make_shared<res_shader>(vertex_path, fragment_path);
        }
    };
};

// Resource Loader for res_shader


//using Shader = std::shared_ptr<res_shader>;

struct Shader {
    std::shared_ptr<res_shader> ptr; // Use composition instead of inheritance
    std::string debug_name;
    // Default constructor
    Shader() = default;

    // Constructor initializing the shared_ptr
    Shader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path) {
        debug_name = debug_name + name;
        init(name, vertex_path, fragment_path);
    }

    // Initialization function
    void init(const std::string& name, const std::string& vertex_path, const std::string& fragment_path) {
        debug_name = debug_name + name;
        ptr = res_shader::load(name, vertex_path, fragment_path);
    }

    // Overload operators for convenience
    res_shader* operator->() const {
        return ptr.get();
    }

    res_shader& operator*() const {
        return *ptr;
    }

    // Check if the shader is valid
    bool valid() const {
        return static_cast<bool>(ptr);
    }
};
