// FrameBuffer.h

#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>

class FrameBuffer {
public:
    FrameBuffer();
    ~FrameBuffer();

    void bind();
    void unbind();
    void setup(unsigned int width, unsigned int height);
    GLuint getTexture() const { return texture; }
    GLuint getID() const { return framebuffer; }
    GLuint framebuffer;
    GLuint texture;
    GLuint depthBuffer;
private:
    
    unsigned int mWidth, mHeight;

    
};
