#pragma once
#include "HCommandBuffer.h" // for PrimitiveTopology
#include "HVertexLayout.h"
#include <array>
#include "HShader.h"
#include "core.h"
#include <span>

namespace qk::gfx::rhi {

    // -------- Pipeline state -------- //

    // What to draw with
    enum class PrimitiveTopology {
        Unknown,
        Triangle,
        Lines,
        Points,
        TriangleStrip,
        LineStrip
    };

    // Which channels to clear
    enum ClearChannels : u32 {
        Color = 1u << 0,
        Depth = 1u << 1,
        Stencil = 1u << 2
    };

    inline constexpr ClearChannels operator|(ClearChannels a, ClearChannels b) {
        return (ClearChannels)((u32)a | (u32)b);
    }

    inline constexpr ClearChannels operator&(ClearChannels a, ClearChannels b) {
        return (ClearChannels)((u32)a & (u32)b);
    }

    inline constexpr ClearChannels operator^(ClearChannels a, ClearChannels b) {
        return (ClearChannels)((u32)a ^ (u32)b);
    }

    inline constexpr ClearChannels operator~(ClearChannels a) {
        return (ClearChannels)(~(u32)a);
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











    // -------- Pipeline creation recipe -------- //

	struct Pipeline_recipe {

		std::vector<ShaderModule>   shader_modules;
		PrimitiveTopology			topology;
		VertexLayout_recipe			layout;
		//CullMode cull;
		//DepthState depth;
		//BlendState blend;

	};

	// Pipeline implementation
	struct Pipeline_impl;

	// Pipeline handle
	using HPipeline = Pipeline_impl*;

	// Pipeline construction
	HPipeline Pipeline_ctor(const Pipeline_recipe& recipe);

	// Pipeline destruction
	void Pipeline_dtor(HPipeline& pipeline);

    // todo: move to commandbuffer running implementation
	bool Pipeline_bind(const HPipeline& pipeline);


}