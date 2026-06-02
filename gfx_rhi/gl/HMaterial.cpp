#include "HMaterial.h"

#include <format>
#include <Resource.h>

namespace qk::gfx::rhi {

    struct Material_impl : qk::Resource
    {
        RShaderProgram program = nullptr;
        HDescriptorSet descriptor_set = nullptr;
    };

    static inline qk::ResourceRegistry<Material_impl>
        g_Materials;

    static inline u32 g_material_index = 0;

    // ---------------- Material_recipe ----------------

    Material_recipe::Material_recipe(
        const str& name,
        const str& vertName,
        const str& vertSrc,
        const str& fragName,
        const str& fragSrc,
        const DescriptorSet_createinfo& setRecipe)
        : matName(name),
        vertName(vertName),
        vertSrc(vertSrc),
        fragName(fragName),
        fragSrc(fragSrc),
        set_recipe(setRecipe)
    {}

    Material_recipe::Material_recipe(
        const str& vertSrc,
        const str& fragSrc)
        : matName(std::format(
            "material_{}",
            g_material_index++)),
        vertName(matName + "::vert"),
        fragName(matName + "::frag"),
        vertSrc(vertSrc),
        fragSrc(fragSrc)
    {}

    // ---------------- internal build ----------------

    static Result<Material_impl, str>
        make_material(const Material_recipe& mat_recipe)
    {
        auto vert =
            Shader_ctor(
                mat_recipe.vertName,
                ShaderType::Vertex);

        if (!vert)
            return vert.error();

        auto vertUpload =
            Shader_upload(
                vert.value(),
                mat_recipe.vertSrc);

        if (!vertUpload.empty())
            return vertUpload;

        auto frag =
            Shader_ctor(mat_recipe.fragName,
                ShaderType::Fragment
                );

        if (!frag)
            return frag.error();

        auto fragUpload =
            Shader_upload(
                frag.value(),
                mat_recipe.fragSrc);

        if (!fragUpload.empty())
            return fragUpload;

        auto program =
            ShaderProgram_ctor(
                mat_recipe.matName,
                frag.value(),
                vert.value(),
                false);

        if (!program)
            return program.error();

        HDescriptorSet set =
            DescriptorSet_ctor(
                mat_recipe.set_recipe);

        Material_impl m;
        m.program = program.value();
        m.descriptor_set = set;

        return m;
    }

    // ---------------- public API ----------------

    Result<RMaterial, str>
        Material_ctor(const Material_recipe& MatInfo)
    {
        auto mat_res = make_material(MatInfo);

        if (!mat_res)
            return mat_res.error();

        auto& mat = mat_res.value();

        if (!ShaderProgram_valid(mat.program))
            return str{ "Invalid shader program" };

        if (!DescriptorSet_valid(mat.descriptor_set))
            return str{ "Invalid descriptor set" };

        DescriptorSet_bindProgram(
            mat.descriptor_set,
            mat.program);

        auto res = g_Materials.insert(
            MatInfo.matName,
            std::move(mat));
        return res;
    }

    void Material_dtor(RMaterial& hMaterial)
    {
        if (!hMaterial)
            return;

        g_Materials.destroy(hMaterial);
        hMaterial = nullptr;
    }

    bool Material_bind(const RMaterial& hMaterial)
    {
        if (!hMaterial)
            return false;

        if (!g_Materials.valid(hMaterial))
            return false;

        auto& mat = *hMaterial;

        if (!ShaderProgram_bind(mat.program))
            return false;

        if (!DescriptorSet_bind(mat.descriptor_set))
            return false;

        return true;
    }

    bool Material_valid(const RMaterial& hMaterial)
    {
        return hMaterial &&
            g_Materials.valid(hMaterial) &&
            ShaderProgram_valid(hMaterial->program) &&
            DescriptorSet_valid(hMaterial->descriptor_set);
    }

}