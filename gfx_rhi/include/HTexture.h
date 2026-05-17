#pragma once
#include <Resource.h>
#include <stdint.h>
#include "ResourcePool.h"
#include "HShaderUniform.h"




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

	using HTexture = qk::Resource<HTextureTag>;
	using HTextureRef = qk::Ref<HTextureTag>;

	HTexture Texture_ctor();

	void Texture_upload_2D(const HTexture& hTex, const uint8_t* pData, int width, int height, int channels);


	void Texture_bind(const HTexture& hTex, TextureType type, uint8_t slot);
	
	void Texture_dtor(HTexture& hTex);

	void Texture_setUniform(HShaderUniform location, uint8_t slot);

	using TextureRegistry = ResourcePool<HTextureTag, Texture_dtor>;


}