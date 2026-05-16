#pragma once
#include "DrawCommand.h"
#include <core.h>

namespace qk::gfx::rhi {

	// TODO: Deal with memory management

	struct HCommandBufferTag {};

	using HCommandBuffer = qk::Resource<HCommandBufferTag>;

	// Lifecycle
	HCommandBuffer CommandBuffer_ctor();
	void CommandBuffer_dtor(HCommandBuffer& hCommandBuffer);

	// Usage
	void CommandBuffer_push(const HCommandBuffer& hCommandBuffer, const DrawCommand& drawCommand);
	void CommandBuffer_call(const HCommandBuffer& hCommandBuffer);



}