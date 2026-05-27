#pragma once
#include "HMaterial.h"
#include "HVertexArray.h"
#include <gfx_rhi.h> // for ClearChannels. Move here eventually.
#include "Draw.h" // for PrimitiveTopology. Move here eventually.



namespace qk::gfx::rhi {



	/*
	
	void clear(float r, float g, float b, float a, ClearChannels channels) {
		g_ClearChannels = channels;
		g_ClearColor[0] = r; g_ClearColor[1] = g;
		g_ClearColor[2] = b; g_ClearColor[3] = a;

		glClearColor(r, g, b, a);
		glClear(ClearChannels_cast(g_ClearChannels));
		

	}

	*/

	struct NullCommand_desc {};

	struct ClearCommand_desc {
		float r, g, b, a;
		ClearChannels channels;
	};

	struct DrawArrays_desc {
		HMaterial material;
		HVertexArray mesh;
		PrimitiveTopology topology;
		int offset;
		size_t num_vertices;
	};

	using DrawCommand_desc = Variant<NullCommand_desc, ClearCommand_desc, DrawArrays_desc>;

	class CommandBuffer_createinfo {

		std::vector<DrawCommand_desc> m_Preexecute;
		std::vector<DrawCommand_desc> m_Commands;
		std::vector<DrawCommand_desc> m_Postexecute;
		public:
			CommandBuffer_createinfo& preexecute(const DrawCommand_desc& cmd) {
				m_Preexecute.push_back(cmd);
				return *this;
			}

			CommandBuffer_createinfo& postexecute(const DrawCommand_desc& cmd) {
				m_Postexecute.push_back(cmd);
				return *this;
			}

			CommandBuffer_createinfo& push(const DrawCommand_desc& cmd) {
				m_Commands.push_back(cmd);
				return *this;
			}

			const std::vector<DrawCommand_desc>& preexec() const { return m_Preexecute; }
			const std::vector<DrawCommand_desc>& commands() const { return m_Commands; }
			const std::vector<DrawCommand_desc>& postexec() const { return m_Postexecute; }

	};

	struct HCommandBuffer : Handle {};

	HCommandBuffer CommandBuffer_ctor(const CommandBuffer_createinfo& desc);

	void CommandBuffer_dtor(const HCommandBuffer& hCmdBuf);

	bool CommandBuffer_valid(const HCommandBuffer& hCmdBuf);


	// NOTE: THIS IS TEMPORARY; REPLACE ONCE PIPELINES AND FRIENDS ARE PRESENT!!!
	bool CommandBuffer_execute(const HCommandBuffer& hCmdBuf);



}