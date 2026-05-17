#include "Mesh.h"




namespace qk::gfx {

	void Mesh::init() {
		assert(!m_VertexArray.valid());
		m_VertexArray.init();
	}

	void Mesh::pushVertexBuffer(VertexBuffer&& vertexBuffer, const VertexBufferLayout& layout) {

		m_VertexArray.apply_layout(vertexBuffer, layout);

		m_VertexBuffers.push_back(std::move(vertexBuffer));

	}

	void Mesh::setIndexBuffer(IndexBuffer&& indexBuffer) {
		assert(!m_IndexBuffer.valid());

		m_IndexBuffer = std::move(indexBuffer);
	}


	void Mesh::bind() const {
		m_VertexArray.bind();
	}

	rhi::HVertexArrayRef Mesh::ref() const {
		return m_VertexArray.ref();
	}




}