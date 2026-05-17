#include "VertexArray.h"


namespace qk::gfx {
		
	void VertexArray::init() {
		assert(!m_hVertexArray.valid());
		m_hVertexArray = rhi::VertexArray_ctor();
	}

	void VertexArray::bind() const {
		rhi::VertexArray_bind(m_hVertexArray);
	}

	void VertexArray::destroy() {
		rhi::VertexArray_dtor(m_hVertexArray);
	}


	void VertexArray::apply_layout(VertexBuffer& vertexBuffer, const VertexBufferLayout& layout) {
		rhi::VertexArray_applyLayout(m_hVertexArray, vertexBuffer.ref(), layout.ref());
	}



	bool VertexArray::valid() const {
		return m_hVertexArray.valid();
	}

	rhi::HVertexArrayRef VertexArray::ref() const {
		return m_hVertexArray.ref();
	}



}