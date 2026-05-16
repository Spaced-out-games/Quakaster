/// **************************************** QUAKASTER ENGINE **************************************** 
/// gfx/Texture.h
/// Purpose: Represents a Texture resource ready for use anywhere you deem fit.
/// **************************************************************************************************

#pragma once
#include "../core.h"
#include "types.h"
#include "../core/res/Image.h"

using namespace qk::resource;

namespace qk::gfx {

	static unsigned int filter_mode = 0x2600; // GL_NEAREST

	class QK_API Texture {
		friend class TextureBuffer;
		Handle m_Handle = 0;
		TextureType m_Type = 0;

		// NOTE: These are NOT wired up! THese are here to test Framebuffer
		uint32_t m_Width = 480;
		uint32_t m_Height = 480;
		public:
			Texture() = default;
			void init(Image& img, TextureType type);
			void bind();
			Handle handle() const;
			TextureType type() const;
			void destroy();
			~Texture();
			uint32_t width() const;
			uint32_t height() const;


	};
}