#include "HMaterial.h"

#include <format>
#include <Resource.h>
#include "rhi_globals.h"
namespace qk::gfx::rhi {





    // ---------------- Material_recipe ----------------

    Material_recipe::Material_recipe(const str& vertSrc, const str& fragSrc,
                                     const DescriptorSet_createinfo& setRecipe):
        vertSrc(vertSrc),
        fragSrc(fragSrc),
        set_recipe(setRecipe)
    {}

    // ---------------- internal build ----------------


    // backend

    Material_impl::Material_impl(const Material_recipe& recipe, str& error) {

        Shader_recipe vertRecipe = {
            .source = recipe.vertSrc,
            .type = ShaderType::Vertex
        };

        Shader_recipe fragRecipe = {
            .source = recipe.fragSrc,
            .type = ShaderType::Fragment
        };


        auto hProgram = ShaderProgram_ctor(vertRecipe, fragRecipe);

        if (!hProgram) {
            error += hProgram.error();
            return;
        }
        program = hProgram.value();
        descriptor_set = DescriptorSet_ctor(recipe.set_recipe);
    }


    bool Material_impl::destroy() {
        // needs to free resource locks here.
        return true;

    }

    void Material_impl::bind() {
        // we used to validate here but that shouldn't be needed
        ShaderProgram_bind(program);
        DescriptorSet_bind(descriptor_set);
    }







    // ---------------- public API ----------------

    Result<RMaterial, str>
    Material_ctor(const Material_recipe& MatInfo)
    {

        str error = "";

        auto* pMaterial = new Material_impl(MatInfo, error);

        if (!error.empty()) {
            delete pMaterial;
            return error;
        }
        return pMaterial;

    }

    void Material_dtor(RMaterial& hMaterial)
    {
        if (!hMaterial)
            return;

        delete hMaterial;
        hMaterial = nullptr;
    }

    bool Material_bind(const RMaterial& hMaterial)
    {
        if (!hMaterial)
            return false;

        hMaterial->bind();
        return true;

    }

    bool Material_valid(const RMaterial& hMaterial)
    {
        return hMaterial != nullptr &&
            hMaterial->descriptor_set != nullptr &&
            hMaterial->program != nullptr;
    }

}