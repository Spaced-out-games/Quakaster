#pragma once
#include <GL/glew.h>
#include <include/graphics/gl_utils.h>
#include <include/graphics/VBO.h>
#include <vector>


class VAO {
public:

    // Doesn't call init();
    VAO() : primitive_type(GL_TRIANGLES) {

    }

    // Constructor that generates a VAO and sets the primitive type
    VAO(GLenum primitive_type)
        : primitive_type(primitive_type), vaoID(0) {
        init();
    }

    // Destructor to clean up VAO
    ~VAO() {
        if (vaoID != 0) {
            glDeleteVertexArrays(1, &vaoID); // Cleanup
            #ifdef PRINT_GL_ERROR
                std::cerr << "Deleted VAO (ID: " << vaoID << ")." << std::endl;
                check_gl_error("glDeleteVertexArrays");
            #endif
        }
    }

    void init()
    {
        glGenVertexArrays(1, &vaoID); // Generate a VAO
        assert(vaoID != 0 && "Failed to generate VAO"); // Ensure VAO is generated
        #ifdef PRINT_GL_ERROR
            std::cerr << "VAO generated with ID: " << vaoID << std::endl;
            check_gl_error("VAO::init()");
        #endif
    }

    // Bind the VAO
    void bind() const {
        glBindVertexArray(vaoID); // Bind the VAO
        #ifdef PRINT_GL_ERROR
            check_gl_error("glBindVertexArray");
            std::cerr << "Bound VAO (ID: " << vaoID << ")." << std::endl;
        #endif
    }

    // Unbind any VAO
    static void unbind() {
        glBindVertexArray(0); // Unbind any VAO
        #ifdef PRINT_GL_ERROR
            check_gl_error("glBindVertexArray (unbind)");
            std::cerr << "Unbound VAO." << std::endl;
        #endif
    }

    // Deleted copy constructor and copy assignment operator
    //VAO(const VAO&) = delete;
    //VAO& operator=(const VAO&) = delete;

    // Deleted move constructor and move assignment operator
    //VAO(VAO&&) = delete;
    //VAO& operator=(VAO&&) = delete;

    // Primitive type for drawing (e.g., GL_TRIANGLES, GL_LINES)
    GLenum primitive_type = GL_TRIANGLES;


private:
    GLuint vaoID = 0; // The ID of the VAO

};