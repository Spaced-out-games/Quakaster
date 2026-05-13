#include "HShaderProgram.h"
#include <glad/glad.h>
#include <stdexcept>
#include <format>


using namespace qk::gfx::rhi;
using qk::set;
using qk::get;

#include "rhi_HandleDefinitions.inl"




namespace qk::gfx::rhi  {
	Result<HShaderProgram, std::string>
		ShaderProgram_ctor(const HShader& hFragment, const HShader& hVertex)
	{
		GLuint frag = get(hFragment);
		GLuint vert = get(hVertex);

		assert(frag);
		assert(vert);

		GLuint program = glCreateProgram();
		if (program == 0)
			return std::string{"glCreateProgram() failed"};

		glAttachShader(program, vert);
		glAttachShader(program, frag);
		glLinkProgram(program);

		GLint success = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &success);

		if (success != GL_TRUE)
		{
			GLint logLen = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

			if (logLen <= 1)
				logLen = 1;

			std::string log;
			log.resize(static_cast<size_t>(logLen));

			glGetProgramInfoLog(program, logLen, nullptr, log.data());

			if (!log.empty() && log.back() == '\0')
				log.pop_back();

			glDeleteProgram(program);

			return log;
		}

		glDetachShader(program, vert);
		glDetachShader(program, frag);

		HShaderProgram hProgram;
		set(hProgram, program);
		return hProgram;
	}

	void ShaderProgram_bind(const HShaderProgram& hProgram) {
		glUseProgram(get(hProgram));
	}

	void ShaderProgram_dtor(HShaderProgram& hProgram) {
		GLuint program = get(hProgram);

		// enforce good hygiene
		assert(program != 0);

		glDeleteProgram(program);
		hProgram = nullptr;

	}

	Result<HShaderUniform, void_t> ShaderProgram_getUniform(const HShaderProgram& hProgram, const char* uniformName) {
		GLuint program = get(hProgram);
		HShaderUniform hUniform;

		int uniformLocation = glGetUniformLocation(program, uniformName);

		if (uniformLocation == -1) return void_t{};

		set(hUniform, uniformLocation);

		return hUniform;

	}
}

