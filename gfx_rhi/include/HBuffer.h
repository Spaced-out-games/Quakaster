#pragma once

#include <cstdint>
#include <vector>
#include <span>
#include <core.h>
#include "Resource.h"

namespace qk::gfx::rhi {

    // Optimizes buffer consumption
    enum BufferUsage : u8 {
        StaticDraw,
        DynamicDraw,
        StreamDraw
    };

    // Denotes buffer type
    enum class BufferType : u8 {
        Unknown,
        Vertex,
        Index,
        Uniform,
        Storage
    };

    enum class EBufferAccessFlags: u8 {
        READ,
        WRITE,
        READWRITE
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

    // NEW:

    void Buffer_alloc(const HBuffer& hBuf, size_t num_bytes);

    void Buffer_update(const HBuffer& hBuf, const void* pSrc, size_t offset, size_t num_bytes);

    void* Buffer_open(const HBuffer& hBuf, EBufferAccessFlags flags);

    void Buffer_close(const HBuffer& hBuf);

    void Buffer_cpy(const HBuffer& src, size_t src_offset, const HBuffer& dest, size_t dest_offset, size_t num_bytes);




    // ============================================================
    // Vertex Buffer API
    // ============================================================
    // make a vertex buffer
    inline HVertexBuffer VertexBuffer_ctor(BufferUsage usage = StaticDraw)
    {
        return Buffer_ctor(BufferType::Vertex,usage);
    }

    inline void VertexBuffer_dtor(const HVertexBuffer& hBuf)
    {
        Buffer_dtor(hBuf);
    }

    inline void VertexBuffer_free(HVertexBuffer& hBuf) {
        Buffer_free(hBuf);
    }

    inline void VertexBuffer_alloc(const HVertexBuffer& hBuf, size_t num_bytes) {
        Buffer_alloc(hBuf, num_bytes);
    }

    inline void VertexBuffer_update(const HVertexBuffer& hBuf, const void* pSrc, size_t offset, size_t num_bytes) {
        Buffer_update(hBuf, pSrc, offset, num_bytes);

    }

    inline void* VertexBuffer_open(const HVertexBuffer& hBuf, EBufferAccessFlags flags) {
        return Buffer_open(hBuf, flags);
    }

    inline void VertexBuffer_close(const HVertexBuffer& hBuf) {
        VertexBuffer_close(hBuf);
    }

    // upload to a vertex buffer
    inline void VertexBuffer_upload(const HVertexBuffer& hBuf, const void* data, size_t num_bytes)
    {
        Buffer_upload(hBuf, data, num_bytes);
    }

    template<typename vertex_t>
    inline void VertexBuffer_upload(const HVertexBuffer& hBuf, const std::vector<vertex_t>& vertices) {
        return VertexBuffer_upload(hBuf, vertices.data(), (u32)vertices.size() * sizeof(vertex_t));
    }

    inline void VertexBuffer_cpy(
        const HVertexBuffer& src,
        size_t src_offset,
        const HVertexBuffer& dest,
        size_t dest_offset,
        size_t num_bytes)
    {
        Buffer_cpy(src, src_offset, dest, dest_offset, num_bytes);
    }

    struct VertexBufferBinding {
        HVertexBuffer hVertexBuffer = nullptr;
        u32 offset = 0;
        u32 stride = 0;
        u8 slot = 0;
    };




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

    inline void IndexBuffer_alloc(const HIndexBuffer& hBuf, size_t num_bytes) {
        Buffer_alloc(hBuf, num_bytes);
    }

    inline void IndexBuffer_update(const HIndexBuffer& hBuf, const void* pSrc, size_t offset, size_t num_bytes) {
        Buffer_update(hBuf, pSrc, offset, num_bytes);

    }

    inline void* IndexBuffer_open(const HIndexBuffer& hBuf, EBufferAccessFlags flags) {
        return Buffer_open(hBuf, flags);
    }

    inline void IndexBuffer_close(const HIndexBuffer& hBuf) {
        IndexBuffer_close(hBuf);
    }

    // upload to a vertex buffer
    inline void IndexBuffer_upload(const HIndexBuffer& hBuf, const void* data, size_t num_bytes)
    {
        Buffer_upload(hBuf, data, num_bytes);
    }

    inline void IndexBuffer_cpy(
        const HIndexBuffer& src,
        size_t src_offset,
        const HIndexBuffer& dest,
        size_t dest_offset,
        size_t num_bytes)
    {
        Buffer_cpy(src, src_offset, dest, dest_offset, num_bytes);
    }

    struct IndexBufferBinding {
        HIndexBuffer hIndexBuffer = nullptr;
        u32 offset = 0;
        u32 stride = 0;
        u8 slot = 0;
        bool operator==(const IndexBufferBinding& other) {
            return 0;
            return hIndexBuffer == other.hIndexBuffer &&
                offset == other.offset &&
                stride == other.stride;
        }
    };


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


    inline void UniformBuffer_alloc(const HUniformBuffer& hBuf, size_t num_bytes) {
        Buffer_alloc(hBuf, num_bytes);
    }

    inline void UniformBuffer_update(const HUniformBuffer& hBuf, const void* pSrc, size_t offset, size_t num_bytes) {
        Buffer_update(hBuf, pSrc, offset, num_bytes);

    }

    inline void* UniformBuffer_open(const HUniformBuffer& hBuf, EBufferAccessFlags flags) {
        return Buffer_open(hBuf, flags);
    }

    inline void UniformBuffer_close(const HUniformBuffer& hBuf) {
        UniformBuffer_close(hBuf);
    }

    // upload to a index buffer
    inline void UniformBuffer_upload(const HUniformBuffer& hBuf, const void* data, size_t num_bytes)
    {
        Buffer_upload(hBuf, data, num_bytes);
    }

    inline void UniformBuffer_cpy(
        const HUniformBuffer& src,
        size_t src_offset,
        const HUniformBuffer& dest,
        size_t dest_offset,
        size_t num_bytes)
    {
        Buffer_cpy(src, src_offset, dest, dest_offset, num_bytes);
    }

    using BufferRegistry = ResourceRegistry<Buffer_impl, Buffer_dtor, Buffer_free>;


}
