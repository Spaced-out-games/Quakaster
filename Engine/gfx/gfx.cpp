#include "gfx.h"


namespace qk::gfx {

	HTexture Texture_fromImage(const media::Image& img) {
		HTexture hTex = rhi::Texture_ctor({
			.type = TextureType::TEX_2D,
			.format = TextureFormat::RGB_8,
			.width = img.width(),
			.height = img.height(),
			.depth = 0,
			.mip_levels = 0,
			.array_layers = 0,
			.usage = TextureUsage::Sampled

			});

		if (!hTex) return nullptr;

		rhi::Texture_upload(hTex, {
			.pData = img.data(),
			.format = rhi::TextureFormat::RGBA_8,
			.mip_level = 0,
			.array_layer = 0,
			.width = img.width(),
			.height = img.height(),
			.depth = 0
			});

		rhi::Texture_setSamplingParameters(hTex, {
			.min_filter = rhi::TextureFilter::Nearest,
			.mag_filter = rhi::TextureFilter::Nearest
			});



		return hTex;

	}

}