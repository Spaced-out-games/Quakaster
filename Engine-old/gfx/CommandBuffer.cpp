#include "CommandBuffer.h"

namespace qk::gfx {

	void CommandBuffer::bindVertexArray(const VertexArray& vertexarray) {

		auto& command = m_CommandBuffer.emplace_back();
		command.type = ECommandType::BIND_VERTEXARRAY;
		command.handles[0] = vertexarray.handle();

	}

	void CommandBuffer::bindTexture(gfx::Handle handle, TextureType type, unsigned int slot) {

		auto& command = m_CommandBuffer.emplace_back();
		command.type = ECommandType::BIND_TEXTURE;
		command.handles[0] = handle;
		command.handles[1] = type;
		command.handles[2] = slot;

	}

	void CommandBuffer::bindShaderProgram(const ShaderProgram& program) {

		auto& command = m_CommandBuffer.emplace_back();
		command.type = ECommandType::BIND_SHADER;
		command.handles[0] = program.handle();

	}
	void CommandBuffer::bindFramebuffer(const FrameBuffer& framebuffer) {
		auto& command = m_CommandBuffer.emplace_back();
		command.type = ECommandType::BIND_FRAMEBUFFER;
		command.handles[0] = framebuffer.handle();
	}

	void CommandBuffer::bindFramebuffer(gfx::Handle handle) {
		auto& command = m_CommandBuffer.emplace_back();
		command.type = ECommandType::BIND_FRAMEBUFFER;
		command.handles[0] = handle;
	}

	void CommandBuffer::drawVertexArray(PrimitiveType primitive_t, unsigned int first, unsigned int count) {

		auto& command = m_CommandBuffer.emplace_back();
		command.type = ECommandType::DRAW_VERTEXARRAY;
		command.handles[0] = primitive_t;
		command.handles[1] = first;
		command.handles[2] = count;

	}
	void CommandBuffer::drawElements(PrimitiveType primitive_t, unsigned int count, VertexAttribute type, uint32_t offsetBytes) {
		auto& command = m_CommandBuffer.emplace_back();
		command.type = ECommandType::DRAW_ELEMENTS;
		command.handles[0] = primitive_t;
		command.handles[1] = count;
		command.handles[2] = type;
		command.handles[3] = offsetBytes;
	}
	void CommandBuffer::setUniformi(unsigned int location, int value) {
		auto& command = m_CommandBuffer.emplace_back();
		command.type = ECommandType::SET_UNIFORM_INT;
		command.handles[0] = location;
		command.handles[1] = std::bit_cast<unsigned int>(value);

	}



	void CommandBuffer::setViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
		auto& command = m_CommandBuffer.emplace_back();
		command.type = ECommandType::SET_VIEWPORT;
		command.handles[0] = x;
		command.handles[1] = y;
		command.handles[2] = w;
		command.handles[3] = h;
	}

	void CommandBuffer::clear(unsigned int flags, float r, float g, float b, float a) {
		auto& command = m_CommandBuffer.emplace_back();
		command.type = ECommandType::CLEAR;
		command.handles[0] = flags;
		command.handles[1] = std::bit_cast<unsigned int>(r);
		command.handles[2] = std::bit_cast<unsigned int>(g);
		command.handles[3] = std::bit_cast<unsigned int>(b);
		command.handles[4] = std::bit_cast<unsigned int>(a);
	}
	void CommandBuffer::drawArraysInstanced(PrimitiveType primitive_t, unsigned int first, unsigned int num_vertices, unsigned int num_instances) {
		auto& command = m_CommandBuffer.emplace_back();
		command.type = ECommandType::DRAW_ARRAYS_INSTANCED;
		command.handles[0] = primitive_t;
		command.handles[1] = first;
		command.handles[2] = num_vertices;
		command.handles[3] = num_instances;


	}


	size_t CommandBuffer::size() const {
		return m_CommandBuffer.size();
	}
	const DrawCommand& CommandBuffer::at(size_t index) const {
		return m_CommandBuffer.at(index);
	}

	void CommandBuffer::reset() {
		m_CommandBuffer.clear();
	}



	void call(const CommandBuffer& commandBuffer) {
		if (commandBuffer.size() == 0) return;
		for (size_t i = 0; i < commandBuffer.size(); i++) {
			call(commandBuffer.at(i));
		}
	}



}