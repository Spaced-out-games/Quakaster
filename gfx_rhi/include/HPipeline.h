#pragma once
#include "HCommandBuffer.h" // for PrimitiveTopology
#include "VertexLayoutDesc.h"
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











    // -------- Pipeline create info -------- //

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


	// Pipeline lifetime
	HPipeline Pipeline_ctor(const Pipeline_recipe& recipe);
	void Pipeline_dtor(const HPipeline& hPipeline);
    void Pipeline_free(HPipeline& hPipeline);
    Resource* Resource_cast(const HPipeline& hPipeline);


    // Registry of pipeline objects
    using PipelineRegistry = ResourceRegistry<Pipeline_impl, Pipeline_dtor, Pipeline_free>;


}