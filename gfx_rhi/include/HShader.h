#pragma once
#include "Resource.h"
#include <core.h>
#include <Result.h>
#include <vector>
#include <span>

namespace qk::gfx::rhi {


    enum class EShaderType {
        Unknown,
        Vertex,
        Fragment,
        Geometry,
        Compute
    };

    struct ShaderModule {
        str source;
        EShaderType type;
    };



    struct ShaderProgram_impl;
    using HShaderProgram = ShaderProgram_impl*;

    Resource* Resource_cast(const HShaderProgram& hShaderProgram);

    HShaderProgram ShaderProgram_ctor(const std::span<ShaderModule>& modules);
    void ShaderProgram_dtor(const HShaderProgram& hProgram);
    void ShaderProgram_free(HShaderProgram& hProgram);



    using ShaderProgramRegistry = ResourceRegistry<ShaderProgram_impl, ShaderProgram_dtor, ShaderProgram_free>;


}