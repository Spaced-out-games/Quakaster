#include "HVertexArray.h"

#include "glad/glad.h"
#include "Scalar.h"

#include <Resource.h>

namespace qk::gfx::rhi {

    struct VertexArray_impl : qk::Resource
    {
        GLuint vao = 0;

        VertexArray_impl()
        {
            glGenVertexArrays(1, &vao);
        }

        ~VertexArray_impl()
        {
           // if (vao)
        }
    };

    static inline qk::ResourceRegistry<VertexArray_impl>
        g_VertexArrays;

    inline GLenum EScalarType_cast(EScalarType type)
    {
        switch (type)
        {
        case EScalarType::i8:  return GL_BYTE;
        case EScalarType::i16: return GL_SHORT;
        case EScalarType::i32: return GL_INT;

        case EScalarType::u8:  return GL_UNSIGNED_BYTE;
        case EScalarType::u16: return GL_UNSIGNED_SHORT;
        case EScalarType::u32: return GL_UNSIGNED_INT;

        case EScalarType::f32: return GL_FLOAT;
        case EScalarType::f64: return GL_DOUBLE;

        default: return GL_NONE;
        }
    }

    HVertexArray VertexArray_ctor(str_view label)
    {
        VertexArray_impl impl;

        return g_VertexArrays.insert(
            label,
            std::move(impl));
    }

    void VertexArray_dtor(HVertexArray& hVertexArray)
    {
        if (!hVertexArray)
            return;
        

        glDeleteVertexArrays(1, &hVertexArray->vao);

        g_VertexArrays.destroy(hVertexArray);
        hVertexArray = nullptr;
    }

    bool VertexArray_valid(const HVertexArray& hVertexArray)
    {
        return g_VertexArrays.valid(hVertexArray);
    }

    bool VertexArray_bind(const HVertexArray& hVertexArray)
    {
        if (!hVertexArray)
            return false;

        if (!g_VertexArrays.valid(hVertexArray))
            return false;

        glBindVertexArray(hVertexArray->vao);
        return true;
    }

}