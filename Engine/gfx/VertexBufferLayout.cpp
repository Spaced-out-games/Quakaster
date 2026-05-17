#include "VertexBufferLayout.h"


namespace qk::gfx {

	void VertexBufferLayout::init(const rhi::VertexBufferLayoutDesc& desc) {
		assert(!m_hVertexBufferLayout.valid());

		m_hVertexBufferLayout = rhi::VertexBufferLayout_ctor(desc);
	}

	void VertexBufferLayout::destroy() {
		rhi::VertexBufferLayout_dtor(m_hVertexBufferLayout);
	}

	rhi::HVertexBufferLayoutRef VertexBufferLayout::ref() const {
		return m_hVertexBufferLayout.ref();
	}

}