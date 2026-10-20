#include "HTexture.h"

#include <glad/glad.h>
//#include <Resource.h>
#include "rhi_globals.h"
#include <spdlog/spdlog.h>

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

    Texture_impl::Texture_impl(const Texture_storage& storage):
        Resource("Texture_impl"),
        type(storage.type),
        storage_format(storage.format)
    {
        GLenum target = TextureType_cast(type);

        u32 mip_levels = storage.mip_levels;

        glGenTextures(1, &texture);
        assert(texture != 0);

        glBindTexture(target, texture);

        if (mip_levels == 0) {
            mip_levels = 1;

            u32 size = std::max({
                storage.width,
                storage.height,
                storage.depth
                });

            while (size > 1) {
                size >>= 1;
                ++mip_levels;
            }
        }

        
        
        switch (type)
        {
        case TextureType::TEX_1D:
            glTexImage1D(
                GL_TEXTURE_1D,
                0,
                TextureStorageFormat_cast(storage_format),
                storage.width,
                0,
                TextureSourceFormat_cast(storage_format),
                GL_UNSIGNED_BYTE,
                nullptr
            );
            break;

        case TextureType::TEX_2D:
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                TextureStorageFormat_cast(storage_format),
                storage.width,
                storage.height,
                0,
                TextureSourceFormat_cast(storage_format),
                GL_UNSIGNED_BYTE,
                nullptr
            );
            break;

        case TextureType::TEX_CUBEMAP:
            for (u32 face = 0; face < 6; ++face)
            {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                    0,
                    TextureStorageFormat_cast(storage_format),
                    storage.width,
                    storage.height,
                    0,
                    TextureSourceFormat_cast(storage_format),
                    GL_UNSIGNED_BYTE,
                    nullptr
                );
            }
            break;

        case TextureType::TEX_3D:
            glTexImage3D(
                GL_TEXTURE_3D,
                0,
                TextureStorageFormat_cast(storage_format),
                storage.width,
                storage.height,
                storage.depth,
                0,
                TextureSourceFormat_cast(storage_format),
                GL_UNSIGNED_BYTE,
                nullptr
            );
            break;

        case TextureType::TEX_1D_ARRAY:
            glTexImage2D(
                GL_TEXTURE_1D_ARRAY,
                0,
                TextureStorageFormat_cast(storage_format),
                storage.width,
                storage.array_layers,
                0,
                TextureSourceFormat_cast(storage_format),
                GL_UNSIGNED_BYTE,
                nullptr
            );
            break;

        case TextureType::TEX_2D_ARRAY:
            glTexImage3D(
                GL_TEXTURE_2D_ARRAY,
                0,
                TextureStorageFormat_cast(storage_format),
                storage.width,
                storage.height,
                storage.array_layers,
                0,
                TextureSourceFormat_cast(storage_format),
                GL_UNSIGNED_BYTE,
                nullptr
            );
            break;
        }


        
    }






    void Texture_impl::upload(const Texture_source& source)
    {
        spdlog::info("Texture_impl @{} upload: format = {}, width = {}, height = {}, mip = {}",
            (const void*)this,
            to_string(source.format),
            source.width,
            source.height,
            source.mip_level
        );


        assert(texture != 0);
        assert(source.pData != nullptr);

        GLenum target = TextureType_cast(type);
        GLenum source_format = TextureSourceFormat_cast(source.format);

        glBindTexture(target, texture);

        switch (type)
        {
        case TextureType::TEX_1D:
            glTexSubImage1D(
                target,
                source.mip_level,
                0,
                source.width,
                source_format,
                GL_UNSIGNED_BYTE,
                source.pData
            );
            break;

        case TextureType::TEX_2D:
            glTexSubImage2D(
                target,
                source.mip_level,
                0, 0,
                source.width,
                source.height,
                source_format,
                GL_UNSIGNED_BYTE,
                source.pData
            );
            break;

        case TextureType::TEX_3D:
            glTexSubImage3D(
                target,
                source.mip_level,
                0, 0, 0,
                source.width,
                source.height,
                source.depth,
                source_format,
                GL_UNSIGNED_BYTE,
                source.pData
            );
            break;

        default:
            assert(false);
            break;
        }

        glBindTexture(target, 0);
    }

    void Texture_impl::set_sampling_parameters(const Sampler_recipe& recipe) {
        assert(texture != 0);

        spdlog::info("Texture_impl @{} sampler: min filter = {}, mag filter = {}, wrap_s = {}, wrap_r = {}, wrap_t = {}",
            (const void*)this,
            to_string(recipe.min_filter),
            to_string(recipe.mag_filter),
            to_string(recipe.wrap_s),
            to_string(recipe.wrap_r),
            to_string(recipe.wrap_t)
        );

        GLenum target = TextureType_cast(type);
        glBindTexture(target, texture);

        // min filter
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, TextureFilter_cast(recipe.min_filter));

        // mag filter
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, TextureFilter_cast(recipe.mag_filter));

        // wrap S
        glTexParameteri(target, GL_TEXTURE_WRAP_S, TextureWrap_cast(recipe.wrap_s));

        // wrap T
        glTexParameteri(target, GL_TEXTURE_WRAP_T, TextureWrap_cast(recipe.wrap_t));

        // wrap R
        glTexParameteri(target, GL_TEXTURE_WRAP_R, TextureWrap_cast(recipe.wrap_r));

    }
   
    HTexture Texture_ctor(const Texture_storage& storage) {
        return new Texture_impl(storage);
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

    void Texture_upload(const HTexture& hTex, const Texture_source& source) {
        assert(hTex);
        hTex->upload(source);
    }

    void Texture_setSamplingParameters(const HTexture& hTex, const Sampler_recipe& recipe) {
        assert(hTex);
        hTex->set_sampling_parameters(recipe);
    }





    

}