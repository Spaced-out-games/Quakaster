#include "VertexBufferLayout.h"

namespace qk::gfx {
    unsigned int get_attribute_size(VertexAttribute attribute) {
        switch (attribute)
        {
        case GL_FLOAT:
            return sizeof(float);
        case GL_INT:
            return sizeof(int);
        case GL_UNSIGNED_BYTE:
            return sizeof(unsigned char);

        default:
            __debugbreak();
            return 0;
            break;
        }
        return 0;
    }
}