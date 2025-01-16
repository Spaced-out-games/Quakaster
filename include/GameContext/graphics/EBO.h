#pragma once
#include <GL/glew.h>
#include <vector>
#include <include/GameContext/graphics/gl_utils.h>
#include <iostream>
#include <cassert>

class EBO {
public:
    // Deleted copy constructor and copy assignment operator
    EBO(const EBO&) = delete;
    EBO& operator=(const EBO&) = delete;

    // Deleted move constructor and move assignment operator
    EBO(EBO&&) = delete;
    EBO& operator=(EBO&&) = delete;

    EBO() {
        //init();
    }

    EBO(const std::vector<uint32_t>& indices) {
        init(indices);
    }

    ~EBO() {
        glDeleteBuffers(1, &eboID); // Cleanup
        // check_gl_error("glDeleteBuffers for EBO");
        // std::cerr << "Deleted EBO ID: " << eboID << std::endl;
    }

    void bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID); // Bind the EBO
        // check_gl_error("glBindBuffer for EBO");
        // std::cerr << "Bound EBO ID: " << eboID << std::endl;
    }

    static void unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind any EBO
        // check_gl_error("glBindBuffer (unbind) for EBO");
        // std::cerr << "Unbound EBO" << std::endl;
    }

    void init(const std::vector<uint32_t>& indices) {
        glGenBuffers(1, &eboID); // Generate an EBO
        // check_gl_error("glGenBuffers for EBO");
        assert(eboID != 0 && "Failed to generate EBO ID");
        // std::cerr << "Generated EBO ID: " << eboID << std::endl;
        bind(); // Bind the EBO
        // std::cerr << "Initializing EBO ID: " << eboID << " with " << indices.size() << " indices." << std::endl;

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
        // check_gl_error("glBufferData for EBO");
        indexCount = indices.size(); // Store the index count

        unbind(); // Unbind the EBO
    }

    uint32_t get_index_count() const {
        return indexCount; // Return the stored index count
    }

private:
    GLuint eboID = 0xff; // The ID of the EBO
    uint32_t indexCount = 0; // Count of indices in the EBO
};
