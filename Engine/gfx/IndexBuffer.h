#pragma once
#include <HIndexBuffer.h>
#include <span>

namespace qk::gfx {


	class IndexBuffer {
		rhi::HIndexBuffer hIndexBuffer;
		size_t m_NumIndices = 0;
	public:
		void init();

		template <class index_t>
		void upload(const index_t* pIndices, size_t num_indices) {
			m_NumIndices = num_indices;
			rhi::IndexBuffer_upload(hIndexBuffer, pIndices, num_indices, sizeof(index_t));
		}

		template <class vertex_t>
		void upload(std::span<vertex_t> indices) {
			m_NumIndices = indices.size();
			rhi::IndexBuffer_upload(hIndexBuffer, indices.data(), m_NumIndices, sizeof(vertex_t));
		}

		rhi::HIndexBufferRef ref() const;

		size_t size() const;


		void destroy();

		bool valid() const;

	};
}