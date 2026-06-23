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


    bool Buffer_impl::destroy() {
        if (!hBuffer) return false;
        glDeleteBuffers(1, &hBuffer);
        return true;
    }



    bool Buffer_impl::upload(const void* pData, u32 count, u32 elem_size) {
        assert(hBuffer);

        elem_count = count;
        elem_size = elem_size;
        initialized = true;

        glBindBuffer(BufferType_cast(type), hBuffer);
        glBufferData(BufferType_cast(type), count * elem_size, pData, BufferUsage_cast(usage));

        return true;
    }

    bool Buffer_impl::VertexBuffer_bind() {
        assert(type == BufferType::Vertex);

        if (!initialized) return false;

        glBindBuffer(GL_ARRAY_BUFFER, hBuffer);

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


    RBuffer Buffer_ctor(str_view label, BufferType type, BufferUsage usage) {



        auto* pBuf = new Buffer_impl(type, usage);


        return g_Buffers.insert(
            label,
            pBuf
        );
    }

    str_view Buffer_name(const RBuffer& hBuf)
    {
        if (!hBuf) return {};

        return hBuf->label;
    }

    bool Buffer_dtor(RBuffer& hBuf)
    {
        if (!hBuf) return false;

        glDeleteBuffers(1, &hBuf->hBuffer);

        g_Buffers.destroy(hBuf);

        hBuf = nullptr;

        return true;
    }

    bool Buffer_upload(const RBuffer& hBuf, const void* pData, u32 count, u32 elem_size)
    {
        return hBuf->upload(pData, count, elem_size);
    }

    bool Buffer_valid(const RBuffer& hBuf)
    {
        return hBuf && g_Buffers.valid(hBuf);
    }

    bool Buffer_initialized(const RBuffer& hBuf)
    {
        return hBuf && hBuf->initialized;
    }

    BufferType Buffer_type(const RBuffer& hBuf)
    {
        if (!hBuf) return BufferType::Unknown;

        return hBuf->type;
    }

    RBuffer Buffer_resolve(str_view label) {
        return g_Buffers.find(label);
    }

    bool VertexBuffer_bind(const RVertexBuffer& hBuf)
    {
        if (!hBuf) return false;

        return hBuf->VertexBuffer_bind();
    }

    bool IndexBuffer_bind(const RIndexBuffer& hBuf)
    {
        if (!hBuf) return false;

        return hBuf->IndexBuffer_bind();

    }

    bool UniformBuffer_bind(const RUniformBuffer& hBuf, u8 slot)
    {

        if (!hBuf) return false;

        return hBuf->UniformBuffer_bind(slot);

    }

}