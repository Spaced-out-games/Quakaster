#include "HShader.h"
#include <glad/glad.h>
#include <stdexcept>


using namespace qk::gfx::rhi;
using qk::set;
using qk::get;

#include "rhi_ResourceDefinitions.inl"

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

		GLuint gl_shader = glCreateShader(ShaderType_cast(type));

		if (gl_shader == 0)
			return std::string{ "glCreateShader failed (invalid context or out of resources)" };



		HShader hShader;
		set(hShader, gl_shader);

		return hShader;
	}

	void Shader_dtor(HShader& hShader)
	{
		QK_ASSERT(hShader.valid());

		GLuint gl_shader = get(hShader);

		
		glDeleteShader(gl_shader);
		hShader = nullptr;

	}



	std::string Shader_upload(const HShader& hShader, const std::string& source) {
		QK_ASSERT(hShader.valid());

		GLuint gl_shader = get(hShader);



		const char* src = source.c_str();
		glShaderSource(gl_shader, 1, &src, nullptr);
		glCompileShader(gl_shader);
		GLint success;
		glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			GLint logLength = 0;
			glGetShaderiv(gl_shader, GL_INFO_LOG_LENGTH, &logLength);

			std::string log;
			log.resize(static_cast<size_t>(logLength));

			glGetShaderInfoLog(gl_shader, logLength, nullptr, log.data());

			if (!log.empty() && log.back() == '\0')
				log.pop_back();

			return log;
		}
		return "";
	}



}
