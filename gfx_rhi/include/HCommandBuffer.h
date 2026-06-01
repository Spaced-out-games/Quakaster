#pragma once
#include "HMaterial.h"
#include "HVertexArray.h"



namespace qk::gfx::rhi {


	enum PrimitiveTopology {
		Triangle,
		Lines,
		Points,
		TriangleStrip,
		LineStrip
	};


	enum ClearChannels : uint32_t {
		Color = 1u << 0,
		Depth = 1u << 1,
		Stencil = 1u << 2
	};

	inline constexpr ClearChannels operator|(ClearChannels a, ClearChannels b) {
		return (ClearChannels)(
			(uint32_t)(a) | (uint32_t)(b)
			);
	}

	inline constexpr ClearChannels operator&(ClearChannels a, ClearChannels b) {
		return (ClearChannels)(
			(uint32_t)(a) & (uint32_t)(b)
			);
	}

	inline constexpr ClearChannels operator^(ClearChannels a, ClearChannels b) {
		return (ClearChannels)(
			(uint32_t)(a) ^ (uint32_t)(b)
			);
	}

	inline constexpr ClearChannels operator~(ClearChannels a) {
		return (ClearChannels)(
			~(uint32_t)(a)
			);
	}

	inline ClearChannels& operator|=(ClearChannels& a, ClearChannels b) {
		a = a | b;
		return a;
	}

	inline ClearChannels& operator&=(ClearChannels& a, ClearChannels b) {
		a = a & b;
		return a;
	}

	inline ClearChannels& operator^=(ClearChannels& a, ClearChannels b) {
		a = a ^ b;
		return a;
	}

	struct NullCommand_desc {};

	struct ClearCommand_desc {
		float r, g, b, a;
		ClearChannels channels;
	};

	struct DrawArrays_desc {
		RMaterial material;
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