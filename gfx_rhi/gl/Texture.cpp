#include "Texture.h"
#include "core.h"
#include "glad/glad.h"
#include <cassert>
using namespace qk::gfx::rhi;




namespace qk {

	template<>
	inline GLuint get<HTextureTag, GLuint>(const Handle<HTextureTag>& handle) {
		return (GLuint)handle.m_Handle;
	}

	template<>
	inline void set<HTextureTag, GLuint>(Handle<HTextureTag>& handle, GLuint value) {
		handle.m_Handle = (uintptr_t)value;
	}


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
		GLuint tex;
		glGenTextures(1, &tex);
		qk::set<HTextureTag, GLuint>(hTex, tex);
		return hTex;
	}


	void Texture_upload_2D(const HTexture& hTex, const uint8_t* data, int width, int height, int channels) {
		GLuint tex = qk::get<HTextureTag, GLuint>(hTex);
		GLenum format = Channel_cast(channels);

		QK_ASSERT(tex);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		
	}

	void Texture_upload_1D(const HTexture& hTex, const uint8_t* data, int size, int channels) {
		GLuint tex = qk::get<HTextureTag, GLuint>(hTex);
		GLenum format = Channel_cast(channels);

		QK_ASSERT(tex);

		glTexImage1D(GL_TEXTURE_1D, 0, format, size, 0, format, GL_UNSIGNED_BYTE, data);
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

	void Texture_bind(const HTexture& hTex, TextureType type) {
		glBindTexture(TextureType_cast(type), qk::get<HTextureTag, GLuint>(hTex));
	}

	void Texture_dtor(HTexture& hTex) {
		GLuint handle = qk::get<HTextureTag, GLuint>(hTex);
		QK_ASSERT(handle);
		glDeleteTextures(1, &handle);
		
		// resets the actual handle
		hTex = nullptr;
	}




}