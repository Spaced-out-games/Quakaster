#include "HBuffer.h"

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
        assert(hBuffer);

        //elem_count = count;
        size = num_bytes;
        initialized = true;

        glBindBuffer(BufferType_cast(type), hBuffer);
        glBufferData(BufferType_cast(type), num_bytes, pData, BufferUsage_cast(usage));

        // unbind it to ensure correctness
        glBindBuffer(BufferType_cast(type), 0);


    }

    void Buffer_impl::alloc(size_t num_bytes) {
        assert(hBuffer);
        size = num_bytes;
        initialized = false;

        glBindBuffer(BufferType_cast(type), hBuffer);
        glBufferData(BufferType_cast(type), num_bytes, nullptr, BufferUsage_cast(usage));

        // unbind it to ensure correctness
        glBindBuffer(BufferType_cast(type), 0);

    }

    void Buffer_impl::update(const void* pSrc, size_t offset, size_t num_bytes) {
        assert(hBuffer);

        glBindBuffer(BufferType_cast(type), hBuffer);

        glBufferSubData(BufferType_cast(type), offset, num_bytes, pSrc);

        // unbind it to ensure correctness
        glBindBuffer(BufferType_cast(type), 0);

    }



    bool Buffer_impl::VertexBuffer_bind(u8 slot, u32 offset, u32 stride) {
        assert(type == BufferType::Vertex);

        if (!initialized) return false;
        glBindVertexBuffer(slot, hBuffer, offset, stride);
        //glBindBuffer(GL_ARRAY_BUFFER, hBuffer);
        return true;

    }

    bool Buffer_impl::IndexBuffer_bind() {

        assert(type == BufferType::Index);

        if (!initialized) return false;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hBuffer);

        return true;
    }

    bool Buffer_impl::UniformBuffer_bind(u8 slot) {
        assert(type == BufferType::Uniform);

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
        assert(hBuf);
        assert(hBuf->refcount() == 0 && hBuf->flagged());

        hBuf->destroy();
        
    }

    void Buffer_free(HBuffer& hBuf) {
        assert(hBuf);
        assert(hBuf->refcount() == 0 && hBuf->flagged());

        delete hBuf;
        hBuf = nullptr;
    }

    void Buffer_upload(const HBuffer& hBuf, const void* pData, size_t num_bytes)
    {
        assert(hBuf);

        hBuf->upload(pData, num_bytes);
    }

    bool Buffer_valid(const HBuffer& hBuf)
    {
        return hBuf != nullptr;
    }

    bool Buffer_initialized(const HBuffer& hBuf)
    {
        assert(hBuf);
        return hBuf->initialized;
    }

    BufferType Buffer_type(const HBuffer& hBuf)
    {
        if (!hBuf) return BufferType::Unknown;

        return hBuf->type;
    }
    

    bool VertexBuffer_bind(const HVertexBuffer& hBuf, u8 slot, u32 offset, u32 stride)
    {
        assert(hBuf);

        return hBuf->VertexBuffer_bind(slot, offset, stride);
    }

    bool IndexBuffer_bind(const HIndexBuffer& hBuf)
    {
        assert(hBuf);

        return hBuf->IndexBuffer_bind();

    }

    bool UniformBuffer_bind(const HUniformBuffer& hBuf, u8 slot)
    {

        assert(hBuf);

        return hBuf->UniformBuffer_bind(slot);

    }

}