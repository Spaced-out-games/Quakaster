#include "HShader.h"
#include <glad/glad.h>
#include <stdexcept>


using namespace qk::gfx::rhi;
using qk::set;
using qk::get;

#include "rhi_HandleDefinitions.inl"

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
			return std::string{ "glCreateShader failed (invalid context or out of resources)" };
		}

		HShader hShader;
		set(hShader, shader);

		return hShader;
	}

	void Shader_dtor(HShader& hShader)
	{
		GLuint shader = get(hShader);

		if (shader != 0)
		{
			glDeleteShader(shader);
			hShader = nullptr;
		}
	}



	Result<void_t, std::string> Shader_upload(const HShader& hShader, const std::string& source) {
		GLuint shader = get(hShader);

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

			return log;
		}
		return void_t{};
	}



}
