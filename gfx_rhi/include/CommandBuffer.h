#pragma once
#include "core.h"
#include "Handle.h"

namespace qk::gfx::rhi {

	using HCommandBuffer = qk::Handle<struct CommandBufferTag>;

	// Lifecycle
	HCommandBuffer QK_API CommandBuffer_ctor();
	void QK_API CommandBuffer_dtor(HCommandBuffer handle);

	// Usage
	void QK_API CommandBuffer_submit(const HCommandBuffer handle);
	void QK_API CommandBuffer_clear(HCommandBuffer handle);

	// Query
	size_t QK_API CommandBuffer_size(const HCommandBuffer handle);

}