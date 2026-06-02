#pragma once
#include <VertexLayoutDesc.h>
#include <Resource.h>
#include <HTexture.h>
#include <glad/glad.h>


namespace qk::gfx::rhi {

    // casts

    inline GLenum EScalarType_cast(
        EScalarType type)
    {
        switch (type)
        {
        case EScalarType::i8:
            return GL_BYTE;

        case EScalarType::i16:
            return GL_SHORT;

        case EScalarType::i32:
            return GL_INT;

        case EScalarType::u8:
            return GL_UNSIGNED_BYTE;

        case EScalarType::u16:
            return GL_UNSIGNED_SHORT;

        case EScalarType::u32:
            return GL_UNSIGNED_INT;

        case EScalarType::f32:
            return GL_FLOAT;

        case EScalarType::f64:
            return GL_DOUBLE;

        default:
            return GL_NONE;
        }
    }

    inline GLuint TextureType_cast(TextureType type)
    {
        switch (type)
        {
        case TEX_1D:       return GL_TEXTURE_1D;
        case TEX_2D:       return GL_TEXTURE_2D;
        case TEX_3D:       return GL_TEXTURE_3D;
        case TEX_CUBEMAP:  return GL_TEXTURE_CUBE_MAP;
        case TEX_2D_ARRAY: return GL_TEXTURE_2D_ARRAY;
        default:           return 0;
        }
    }

    inline GLint TextureFilterMode_cast(
        TextureFilterMode mode)
    {
        switch (mode)
        {
        case TEXEL:    return GL_NEAREST;
        case BILINEAR: return GL_LINEAR;
        default:       return 0;
        }
    }

    inline GLenum Channel_cast(int channels)
    {
        QK_ASSERT(channels != 2);

        switch (channels)
        {
        case 1: return GL_RED;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default:return GL_RGB;
        }
    }

    // Implementations


    struct VertexLayout_impl : qk::Resource
    {
        std::vector<VertexAttributeBinding> bindings;
    };



    struct VertexArray_impl : qk::Resource
    {
        GLuint vao = 0;

        VertexArray_impl()
        {
            glGenVertexArrays(1, &vao);
        }
    };

    struct Texture_impl : qk::Resource
    {
        TextureType type = TEX_2D;
        GLuint texture = 0;
    };

    struct Shader_impl : qk::Resource
    {
        GLuint hShader = 0;

        Shader_impl(GLuint hShader): hShader(hShader) {}

    };


    static inline qk::ResourceRegistry<VertexLayout_impl>   g_VertexLayouts;
    static inline qk::ResourceRegistry<VertexArray_impl>    g_VertexArrays;
    static inline qk::ResourceRegistry<Texture_impl>        g_Textures;
    static inline qk::ResourceRegistry<Shader_impl>         g_Shaders;
}