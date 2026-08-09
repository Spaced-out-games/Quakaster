#include "HTexture.h"

#include <glad/glad.h>
//#include <Resource.h>
#include "rhi_globals.h"

namespace qk::gfx::rhi {

    void Texture_impl::destroy() {
        if (!texture) return;
        glDeleteTextures(1, &texture);
        texture = 0;
        return;

    }
    void Texture_impl::bind(u8 slot) const {
        assert(texture);
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    Texture_impl::Texture_impl(TextureType type): type(type), Resource("Texture_impl") {
        glGenTextures(1, &texture);

        assert(texture != 0);
    }

    void Texture_impl::upload_2D(const u8* pData, int width, int height, int channels) {
        assert(texture && type == TextureType::TEX_2D);
        GLenum format = Channel_cast(channels);
        glBindTexture(
            GL_TEXTURE_2D,
            texture
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





    Result<HTexture, str>
    Texture_ctor(TextureType type)
    {
        return new Texture_impl(type);

    }

    Resource* Resource_cast(HTexture& hTex) {
        return static_cast<Resource*>(hTex);
    }

    void Texture_dtor(const HTexture& hTex)
    {
        assert(hTex);
        assert(hTex->refcount() == 0 && hTex->flagged());

        hTex->destroy();

        return;

    }

    void Texture_free(HTexture& hTex) {

        assert(hTex);
        assert(hTex->refcount() == 0 && hTex->flagged());

        delete hTex;

        hTex = nullptr;
    }

    void Texture_upload_2D(const HTexture& hTex, const u8* pData,
        int width, int height, int channels) {

        assert(hTex);

        hTex->upload_2D(pData, width, height, channels);
        
    }





    

}