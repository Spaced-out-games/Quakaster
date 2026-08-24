#pragma once

#include <cstdint>
#include <vector>

#include <core.h>
#include "Resource.h"

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

    using HBuffer = Buffer_impl*;
    using HVertexBuffer = Buffer_impl*;
    using HIndexBuffer = Buffer_impl*;
    using HUniformBuffer = Buffer_impl*;

    // ============================================================
    // Core API
    // ============================================================
    
    Resource* Resource_cast(const HBuffer& hBuffer);


    // Make a buffer and return the handle
    HBuffer Buffer_ctor(BufferType type, BufferUsage usage = StaticDraw);


    // Destroy the buffer
    void Buffer_dtor(const HBuffer& hBuf);

    // free the buffer
    void Buffer_free(HBuffer& hBuf);

    // Upload to the buffer
    void Buffer_upload(const HBuffer& hBuf, const void* pData, size_t num_bytes);








    // ============================================================
    // Vertex Buffer API
    // ============================================================
    // make a vertex buffer
    inline HVertexBuffer VertexBuffer_ctor(BufferUsage usage = StaticDraw)
    {
        return Buffer_ctor(
            BufferType::Vertex,
            usage);
    }

    inline void VertexBuffer_dtor(const HVertexBuffer& hBuf)
    {
        Buffer_dtor(hBuf);
    }

    inline void VertexBuffer_free(HVertexBuffer& hBuf) {
        Buffer_free(hBuf);
    }


    // bind a vertex buffer
    bool VertexBuffer_bind(
        const HVertexBuffer& hBuf, u8 slot, u32 offset = 0, u32 stride = 0);

    // destroy a vertex buffer


    // upload to a vertex buffer
    inline void VertexBuffer_upload(const HVertexBuffer& hBuf, const void* data, size_t num_bytes)
    {
        Buffer_upload(hBuf, data, num_bytes);
    }

    // upload to a vertex buffer
    template<typename vertex_t>
    inline void VertexBuffer_upload(const HVertexBuffer& hBuf, const std::vector<vertex_t>& vertices)
        const HVertexBuffer& hBuf,
        VertexBuffer_upload(hBuf,vertices.data(), vertices.size() * sizeof(vertex_t));
    {
        return VertexBuffer_upload(
            hBuf,
            vertices.data(),
            (u32)vertices.size(),
            sizeof(vertex_t));
    }





    // ============================================================
    // Index Buffer API
    // ============================================================

    // Construct index buffer
    inline HIndexBuffer IndexBuffer_ctor(BufferUsage usage = StaticDraw)
    {
        return Buffer_ctor(
            BufferType::Index,
            usage);
    }

    inline void IndexBuffer_dtor(const HIndexBuffer& hBuf)
    {
        Buffer_dtor(hBuf);
    }
    
    inline void IndexBuffer_free(HIndexBuffer& hBuf) {
        Buffer_free(hBuf);
    }

    // bind the index buffer
    bool IndexBuffer_bind(const HIndexBuffer& hBuf);



    // upload to the index buffer
    inline void IndexBuffer_upload(const HIndexBuffer& hBuf, const void* data, size_t num_bytes) {
        Buffer_upload(hBuf, data, num_bytes);
    }



    // ============================================================
    // Uniform Buffer API
    // ============================================================
    
    // Construct uniform buffer
    inline HUniformBuffer UniformBuffer_ctor()
    {
        return Buffer_ctor(
            BufferType::Uniform,
            DynamicDraw);
    }

    // destroy uniform buffer
    inline void UniformBuffer_dtor(const HUniformBuffer& hBuf)
    {
        Buffer_dtor(hBuf);
    }

    inline void UniformBuffer_free(HUniformBuffer& hBuf) {
        Buffer_free(hBuf);
    }

    // bind uniform buffer
    bool UniformBuffer_bind(
        const HUniformBuffer& hBuf,
        u8 slot);



    // upload to uniform buffer
    inline void UniformBuffer_upload(const HUniformBuffer& hBuf, const void* data, size_t num_bytes) {
        Buffer_upload(hBuf, data, num_bytes);
    }



    using BufferRegistry = ResourceRegistry<Buffer_impl, Buffer_dtor, Buffer_free>;


}