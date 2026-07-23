#pragma once
#include <VertexLayoutDesc.h>
#include "HDescriptorSet.h"
#include "HPipeline.h"
#include <Resource.h>
#include <HTexture.h>
#include <HShader.h>
#include <HBuffer.h>
#include <glad/glad.h>
#include "DrawCommand.h" // temporary, so that I can check for a compiler error
#include <unordered_map>

#include <span> // temporary

namespace qk::gfx::rhi {

    // ------------------------------------------------------------ casts ------------------------------------------------------------ //


    inline GLuint InputRate_cast(InputRate rate) {
        switch (rate)
        {
        case qk::gfx::rhi::PerVertex:
            return 0;
        case qk::gfx::rhi::PerInstance:
            return 1;
        case qk::gfx::rhi::Unknown:
            throw std::runtime_error("unexpected Input Rate!");
        default:
            break;
        }
    }

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

    struct DescriptorSetVTableEntry {
        EDescriptorType type = EDescriptorType::Unknown;
        u32 index = 0;
    };

    using DescriptorSetVTable = std::vector<DescriptorSetVTableEntry>;




        // Declaration. If you need it publicly, just move it to the `rhi_globals` header.
    struct ShaderProgram_impl : qk::Resource
    {
        GLuint hProgram = 0;

        static Result <GLuint, str>
            Shader_compile(const ShaderModule& recipe);

        static Result <GLuint, str>
            Shader_link(const std::span<GLuint>& recipes);


        // temporary fix
        ShaderProgram_impl(GLuint hProgram) : hProgram(hProgram) {}

        ShaderProgram_impl(const std::span<const ShaderModule>& recipes, str& error_out);

        void destroy() override;

        bool bind() const;

        ~ShaderProgram_impl();

        bool bind_uniform_block(const char* pName, u8 slot);

    };

    struct Pipeline_impl : qk::Resource {
        HShaderProgram				program;
        PrimitiveTopology			topology;
        GLuint						vao;
        std::unordered_map<u8, u32> strides;
        //CullMode cull;
        //DepthState depth;
        //BlendState blend;

        Pipeline_impl(const Pipeline_recipe& recipe);

        void destroy() override;


        void bind() const;

    };

    struct VertexLayout_impl : qk::Resource
    {
        std::vector<VertexAttributeBinding> bindings;

        VertexLayout_impl(const VertexLayout_recipe& recipe);

        virtual void destroy() override;

        ~VertexLayout_impl() = default; // just free it XD

        bool apply() const;

    };





    struct Texture_impl : qk::Resource
    {
        TextureType type = TEX_2D;
        GLuint texture = 0;

        void destroy() override;

        void bind(u8 slot = 0) const;

    };


    struct CommandBuffer_impl : qk::Resource {
        std::unordered_set<Resource*> dependencies;

        std::vector<DrawCmd>          draw_commands;
        // belongs here for now
        PrimitiveTopology             topology;
        CommandBuffer_impl(const std::unordered_set<Resource*>& resources, const std::vector<DrawCmd>& commands);

        void destroy() override;
    };








   

    struct Buffer_impl : qk::Resource
    {
        GLuint hBuffer = 0;
        BufferType type = BufferType::Unknown;
        BufferUsage usage = StaticDraw;
        u32 elem_size = 0;
        u32 elem_count = 0;
        bool initialized = false;

        void destroy() override;

        Buffer_impl(BufferType type, BufferUsage usage, u32 elem_size = 0, u32 elem_count = 0, bool initialized = 0);

        ~Buffer_impl();

        // Consider removing elem_size since it's kind of worthless
        bool upload(const void* pData, u32 count, u32 elem_size);

        bool VertexBuffer_bind(u8 slot = 0, u32 offset = 0, u32 stride = 0);

        bool IndexBuffer_bind();

        bool UniformBuffer_bind(u8 slot);
        


    };

    struct DescriptorSetsContext {
        u8 TexOffset = 0;
        u8 UniformBufferOffset = 0;

        void next(HDescriptorSet hDescriptorSet);
    };

    struct TextureDescriptor {
        HTexture hTex = nullptr;
        u8 slot = 0; // compiled in
    };

    struct UniformBufferDescriptor {
        HUniformBuffer hUniformBuffer = nullptr;
        u8 slot = 0; // compiled in
    };


    struct DescriptorSet_impl: qk::Resource {
        std::vector<TextureDescriptor> m_Textures;
        std::vector<UniformBufferDescriptor> m_UniformBuffers;

        // Maps virtual descriptor binding to the real descriptor index
        DescriptorSetVTable m_DescriptorVTable;

        

        DescriptorSet_impl(const DescriptorSet_recipe& recipe);
        ~DescriptorSet_impl();

        void destroy() override;

        void setResource(u32 binding, HTexture hTexture);

        void setResource(u32 binding, HUniformBuffer hUniformBuffer);

        
        void bind(DescriptorSetsContext& ctx) const;

    };



    // --------------------------------------------------------- registries --------------------------------------------------------- //



    // Just the descriptor-ables for now. Eventually heap-allocate, then add an arena allocator.

    //static inline qk::ResourceRegistry<Texture_impl>        g_Textures;
    //static inline qk::ResourceRegistry<Buffer_impl>         g_Buffers;


}