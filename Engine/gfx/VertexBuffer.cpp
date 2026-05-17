#include "VertexBuffer.h"



namespace qk::gfx {

	void VertexBuffer::init() {
		assert(!m_hVertexBuffer.valid());
		m_hVertexBuffer = rhi::VertexBuffer_ctor();
	}

	void VertexBuffer::destroy() {
		rhi::VertexBuffer_dtor(m_hVertexBuffer);
	}

	rhi::HVertexBufferRef VertexBuffer::ref() const {
		return m_hVertexBuffer.ref();
	}
	
	size_t VertexBuffer::size() const {
		return m_NumVertices;
	}

	bool VertexBuffer::valid() const {
		return m_hVertexBuffer.valid();
	}

}