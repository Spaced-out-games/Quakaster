#include "FullscreenBlitter.h"


namespace qk::gfx {

	const std::vector<glm::vec2> points = {
		{-1.0f, -1.0f},
		{-1.0f,  1.0f},
		{ 1.0f, -1.0f},
		{ 1.0f,  1.0f}
	};

	const std::string onscreen_vert_src = R"(
		#version 330 core
		layout(location = 0) in vec2 aPos;
		out vec2 aTexCoord;
		void main() {
			gl_Position.xy = aPos;
			gl_Position.zw = vec2(0,1);
			aTexCoord = aPos * 0.5 + 0.5; // maps NDC [-1,1] → [0,1]
		}
	)";

	const std::string onscreen_frag_src = R"(
		#version 330 core

		in vec2 aTexCoord;

		out vec4 FragColor;

		uniform sampler2D texture1;

		void main() {
			FragColor = texture(texture1, aTexCoord);
		}

	)";


	void FullscreenBlitter::setRenderTarget(gfx::Handle offscreen_handle) {
		m_OffscreenTexture = offscreen_handle;
	}


	void FullscreenBlitter::init(int width, int height) {
		gfx::Shader blitter_vert(onscreen_vert_src, GL_VERTEX_SHADER);
		gfx::Shader blitter_frag(onscreen_frag_src, GL_FRAGMENT_SHADER);
		m_BlitterShader.init(blitter_frag.compile(), blitter_vert.compile());

		m_VertexArray.init();
		m_VertexArray.bind();

		m_VertexBuffer.init();
		m_VertexBuffer.upload(points.data(), points.size(), GL_STATIC_DRAW);

		gfx::VertexBufferLayout blit_layout;
		blit_layout.push<float>(2);
		m_VertexArray.apply(m_VertexBuffer, blit_layout);
		auto handle = m_BlitterShader.uniform("texture1");
		m_CommandBuffer.bindFramebuffer(0);
		m_CommandBuffer.setViewport(0, 0, width, height); // REQUIRED
		m_CommandBuffer.bindVertexArray(m_VertexArray);
		m_CommandBuffer.bindShaderProgram(m_BlitterShader);
		m_CommandBuffer.bindTexture(m_OffscreenTexture, GL_TEXTURE_2D, 0);
		m_CommandBuffer.setUniformi(handle, 0);
		m_CommandBuffer.drawVertexArray(GL_TRIANGLE_STRIP, 0, 4);


	}

	void FullscreenBlitter::update_viewport(unsigned int x, unsigned y, unsigned int w, unsigned h) {
		// empty for now
	}

}