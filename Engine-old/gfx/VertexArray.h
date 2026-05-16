/// **************************************** QUAKASTER ENGINE **************************************** 
/// gfx/VertexArray.h
/// Purpose: Represents a Vertex Array stored on the GPU
/// **************************************************************************************************

#pragma once
#include "../core.h"
#include "types.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


namespace qk::gfx {
	class QK_API VertexArray {
		Handle m_Handle = NULL_HANDLE;

		void apply_impl(const VertexBufferLayout& layout, VertexBufferLayoutCursor cursor) const;

		inline void apply_impl(const VertexBufferLayout& layout) const {
			VertexBufferLayoutCursor cursor = 0;
			apply_impl(layout, cursor);
		}

		public:
			void bind() const;
			void unbind() const;
			Handle handle() const;
			explicit operator Handle() const noexcept;
			static Handle current_bound() noexcept;
			void init();


			template <class vertex_t>
			void apply(const VertexBuffer<vertex_t>& buffer, const VertexBufferLayout& layout) const {
				bind();
				buffer.bind();

				apply_impl(layout);
			}

			
			void apply(const VertexBuffer_impl& buffer, const VertexBufferLayout& layout) const {
				bind();
				buffer.bind_impl();

				apply_impl(layout);
			}

			void destroy();

			VertexArray();
			~VertexArray();

	};
}