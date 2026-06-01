#include "HCommandBuffer.h"
#include <glad/glad.h>


namespace qk::gfx::rhi {

	//using CommandBuffer_impl = CommandBuffer_recipe;

	struct CommandBuffer_impl {
		std::vector<DrawCommand_desc> commands;
	};


	static inline qk::HandleResourceRegistry<CommandBuffer_impl, HCommandBuffer> g_CommandBuffers;




	inline GLbitfield ClearChannels_cast(qk::gfx::rhi::ClearChannels channels) {
		GLbitfield result = 0;

		if ((channels & qk::gfx::rhi::ClearChannels::Color) == qk::gfx::rhi::ClearChannels::Color)
			result |= GL_COLOR_BUFFER_BIT;

		if ((channels & qk::gfx::rhi::ClearChannels::Depth) == qk::gfx::rhi::ClearChannels::Depth)
			result |= GL_DEPTH_BUFFER_BIT;

		if ((channels & qk::gfx::rhi::ClearChannels::Stencil) == qk::gfx::rhi::ClearChannels::Stencil)
			result |= GL_STENCIL_BUFFER_BIT;

		return result;
	}

	inline GLuint PrimitiveType_cast(PrimitiveTopology type) {
		switch (type)
		{
		case qk::gfx::rhi::PrimitiveTopology::Triangle:
			return GL_TRIANGLES;
		case qk::gfx::rhi::PrimitiveTopology::Lines:
			return GL_LINES;
		case qk::gfx::rhi::PrimitiveTopology::Points:
			return GL_POINTS;
		case qk::gfx::rhi::PrimitiveTopology::TriangleStrip:
			return GL_TRIANGLE_STRIP;
		case qk::gfx::rhi::PrimitiveTopology::LineStrip:
			return GL_LINE_STRIP;
		default:
			return GL_TRIANGLES;
			break;
		}
	}


	bool DrawCommand_execute(const DrawCommand_desc& cmd)
	{
		switch (cmd.type)
		{
		case qk::gfx::rhi::DrawCommandType::Null:
			return 1;
		case qk::gfx::rhi::DrawCommandType::Clear:
			glClearColor(cmd.clear.r, cmd.clear.g, cmd.clear.b, cmd.clear.a);
			glClear(ClearChannels_cast(cmd.clear.channels));
			return 1;
		case qk::gfx::rhi::DrawCommandType::DrawArrays:
			
			if (!VertexArray_bind(cmd.drawArrays.mesh)) return 0;

			if (!Material_bind(cmd.drawArrays.material)) return 0;

			glDrawArrays(
				PrimitiveType_cast(cmd.drawArrays.topology),
				cmd.drawArrays.offset,
				cmd.drawArrays.num_vertices);
			return 1;

		default:
			throw std::runtime_error("Not implemented!");
			break;
		}

	}







	HCommandBuffer CommandBuffer_ctor(const CommandBuffer_recipe& desc) {
		return g_CommandBuffers.insert(CommandBuffer_impl{ desc.commands() });
	}

	void CommandBuffer_dtor(const HCommandBuffer& hCmdBuf) {
		g_CommandBuffers.destroy(hCmdBuf);
	}

	bool CommandBuffer_valid(const HCommandBuffer& hCmdBuf) {
		return g_CommandBuffers.valid(hCmdBuf);
	}


	bool CommandBuffer_execute(const HCommandBuffer& hCmdBuf) {
		if (!g_CommandBuffers.valid(hCmdBuf)) return 0;

		const auto& CmdBuf = g_CommandBuffers.get(hCmdBuf);

		const auto& exec = CmdBuf.commands;

		for (auto& cmd : exec) {

			if (!DrawCommand_execute(cmd)) return 0;

		}

		return 1;


	}



}