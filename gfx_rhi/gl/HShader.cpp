#include "HShader.h"

#include "glad/glad.h"
#include <Resource.h>
#include "rhi_globals.h"
#include <Result.h>
namespace qk::gfx::rhi {




    // construction
    ShaderProgram_impl::ShaderProgram_impl(const std::span<const ShaderModule>& recipes, str& error_out): Resource("CommandBuffer_impl") {


        std::vector<GLuint> shaders;
        shaders.reserve(recipes.size());

        // try to compile everything. Consider some formatting
        for (const auto& recipe : recipes) {
            auto result = Shader_compile(recipe);

            if (!result)
                error_out += result.error();
            else
                shaders.push_back(result.value());

        }

        auto link_result = Shader_link(shaders);

        if (!link_result)
            error_out += link_result.error();
        else
            hProgram = link_result.value();

    }


    // destruction
    void ShaderProgram_impl::destroy() {
        assert(refcount() == 0 && flagged());
        if (!hProgram) return;

        glDeleteProgram(hProgram);
        hProgram = 0;
        return;
    }


    // binding
    bool ShaderProgram_impl::bind() const {
        if (!hProgram) return false;
        glUseProgram(hProgram);
        return true;
    }


    bool ShaderProgram_impl::bind_uniform_block(const char* pName, u8 slot) {
        assert(hProgram);

        GLuint block = glGetUniformBlockIndex(hProgram, pName);

        if (block == GL_INVALID_INDEX) return false;

        glUniformBlockBinding(hProgram, block, slot);

        return true;

    }




    // helpers
    Result <GLuint, str>
    ShaderProgram_impl::Shader_compile(const ShaderModule& recipe) {
        GLuint hShader = glCreateShader(ShaderType_cast(recipe.type));
        if (!hShader) return qk::Error<str>{ "glCreateShader failed" };


        const char* pSrc = recipe.source.c_str();
        glShaderSource(hShader, 1, &pSrc, nullptr);
        glCompileShader(hShader);

        GLint code = 0;
        glGetShaderiv(hShader, GL_COMPILE_STATUS, &code);

        if (!code) {
            // get the log
            GLint logLength = 0;
            glGetShaderiv(hShader, GL_INFO_LOG_LENGTH, &logLength);
            str log;
            log.resize(logLength);
            glGetShaderInfoLog(hShader, logLength, nullptr, log.data());

            // get rid of the extra null terminator
            if (!log.empty() && log.back() == '\0')
                log.pop_back();

            return qk::Error<str>{ log };
        }

        return hShader;

    }


    Result <GLuint, str>
    ShaderProgram_impl::Shader_link(const std::span<GLuint>& shaders) {

        str error;

        // create program and link
        GLuint program = glCreateProgram();

        // attach and link all the shaders
        for (const auto& shader : shaders) {
            glAttachShader(program, shader);
        }

        glLinkProgram(program);

        GLint code = 0;

        // get linker errors and append
        glGetProgramiv(program, GL_LINK_STATUS, &code);

        if (!code) {

            GLint log_length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
            str log(log_length, '\0');
            glGetProgramInfoLog(program, log_length, nullptr, log.data());
            glDeleteProgram(program);

            error += log;

        }

        for (const auto& shader : shaders) {
            glDetachShader(program, shader);
            glDeleteShader(shader);

        }

        

        if (!error.empty()) return qk::Error<str>{error};

        return program;

    }






















    // RHI frontend

    Resource* Resource_cast(const HShaderProgram& hShaderProgram) {
        return static_cast<Resource*>(hShaderProgram);
    }
    Result<HShaderProgram, str>
    ShaderProgram_ctor(const std::span<ShaderModule>& modules) {
        str error = "";


        auto* pProgram = new ShaderProgram_impl(
            modules,
            error
        );

        if (!error.empty()) {
            delete pProgram;
            return qk::Error<str>{error};
        }

        return pProgram;

    }

    void ShaderProgram_dtor(const HShaderProgram& hProgram) {

        assert(hProgram);
        assert(hProgram->refcount() == 0 && hProgram->flagged());

        hProgram->destroy();


        return;
    }

    void ShaderProgram_free(HShaderProgram& hProgram) {
        assert(hProgram);
        assert(hProgram->refcount() == 0 && hProgram->flagged());

        delete hProgram;
        hProgram = nullptr;
        return;
    }



    bool ShaderProgram_valid(const HShaderProgram& hProgram) {
        return hProgram != nullptr;
    }

    bool ShaderProgram_bind(const HShaderProgram& hProgram) {
        assert(hProgram);
        return hProgram->bind();
    }

    

    bool ShaderProgram_bind_uniform_block(const HShaderProgram& hProgram, const char* id, u8 slot) {
        return hProgram->bind_uniform_block(id, slot);
    }

}