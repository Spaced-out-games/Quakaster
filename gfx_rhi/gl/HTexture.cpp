#include "HTexture.h"

#include <glad/glad.h>
//#include <Resource.h>
#include "rhi_globals.h"

namespace qk::gfx::rhi {

    bool Texture_impl::destroy() {
        if (!texture) return false;
        glDeleteTextures(1, &texture);

        return true;

    }




    Result<HTexture, str>
    Texture_ctor(str_view label, TextureType type)
    {
        GLuint gl_tex = 0;

        glGenTextures(1, &gl_tex);

        if (!gl_tex)
            return str{ "glGenTextures failed" };


        auto* pTex = new Texture_impl();

        pTex->type = type;
        pTex->texture = gl_tex;

        return g_Textures.insert(label, pTex);
    }

    void Texture_upload_2D(const HTexture& hTex, const u8* pData,
        int width, int height, int channels) {

        if (!hTex) return;

        GLenum format = Channel_cast(channels);

        glBindTexture(
            GL_TEXTURE_2D,
            hTex->texture
        );

        glTexImage2D(
            GL_TEXTURE_2D, 0, format,
            width, height, 0, format,
            GL_UNSIGNED_BYTE, pData
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_NEAREST);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_S,
            GL_REPEAT
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T,
            GL_REPEAT
        );
    }

    bool Texture_bind(const HTexture& hTex, uint8_t slot)
    {
        if (!hTex) return false;

        if (!g_Textures.valid(hTex)) return false;

        glActiveTexture(GL_TEXTURE0 + slot);

        glBindTexture(
            TextureType_cast(hTex->type),
            hTex->texture
        );

        return true;
    }

    bool Texture_valid(const HTexture& hTex)
    {
        return hTex && g_Textures.valid(hTex);
    }

    bool Texture_dtor(HTexture& hTex)
    {
        if (!hTex) return false;

        glDeleteTextures(1, &hTex->texture);

        g_Textures.destroy(hTex);

        hTex = nullptr;

        return true;
    }
    HTexture Texture_resolve(str_view label)
    {
        return g_Textures.find(label);
    }
}