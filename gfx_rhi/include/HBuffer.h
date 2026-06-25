#pragma once

#include <cstdint>
#include <vector>

#include <core.h>
#include <rhi_Resource.h>

namespace qk::gfx::rhi {

    // Optimizes buffer consumption
    enum BufferUsage: u8 {
        StaticDraw,
        DynamicDraw,
        StreamDraw
    };

    // Denotes buffer type
    enum class BufferType: u8 {
        Unknown,
        Vertex,
        Index,
        Uniform,
        Storage
    };

    struct Buffer_impl;

    using RBuffer = Buffer_impl*;
    using RVertexBuffer = Buffer_impl*;
    using RIndexBuffer = Buffer_impl*;
    using RUniformBuffer = Buffer_impl*;

    // ============================================================
    // Core API
    // ============================================================
    
    // Make a buffer and return the handle
    RBuffer Buffer_ctor(
        str_view label,
        BufferType type,
        BufferUsage usage = StaticDraw);

    // Get the internal name of the buffer
    str_view Buffer_name(
        const RBuffer& hBuf);

    // Destroy the buffer
    bool Buffer_dtor(
        RBuffer& hBuf);

    // Upload to the buffer
    bool Buffer_upload(
        const RBuffer& hBuf,
        const void* pData,
        u32 count,
        u32 elem_size);

    // check handle validity
    bool Buffer_valid(
        const RBuffer& hBuf);

    // check buffer initialization status
    bool Buffer_initialized(
        const RBuffer& hBuf);

    // get the type of a generic buffer
    BufferType Buffer_type(
        const RBuffer& hBuf);

    //RBuffer Buffer_resolve(str_view label);


    // ============================================================
    // Vertex Buffer API
    // ============================================================
    // make a vertex buffer
    inline RVertexBuffer VertexBuffer_ctor(
        str_view label,
        BufferUsage usage = StaticDraw)
    {
        return Buffer_ctor(
            label,
            BufferType::Vertex,
            usage);
    }
    // bind a vertex buffer
    bool VertexBuffer_bind(
        const RVertexBuffer& hBuf);

    // destroy a vertex buffer
    inline bool VertexBuffer_dtor(
        RVertexBuffer& hBuf)
    {
        return Buffer_dtor(hBuf);
    }

    // upload to a vertex buffer
    inline bool VertexBuffer_upload(
        const RVertexBuffer& hBuf,
        const void* data,
        u32 count,
        u32 elem_size)
    {
        return Buffer_upload(
            hBuf,
            data,
            count,
            elem_size);
    }

    // upload to a vertex buffer
    template<typename vertex_t>
    inline bool VertexBuffer_upload(
        const RVertexBuffer& hBuf,
        const std::vector<vertex_t>& vertices)
    {
        return VertexBuffer_upload(
            hBuf,
            vertices.data(),
            (u32)vertices.size(),
            sizeof(vertex_t));
    }

    // Check vertex buffer validity
    inline bool VertexBuffer_valid(
        const RVertexBuffer& hBuf)
    {
        return Buffer_valid(hBuf);
    }

    // check whether vertex buffer is initialized
    inline bool VertexBuffer_initialized(
        const RVertexBuffer& hBuf)
    {
        return Buffer_initialized(hBuf);
    }

    // ============================================================
    // Index Buffer API
    // ============================================================

    // Construct index buffer
    inline RIndexBuffer IndexBuffer_ctor(
        str_view label,
        BufferUsage usage = StaticDraw)
    {
        return Buffer_ctor(
            label,
            BufferType::Index,
            usage);
    }

    // bind the index buffer
    bool IndexBuffer_bind(
        const RIndexBuffer& hBuf);

    // upload to the index buffer
    inline bool IndexBuffer_upload(
        const RIndexBuffer& hBuf,
        const void* data,
        u32 count,
        u32 elem_size)
    {
        return Buffer_upload(
            hBuf,
            data,
            count,
            elem_size);
    }

    // check index buffer validity
    inline bool IndexBuffer_valid(
        const RIndexBuffer& hBuf)
    {
        return Buffer_valid(hBuf);
    }

    // ============================================================
    // Uniform Buffer API
    // ============================================================
    
    // Construct uniform buffer
    inline RUniformBuffer UniformBuffer_ctor(
        str_view label)
    {
        return Buffer_ctor(
            label,
            BufferType::Uniform,
            DynamicDraw);
    }
    // bind uniform buffer
    bool UniformBuffer_bind(
        const RUniformBuffer& hBuf,
        u8 slot);

    // destroy uniform buffer
    inline bool UniformBuffer_dtor(
        RUniformBuffer& hBuf)
    {
        return Buffer_dtor(hBuf);
    }
    // upload to uniform buffer
    inline bool UniformBuffer_upload(
        const RUniformBuffer& hBuf,
        const void* data,
        u32 count,
        u32 elem_size)
    {
        return Buffer_upload(
            hBuf,
            data,
            count,
            elem_size);
    }

    // check uniform buffer validity
    inline bool UniformBuffer_valid(
        const RUniformBuffer& hBuf)
    {
        return Buffer_valid(hBuf);
    }

}