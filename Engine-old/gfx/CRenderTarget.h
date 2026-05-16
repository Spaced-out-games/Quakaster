#pragma once
#include "../core/res/Image.h"
#include "spdlog/spdlog.h"
#include "CommandBuffer.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "../core.h"

namespace qk::gfx {
	struct QK_API CRenderTarget {
		gfx::CommandBuffer	m_OffscreenCommandBuffer;
		gfx::FrameBuffer	m_OffscreenFramebuffer;
		gfx::Texture		m_OffscreenTarget;
		int					m_Width = 0;
		int					m_Height = 0;

		void init(int width, int height);

	};
}