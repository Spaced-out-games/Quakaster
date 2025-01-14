#pragma once
#include <GL/glew.h>
#include <vector>


class EBO {
public:
    EBO() {
        glGenBuffers(1, &eboID); // Generate an EBO
    }
    EBO(const std::vector<uint32_t>& indices) {
        glGenBuffers(1, &eboID); // Generate an EBO
        init(indices);
    }

    ~EBO() {
        glDeleteBuffers(1, &eboID); // Cleanup
    }

    void bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID); // Bind the EBO
    }

    static void unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind any EBO
    }

    void init(const std::vector<uint32_t>& indices) {
        bind(); // Bind the EBO
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
        indexCount = indices.size(); // Store the index count
        unbind(); // Unbind the EBO
    }

    uint32_t get_index_count() const {
        return indexCount; // Return the stored index count
    }

private:
    GLuint eboID; // The ID of the EBO
    uint32_t indexCount = 0; // Count of indices in the EBO
};
