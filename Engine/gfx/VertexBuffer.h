#pragma once
#include <HVertexBuffer.h>
#include <span>


namespace qk::gfx {


	class VertexBuffer {
		rhi::HVertexBuffer m_hVertexBuffer;
		size_t m_NumVertices = 0;
		public:
		void init();

		template <class vertex_t>
		void upload(const vertex_t* pVertices, size_t num_vertices, rhi::BufferUsage usage) {
			m_NumVertices = num_vertices;
			rhi::VertexBuffer_upload(m_hVertexBuffer, pVertices, num_vertices, sizeof(vertex_t), usage);
		}

		template <class vertex_t>
		void upload(std::span<vertex_t> vertices, rhi::BufferUsage usage) {
			m_NumVertices = vertices.size();
			rhi::VertexBuffer_upload(m_hVertexBuffer, vertices.data(), m_NumVertices, sizeof(vertex_t), usage);
		}

		rhi::HVertexBufferRef ref() const;

		size_t size() const;


		void destroy();


		bool valid() const;
	};
}