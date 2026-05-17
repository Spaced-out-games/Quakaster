#include "HTexture.h"
#include "core.h"
#include "glad/glad.h"
#include <cassert>
using namespace qk::gfx::rhi;




namespace qk {
	template<>
	struct ResourceTraits<HTextureTag> {
		using value_type = GLuint;
	};

	template<>
	struct ResourceTraits<HShaderUniformTag> {
		using value_type = GLint;
	};

}

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

	GLenum Channel_cast(int num_channels) {
		QK_ASSERT(num_channels != 2);
		switch (num_channels) {
			case 1: return GL_RED; break;
			case 3: return GL_RGB; break;
			case 4: return GL_RGBA; break;
			default: return GL_RGB; break;
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

	HTexture Texture_ctor() {
		HTexture hTex;
		GLuint gl_tex;
		glGenTextures(1, &gl_tex);
		qk::set(hTex, gl_tex);
		return hTex;
	}


	void Texture_upload_2D(const HTexture& hTex, const uint8_t* pData, int width, int height, int channels) {
		QK_ASSERT(hTex.valid());
		GLuint gl_tex = qk::get(hTex);
		GLenum format = Channel_cast(channels);
		glBindTexture(GL_TEXTURE_2D, gl_tex);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pData);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void Texture_upload_1D(const HTexture& hTex, const uint8_t* pData, int size, int channels) {
		QK_ASSERT(hTex.valid());
		GLuint gl_tex = qk::get(hTex);
		GLenum format = Channel_cast(channels);


		glTexImage1D(GL_TEXTURE_1D, 0, format, size, 0, format, GL_UNSIGNED_BYTE, pData);
	}

	/*
	
			// #TODO: disaggregate and move this into each respective function (rhi::Texture_upload_*)
		switch (type)
		{
		case qk::gfx::rhi::TextureType::TEX_1D:
			glTexImage1D(tex_type, 0, format, width, 0, 4, GL_UNSIGNED_BYTE, data);
			break;
		case qk::gfx::rhi::TextureType::TEX_2D:
			glTexImage2D(tex_type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			break;
		case qk::gfx::rhi::TextureType::TEX_3D:
			//glTexImage3D(tex_type, 0, format, width, height, depth, 0, format, GL_UNSIGNED_BYTE, data);
			break;
		case qk::gfx::rhi::TextureType::TEX_CUBEMAP:
			
			break;
		case qk::gfx::rhi::TextureType::TEX_1D_ARRAY:
			break;
		case qk::gfx::rhi::TextureType::TEX_2D_ARRAY:
			break;
		default:
			break;
		}
	
	
	*/

	void Texture_bind(const HTexture& hTex, TextureType type, uint8_t slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(TextureType_cast(type), get(hTex));
	}

	void Texture_dtor(HTexture& hTex) {
		QK_ASSERT(hTex.valid());

		GLuint gl_tex = get(hTex);
		glDeleteTextures(1, &gl_tex);
		
		// resets the actual handle
		hTex = nullptr;
	}


	void rhi::Texture_setUniform(HShaderUniform location, uint8_t slot) {
		glUniform1i(get(location), slot);
	}
	



}