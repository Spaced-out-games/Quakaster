#pragma once
#include <Handle.h>
#include "core.h"
using qk::Handle;

namespace qk::gfx::rhi {

	enum TextureType : u32 {
		TEX_1D,
		TEX_2D,
		TEX_3D,
		TEX_CUBEMAP,
		TEX_1D_ARRAY,
		TEX_2D_ARRAY,
	};

	enum TextureFilterMode : u32 {
		TEXEL,
		BILINEAR
	};

	struct HTexture : Handle {};

	void Texture_upload_2D(const HTexture& hTex, const char* pData, int width, int height, int channels);

	void Texture_upload_2D(const HTexture& hTex, const char* pData, int width, int height, int channels);


	// TODO: deprecate texture bindings at this layer of the RHI once descriptor sets and
	// friends are well-defined and proven.
	void Texture_bind(const HTexture& hTex, uint8_t slot);

	bool Texture_valid(const HTexture& hTex);

	void Texture_dtor(const HTexture& hTex);


}