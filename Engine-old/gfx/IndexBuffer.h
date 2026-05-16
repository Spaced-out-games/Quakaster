/// **************************************** QUAKASTER ENGINE **************************************** 
/// gfx/IndexBuffer.h
/// Purpose: Represents an Index Buffer stored on the GPU
/// **************************************************************************************************
#pragma once
#include "../core.h"
#include <stdint.h>
#include "gfx.h"

namespace qk::gfx {

    class QK_API IndexBuffer_impl {

    protected:
        Handle m_Handle = NULL_HANDLE;
        uint32_t m_Count = 0;

        IndexBuffer_impl();
        ~IndexBuffer_impl();
        void init_impl();
        void upload_impl(const void* indices, uint32_t count, uint32_t elem_size);
        void bind_impl() const;
        void unbind_impl() const;
        uint32_t size_impl(uint32_t elem_size) const;
        Handle handle_impl();
        void destroy_impl();
        static Handle current_bound() noexcept;
        template<class> friend class IndexBuffer;

    };

    template <class index_t = uint32_t>
    class IndexBuffer : protected IndexBuffer_impl {
    public:
        IndexBuffer() = default;


        void upload(const index_t* data, uint32_t count) {
            upload_impl(data, count, sizeof(index_t));
        }
        void init() {
            init_impl();
        }

        void bind() const { bind_impl(); }
        void unbind() const { unbind_impl(); }

        uint32_t size() const {
            return size_impl(sizeof(index_t));
        }

        Handle handle() const {
            return handle_impl();
        }

        static Handle current_bound() noexcept {
            return IndexBuffer_impl::current_bound();
        }

        void destroy() {
            destroy_impl();
        }

    };


}