#include "HBuffer.h"

#include <Resource.h>
#include <glad/glad.h>
#include <stdexcept>

namespace qk::gfx::rhi {

    static GLenum BufferUsage_cast(
        BufferUsage usage)
    {
        switch (usage)
        {
        case StaticDraw:  return GL_STATIC_DRAW;
        case DynamicDraw: return GL_DYNAMIC_DRAW;
        case StreamDraw:  return GL_STREAM_DRAW;
        }

        throw std::runtime_error(
            "Invalid BufferUsage");
    }

    static GLenum BufferType_cast(
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

    struct Buffer_impl : qk::Resource
    {
        GLuint hBuffer = 0;

        BufferType type =
            BufferType::Unknown;

        BufferUsage usage =
            StaticDraw;

        u32 elem_size = 0;
        u32 elem_count = 0;

        bool initialized = false;
    };

    static inline
        qk::ResourceRegistry<Buffer_impl>
        g_Buffers;

    RBuffer Buffer_ctor(
        str_view label,
        BufferType type,
        BufferUsage usage)
    {
        Buffer_impl buffer;

        buffer.type = type;
        buffer.usage = usage;

        glGenBuffers(
            1,
            &buffer.hBuffer);

        return g_Buffers.insert(
            label,
            std::move(buffer));
    }

    str_view Buffer_name(
        const RBuffer& hBuf)
    {
        if (!hBuf)
            return {};

        return hBuf->label;
    }

    bool Buffer_dtor(
        RBuffer& hBuf)
    {
        if (!hBuf)
            return false;

        glDeleteBuffers(
            1,
            &hBuf->hBuffer);

        g_Buffers.destroy(
            hBuf);

        hBuf = nullptr;

        return true;
    }

    bool Buffer_upload(
        const RBuffer& hBuf,
        const void* data,
        u32 count,
        u32 elem_size)
    {
        if (!hBuf)
            return false;

        hBuf->elem_count = count;
        hBuf->elem_size = elem_size;
        hBuf->initialized = true;

        glBindBuffer(
            BufferType_cast(hBuf->type),
            hBuf->hBuffer);

        glBufferData(
            BufferType_cast(hBuf->type),
            count * elem_size,
            data,
            BufferUsage_cast(hBuf->usage));

        return true;
    }

    bool Buffer_valid(
        const RBuffer& hBuf)
    {
        return hBuf &&
            g_Buffers.valid(hBuf);
    }

    bool Buffer_initialized(
        const RBuffer& hBuf)
    {
        return hBuf &&
            hBuf->initialized;
    }

    BufferType Buffer_type(
        const RBuffer& hBuf)
    {
        if (!hBuf)
            return BufferType::Unknown;

        return hBuf->type;
    }

    RBuffer Buffer_resolve(str_view label) {
        return g_Buffers.find(label);
    }

    bool VertexBuffer_bind(
        const RVertexBuffer& hBuf)
    {
        if (!hBuf)
            return false;

        if (!hBuf->initialized)
            return false;

        if (hBuf->type != BufferType::Vertex)
            return false;

        glBindBuffer(
            GL_ARRAY_BUFFER,
            hBuf->hBuffer);

        return true;
    }

    bool IndexBuffer_bind(
        const RIndexBuffer& hBuf)
    {
        if (!hBuf)
            return false;

        if (!hBuf->initialized)
            return false;

        if (hBuf->type != BufferType::Index)
            return false;

        glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER,
            hBuf->hBuffer);

        return true;
    }

    bool UniformBuffer_bind(
        const RUniformBuffer& hBuf,
        u8 slot)
    {
        if (!hBuf)
            return false;

        if (!hBuf->initialized)
            return false;

        if (hBuf->type != BufferType::Uniform)
            return false;

        glBindBufferBase(
            GL_UNIFORM_BUFFER,
            slot,
            hBuf->hBuffer);

        return true;
    }

}