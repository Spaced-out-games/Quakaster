#pragma once
#include <GL/glew.h>
#include <vector>
#include <include/GameContext/graphics/VBO.h>

class VAO {
public:
    VAO() {
        glGenVertexArrays(1, &vaoID); // Generate a VAO
    }

    ~VAO() {
        glDeleteVertexArrays(1, &vaoID); // Cleanup
    }

    void bind() const {
        glBindVertexArray(vaoID); // Bind the VAO
    }

    static void unbind() {
        glBindVertexArray(0); // Unbind any VAO
    }

    template <typename vertex_t>
    void add_buffer(const VBO& vbo, GLuint layout) {
        bind(); // Bind the VAO

        vbo.bind(); // Bind the VBO

        // Set the vertex attribute pointers
        // Assuming vertex_t is a struct that defines attributes
        // The following is an example; you may need to adjust based on your vertex structure
        glEnableVertexAttribArray(layout); // Enable the vertex attribute
        glVertexAttribPointer(layout, sizeof(vertex_t) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)0);

        VBO::unbind(); // Unbind the VBO
        unbind(); // Unbind the VAO
    }

private:
    GLuint vaoID; // The ID of the VAO
};