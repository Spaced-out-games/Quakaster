#include "HBuffer.h"
#include <core.h>
#include <Resource.h>
#include <glad/glad.h>
#include <stdexcept>
#include "rhi_globals.h"
#include <format>
namespace qk::gfx::rhi {



    Buffer_impl::Buffer_impl(BufferType type, BufferUsage usage, u32 size, u32 count, bool ready) :
        initialized(ready), type(type), usage(usage),
        size(size), Resource("Buffer_impl")
        {
            
            glGenBuffers(1, &hBuffer);

        }





    void Buffer_impl::destroy() {
        if (!hBuffer) return;
        glDeleteBuffers(1, &hBuffer);
        return;
    }



    void Buffer_impl::upload(const void* pData, size_t num_bytes) {
        QK_ASSERT(hBuffer != 0, "Invalid hBuffer");

        //elem_count = count;
        size = num_bytes;
        initialized = true;

        glBindBuffer(BufferType_cast(type), hBuffer);
        glBufferData(BufferType_cast(type), num_bytes, pData, BufferUsage_cast(usage));

        // unbind it to ensure correctness
        glBindBuffer(BufferType_cast(type), 0);


    }

    void Buffer_impl::alloc(size_t num_bytes) {
        QK_ASSERT(hBuffer != 0, "Invalid hBuffer");
        size = num_bytes;
        initialized = false;

        glBindBuffer(BufferType_cast(type), hBuffer);
        glBufferData(BufferType_cast(type), num_bytes, nullptr, BufferUsage_cast(usage));

        // unbind it to ensure correctness
        glBindBuffer(BufferType_cast(type), 0);

    }

    void Buffer_impl::update(const void* pSrc, size_t offset, size_t num_bytes) {
        QK_ASSERT(hBuffer != 0, "Invalid hBuffer");

        glBindBuffer(BufferType_cast(type), hBuffer);

        glBufferSubData(BufferType_cast(type), offset, num_bytes, pSrc);

        // unbind it to ensure correctness
        glBindBuffer(BufferType_cast(type), 0);

    }

    void* Buffer_impl::open(EBufferAccessFlags flags) {
        QK_ASSERT(hBuffer != 0, "Invalid hBuffer");
        glBindBuffer(BufferType_cast(type), hBuffer);
        return glMapBuffer(BufferType_cast(type), EBufferAccessFlags_cast(flags));
    }

    void Buffer_impl::close() {
        QK_ASSERT(hBuffer != 0, "Invalid hBuffer");
        glBindBuffer(BufferType_cast(type), hBuffer);
        glUnmapBuffer(BufferType_cast(type));
    }

    void Buffer_impl::copy(const HBuffer& src, size_t src_offset, const HBuffer& dest, size_t dest_offset, size_t num_bytes) {
        QK_ASSERT(src->hBuffer != 0, "Invalid src->hBuffer");
        QK_ASSERT(dest->hBuffer != 0, "Invalid dest->hBuffer");

        glBindBuffer(GL_COPY_READ_BUFFER, src->hBuffer);
        glBindBuffer(GL_COPY_WRITE_BUFFER, dest->hBuffer);
        
        glCopyBufferSubData(
            GL_COPY_READ_BUFFER,
            GL_COPY_WRITE_BUFFER,
            src_offset,
            dest_offset,
            num_bytes
        );
    }

    bool Buffer_impl::VertexBuffer_bind(u8 slot, u32 offset, u32 stride) {
        QK_ASSERT(hBuffer != 0, "Invalid hBuffer");
        QK_ASSERT(type == BufferType::Vertex, "Attempted to bind non-vertexbuffer");
        QK_ASSERT(initialized, "IndexBuffer was not initialized");

        if (!initialized) return false;
        glBindVertexBuffer(slot, hBuffer, offset, stride);
        return true;

    }

    bool Buffer_impl::IndexBuffer_bind() {
        QK_ASSERT(hBuffer != 0, "Invalid hBuffer");
        QK_ASSERT(type == BufferType::Index, "Attempted to bind non-indexbuffer");
        QK_ASSERT(initialized, "IndexBuffer was not initialized");

        if (!initialized) return false;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hBuffer);
        

        return true;
    }

    bool Buffer_impl::UniformBuffer_bind(u8 slot) {
        QK_ASSERT(hBuffer != 0, "Invalid hBuffer");
        QK_ASSERT(type == BufferType::Uniform, "Attempted to bind non-uniformbuffer");
        QK_ASSERT(initialized, "UniformBuffer was not initialized");

        if (!initialized) return false;
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, hBuffer);

        return false;
    }

    Resource* Resource_cast(const HBuffer& hBuffer) {
        return static_cast<Resource*>(hBuffer);
    }


    HBuffer Buffer_ctor(BufferType type, BufferUsage usage) {

        auto* pBuf = new Buffer_impl(type, usage);

        return pBuf;

    }


    void Buffer_dtor(const HBuffer& hBuf)
    {
        QK_ASSERT(hBuf != nullptr, "hBuf was nullptr");
        QK_ASSERT(hBuf->refcount() == 0 && hBuf->flagged(), "hBuf was locked!");

        hBuf->destroy();
        
    }

    void Buffer_free(HBuffer& hBuf) {
        QK_ASSERT(hBuf != nullptr, "hBuf was nullptr");
        QK_ASSERT(hBuf->refcount() == 0 && hBuf->flagged(), "hBuf was locked!");

        delete hBuf;
        hBuf = nullptr;
    }

    void Buffer_upload(const HBuffer& hBuf, const void* pData, size_t num_bytes)
    {
        QK_ASSERT(hBuf != nullptr, "hBuf was nullptr");


        hBuf->upload(pData, num_bytes);
    }

    

    bool VertexBuffer_bind(const HVertexBuffer& hBuf, u8 slot, u32 offset, u32 stride)
    {
        QK_ASSERT(hBuf != nullptr, "hBuf was nullptr");


        return hBuf->VertexBuffer_bind(slot, offset, stride);
    }

    bool IndexBuffer_bind(const HIndexBuffer& hBuf)
    {
        QK_ASSERT(hBuf != nullptr, "hBuf was nullptr");


        return hBuf->IndexBuffer_bind();

    }

    bool UniformBuffer_bind(const HUniformBuffer& hBuf, u8 slot)
    {

        QK_ASSERT(hBuf != nullptr, "hBuf was nullptr");


        return hBuf->UniformBuffer_bind(slot);

    }


    void Buffer_alloc(const HBuffer& hBuf, size_t num_bytes) {
        QK_ASSERT(hBuf != nullptr, "hBuf was nullptr");
        hBuf->alloc(num_bytes);
    }

    void Buffer_update(const HBuffer& hBuf, const void* pSrc, size_t offset, size_t num_bytes) {
        QK_ASSERT(hBuf != nullptr, "hBuf was nullptr");
        hBuf->update(pSrc, offset, num_bytes);
    }

    void* Buffer_open(const HBuffer& hBuf, EBufferAccessFlags flags) {
        QK_ASSERT(hBuf != nullptr, "hBuf was nullptr");
        return hBuf->open(flags);
    }

    void Buffer_close(const HBuffer& hBuf) {
        QK_ASSERT(hBuf != nullptr, "hBuf was nullptr");
        hBuf->close();
        hBuf->initialized = true;
    }
    void Buffer_cpy(const HBuffer& src, size_t src_offset, const HBuffer& dest, size_t dest_offset, size_t num_bytes) {
        Buffer_impl::copy(src, src_offset, dest, dest_offset, num_bytes);
    }

}
