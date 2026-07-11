#include "HBuffer.h"

#include <Resource.h>
#include <glad/glad.h>
#include <stdexcept>
#include "rhi_globals.h"
namespace qk::gfx::rhi {



    Buffer_impl::Buffer_impl( BufferType type, BufferUsage usage, u32 size, u32 count, bool ready):
        initialized(ready), type(type), usage(usage),
        elem_size(size), elem_count(count)
        {
            glGenBuffers(1, &hBuffer);
        }


    Buffer_impl::~Buffer_impl() {
        destroy();
    }


    void Buffer_impl::destroy() {
        if (!hBuffer) return;
        glDeleteBuffers(1, &hBuffer);
        return;
    }



    bool Buffer_impl::upload(const void* pData, u32 count, u32 elem_size) {
        assert(hBuffer);

        elem_count = count;
        elem_size = elem_size;
        initialized = true;

        glBindBuffer(BufferType_cast(type), hBuffer);
        glBufferData(BufferType_cast(type), count * elem_size, pData, BufferUsage_cast(usage));

        // unbind it to ensure correctness
        glBindBuffer(BufferType_cast(type), 0);


        return true;
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


    HBuffer Buffer_ctor(BufferType type, BufferUsage usage) {



        auto* pBuf = new Buffer_impl(type, usage);

        return pBuf;
    }


    bool Buffer_dtor(HBuffer& hBuf)
    {
        if (!hBuf) return false;

        glDeleteBuffers(1, &hBuf->hBuffer);


        //delete hBuf;
        delete hBuf;

        hBuf = nullptr;

        return true;
    }

    bool Buffer_upload(const HBuffer& hBuf, const void* pData, u32 count, u32 elem_size)
    {
        return hBuf->upload(pData, count, elem_size);
    }

    bool Buffer_valid(const HBuffer& hBuf)
    {
        //return hBuf != nullptr;//&& g_Buffers.valid(hBuf);
        return hBuf != nullptr;
    }

    bool Buffer_initialized(const HBuffer& hBuf)
    {
        return hBuf && hBuf->initialized;
    }

    BufferType Buffer_type(const HBuffer& hBuf)
    {
        if (!hBuf) return BufferType::Unknown;

        return hBuf->type;
    }
    

    bool VertexBuffer_bind(const HVertexBuffer& hBuf, u8 slot, u32 offset, u32 stride)
    {
        if (!hBuf) return false;

        return hBuf->VertexBuffer_bind(slot, offset, stride);
    }

    bool IndexBuffer_bind(const HIndexBuffer& hBuf)
    {
        if (!hBuf) return false;

        return hBuf->IndexBuffer_bind();

    }

    bool UniformBuffer_bind(const HUniformBuffer& hBuf, u8 slot)
    {

        if (!hBuf) return false;

        return hBuf->UniformBuffer_bind(slot);

    }

}