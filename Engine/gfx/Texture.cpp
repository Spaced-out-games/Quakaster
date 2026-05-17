#include "Texture.h"


namespace qk::gfx {

	void Texture::init() {
		assert(!m_hTexture.valid());
		m_hTexture = rhi::Texture_ctor();
	}


	void Texture::upload_2D(const uint8_t* pPixels, int width, int height, int channels) {
		m_Type = rhi::TextureType::TEX_2D;
		rhi::Texture_upload_2D(m_hTexture, pPixels, width, height, channels);
	}

	void Texture::bind(uint8_t slot) const {
		rhi::Texture_bind(m_hTexture, m_Type, slot);
	}

	void Texture::destroy() {
		rhi::Texture_dtor(m_hTexture);
	}

	rhi::HTextureRef Texture::ref() const {
		return m_hTexture.ref();
	}


}