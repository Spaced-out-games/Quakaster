
/*
#pragma once
#include <gl/GL.h>
#include <string>
#include <stdexcept>

class FBO
{
public:
    static constexpr int MAX_COLOR_ATTACHMENTS = 8;

    FBO(int width, int height)
        : width(width), height(height), fbo(0), depthBuffer(0), numColorAttachments(0) {}

    ~FBO()
    {
        if (fbo) glDeleteFramebuffers(1, &fbo);
        if (depthBuffer) glDeleteRenderbuffers(1, &depthBuffer);
        for (int i = 0; i < numColorAttachments; ++i)
        {
            if (colorAttachments[i]) glDeleteTextures(1, &colorAttachments[i]);
        }
    }

    void init(bool useDepthBuffer = true)
    {
        // Generate and bind the FBO
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Create color attachments
        for (int i = 0; i < numColorAttachments; ++i)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorAttachments[i], 0);
        }

        // Create depth buffer if requested
        if (useDepthBuffer)
        {
            glGenRenderbuffers(1, &depthBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
        }

        // Check framebuffer completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error("Framebuffer is not complete!");
        }

        // Unbind the FBO
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    static void unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void add_attachment(GLenum format, GLenum type)
    {
        if (numColorAttachments >= MAX_COLOR_ATTACHMENTS)
        {
            throw std::runtime_error("Maximum number of color attachments reached!");
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, type, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        colorAttachments[numColorAttachments] = texture;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + numColorAttachments, GL_TEXTURE_2D, texture, 0);
        ++numColorAttachments;

        // Check framebuffer completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error("Framebuffer is not complete after adding attachment!");
        }
    }

    GLuint getColorAttachment(int index) const
    {
        if (index < 0 || index >= numColorAttachments)
            throw std::out_of_range("Invalid color attachment index.");
        return colorAttachments[index];
    }

    GLuint getDepthBuffer() const { return depthBuffer; }

    void resize(int newWidth, int newHeight) 
    {
        if (newWidth == width && newHeight == height) return;

        width = newWidth;
        height = newHeight;

        // Delete current attachments
        for (int i = 0; i < numColorAttachments; ++i)
        {
            if (colorAttachments[i]) glDeleteTextures(1, &colorAttachments[i]);
        }
        if (depthBuffer) glDeleteRenderbuffers(1, &depthBuffer);

        // Reinitialize the FBO
        init(depthBuffer != 0);
    }

private:
    int width;
    int height;
    GLuint fbo;
    GLuint depthBuffer;
    GLuint colorAttachments[MAX_COLOR_ATTACHMENTS] = { 0 }; // Array of color attachments
    int numColorAttachments; // Number of active color attachments
};
*/