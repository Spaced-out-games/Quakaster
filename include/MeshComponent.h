#pragma once
#include <unordered_map>
#include <string>
#include <GL/glew.h>

class MeshPool {
    struct Mesh {
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        GLsizei indexCount;

        Mesh(GLuint vao, GLuint vbo, GLuint ebo, GLsizei count)
            : VAO(vao), VBO(vbo), EBO(ebo), indexCount(count) {}
    };

    std::unordered_map<std::string, Mesh> meshes;

public:
    GLuint loadMesh(const std::string& name, float* vertices, size_t vertSize, unsigned int* indices, size_t idxSize) {
        if (meshes.find(name) != meshes.end())
            return meshes[name].VAO;

        GLuint VAO, VBO, EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxSize, indices, GL_STATIC_DRAW);

        // Set vertex attribute pointers (example layout: position + texcoords)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0); // Position

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1); // Texture coordinates

        glBindVertexArray(0);

        meshes[name] = Mesh(VAO, VBO, EBO, idxSize / sizeof(unsigned int));
        return VAO;
    }

    Mesh getMesh(const std::string& name) {
        return meshes[name];
    }
};


struct MeshComponent {
    GLuint VAO;
    GLsizei indexCount;

    MeshComponent() : VAO(0), indexCount(0) {}

    void load(const std::string& name, MeshPool& pool, float* vertices, size_t vertSize, unsigned int* indices, size_t idxSize) {
        VAO = pool.loadMesh(name, vertices, vertSize, indices, idxSize);
        indexCount = idxSize / sizeof(unsigned int);
    }

    void render() const {
        if (VAO) {
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
};
