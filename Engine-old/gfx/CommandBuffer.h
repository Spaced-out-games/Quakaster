#pragma once
#include "ShaderProgram.h"
#include "DrawCommand.h"
#include "CommandBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Framebuffer.h"
#include <vector>
#include "../core.h"
#include <cstdint>

namespace qk::gfx {

	struct CommandBuffer;

	void QK_API call(const CommandBuffer& commandBuffer);


	class QK_API CommandBuffer {
		std::vector<DrawCommand> m_CommandBuffer;
		public:
			void bindVertexArray(const VertexArray& vertexarray);
			void bindTexture(gfx::Handle handle, TextureType type, unsigned int slot);
			void bindShaderProgram(const ShaderProgram& program);
			void bindFramebuffer(gfx::Handle handle);
			void bindFramebuffer(const FrameBuffer& framebuffer);
			void drawVertexArray(PrimitiveType primitive_t, unsigned int first, unsigned int count);
			void drawElements(PrimitiveType primitive_t, unsigned int count, VertexAttribute type, uint32_t offsetBytes);
			void drawArraysInstanced(PrimitiveType primitive_t, unsigned int first, unsigned int num_vertices, unsigned int num_instances);

			void setUniformi(unsigned int location, int value);
			void setViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
			void clear(unsigned int flags, float r, float g, float b, float a);

			size_t size() const;
			const DrawCommand& at(size_t index) const;

			void reset();


	};

}