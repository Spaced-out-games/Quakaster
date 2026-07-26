#pragma once
#include "Resource.h"
#include <core.h>
#include <Result.h>
#include <vector>
#include <span>

namespace qk::gfx::rhi {


    enum class ShaderType {
        Unknown,
        Vertex,
        Fragment,
        Geometry,
        Compute
    };

    struct ShaderModule {
        str source;
        ShaderType type;
    };



    struct ShaderProgram_impl;
    using HShaderProgram = ShaderProgram_impl*;

    Resource* Resource_cast(const HShaderProgram& hShaderProgram);

    Result<HShaderProgram, str>
    ShaderProgram_ctor(const std::span<ShaderModule>& modules);



    void ShaderProgram_dtor(const HShaderProgram& hProgram);

    void ShaderProgram_free(HShaderProgram& hProgram);

    bool ShaderProgram_bind(const HShaderProgram& hProgram);

    bool ShaderProgram_valid(const HShaderProgram& hProgram);

    bool ShaderProgram_bind_uniform_block(
        const HShaderProgram& hProgram,
        const char* id,
        u8 slot);

}