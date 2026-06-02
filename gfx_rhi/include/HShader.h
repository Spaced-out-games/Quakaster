#pragma once

#include <core.h>
#include <Result.h>

namespace qk::gfx::rhi {



    enum class ShaderType {
        Vertex,
        Fragment,
        Geometry
    };

    struct Shader_recipe {
        str source;
        ShaderType type;
    };


    struct Shader_impl;
    using HShader = Shader_impl*;

    Result<HShader, str> Shader_ctor(str_view label, ShaderType type);

    str Shader_upload(
        const HShader& hShader,
        const str& source);

    bool Shader_dtor(HShader& hShader);

    bool Shader_valid(const HShader& hShader);


    struct ShaderProgram_impl;
    using RShaderProgram = ShaderProgram_impl*;

    Result<RShaderProgram, str>
        ShaderProgram_ctor(
            str_view label,
            const HShader& hFragment,
            const HShader& hVertex,
            bool);

    bool ShaderProgram_dtor(RShaderProgram& hProgram);

    bool ShaderProgram_bind(const RShaderProgram& hProgram);

    bool ShaderProgram_valid(const RShaderProgram& hProgram);

    bool ShaderProgram_bind_uniform_block(
        const RShaderProgram& hProgram,
        const char* id,
        u8 slot);

}