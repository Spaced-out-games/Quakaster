#pragma once
#include <iostream>
#include <vector>
#include <cassert>
#include <include/thirdparty/glm/glm.hpp>
#include <GL/glew.h>
#include <include/GameContext/graphics/gl_utils.h>

class VBO {
public:
    /*
    Ensure a function with signature "static void vertex_t::pointer_setup()" exists.
    This function sets up function pointers. See default_vertex_t for reference.
    */
    VBO(){
//        __debugbreak();
    }

    template <typename vertex_t = default_vertex_t>
    VBO(const std::vector<vertex_t>& vertices) {
        init<vertex_t>(vertices);
    }

    // Deleted copy constructor and copy assignment operator
    VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;

    // Deleted move constructor and move assignment operator
    VBO(VBO&&) = delete;
    VBO& operator=(VBO&&) = delete;

    template <typename vertex_t>
    void init(const std::vector<vertex_t>& vertices) {
        glGenBuffers(1, &vboID); // Generate a VBO
        assert(vboID != 0 && "Failed to generate VBO"); // Ensure VBO is generated
        std::cerr << "VBO generated with ID: " << vboID << std::endl;

        bind(); // Bind the VBO

        // Upload vertex data to the GPU
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex_t), vertices.data(), GL_STATIC_DRAW);
        check_gl_error("glBufferData");

        size = vertices.size();
        std::cerr << "Uploaded " << size << " vertices to VBO (ID: " << vboID << ")." << std::endl;

        // Set up vertex attribute pointers
        std::cerr << "Setting up vertex attribute pointers for VBO (ID: " << vboID << ")." << std::endl;
        vertex_t::set_pointers();
        check_gl_error("vertex_t::set_pointers");
    }


    ~VBO() {
        if (vboID != 0) {
            glDeleteBuffers(1, &vboID); // Cleanup
            std::cerr << "Deleted VBO (ID: " << vboID << ")." << std::endl;
        }
    }

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, vboID); // Bind the VBO
        check_gl_error("glBindBuffer");
        std::cerr << "Bound VBO (ID: " << vboID << ")." << std::endl;
    }

    static void unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind any VBO
        check_gl_error("glBindBuffer (unbind)");
        std::cerr << "Unbound VBO." << std::endl;
    }

    // Static functions to set up attribute pointers
    static void add_vec3_pointer(GLuint index, GLsizei stride, const void* offset) {
        glEnableVertexAttribArray(index);
        check_gl_error("glEnableVertexAttribArray");
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, offset);
        check_gl_error("glVertexAttribPointer (vec3)");
        std::cerr << "Set vec3 attribute pointer at index " << index << "." << std::endl;
    }

    static void add_vec2_pointer(GLuint index, GLsizei stride, const void* offset) {
        glEnableVertexAttribArray(index);
        check_gl_error("glEnableVertexAttribArray");
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride, offset);
        check_gl_error("glVertexAttribPointer (vec2)");
        std::cerr << "Set vec2 attribute pointer at index " << index << "." << std::endl;
    }

    static void add_vec4_pointer(GLuint index, GLsizei stride, const void* offset) {
        glEnableVertexAttribArray(index);
        check_gl_error("glEnableVertexAttribArray");
        glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, stride, offset);
        check_gl_error("glVertexAttribPointer (vec4)");
        std::cerr << "Set vec4 attribute pointer at index " << index << "." << std::endl;
    }

    static void add_float_pointer(GLuint index, GLsizei stride, const void* offset) {
        glEnableVertexAttribArray(index);
        check_gl_error("glEnableVertexAttribArray");
        glVertexAttribPointer(index, 1, GL_FLOAT, GL_FALSE, stride, offset);
        check_gl_error("glVertexAttribPointer (float)");
        std::cerr << "Set float attribute pointer at index " << index << "." << std::endl;
    }

    static void add_int_pointer(GLuint index, GLsizei stride, const void* offset) {
        glEnableVertexAttribArray(index);
        check_gl_error("glEnableVertexAttribArray");
        glVertexAttribIPointer(index, 1, GL_INT, stride, offset);
        check_gl_error("glVertexAttribIPointer (int)");
        std::cerr << "Set int attribute pointer at index " << index << "." << std::endl;
    }

    GLsizei get_vertex_count() const { return size; }

private:
    GLsizei size = 0;
    GLuint vboID = 0xff; // The ID of the VBO

    
};

struct default_vertex_t {
    glm::vec3 position; // Explicit member for position

    static void set_pointers() {
        VBO::add_vec3_pointer(0, sizeof(default_vertex_t), (void*)offsetof(default_vertex_t, position));
        std::cerr << "Default vertex pointer set for position (vec3)." << std::endl;
    }
};
