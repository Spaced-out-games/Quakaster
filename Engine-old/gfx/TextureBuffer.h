#pragma once
#include "../core.h"
#include "Texture.h"

namespace qk::gfx {

	struct QK_API TextureBuffer {
		Texture m_Texture;         // Handles texture binding & handle
		gfx::Handle m_Buffer = NULL_HANDLE;
		unsigned int m_Usage = 0;
		unsigned int m_Type = 0;   // Internal format

		void init(unsigned int usage = GL_STATIC_DRAW, unsigned int type = GL_RGBA32F);
		void bind_tex(unsigned int slot);
		void bind_buff();
		void upload(const void* data, size_t SizeBytes);
	};
}