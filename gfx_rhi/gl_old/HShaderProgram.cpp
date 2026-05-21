#include "HShaderProgram.h"
#include <glad/glad.h>
#include <stdexcept>
#include <format>


using namespace qk::gfx::rhi;
using qk::set;
using qk::get;

#include "rhi_ResourceDefinitions.inl"




namespace qk::gfx::rhi  {
	Result<HShaderProgram, std::string>
		ShaderProgram_ctor(const HShader& hFragment, const HShader& hVertex)
	{
		QK_ASSERT(hFragment.valid());
		QK_ASSERT(hVertex.valid());

		GLuint gl_fragShader = get(hFragment);
		GLuint gl_vertShader = get(hVertex);



		GLuint program = glCreateProgram();
		if (program == 0)
			return std::string{"glCreateProgram() failed"};

		glAttachShader(program, gl_vertShader);
		glAttachShader(program, gl_fragShader);
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

		glDetachShader(program, gl_vertShader);
		glDetachShader(program, gl_fragShader);

		HShaderProgram hProgram;
		set(hProgram, program);
		return hProgram;
	}

	void ShaderProgram_bind(const HShaderProgram& hProgram) {
		glUseProgram(get(hProgram));
	}

	void ShaderProgram_dtor(HShaderProgram& hProgram) {
		QK_ASSERT(hProgram.valid());
		GLuint gl_program = get(hProgram);

		glDeleteProgram(gl_program);
		hProgram = nullptr;

	}

	HShaderUniform ShaderProgram_getUniform(const HShaderProgram& hProgram, const char* uniformName) {

		QK_ASSERT(hProgram.valid());


		GLuint gl_program = get(hProgram);

		HShaderUniform hUniform;

		int uniformLocation = glGetUniformLocation(gl_program, uniformName);

		if (uniformLocation == -1) return HShaderUniform{ nullptr };// void_t{};

		set(hUniform, uniformLocation);

		return hUniform;

	}
}

