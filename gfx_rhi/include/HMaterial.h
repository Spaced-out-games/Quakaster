#pragma once

#include <HDescriptorSet.h>
#include <HShader.h>
#include <Result.h>
#include <core.h>

namespace qk::gfx::rhi {

    struct Material_recipe
    {
        str matName, vertName, fragName;

        str vertSrc, fragSrc;

        DescriptorSet_createinfo set_recipe;

        Material_recipe(
            const str& name,
            const str& vertName,
            const str& vertSrc,
            const str& fragName,
            const str& fragSrc,
            const DescriptorSet_createinfo& setRecipe = {});

        Material_recipe(
            const str& vertSrc,
            const str& fragSrc);

        Material_recipe() = default;
    };

    struct Material_impl;

    using RMaterial = Material_impl*;

    Result<RMaterial, str>
        Material_ctor(const Material_recipe& MatInfo);

    void Material_dtor(RMaterial& hMaterial);

    bool Material_bind(const RMaterial& hMaterial);

    bool Material_valid(const RMaterial& hMaterial);

}