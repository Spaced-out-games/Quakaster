#include "Texture.h"
#include <cassert>
#include "GL/glew.h"

using namespace qk::resource;


namespace qk::gfx {
	void Texture::init(Image& img, TextureType type) {
		assert(!m_Handle);
		m_Width = img.width();
		m_Height = img.height();



		m_Type = type;

		glGenTextures(1, &m_Handle);

		bind();


		GLenum format = GL_RGB;

		switch (img.channels()) {
		case 1: format = GL_RED; break;
		case 3: format = GL_RGB; break;
		case 4: format = GL_RGBA; break;
		}

		// I plan to make a switch-case on type eventually, as well as switch-case on channel count
		glTexImage2D(
			m_Type,
			0,
			format,
			img.width(),
			img.height(),
			0,
			format,
			GL_UNSIGNED_BYTE,
			img.data()
		);
		glTexParameteri(m_Type, GL_TEXTURE_MAG_FILTER, filter_mode);
		glTexParameteri(m_Type, GL_TEXTURE_MIN_FILTER, filter_mode);

	}

	void Texture::bind() {
		glBindTexture(m_Type, m_Handle);
	}

	Handle Texture::handle() const {
		return m_Handle;
	}

	TextureType Texture::type() const {
		return m_Type;
	}

	Texture::~Texture() {
		destroy();
	}


	void Texture::destroy() {
		if (m_Handle == NULL_HANDLE) return;
		glDeleteTextures(1, &m_Handle);
		m_Handle = NULL_HANDLE;
	}

	uint32_t Texture::width() const {
		return m_Width;
	}
	uint32_t Texture::height() const {
		return m_Height;
	}

}