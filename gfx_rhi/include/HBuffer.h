#pragma once

#include <cstdint>
#include <vector>

#include <core.h>
#include <rhi_Resource.h>

namespace qk::gfx::rhi {

    enum BufferUsage: u8 {
        StaticDraw,
        DynamicDraw,
        StreamDraw
    };

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

    RBuffer Buffer_ctor(
        str_view label,
        BufferType type,
        BufferUsage usage = StaticDraw);

    str_view Buffer_name(
        const RBuffer& hBuf);

    bool Buffer_dtor(
        RBuffer& hBuf);

    bool Buffer_upload(
        const RBuffer& hBuf,
        const void* pData,
        u32 count,
        u32 elem_size);

    bool Buffer_valid(
        const RBuffer& hBuf);

    bool Buffer_initialized(
        const RBuffer& hBuf);

    BufferType Buffer_type(
        const RBuffer& hBuf);

    RBuffer Buffer_resolve(str_view label);


    // ============================================================
    // Vertex Buffer API
    // ============================================================

    inline RVertexBuffer VertexBuffer_ctor(
        str_view label,
        BufferUsage usage = StaticDraw)
    {
        return Buffer_ctor(
            label,
            BufferType::Vertex,
            usage);
    }

    bool VertexBuffer_bind(
        const RVertexBuffer& hBuf);

    inline bool VertexBuffer_dtor(
        RVertexBuffer& hBuf)
    {
        return Buffer_dtor(hBuf);
    }

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

    inline bool VertexBuffer_valid(
        const RVertexBuffer& hBuf)
    {
        return Buffer_valid(hBuf);
    }

    inline bool VertexBuffer_initialized(
        const RVertexBuffer& hBuf)
    {
        return Buffer_initialized(hBuf);
    }

    // ============================================================
    // Index Buffer API
    // ============================================================

    inline RIndexBuffer IndexBuffer_ctor(
        str_view label,
        BufferUsage usage = StaticDraw)
    {
        return Buffer_ctor(
            label,
            BufferType::Index,
            usage);
    }

    bool IndexBuffer_bind(
        const RIndexBuffer& hBuf);

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

    inline bool IndexBuffer_valid(
        const RIndexBuffer& hBuf)
    {
        return Buffer_valid(hBuf);
    }

    // ============================================================
    // Uniform Buffer API
    // ============================================================

    inline RUniformBuffer UniformBuffer_ctor(
        str_view label)
    {
        return Buffer_ctor(
            label,
            BufferType::Uniform,
            DynamicDraw);
    }

    bool UniformBuffer_bind(
        const RUniformBuffer& hBuf,
        u8 slot);

    inline bool UniformBuffer_dtor(
        RUniformBuffer& hBuf)
    {
        return Buffer_dtor(hBuf);
    }

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

    inline bool UniformBuffer_valid(
        const RUniformBuffer& hBuf)
    {
        return Buffer_valid(hBuf);
    }

}