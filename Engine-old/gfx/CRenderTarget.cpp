#include "CRenderTarget.h"

namespace qk::gfx {

	void CRenderTarget::init(int width, int height) {
		m_Width = width;
		m_Height = height;
		Image offscreen_image(width, height, 3);
		m_OffscreenTarget.init(offscreen_image, GL_TEXTURE_2D);
		m_OffscreenFramebuffer.init();
		if (!m_OffscreenFramebuffer.attach(m_OffscreenTarget, 0)) {
			spdlog::error("INCOMPLETE FRAMEBUFFER!");
		}

		m_OffscreenCommandBuffer.bindFramebuffer(m_OffscreenFramebuffer);
		m_OffscreenCommandBuffer.setViewport(0, 0, m_Width, m_Height);

		m_OffscreenFramebuffer.unbind();

	}

}