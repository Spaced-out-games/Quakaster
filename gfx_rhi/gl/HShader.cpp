#include "HShader.h"
#include <glad/glad.h>
#include <stdexcept>


using qk::gfx::rhi::HShader;
using qk::gfx::rhi::HShaderTag;
using qk::set;
using qk::get;

namespace qk {

	template<>
	inline GLuint get<HShaderTag, GLuint>(const Handle<HShaderTag>& handle) {
		return (GLuint)handle.m_Handle;
	}

	template<>
	inline void set<HShaderTag, GLuint>(Handle<HShaderTag>& handle, GLuint value) {
		handle.m_Handle = (uintptr_t)value;
	}


}


namespace qk::gfx::rhi {
	GLuint ShaderType_cast(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;

		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;

		case ShaderType::Geometry:
			return GL_GEOMETRY_SHADER;

		default:
			throw std::runtime_error(
				"OpenGL 3.3 only supports Vertex, Fragment, and Geometry shaders"
			);
		}
	}

	Result<HShader, std::string> Shader_ctor(ShaderType type)
	{
		GLuint shader = glCreateShader(ShaderType_cast(type));

		if (shader == 0)
		{
			return std::unexpected("glCreateShader failed (invalid context or out of resources)");
		}

		HShader hShader;
		set(hShader, shader);

		return hShader;
	}

	void Shader_dtor(HShader& hShader)
	{
		GLuint shader = get<HShaderTag, GLuint>(hShader);

		if (shader != 0)
		{
			glDeleteShader(shader);
			hShader = nullptr;
		}
	}



	Result<void, std::string> Shader_upload(const HShader& hShader, const std::string& source) {
		GLuint shader = get<HShaderTag, GLuint>(hShader);

		// prevent bad hygeine.
		assert(shader != 0);

		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			GLint logLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

			std::string log;
			log.resize(static_cast<size_t>(logLength));

			glGetShaderInfoLog(shader, logLength, nullptr, log.data());

			if (!log.empty() && log.back() == '\0')
				log.pop_back();

			return std::unexpected(log);
		}
		return {};
	}



}
