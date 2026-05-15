#pragma once
#include <Handle.h>
#include <stdint.h>





namespace qk::gfx::rhi {

	// you need to make mappings of these to GLuint or equivalent in the implementation...
	enum class TextureType: uint32_t {
		TEX_1D,
		TEX_2D,
		TEX_3D,
		TEX_CUBEMAP,
		TEX_1D_ARRAY,
		TEX_2D_ARRAY,
	};

	enum TextureFilterMode : uint32_t {
		TEXEL,
		BILINEAR
	};


	struct HTextureTag {};


	using HTexture = qk::Handle<HTextureTag>;

	HTexture Texture_ctor();

	void Texture_upload_2D(const HTexture& hTex, const uint8_t* pData, int width, int height, int channels);


	void Texture_bind(const HTexture& hTex, TextureType type);
	
	void Texture_dtor(HTexture& hTex);

}