#pragma once
#include "../core.h"
#include "types.h"
#include "Texture.h"


namespace qk::gfx {

	class QK_API FrameBuffer {
			Handle m_FrameBuffer = NULL_HANDLE;
			Handle m_DepthBuffer = NULL_HANDLE;
		public:
		void init();

		void bind();

		void unbind();


		bool attach(Texture& texture, unsigned int slot);

		Handle handle() const;
	};
}