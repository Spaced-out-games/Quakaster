#pragma once
#include <GL/glew.h>
#include <vector>




class VBO {
public:
    /*
    Make sure a function with signature "static void vertex_t::pointer_setup()" exists. This function,
    as implied, sets up function pointers. See default_vertex_t for reference
    */
    template <typename vertex_t>
    VBO(const std::vector<vertex_t>& vertices) {
        glGenBuffers(1, &vboID); // Generate a VBO
        bind(); // Bind the VBO

        // Upload vertex data to the GPU
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex_t), vertices.data(), GL_STATIC_DRAW);
        size = vertices.size();
        // Set up vertex attribute pointers
        vertex_t::set_pointers();
    }

    VBO() : vboID(0) {}

    ~VBO() {
        glDeleteBuffers(1, &vboID); // Cleanup
    }

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, vboID); // Bind the VBO
    }

    static void unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind any VBO
    }

    // Static functions to set up attribute pointers
    static void add_vec3_pointer(GLuint index, GLsizei stride, const void* offset) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, offset);
    }

    static void add_vec2_pointer(GLuint index, GLsizei stride, const void* offset) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride, offset);
    }

    static void add_vec4_pointer(GLuint index, GLsizei stride, const void* offset) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, stride, offset);
    }

    static void add_float_pointer(GLuint index, GLsizei stride, const void* offset) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 1, GL_FLOAT, GL_FALSE, stride, offset);
    }

    static void add_int_pointer(GLuint index, GLsizei stride, const void* offset) {
        glEnableVertexAttribArray(index);
        glVertexAttribIPointer(index, 1, GL_INT, stride, offset);
    }
    GLsizei get_vertex_count() { return size; }
private:
    GLsizei size;
    GLuint vboID; // The ID of the VBO
};

struct default_vertex_t {
    glm::vec3 position; // Explicit member for position
    

    static void set_pointers() {
        VBO::add_vec3_pointer(0, sizeof(default_vertex_t), (void*)offsetof(default_vertex_t, position));
        //VBO::add_vec3_pointer(0, sizeof(default_vertex_t), (void*)offsetof(default_vertex_t, position));

    }
};