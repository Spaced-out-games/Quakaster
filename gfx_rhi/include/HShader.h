#pragma once

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


    Result<HShaderProgram, str>
    ShaderProgram_ctor(const std::span<ShaderModule>& modules);



    bool ShaderProgram_dtor(HShaderProgram& hProgram);

    bool ShaderProgram_bind(const HShaderProgram& hProgram);

    bool ShaderProgram_valid(const HShaderProgram& hProgram);

    bool ShaderProgram_bind_uniform_block(
        const HShaderProgram& hProgram,
        const char* id,
        u8 slot);

}