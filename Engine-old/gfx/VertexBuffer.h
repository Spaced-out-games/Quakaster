/// **************************************** QUAKASTER ENGINE **************************************** 
/// gfx/VertexBuffer.h
/// Purpose: Represents a Vertex Buffer stored on the GPU
/// **************************************************************************************************

#pragma once
#include "gfx.h"


namespace qk::gfx {
	// TODO: Separate initialization from uploads

	class QK_API VertexBuffer_impl {
			//protected:
				Handle m_Handle = NULL_HANDLE;
				size_t m_Count = 0;
		public:

				VertexBuffer_impl();
				~VertexBuffer_impl();
				void init_impl();
				void upload_impl(const void* data, size_t count, size_t elem_size, unsigned int usage);
				void bind_impl() const;
				void unbind_impl() const;
				void destroy_impl();
				size_t count() const;
				
	};


	template <class vertex_t>
	struct VertexBuffer : VertexBuffer_impl {
		size_t elem_size() const {
			return sizeof(vertex_t);
		}

		size_t count() const {
			return m_Count;
		}

		size_t size_bytes() const {
			return count() * elem_size();
		}


		void init() {
			init_impl();
		}

		void upload(const vertex_t* data, size_t numVertices, unsigned int usage) {
			upload_impl(data, numVertices, sizeof(vertex_t), usage);
		}

		void bind() const { bind_impl(); }
		void unbind() const { unbind_impl(); }

		void destroy() {
			destroy_impl();
		}


		~VertexBuffer() {
			destroy();
		}

	};

}