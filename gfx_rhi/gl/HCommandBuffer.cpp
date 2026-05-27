#include "HCommandBuffer.h"
#include <glad/glad.h>


namespace qk::gfx::rhi {

	using CommandBuffer_impl = CommandBuffer_createinfo;


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
		return cmd.visit([](const auto& desc) -> bool {

			using T = std::decay_t<decltype(desc)>;

		if constexpr (std::is_same_v<T, NullCommand_desc>)
		{
			return true;
		}
		else if constexpr (std::is_same_v<T, ClearCommand_desc>)
		{
			glClearColor(desc.r, desc.g, desc.b, desc.a);
			glClear(ClearChannels_cast(desc.channels));

			return true;
		}
		else if constexpr (std::is_same_v<T, DrawArrays_desc>)
		{
			if (!VertexArray_valid(desc.mesh))
				return false;
			VertexArray_bind(desc.mesh);

			if (!Material_bind(desc.material))
				return false;

			glDrawArrays(
				PrimitiveType_cast(desc.topology),
				desc.offset,
				desc.num_vertices);

			return true;
		}

		return false;
		});
	}







	HCommandBuffer CommandBuffer_ctor(const CommandBuffer_createinfo& desc) {
		return g_CommandBuffers.insert(desc);
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

		const auto& pre = CmdBuf.preexec();
		const auto& exec = CmdBuf.commands();
		const auto& post = CmdBuf.postexec();



		for (auto& cmd : pre) {

			if (!DrawCommand_execute(cmd)) return 0;

		}

		for (auto& cmd : exec) {

			if (!DrawCommand_execute(cmd)) return 0;

		}

		for (auto& cmd : post) {

			if (!DrawCommand_execute(cmd)) return 0;

		}

		return 1;


	}



}