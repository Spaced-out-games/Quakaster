#pragma once

#include <rhi_Resource.h>
#include <Result.h>
#include <core.h>

namespace qk::gfx::rhi {

    enum TextureType : u32 {
        TEX_1D,
        TEX_2D,
        TEX_3D,
        TEX_CUBEMAP,
        TEX_1D_ARRAY,
        TEX_2D_ARRAY,
    };

    enum TextureFilterMode : u32 {
        TEXEL,
        BILINEAR
    };

    struct Texture_impl;
    using HTexture = Texture_impl*;

    Result<HTexture, str> Texture_ctor(
        TextureType type = TEX_2D);

    void Texture_upload_2D(
        const HTexture& hTex,
        const u8* pData,
        int width,
        int height,
        int channels);

    bool Texture_bind(
        const HTexture& hTex,
        uint8_t slot);

    bool Texture_valid(
        const HTexture& hTex);

    bool Texture_dtor(
        HTexture& hTex);


}