#pragma once
#include "Resource.h"
#include <Result.h>
#include <core.h>

namespace qk::gfx::rhi {

    // Texture type
    enum class TextureType : u32 {
        TEX_1D,
        TEX_2D,
        TEX_3D,
        TEX_CUBEMAP,
        TEX_1D_ARRAY,
        TEX_2D_ARRAY,
    };


    // what format a texture is represented as
    enum class TextureFormat {
        R,
        RG,
        RGB,
        RGB_8,
        RGBA,
        RGBA_8
    };

    // How the texture is used
    enum class TextureUsage : u32 {
        None = 0,
        Sampled = 1 << 0,
        Storage = 1 << 1,
        ColorAttachment = 1 << 2,
        DepthStencilAttachment = 1 << 3,
        TransferSrc = 1 << 4,
        TransferDst = 1 << 5,
    };
    
    // how the texture handles UVs > 1.0
    enum class TextureWrap {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
    };

    // how the texture is filtered
    enum class TextureFilter {
        Nearest,
        Linear,

        NearestMipmapNearest,
        LinearMipmapNearest,

        NearestMipmapLinear,
        LinearMipmapLinear,
    };

    // TextureFormat string conversion
    inline str_view to_string(TextureFormat format) {
        switch (format)
        {
        case qk::gfx::rhi::TextureFormat::R:
            return "R";
            break;
        case qk::gfx::rhi::TextureFormat::RG:
            return "RG";
            break;
        case qk::gfx::rhi::TextureFormat::RGB:
            return "RGB";
            break;
        case qk::gfx::rhi::TextureFormat::RGB_8:
            return "RGB-8";
            break;
        case qk::gfx::rhi::TextureFormat::RGBA:
            return "RBGA";
            break;
        case qk::gfx::rhi::TextureFormat::RGBA_8:
            return "RBGA-u8";
            break;
        default:
            break;
        }
    }

    // TextureWrap string conversion
    inline str_view to_string(TextureWrap wrap) {
        switch (wrap)
        {
        case qk::gfx::rhi::TextureWrap::Repeat:
            return "repeat";
            break;
        case qk::gfx::rhi::TextureWrap::MirroredRepeat:
            return "mirrored-repeat";
            break;
        case qk::gfx::rhi::TextureWrap::ClampToEdge:
            return "clamp to edge";
            break;
        case qk::gfx::rhi::TextureWrap::ClampToBorder:
            return "clamp to border";
            break;
        default:
            break;
        }
    }

    // TextureFilter string conversion
    inline str_view to_string(TextureFilter filter) {
        switch (filter)
        {
        case qk::gfx::rhi::TextureFilter::Nearest:
            return "nearest";
            break;
        case qk::gfx::rhi::TextureFilter::Linear:
            return "linear";
            break;
        case qk::gfx::rhi::TextureFilter::NearestMipmapNearest:
            return "nearest mipmap - nearest";
            break;
        case qk::gfx::rhi::TextureFilter::LinearMipmapNearest:
            return "linear mipmap - nearest";
            break;
        case qk::gfx::rhi::TextureFilter::NearestMipmapLinear:
            return "nearest mipmap - linear";
            break;
        case qk::gfx::rhi::TextureFilter::LinearMipmapLinear:
            return "linear mipmap - linear";
            break;
        default:
            break;
        }
    }

    // Sampler createinfo. NOTE: Consider moving to HSampler.h when you get the chance
    struct Sampler_recipe {
        TextureFilter min_filter = TextureFilter::Linear;
        TextureFilter mag_filter = TextureFilter::Linear;

        TextureWrap wrap_s = TextureWrap::Repeat;
        TextureWrap wrap_t = TextureWrap::Repeat;
        TextureWrap wrap_r = TextureWrap::Repeat;
    };

    // Represents the source texture buffer's layout
    struct Texture_source {
        const void* pData = nullptr;

        TextureFormat format = TextureFormat::RGB_8;

        u32 mip_level = 0;
        u32 array_layer = 0;

        u32 width = 1024;
        u32 height = 1024;
        u32 depth = 0;
    };

    
    // Represents the GPU-sided texture buffer's layout 
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


    
    struct Texture_impl;
    using HTexture = Texture_impl*;



    // Lifetime
    HTexture Texture_ctor(const Texture_storage& recipe);
    void Texture_dtor(const HTexture& hTex);
    void Texture_free(HTexture& hTex);
    Resource* Resource_cast(HTexture& hTex);


    // Upload a buffer to the GPU
    void Texture_upload(const HTexture& hTex, const Texture_source& source);

    // Set texture sampling state
    void Texture_setSamplingParameters(const HTexture& hTex, const Sampler_recipe& recipe);



    using TextureRegistry = ResourceRegistry<Texture_impl, Texture_dtor, Texture_free>;

}