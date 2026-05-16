#pragma once
#include <vector>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#include "../core/utility/Service.h"
#include "../core/io/Layer.h"
#include "../core.h"


#include "../gfx/VertexBufferLayout.h"
#include "../gfx/ShaderProgram.h"
#include "../gfx/CommandBuffer.h"
#include "../gfx/VertexBuffer.h"
#include "../gfx/VertexArray.h"
#include "../gfx/IndexBuffer.h"
#include "../gfx/Texture.h"
#include "../gfx/Shader.h"


using namespace qk::io;


namespace qk::gfx {
	struct QK_API c2d : util::IService, public Layer {
		private:
			// vertex type
			struct Vertex {
				glm::vec2 position = { 0.0,0.0 };
				glm::vec2 uv = { 0.0,0.0 };
				glm::vec4 color = { 1.0, 1.0, 1.0, 1.0 }; // supports transparency I guess
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
			std::vector<PrimitiveDrawCall>	m_DrawCalls;


			gfx::VertexArray				m_VertexArray;


			// buffers
			gfx::VertexBuffer<Vertex>		m_VertexBuffer;
			gfx::IndexBuffer<uint32_t>		m_IndexBuffer;

			// shaders and textures
			gfx::ShaderProgram				m_Shader;
			gfx::Handle m_UniformLocation = NULL_HANDLE;
			gfx::Texture					m_WhiteTex;

			// state
			uint32_t						m_PrimitiveStartVertex = 0;
			uint32_t						m_PrimitiveStartIndex = 0;
			bool							m_primitiveActive = false;
			gfx::Handle m_LastBoundTexture = gfx::NULL_HANDLE;
			// command buffer

		public:
			size_t							m_LastDrawCallIndex = 0;

			// here for testing
			gfx::CommandBuffer* m_CommandBuffer = nullptr;

			// lifetime

			c2d();
			~c2d();
			void init(size_t reserve = 1024);
			void destroy();

			// state management
			void clear();
			void rebuild_partial();
			void rebuild();


			// overides
			EBlock on_event(const Event& evt) override;
			void on_render() override;
			void on_attach() override;
			void on_detach() override;

			// getters
			uint32_t vertex_count() const;
			uint32_t index_count() const; 



			// constructing primitives from scratch
			void primitive_begin(gfx::Handle texture = gfx::NULL_HANDLE);
			void primitive_end();
			void add_vertex(glm::vec2 position, glm::vec2 uv, glm::vec4 color);
			void add_index(uint32_t index);

			// helper methods to build primitives

			inline void draw_triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec4 color);
			inline void draw_rect(glm::vec2 pos, glm::vec2 size, glm::vec4 color);
			inline void draw_image(gfx::Handle texture, glm::vec2 pos, glm::vec2 size);
			inline void draw_polygon(const glm::vec2* points, size_t count, glm::vec4 color);
	};
}