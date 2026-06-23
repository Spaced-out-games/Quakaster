#pragma once

#include <HDescriptorSet.h>
#include <HShader.h>
#include <Result.h>
#include <core.h>

namespace qk::gfx::rhi {

    // Material recipe
    struct Material_recipe
    {
        str vertName, fragName;

        str vertSrc, fragSrc;

        DescriptorSet_createinfo set_recipe;

        Material_recipe(const str& vertSrc, const str& fragSrc,
                        const DescriptorSet_createinfo& setRecipe = {}
        );

        Material_recipe() = default;
    };

    // Material implementation
    struct Material_impl;

    // Material handle
    using RMaterial = Material_impl*;

    // Material constructor
    Result<RMaterial, str>
    Material_ctor(const Material_recipe& MatInfo);

    // Material destructor
    void Material_dtor(RMaterial& hMaterial);

    // bind material
    bool Material_bind(const RMaterial& hMaterial);

    // Material handle validity check
    bool Material_valid(const RMaterial& hMaterial);

}