#pragma once
#include <HTexture.h>
#include <stdint.h>


namespace qk::gfx {

	constexpr uint8_t AUTO_TEX_SLOT = 255;

	struct Texture {
		rhi::HTexture m_hTexture;
		rhi::TextureType m_Type;
		public:
			void init();

			void upload_2D(const uint8_t* pPixels, int width, int height, int channels);

			void bind(uint8_t slot) const;

			void destroy();

			rhi::HTextureRef ref() const;


	};

}