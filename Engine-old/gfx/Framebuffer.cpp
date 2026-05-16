#include "FrameBuffer.h"
#include "GL\glew.h"

namespace qk::gfx {

    void FrameBuffer::init() {
        if (m_FrameBuffer != NULL_HANDLE) return;

        glGenFramebuffers(1, &m_FrameBuffer);
    }

    void FrameBuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    }
    void FrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    bool FrameBuffer::attach(Texture& texture, unsigned int slot) {
        bind();
        texture.bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture.handle(), 0);
        GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, DrawBuffers);

        bool ok = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        return ok;
    }

    Handle FrameBuffer::handle() const {
        return m_FrameBuffer;
    }


}