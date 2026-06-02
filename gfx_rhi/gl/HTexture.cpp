#include "HTexture.h"

#include <glad/glad.h>
#include <Resource.h>

namespace qk::gfx::rhi {

    static GLuint TextureType_cast(TextureType type)
    {
        switch (type)
        {
        case TEX_1D:       return GL_TEXTURE_1D;
        case TEX_2D:       return GL_TEXTURE_2D;
        case TEX_3D:       return GL_TEXTURE_3D;
        case TEX_CUBEMAP:  return GL_TEXTURE_CUBE_MAP;
        case TEX_2D_ARRAY: return GL_TEXTURE_2D_ARRAY;
        default:           return 0;
        }
    }

    static GLint TextureFilterMode_cast(
        TextureFilterMode mode)
    {
        switch (mode)
        {
        case TEXEL:    return GL_NEAREST;
        case BILINEAR: return GL_LINEAR;
        default:       return 0;
        }
    }

    static GLenum Channel_cast(int channels)
    {
        QK_ASSERT(channels != 2);

        switch (channels)
        {
        case 1: return GL_RED;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default:return GL_RGB;
        }
    }

    struct Texture_impl : qk::Resource
    {
        TextureType type = TEX_2D;
        GLuint texture = 0;
    };

    static inline qk::ResourceRegistry<Texture_impl>
        g_Textures;

    Result<HTexture, str>
        Texture_ctor(
            str_view label,
            TextureType type)
    {
        GLuint gl_tex = 0;

        glGenTextures(1, &gl_tex);

        if (!gl_tex)
            return str{ "glGenTextures failed" };

        Texture_impl tex;
        tex.type = type;
        tex.texture = gl_tex;

        return g_Textures.insert(
            label,
            std::move(tex));
    }

    void Texture_upload_2D(
        const HTexture& hTex,
        const char* pData,
        int width,
        int height,
        int channels)
    {
        if (!hTex)
            return;

        GLenum format =
            Channel_cast(channels);

        glBindTexture(
            GL_TEXTURE_2D,
            hTex->texture);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            pData);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE);
    }

    bool Texture_bind(
        const HTexture& hTex,
        uint8_t slot)
    {
        if (!hTex)
            return false;

        if (!g_Textures.valid(hTex))
            return false;

        glActiveTexture(
            GL_TEXTURE0 + slot);

        glBindTexture(
            TextureType_cast(
                hTex->type),
            hTex->texture);

        return true;
    }

    bool Texture_valid(
        const HTexture& hTex)
    {
        return hTex &&
            g_Textures.valid(hTex);
    }

    bool Texture_dtor(
        HTexture& hTex)
    {
        if (!hTex)
            return false;

        glDeleteTextures(
            1,
            &hTex->texture);

        g_Textures.destroy(hTex);

        hTex = nullptr;

        return true;
    }
    HTexture Texture_resolve(str_view label)
    {
        return g_Textures.find(label);
    }
}