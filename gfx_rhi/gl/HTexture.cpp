#include "HTexture.h"
#include "glad/glad.h"
#include "core.h"


namespace qk::gfx::rhi {

	inline GLuint TextureType_cast(TextureType type) {
		switch (type)
		{
		case qk::gfx::rhi::TextureType::TEX_1D:
			return GL_TEXTURE_1D;
			break;
		case qk::gfx::rhi::TextureType::TEX_2D:
			return GL_TEXTURE_2D;
			break;
		case qk::gfx::rhi::TextureType::TEX_3D:
			return GL_TEXTURE_3D;
			break;
		case qk::gfx::rhi::TextureType::TEX_CUBEMAP:
			return GL_TEXTURE_CUBE_MAP;
			break;
		case qk::gfx::rhi::TextureType::TEX_2D_ARRAY:
			return GL_TEXTURE_2D_ARRAY;
			break;
		default:
			return 0;
			break;
		}
	}

	inline GLint TextureFilterMode_cast(TextureFilterMode mode) {
		switch (mode)
		{
		case qk::gfx::rhi::TextureFilterMode::TEXEL:
			return GL_NEAREST;
			break;
		case qk::gfx::rhi::TextureFilterMode::BILINEAR:
			return GL_LINEAR;
			break;
		default:
			return 0;
			break;
		}
	}

	inline GLenum Channel_cast(int num_channels) {
		QK_ASSERT(num_channels != 2);
		switch (num_channels) {
		case 1: return GL_RED; break;
		case 3: return GL_RGB; break;
		case 4: return GL_RGBA; break;
		default: return GL_RGB; break;
		}
	}

	struct Texture_impl {
		TextureType type	= TEX_2D;
		GLuint		texture	= 0;
	};


	static inline qk::HandleResourceRegistry<Texture_impl, HTexture> g_Textures;

	void Texture_upload_2D(const HTexture& hTex, const char* pData, int width, int height, int channels) {
		auto* pTexDesc = g_Textures.try_get(hTex);
		if (!pTexDesc) return;
		GLenum format = Channel_cast(channels);
		pTexDesc->type = TEX_2D;

		glBindTexture(TextureType_cast(pTexDesc->type), pTexDesc->texture);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pData);

		// TODO: Like a real American patriot, offshore this to HTextureSampler.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
	}

	void Texture_bind(const HTexture& hTex, uint8_t slot) {
		QK_ASSERT(g_Textures.valid(hTex));



		Texture_impl& Tex = g_Textures.get(hTex);

		

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(TextureType_cast(Tex.type), Tex.texture);

	}

	bool Texture_valid(const HTexture& hTex) {
		return g_Textures.valid(hTex);
	}


	void Texture_dtor(const HTexture& hTex) {
		QK_ASSERT(g_Textures.valid(hTex));

		auto& tex = g_Textures.get(hTex);

		glDeleteTextures(1, &tex.texture);

		g_Textures.destroy(hTex);

	}


}