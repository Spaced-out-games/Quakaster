
#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
~res_mesh() {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }
*/

struct res_mesh{
    uint32_t ref_count = 0;
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    size_t index_count = 0;
    GLenum DrawingType = GL_STATIC_DRAW;
    GLenum PrimitiveType = GL_TRIANGLES;

    res_mesh() = default;

    template <typename vertex_t = glm::vec3>
    res_mesh(const vertex_t* vertices, size_t vertex_count, const GLuint* indices, size_t index_count)
        : index_count(index_count)
    {
        // Generate and bind VAO
        glGenVertexArrays(1, &VAO);
        bind();

        // Generate and bind VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(vertex_t), vertices, DrawingType);

        // Generate and bind EBO
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(GLuint), indices, DrawingType);

        unbind();
    }

    ~res_mesh() {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void bind() const {
        glBindVertexArray(VAO);
    }

    void unbind() const {
        glBindVertexArray(0);
    }

    void draw() const {
        bind();
        glDrawElements(PrimitiveType, static_cast<GLsizei>(index_count), GL_UNSIGNED_INT, 0);
        unbind();
    }
    /*
    void addVec2(GLuint index, std::size_t offset) {
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addVec3(GLuint index, std::size_t offset) {
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addVec4(GLuint index, std::size_t offset) {
        glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    // New methods for integer types
    void addInt8(GLuint index, std::size_t offset, bool normalized = false) {
        glVertexAttribIPointer(index, 1, GL_BYTE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addUInt8(GLuint index, std::size_t offset, bool normalized = false) {
        glVertexAttribIPointer(index, 1, GL_UNSIGNED_BYTE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addInt16(GLuint index, std::size_t offset) {
        glVertexAttribIPointer(index, 1, GL_SHORT, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addUInt16(GLuint index, std::size_t offset) {
        glVertexAttribIPointer(index, 1, GL_UNSIGNED_SHORT, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addInt32(GLuint index, std::size_t offset) {
        glVertexAttribIPointer(index, 1, GL_INT, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addUInt32(GLuint index, std::size_t offset) {
        glVertexAttribIPointer(index, 1, GL_UNSIGNED_INT, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    // OpenGL doesn't support 64-bit integers for vertex attributes, but we can emulate with two 32-bit integers
    void addInt64(GLuint index, std::size_t offset) {
        addUInt64(index, offset); // Can be implemented similarly
    }

    void addUInt64(GLuint index, std::size_t offset) {
        // OpenGL does not support native 64-bit integers for attributes, can be split into two 32-bit
        glVertexAttribIPointer(index, 2, GL_UNSIGNED_INT, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    // New methods for floating-point types (float and double)
    void addFloat(GLuint index, std::size_t offset) {
        glVertexAttribPointer(index, 1, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addVec2Float(GLuint index, std::size_t offset) {
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addVec3Float(GLuint index, std::size_t offset) {
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addVec4Float(GLuint index, std::size_t offset) {
        glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addDouble(GLuint index, std::size_t offset) {
        glVertexAttribLPointer(index, 1, GL_DOUBLE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addVec2Double(GLuint index, std::size_t offset) {
        glVertexAttribLPointer(index, 2, GL_DOUBLE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addVec3Double(GLuint index, std::size_t offset) {
        glVertexAttribLPointer(index, 3, GL_DOUBLE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }

    void addVec4Double(GLuint index, std::size_t offset) {
        glVertexAttribLPointer(index, 4, GL_DOUBLE, sizeof(vertex_t), (GLvoid*)offset);
        glEnableVertexAttribArray(index);
    }
    */

};

class mesh_manager;

struct mesh_component
{
    GLuint VAO;
    mesh_manager* owner = nullptr;
    // FIX ME: Not properly integrated, just to fix compiler errors
    int index_count;

    void bind() const {
        glBindVertexArray(VAO);
    }
    void unbind() const {
        glBindVertexArray(0);
    }

    void draw() const {
        bind();
        GLenum PrimitiveType = GL_STATIC_DRAW;
        glDrawElements(PrimitiveType, static_cast<GLsizei>(index_count), GL_UNSIGNED_INT, 0);
        unbind();
    }

};


class mesh_manager
{
    struct entry
    {
        res_mesh mesh;
        int refcount;
    };
    std::unordered_map<std::string, entry> meshes;

    template <typename vertex_t = glm::vec3>
    static mesh_component add_mesh(std::string label, const vertex_t* vertices, size_t vertex_count, const GLuint* indices, size_t index_count)
    {
        meshes[label] = { res_mesh<vertex_t>(vertices, vertex_count, indices, index_count), 0 };
    }

};


//EVENT_FIRE(app.scene.dispatcher, console_log_request, request);