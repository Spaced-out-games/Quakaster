#include "HShader.h"

#include "glad/glad.h"
#include <Resource.h>
#include "rhi_globals.h"
#include <Result.h>
namespace qk::gfx::rhi {


    Result <GLuint, str>
    make_Shader(const Shader_recipe& recipe) {
        // create shader
        GLuint gl_shader = glCreateShader(ShaderType_cast(recipe.type));
        if (!gl_shader) return str{ "glCreateShader failed" };

        // try to compile it
        const char* src = recipe.source.c_str();
        glShaderSource(gl_shader, 1, &src, nullptr);
        glCompileShader(gl_shader);

        // get error code
        GLint code = 0;
        glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &code);


        if (!code) {
            // get the log
            GLint logLength = 0;
            glGetShaderiv(gl_shader, GL_INFO_LOG_LENGTH, &logLength);
            str log;
            log.resize(logLength);
            glGetShaderInfoLog(gl_shader, logLength, nullptr, log.data());

            // get rid of the extra null terminator
            if (!log.empty() && log.back() == '\0')
                log.pop_back();

            return str{ log };
        }


        return gl_shader;

    }


    Result<RShaderProgram, str>
    make_Program(str_view label, const Shader_recipe& vertRecipe, const Shader_recipe& fragRecipe) {
        // maek sure the shaders are organized correctly
        if (fragRecipe.type != ShaderType::Fragment) return str{ "Fragment shader has wrong type." };
        if (vertRecipe.type != ShaderType::Vertex) return str{ "Vertex shader has wrong type." };


        // compile the frag and vert shaders
        auto vert_shader = make_Shader(vertRecipe);
        if (!vert_shader) return str{ vert_shader.error() };
        GLuint vert = vert_shader.value();

        auto frag_shader = make_Shader(fragRecipe);
        if (!frag_shader) return str{ frag_shader.error() };
        GLuint frag = frag_shader.value();


        // create program and link
        GLuint program = glCreateProgram();
        if (!program) return str{ "glCreateProgram failed" };
        glAttachShader(program, vert);
        glAttachShader(program, frag);
        glLinkProgram(program);

        // get the error code
        GLint code = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &code);
        // error handling
        if (!code) {
            GLint log_length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
            str log(log_length, '\0');
            glGetProgramInfoLog(program, log_length, nullptr, log.data());
            glDeleteProgram(program);
            return str{ log };
        }

        // get rid of the shaders
        glDetachShader(program, vert);
        glDetachShader(program, frag);
        glDeleteShader(vert);
        glDeleteShader(frag);

        // return the program
        return g_Programs.insert(label, { program });
    }



    Result<RShaderProgram, str>
    ShaderProgram_ctor(str_view label, const Shader_recipe& vertRecipe, const Shader_recipe& fragRecipe) {
        return make_Program(label, vertRecipe, fragRecipe);
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