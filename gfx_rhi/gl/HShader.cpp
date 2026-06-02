#include "HShader.h"

#include "glad/glad.h"
#include <Resource.h>
#include "rhi_globals.h"
namespace qk::gfx::rhi {




    Result<HShader, str>
    Shader_ctor(str_view label, ShaderType type)
    {
        GLuint gl_shader = glCreateShader(
                type == ShaderType::Vertex
                ? GL_VERTEX_SHADER
                : type == ShaderType::Fragment
                ? GL_FRAGMENT_SHADER
                : GL_GEOMETRY_SHADER
        );

        if (!gl_shader) return str{"glCreateShader failed" };


        auto* pShader = g_Shaders.insert(
            label,
            { gl_shader }
        );
        return pShader;

    }

    bool Shader_valid(const HShader& hShader)
    {
        return hShader && g_Shaders.valid(hShader);
    }

    str Shader_upload(const HShader& hShader, const str& source) {
        
        if (!hShader) return "Invalid shader";

        QK_ASSERT(g_Shaders.valid(hShader));

        GLuint gl_shader = hShader->hShader;

        const char* src = source.c_str();

        glShaderSource(gl_shader, 1, &src, nullptr);
        glCompileShader(gl_shader);

        GLint success = 0;
        glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);

        if (success)
            return "";

        GLint logLength = 0;
        glGetShaderiv(gl_shader, GL_INFO_LOG_LENGTH, &logLength);

        str log;
        log.resize(logLength);

        glGetShaderInfoLog(
            gl_shader,
            logLength,
            nullptr,
            log.data());

        if (!log.empty() && log.back() == '\0')
            log.pop_back();

        return log;
    }

    bool Shader_dtor(HShader& hShader)
    {
        if (!hShader)
            return false;
        glDeleteShader(hShader->hShader);

        g_Shaders.destroy(hShader);
        hShader = nullptr;

        return true;
    }

    // ====================== PROGRAM ======================

    struct ShaderProgram_impl : qk::Resource
    {
        GLuint hProgram = 0;


    };

    static inline qk::ResourceRegistry<ShaderProgram_impl>
        g_Programs;

    Result<RShaderProgram, str>
        ShaderProgram_ctor(
            str_view label,
            const HShader& hFragment,
            const HShader& hVertex,
            bool)
    {
        if (!hFragment || !hVertex) return str{ "Invalid shader handle" };

        GLuint program = glCreateProgram();

        if (!program) return str{"glCreateProgram failed" };

        glAttachShader(program, hVertex->hShader);
        glAttachShader(program, hFragment->hShader);
        glLinkProgram(program);

        GLint ok = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &ok);

        if (!ok)
        {
            GLint len = 0;
            glGetProgramiv(
                program,
                GL_INFO_LOG_LENGTH,
                &len);

            str log(len, '\0');

            glGetProgramInfoLog(
                program,
                len,
                nullptr,
                log.data());

            glDeleteProgram(program);

            return log;
        }

        glDetachShader(program, hVertex->hShader);
        glDetachShader(program, hFragment->hShader);

        ShaderProgram_impl impl;

        return g_Programs.insert(
            label,
            {.hProgram = program});
    }

    bool ShaderProgram_valid(
        const RShaderProgram& hProgram)
    {
        return hProgram &&
            g_Programs.valid(hProgram);
    }

    bool ShaderProgram_bind(
        const RShaderProgram& hProgram)
    {
        if (!hProgram)
            return false;

        glUseProgram(hProgram->hProgram);
        return true;
    }

    bool ShaderProgram_dtor(
        RShaderProgram& hProgram)
    {
        if (!hProgram)
            return false;

        g_Programs.destroy(hProgram);
        hProgram = nullptr;
        glDeleteProgram(hProgram->hProgram);

        return true;
    }

    bool ShaderProgram_bind_uniform_block(
        const RShaderProgram& hProgram,
        const char* id,
        u8 slot)
    {
        if (!hProgram)
            return false;

        GLuint block =
            glGetUniformBlockIndex(
                hProgram->hProgram,
                id);

        if (block == GL_INVALID_INDEX)
            return false;

        glUniformBlockBinding(
            hProgram->hProgram,
            block,
            slot);

        return true;
    }

}