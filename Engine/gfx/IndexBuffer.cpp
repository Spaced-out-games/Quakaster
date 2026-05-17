#include "IndexBuffer.h"



namespace qk::gfx {

	void IndexBuffer::init() {
		assert(!hIndexBuffer.valid());
		hIndexBuffer = rhi::IndexBuffer_ctor();
	}

	void IndexBuffer::destroy() {
		rhi::IndexBuffer_dtor(hIndexBuffer);
	}

	rhi::HIndexBufferRef IndexBuffer::ref() const {
		return hIndexBuffer.ref();
	}

	size_t IndexBuffer::size() const {
		return m_NumIndices;
	}

	bool IndexBuffer::valid() const {
		return hIndexBuffer.valid();
	}


}