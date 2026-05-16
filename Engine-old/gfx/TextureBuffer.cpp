#include "TextureBuffer.h"
#include "GL/glew.h"

namespace qk::gfx {

	void TextureBuffer::init(unsigned int usage, unsigned int type) {
		m_Usage = usage;
		m_Type = type;

		// Create the buffer
		glGenBuffers(1, &m_Buffer);

		// Create the texture handle through Texture member
		GLuint texHandle;
		glGenTextures(1, &texHandle);
		m_Texture.m_Handle = texHandle;
		m_Texture.m_Type = GL_TEXTURE_BUFFER;

		// Bind texture and attach buffer
		glBindTexture(GL_TEXTURE_BUFFER, m_Texture.handle());
		glTexBuffer(GL_TEXTURE_BUFFER, m_Type, m_Buffer);
	}

	void TextureBuffer::bind_tex(unsigned int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		m_Texture.bind();
	}

	void TextureBuffer::bind_buff() {
		glBindBuffer(GL_TEXTURE_BUFFER, m_Buffer);
	}

	void TextureBuffer::upload(const void* data, size_t SizeBytes) {
		bind_buff();
		glBufferData(GL_TEXTURE_BUFFER, SizeBytes, data, m_Usage);

		glBindTexture(GL_TEXTURE_BUFFER, m_Texture.handle());
		glTexBuffer(GL_TEXTURE_BUFFER, m_Type, m_Buffer);
	}
}