#pragma once
#include <vector>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "../core.h"


#include "VertexBufferLayout.h"
#include "ShaderProgram.h"
#include "CommandBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"




namespace qk::gfx {
	struct QK_API Renderer2D {
	private:
		// vertex type
		struct Vertex {
			glm::vec2 position = { 0.0,0.0 };
			glm::vec2 uv = { 0.0,0.0 };
			glm::vec3 color = { 1.0, 1.0, 1.0 }; // supports transparency I guess
		};

		struct PrimitiveDrawCall {
			gfx::Handle texture = gfx::NULL_HANDLE;
			uint32_t startVertex = 0;
			uint32_t vertexCount = 0;
			uint32_t startIndex = 0;
			uint32_t indexCount = 0;
		};



		// storages
		std::vector<Vertex>				m_Vertices;
		std::vector<uint32_t>			m_Indices;


		gfx::VertexArray				m_VertexArray;
		gfx::VertexBuffer<Vertex>		m_VertexBuffer;
		gfx::IndexBuffer<uint32_t>		m_IndexBuffer;



		// shaders and textures
		gfx::ShaderProgram				m_Shader;
		gfx::Texture					m_WhiteTex;
		gfx::Handle						m_TextureUniform = NULL_HANDLE;

		// state
		CommandBuffer* m_CommandBuffer = nullptr;

	public:

		// here for testing

		// lifetime

		Renderer2D() = default;
		~Renderer2D() = default;
		void init();
		void destroy();

		// returns 0 on success
		int begin(CommandBuffer* pCommandBuffer);
		void end();

		// getters
		uint32_t vertex_count() const;
		uint32_t index_count() const;


		inline void bind_texture(Texture& target, unsigned int slot);

		inline void draw_triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec3 color);
		inline void draw_rect(glm::vec2 pos, glm::vec2 size, glm::vec3 color);
		inline void draw_polygon(const glm::vec2* points, size_t count, glm::vec3 color);
	};
}