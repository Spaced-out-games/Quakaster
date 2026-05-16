#pragma once
#include "../gfx/CommandBuffer.h"
#include "../gfx/ShaderProgram.h"
#include "../gfx/VertexBuffer.h"
#include "../gfx/VertexArray.h"
#include "../gfx/Texture.h"
#include "../gfx/types.h"
#include "glm/vec2.hpp"
#include "../core.h"


namespace qk::gfx {

	

	struct QK_API FullscreenBlitter {
		// the target texture
		gfx::Handle						m_OffscreenTexture;
		gfx::ShaderProgram				m_BlitterShader;
		gfx::VertexBuffer<glm::vec2>	m_VertexBuffer;
		gfx::VertexArray				m_VertexArray;

		gfx::CommandBuffer				m_CommandBuffer;

		void setRenderTarget(gfx::Handle offscreen_handle);

		void init(int width, int height);

		void update_viewport(unsigned int x, unsigned y, unsigned int w, unsigned h);

	};

}