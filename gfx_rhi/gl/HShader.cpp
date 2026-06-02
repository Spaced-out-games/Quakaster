#include "HShader.h"

#include "glad/glad.h"
#include <Resource.h>
#include "rhi_globals.h"
#include <Result.h>
namespace qk::gfx::rhi {

    // Declaration. If you need it publicly, just move it to the `rhi_globals` header.
    struct ShaderProgram_impl : qk::Resource
    {
        GLuint hProgram = 0;

        static Result <GLuint, str>
            Shader_compile(const Shader_recipe& recipe);

        static Result <GLuint, str>
            Shader_link(const std::span<GLuint>& recipes);


        // temporary fix
        ShaderProgram_impl(GLuint hProgram) : hProgram(hProgram) {}

        ShaderProgram_impl(std::span<Shader_recipe> recipes, str& error_out);

        bool destroy() override;

        bool bind() const;

        ~ShaderProgram_impl();

        bool bind_uniform_block(const char* pName, u8 slot);

    };


    // construction
    ShaderProgram_impl::ShaderProgram_impl(std::span<Shader_recipe> recipes, str& error_out) {


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
    bool ShaderProgram_impl::destroy() {

        if (!ready_for_deletion() || !hProgram) return false;

        glDeleteProgram(hProgram);
        hProgram = 0;
        return true;
    }

    ShaderProgram_impl::~ShaderProgram_impl() {
        destroy();
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
    ShaderProgram_impl::Shader_compile(const Shader_recipe& recipe) {
        GLuint hShader = glCreateShader(ShaderType_cast(recipe.type));
        if (!hShader) return str{ "glCreateShader failed" };


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

            return str{ log };
        }

        return hShader;

    }




    Result <GLuint, str>
    ShaderProgram_impl::Shader_link(const std::span<GLuint>& shaders) {

        str error;

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

        if (!error.empty()) return error;

        return program;

    }






















    // RHI frontend


    Result<RShaderProgram, str>
        ShaderProgram_ctor(const Shader_recipe& vertRecipe, const Shader_recipe& fragRecipe) {
        str error = "";

        Shader_recipe recipes[] = { vertRecipe, fragRecipe };

        auto* pProgram = new ShaderProgram_impl(
            std::span{ recipes },
            error
        );

        if (!error.empty()) {
            delete pProgram;
            return error;
        }

        return pProgram;

    }



    bool ShaderProgram_valid(const RShaderProgram& hProgram) {
        return hProgram;
    }

    bool ShaderProgram_bind(const RShaderProgram& hProgram) {
        if (!hProgram) return false;
        return hProgram->bind();
    }

    bool ShaderProgram_dtor(
        RShaderProgram& hProgram)
    {
        if (!hProgram) return false;

        hProgram->destroy();


        return true;
    }

    bool ShaderProgram_bind_uniform_block(const RShaderProgram& hProgram, const char* id, u8 slot) {
        return hProgram->bind_uniform_block(id, slot);
    }

}