#include "HShaderProgram.h"
#include <glad/glad.h>
#include <stdexcept>
#include <format>



using qk::gfx::rhi::HShader;
using qk::gfx::rhi::HShaderTag;

using qk::gfx::rhi::HShaderProgram;
using qk::gfx::rhi::HShaderProgramTag;

using qk::gfx::rhi::HShaderUniform;
using qk::gfx::rhi::HShaderUniformTag;

using qk::set;
using qk::get;

namespace qk {

	// shader
	template<>
	inline GLuint get<HShaderTag, GLuint>(const Handle<HShaderTag>& handle) {
		return (GLuint)handle.m_Handle;
	}

	template<>
	inline void set<HShaderTag, GLuint>(Handle<HShaderTag>& handle, GLuint value) {
		handle.m_Handle = (uintptr_t)value;
	}



	// shader program
	template<>
	inline GLuint get<HShaderProgramTag, GLuint>(const Handle<HShaderProgramTag>& handle) {
		return (GLuint)handle.m_Handle;
	}

	template<>
	inline void set<HShaderProgramTag, GLuint>(Handle<HShaderProgramTag>& handle, GLuint value) {
		handle.m_Handle = (uintptr_t)value;
	}



	// shader uniform
	template<>
	inline int get<HShaderUniformTag, int>(const Handle<HShaderUniformTag>& handle) {
		return (int)handle.m_Handle;
	}

	template<>
	inline void set<HShaderUniformTag, int>(Handle<HShaderUniformTag>& handle, int value) {
		handle.m_Handle = (uintptr_t)value;
	}



}


namespace qk::gfx::rhi  {
	Result<HShaderProgram, std::string>
		ShaderProgram_ctor(const HShader& hFragment, const HShader& hVertex)
	{
		GLuint frag = get<HShaderTag, GLuint>(hFragment);
		GLuint vert = get<HShaderTag, GLuint>(hVertex);

		assert(frag);
		assert(vert);

		GLuint program = glCreateProgram();
		if (program == 0)
			return std::unexpected("glCreateProgram() failed");

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

			return std::unexpected(log);
		}

		glDetachShader(program, vert);
		glDetachShader(program, frag);

		HShaderProgram hProgram;
		set(hProgram, program);
		return hProgram;
	}

	void ShaderProgram_bind(const HShaderProgram& hProgram) {
		glUseProgram(get<HShaderProgramTag, GLuint>(hProgram));
	}

	void ShaderProgram_dtor(HShaderProgram& hProgram) {
		GLuint program = get<HShaderProgramTag, GLuint>(hProgram);

		// enforce good hygiene
		assert(program != 0);

		glDeleteProgram(program);
		hProgram = nullptr;

	}

	Result<HShaderUniform, std::string> ShaderProgram_getUniform(const HShaderProgram& hProgram, const char* uniformName) {
		GLuint program = get<HShaderProgramTag, GLuint>(hProgram);
		HShaderUniform hUniform;

		int uniformLocation = glGetUniformLocation(program, uniformName);

		if (uniformLocation == -1) return std::unexpected(std::format("uniform `{}` does not exist in shader program {}", uniformName, program));

		set(hUniform, uniformLocation);

		return hUniform;

	}
}

