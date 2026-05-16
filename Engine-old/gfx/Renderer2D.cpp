#include "Renderer2D.h"


namespace qk::gfx {


	const std::string vert_src = R"(
		#version 330 core

		layout(location = 0) in vec2 aPos;
		layout(location = 1) in vec2 aUV;
		layout(location = 2) in vec4 aColor;

		out vec2 vUV;
		out vec4 vColor;

		

		void main()
		{
			vUV = aUV;
			vColor = aColor;

			float x = aPos.x;//(aPos.x / 1080.0) * 2.0 - 1.0;
			float y = aPos.y;//1.0 - (aPos.y / 1920.0) * 2.0;

			gl_Position = vec4(x, y, 0.0, 1.0);
		}
	)";
	const std::string frag_src = R"(
		#version 330 core

		in vec2 vUV;
		in vec4 vColor;

		out vec4 FragColor;

		uniform sampler2D uTexture;

		void main()
		{
			vec4 tex = texture(uTexture, vUV);
			FragColor = tex * vColor;
		}
	)";

	// temporary; for TextureBuffer testing
	const std::string frag_bypass_src = R"(
		#version 330 core

		in vec2 vUV;
		in vec4 vColor;

		out vec4 FragColor;

		uniform sampler2D uTexture;
		uniform samplerBuffer uTextureBuffer;

		void main()
		{
			// vec4 tex = texture(uTexture, vUV);
			vec4 tex = texelFetch(uTextureBuffer, 0); // fetch just the first vec4
			FragColor = vec4(tex.rgb, 1.0f);// * vColor;
		}
	)";



	void Renderer2D::init() {
		m_VertexArray.init();
		m_VertexArray.bind();

		m_VertexBuffer.init();
		m_VertexBuffer.bind();

		m_IndexBuffer.init();
		m_IndexBuffer.bind();

		Shader frag(frag_bypass_src, GL_FRAGMENT_SHADER);
		Shader vert(vert_src, GL_VERTEX_SHADER);
		m_Shader.init(frag.compile(), vert.compile());
		m_Shader.bind();
		m_TextureUniform = glGetUniformLocation(m_Shader.handle(), "uTexture");


		Image white_img(1, 1, 3);
		auto* pPixel = white_img.data();
		pPixel[0] = 255; pPixel[1] = 255; pPixel[2] = 255;
		m_WhiteTex.init(white_img, GL_TEXTURE_2D);
		VertexBufferLayout layout;
		layout.push<float>(2); // position
		layout.push<float>(2); // uv
		layout.push<float>(3); // color
		m_VertexArray.apply(m_VertexBuffer, layout);

		m_VertexArray.unbind();
	}

	void Renderer2D::destroy() {
		m_VertexArray.destroy();
		m_VertexBuffer.destroy();
		m_Shader.destroy();
	}

	int Renderer2D::begin(CommandBuffer* pCommandBuffer) {
		if (m_CommandBuffer) return 1;
		m_CommandBuffer = pCommandBuffer;

		m_CommandBuffer->bindVertexArray(m_VertexArray);
		m_CommandBuffer->bindShaderProgram(m_Shader);
		m_CommandBuffer->bindTexture(m_WhiteTex.handle(), GL_TEXTURE_2D, 0);

		return 0;
	}

	void Renderer2D::end() {
		assert(m_CommandBuffer != nullptr);
		m_VertexArray.bind();
		m_VertexBuffer.upload(m_Vertices.data(), m_Vertices.size(), GL_STATIC_DRAW);
		m_IndexBuffer.upload(m_Indices.data(), m_Indices.size());
		m_VertexArray.unbind();
		m_CommandBuffer = nullptr;
	}

	uint32_t Renderer2D::vertex_count() const {
		return m_Vertices.size();
	}
	uint32_t Renderer2D::index_count() const {
		return m_Indices.size();
	}

	void Renderer2D::draw_triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec3 color) {

		if (!m_CommandBuffer) return;


		auto vstart = vertex_count();
		auto istart = index_count();

		// add vertices
		m_Vertices.emplace_back(a, glm::vec2{ 0.0,0.0 }, color);
		m_Vertices.emplace_back(b, glm::vec2{ 0.0,0.0 }, color);
		m_Vertices.emplace_back(c, glm::vec2{ 0.0,0.0 }, color);

		// add indices
		m_Indices.push_back(vstart + 0);
		m_Indices.push_back(vstart + 1);
		m_Indices.push_back(vstart + 2);

		m_CommandBuffer->drawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, istart * sizeof(uint32_t));

	}

	void Renderer2D::draw_rect(glm::vec2 pos, glm::vec2 size, glm::vec3 color) {

		if (!m_CommandBuffer) return;

		auto vstart = vertex_count();
		auto istart = index_count();

		glm::vec2 p0 = pos;
		glm::vec2 p1 = pos + glm::vec2(size.x, 0);
		glm::vec2 p2 = pos + size;
		glm::vec2 p3 = pos + glm::vec2(0, size.y);

		// vertices (with UVs)
		m_Vertices.emplace_back(p0, glm::vec2{ 0, 1 }, color);
		m_Vertices.emplace_back(p1, glm::vec2{ 1, 1 }, color);
		m_Vertices.emplace_back(p2, glm::vec2{ 1, 0 }, color);
		m_Vertices.emplace_back(p3, glm::vec2{ 0, 0 }, color);

		// indices
		m_Indices.push_back(vstart + 0);
		m_Indices.push_back(vstart + 1);
		m_Indices.push_back(vstart + 2);

		m_Indices.push_back(vstart + 0);
		m_Indices.push_back(vstart + 2);
		m_Indices.push_back(vstart + 3);

		m_CommandBuffer->drawElements(
			GL_TRIANGLES,
			6,
			GL_UNSIGNED_INT,
			istart * sizeof(uint32_t)
		);
	}

	inline void Renderer2D::bind_texture(Texture& target, unsigned int slot) {
		if (!m_CommandBuffer) return;

		m_CommandBuffer->bindTexture(target.handle(), target.type(), slot);
	}



	void Renderer2D::draw_polygon(const glm::vec2* points, size_t count, glm::vec3 color) {

		if (!m_CommandBuffer || count < 3) return;

		auto vstart = vertex_count();
		auto istart = index_count();

		// vertices
		for (size_t i = 0; i < count; ++i) {
			m_Vertices.emplace_back(points[i], glm::vec2{ 0, 0 }, color);
		}

		// indices (triangle fan)
		for (size_t i = 1; i < count - 1; ++i) {
			m_Indices.push_back(vstart + 0);
			m_Indices.push_back(vstart + i);
			m_Indices.push_back(vstart + i + 1);
		}

		uint32_t indexCount = (count - 2) * 3;

		m_CommandBuffer->drawElements(
			GL_TRIANGLES,
			indexCount,
			GL_UNSIGNED_INT,
			istart * sizeof(uint32_t)
		);
	}

}