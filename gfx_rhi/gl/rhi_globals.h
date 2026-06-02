#pragma once
#include <VertexLayoutDesc.h>
#include <HDescriptorSet.h>
#include <HCommandBuffer.h>
#include <Resource.h>
#include <HTexture.h>
#include <HShader.h>
#include <HBuffer.h>
#include <glad/glad.h>


namespace qk::gfx::rhi {

    // ------------------------------------------------------------ casts ------------------------------------------------------------ //


    inline GLenum BufferUsage_cast(
        BufferUsage usage)
    {
        switch (usage)
        {
        case StaticDraw:  return GL_STATIC_DRAW;
        case DynamicDraw: return GL_DYNAMIC_DRAW;
        case StreamDraw:  return GL_STREAM_DRAW;
        }

        throw std::runtime_error("Invalid BufferUsage");
    }

    inline GLenum BufferType_cast(
        BufferType type)
    {
        switch (type)
        {
        case BufferType::Vertex:
            return GL_ARRAY_BUFFER;

        case BufferType::Index:
            return GL_ELEMENT_ARRAY_BUFFER;

        case BufferType::Uniform:
            return GL_UNIFORM_BUFFER;

        default:
            throw std::runtime_error(
                "Invalid BufferType");
        }
    }


    inline GLenum EScalarType_cast(EScalarType type)
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

    inline GLint TextureFilterMode_cast(TextureFilterMode mode)
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

    inline GLenum ShaderType_cast(ShaderType type)
    {
        switch (type)
        {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        case ShaderType::Geometry:
            return GL_GEOMETRY_SHADER;
        default:
            throw std::runtime_error("Shader type not supported");
            break;
        }
    }

    inline GLbitfield ClearChannels_cast(ClearChannels channels)
    {
        GLbitfield result = 0;

        if ((channels & ClearChannels::Color) == ClearChannels::Color)
            result |= GL_COLOR_BUFFER_BIT;

        if ((channels & ClearChannels::Depth) == ClearChannels::Depth)
            result |= GL_DEPTH_BUFFER_BIT;

        if ((channels & ClearChannels::Stencil) == ClearChannels::Stencil)
            result |= GL_STENCIL_BUFFER_BIT;

        return result;
    }

    inline GLuint PrimitiveType_cast(PrimitiveTopology type)
    {
        switch (type)
        {
        case PrimitiveTopology::Triangle:
            return GL_TRIANGLES;

        case PrimitiveTopology::Lines:
            return GL_LINES;

        case PrimitiveTopology::Points:
            return GL_POINTS;

        case PrimitiveTopology::TriangleStrip:
            return GL_TRIANGLE_STRIP;

        case PrimitiveTopology::LineStrip:
            return GL_LINE_STRIP;

        default:
            return GL_TRIANGLES;
        }
    }


    // ------------------------------------------------------- Implementations ------------------------------------------------------- //


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

    struct ShaderProgram_impl : qk::Resource
    {
        GLuint hProgram = 0;
        ShaderProgram_impl(GLuint hProgram): hProgram(hProgram) {}
    };

    struct Material_impl : qk::Resource
    {
        RShaderProgram program = nullptr;


        HDescriptorSet descriptor_set = nullptr;
    };

    struct Descriptor_impl : public Descriptor {

        // we need a linker!
        union {
            Texture_desc texture;
            UniformBuffer_desc uniform;
            StorageBuffer_desc storage;
        };
        Descriptor_impl() = default;
        Descriptor_impl(EDescriptorType type, u8 binding, str id, UniformBuffer_desc uniform = {}) :
            Descriptor(type, binding, id), uniform(uniform) {

        }
    };

    struct DescriptorSet_impl : qk::Resource
    {
        std::vector<Descriptor_impl> descriptors;
    };

    struct CommandBuffer_impl : qk::Resource {
        std::vector<DrawCommand_desc> commands;
    };

    struct Buffer_impl : qk::Resource
    {
        GLuint hBuffer = 0;
        BufferType type = BufferType::Unknown;
        BufferUsage usage = StaticDraw;
        u32 elem_size = 0;
        u32 elem_count = 0;
        bool initialized = false;
    };

    // --------------------------------------------------------- registries --------------------------------------------------------- //


    static inline qk::ResourceRegistry<VertexLayout_impl>   g_VertexLayouts;
    static inline qk::ResourceRegistry<VertexArray_impl>    g_VertexArrays;
    static inline qk::ResourceRegistry<Texture_impl>        g_Textures;
    static inline qk::ResourceRegistry<Shader_impl>         g_Shaders;
    static inline qk::ResourceRegistry<ShaderProgram_impl>  g_Programs;
    static inline u32                                       g_material_index = 0;
    static inline qk::ResourceRegistry<Material_impl>       g_Materials;
    static inline qk::ResourceRegistry<DescriptorSet_impl>  g_DescriptorSets;
    static inline u32                                       g_CommandBufferCounter = 0;
    static inline qk::ResourceRegistry<CommandBuffer_impl>  g_CommandBuffers;
    static inline qk::ResourceRegistry<Buffer_impl>         g_Buffers;


}