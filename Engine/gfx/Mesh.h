#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include <vector>

namespace qk::gfx {

	
	class Mesh {
		VertexArray m_VertexArray;
		std::vector<VertexBuffer> m_VertexBuffers;
		IndexBuffer m_IndexBuffer;
		public:
			void init();

			void pushVertexBuffer(VertexBuffer&& vertexBuffer, const VertexBufferLayout& layout);

			template <class vertex_t>
			void pushVertexBuffer(std::span<vertex_t> vertices, rhi::BufferUsage usage, const VertexBufferLayout& layout) {
				VertexBuffer buf;
				buf.init();
				buf.upload(vertices, usage);

				pushVertexBuffer(std::move(buf), layout);

			}


			void setIndexBuffer(IndexBuffer&& indexBuffer);


			void bind() const;

			rhi::HVertexArrayRef ref() const;

	};


}