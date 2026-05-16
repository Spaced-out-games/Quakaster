#include "c2d.h"

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





    void c2d::init(size_t reserve) {
        m_Vertices.reserve(reserve);
        m_Indices.reserve(reserve);
        m_DrawCalls.reserve(reserve);

        // initialize everything
        m_VertexArray.init();
        m_VertexArray.bind();

        m_VertexBuffer.init();
        m_VertexBuffer.bind();

        m_IndexBuffer.init();
        m_IndexBuffer.bind();

        // set up the vertex attributes
        gfx::VertexBufferLayout layout;

        layout.push<float>(2);
        layout.push<float>(2);
        layout.push<float>(4);
        m_VertexArray.apply(m_VertexBuffer, layout);

        gfx::Shader vert(vert_src, GL_VERTEX_SHADER);
        gfx::Shader frag(frag_src, GL_FRAGMENT_SHADER);

        m_Shader.init(frag.compile(), vert.compile());
        m_Shader.bind();
        m_UniformLocation = glGetUniformLocation(m_Shader.handle(), "uTexture");

        

        Image img;
        make_image(img, 1, 1, 4);
        auto* pPixel = (uint32_t*)(img.data());

        *pPixel = (uint32_t)0xffffffff;

        m_WhiteTex.init(img, GL_TEXTURE_2D);

    }
    void c2d::destroy() {
        m_Shader.destroy();
        m_WhiteTex.destroy();
        m_VertexBuffer.destroy();
        m_IndexBuffer.destroy();
        m_VertexArray.destroy();
    }

    c2d::c2d() {
    }

    c2d::~c2d() {
        destroy();
    }

	void c2d::clear() {
		m_Vertices.clear();
		m_Indices.clear();
		m_DrawCalls.clear();
		m_PrimitiveStartVertex = 0;
		m_PrimitiveStartIndex = 0;
	}

	uint32_t c2d::vertex_count() const {
		return m_Vertices.size();
	}

	uint32_t c2d::index_count() const {
		return m_Indices.size();
	}

	void c2d::primitive_begin(gfx::Handle texture) {
		m_DrawCalls.emplace_back(
			texture,
			m_Vertices.size(),
			0,
			m_Indices.size(),
			0
		);
	}

	void c2d::add_vertex(glm::vec2 position, glm::vec2 uv, glm::vec4 color) {
		m_Vertices.emplace_back(position, uv, color);
	}

	void c2d::add_index(uint32_t index) {
		m_Indices.emplace_back(index);
	}

	void c2d::primitive_end() {
		PrimitiveDrawCall& target = m_DrawCalls.back();
		target.indexCount = m_Indices.size() - target.startIndex;
		target.vertexCount = m_Vertices.size() - target.startVertex;
	}

    void c2d::draw_triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec4 color) {
        primitive_begin();

        size_t start = vertex_count();

        add_vertex(a, { 0,0 }, color);
        add_vertex(b, { 0,0 }, color);
        add_vertex(c, { 0,0 }, color);

        add_index(start + 0);
        add_index(start + 1);
        add_index(start + 2);

        primitive_end();

        rebuild_partial();
    }


    void c2d::draw_rect(glm::vec2 pos, glm::vec2 size, glm::vec4 color)
    {
        primitive_begin();

        size_t start = vertex_count();

        glm::vec2 p0 = pos;
        glm::vec2 p1 = pos + glm::vec2(size.x, 0);
        glm::vec2 p2 = pos + size;
        glm::vec2 p3 = pos + glm::vec2(0, size.y);

        add_vertex(p0, { 0,1 }, color);
        add_vertex(p1, { 1,1 }, color);
        add_vertex(p2, { 1,0 }, color);
        add_vertex(p3, { 0,0 }, color);

        add_index(start + 0);
        add_index(start + 1);
        add_index(start + 2);

        add_index(start + 0);
        add_index(start + 2);
        add_index(start + 3);

        primitive_end();

        if (!m_CommandBuffer) return;

        auto& back = m_DrawCalls.back();

        m_CommandBuffer->bindVertexArray(m_VertexArray);
        m_CommandBuffer->bindShaderProgram(m_Shader);
        //m_CommandBuffer->setUniformi(m_UniformLocation, 0);

        m_CommandBuffer->bindTexture(back.texture, GL_TEXTURE_2D, 0);
        m_CommandBuffer->drawElements(
            GL_TRIANGLES,
            back.indexCount,
            GL_UNSIGNED_INT,
            back.startIndex * sizeof(uint32_t)
        );
        //rebuild_partial();

    }

    void c2d::draw_image(gfx::Handle texture, glm::vec2 pos, glm::vec2 size)
    {
        primitive_begin(texture);

        size_t start = vertex_count();

        glm::vec2 p0 = pos;
        glm::vec2 p1 = pos + glm::vec2(size.x, 0);
        glm::vec2 p2 = pos + size;
        glm::vec2 p3 = pos + glm::vec2(0, size.y);

        add_vertex(p0, { 0,0 }, { 1,1,1,1 });
        add_vertex(p1, { 1,0 }, { 1,1,1,1 });
        add_vertex(p2, { 1,1 }, { 1,1,1,1 });
        add_vertex(p3, { 0,1 }, { 1,1,1,1 });

        add_index(start + 0);
        add_index(start + 1);
        add_index(start + 2);

        add_index(start + 0);
        add_index(start + 2);
        add_index(start + 3);

        primitive_end();

        rebuild_partial();

    }

    void c2d::draw_polygon(const glm::vec2* points, size_t count, glm::vec4 color)
    {
        assert(count >= 3 && "Polygon must have at least 3 points");

        primitive_begin();

        size_t start = vertex_count();


        for (size_t i = 0; i < count; ++i)
        {
            add_vertex(points[i], { 0,0 }, color);
        }


        for (size_t i = 1; i < count - 1; ++i)
        {
            add_index(start + 0);
            add_index(start + i);
            add_index(start + i + 1);
        }

        primitive_end();

        rebuild_partial();

    }


    void c2d::rebuild_partial() {
        // nothing to rebuild
        //if (m_LastDrawCallIndex >= m_DrawCalls.size() || !m_CommandBuffer) return;

        // if first time, initialize command buffer
        if (m_LastDrawCallIndex == 0) {
            m_CommandBuffer->reset();
            m_CommandBuffer->bindVertexArray(m_VertexArray);
            m_CommandBuffer->bindShaderProgram(m_Shader);
            m_LastBoundTexture = gfx::NULL_HANDLE;
        }

        // compile only the draw calls that haven't been pushed yet
        for (size_t i = m_LastDrawCallIndex; i < m_DrawCalls.size(); ++i) {
            auto& draw_call = m_DrawCalls[i];

            gfx::Handle tex = (draw_call.texture != gfx::NULL_HANDLE)
                ? draw_call.texture
                : m_WhiteTex.handle();

            if (tex != m_LastBoundTexture) {
                m_LastBoundTexture = tex;
                m_CommandBuffer->bindTexture(tex, GL_TEXTURE_2D, 0);
            }

            m_CommandBuffer->drawElements(
                GL_TRIANGLES,
                draw_call.indexCount,
                GL_UNSIGNED_INT,
                draw_call.startIndex * sizeof(uint32_t)
            );
        }

        // mark all current draw calls as compiled
        m_LastDrawCallIndex = m_DrawCalls.size();
    }



    /* -------------------------------------- overrides -------------------------------------- */

    void c2d::on_detach() {}


    void c2d::on_attach() {


    }


    void c2d::on_render() {

        m_VertexBuffer.bind();
        m_IndexBuffer.bind();

        m_VertexBuffer.upload(
            m_Vertices.data(),
            m_Vertices.size(),
            GL_DYNAMIC_DRAW
        );

        m_IndexBuffer.upload(
            m_Indices.data(),
            m_Indices.size()
        );

        if(m_CommandBuffer) gfx::call(*m_CommandBuffer);
    }

    Layer::EBlock c2d::on_event(const Event& evt) {
        return Layer::EBlock::Pass;
    }

    

    void c2d::rebuild() {
        m_LastDrawCallIndex = 0;
        m_LastBoundTexture = gfx::NULL_HANDLE;
        rebuild_partial(); // just calls from index 0
    }

}