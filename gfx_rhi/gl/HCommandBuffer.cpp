#include "rhi_ResourceDefinitions.inl"
#include "HCommandBuffer.h"
#include <vector>

namespace qk::gfx::rhi {

	struct CommandBuffer {
		std::vector<DrawCommand> m_Commands;
	};

	// Lifecycle
	HCommandBuffer CommandBuffer_ctor() {
		HCommandBuffer hCommandBuffer;

		CommandBuffer* commandBuf = new CommandBuffer();
		set(hCommandBuffer, commandBuf);


		return hCommandBuffer;

	}
	void CommandBuffer_dtor(HCommandBuffer& hCommandBuffer) {
		assert(hCommandBuffer.valid());
		delete get(hCommandBuffer);
		hCommandBuffer = nullptr;
	}

	// Usage
	void CommandBuffer_push(const HCommandBuffer& hCommandBuffer, const DrawCommand& drawCommand) {
		assert(hCommandBuffer.valid());

		get(hCommandBuffer)->m_Commands.push_back(drawCommand);

	}
	void CommandBuffer_call(const HCommandBuffer& hCommandBuffer) {

		assert(hCommandBuffer.valid());

		auto& cmdBuf = get(hCommandBuffer)->m_Commands;

		for (const auto& cmd : cmdBuf) {
			call(cmd);
		}

	}



}