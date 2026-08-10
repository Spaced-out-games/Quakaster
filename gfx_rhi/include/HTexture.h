#pragma once
#include "Resource.h"
#include <Result.h>
#include <core.h>

namespace qk::gfx::rhi {

    enum class TextureType : u32 {
        TEX_1D,
        TEX_2D,
        TEX_3D,
        TEX_CUBEMAP,
        TEX_1D_ARRAY,
        TEX_2D_ARRAY,
    };



    enum class TextureFormat {
        R,
        RG,
        RGB,
        RGB_8,
        RGBA,
        RGBA_8
    };

    enum class TextureUsage : u32 {
        None = 0,
        Sampled = 1 << 0,
        Storage = 1 << 1,
        ColorAttachment = 1 << 2,
        DepthStencilAttachment = 1 << 3,
        TransferSrc = 1 << 4,
        TransferDst = 1 << 5,
    };

    enum class TextureWrap {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
    };

    enum class TextureFilter {
        Nearest,
        Linear,

        NearestMipmapNearest,
        LinearMipmapNearest,

        NearestMipmapLinear,
        LinearMipmapLinear,
    };

    struct Sampler_recipe {
        TextureFilter min_filter = TextureFilter::Linear;
        TextureFilter mag_filter = TextureFilter::Linear;

        TextureWrap wrap_s = TextureWrap::Repeat;
        TextureWrap wrap_t = TextureWrap::Repeat;
        TextureWrap wrap_r = TextureWrap::Repeat;
    };

    struct Texture_source {
        const void* pData = nullptr;

        TextureFormat format = TextureFormat::RGB_8;

        u32 mip_level = 0;
        u32 array_layer = 0;

        u32 width = 1024;
        u32 height = 1024;
        u32 depth = 0;
    };

    struct Texture_storage {
        TextureType type = TextureType::TEX_2D;

        TextureFormat format = TextureFormat::RGB_8;

        u32 width = 1024;
        u32 height = 1024;
        u32 depth = 0;

        u32 mip_levels = 0;
        u32 array_layers = 0;

        TextureUsage usage = TextureUsage::Sampled;
    };

    struct Texture_recipe {
        Texture_storage storage;
        Texture_source source;
    };



    struct Texture_impl;
    using HTexture = Texture_impl*;

    Resource* Resource_cast(HTexture& hTex);

    HTexture Texture_ctor(const Texture_storage& recipe);

    void Texture_dtor(const HTexture& hTex);

    void Texture_free(HTexture& hTex);

    void Texture_upload(const HTexture& hTex, const Texture_source& source);

    void Texture_setSamplingParameters(const HTexture& hTex, const Sampler_recipe& recipe);


    using TextureRegistry = ResourceRegistry<Texture_impl, Texture_dtor, Texture_free>;


}