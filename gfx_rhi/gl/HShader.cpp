#include "HShader.h"
#include "glad/glad.h"
#include "core.h"




namespace qk::gfx::rhi {

	static inline qk::HandleResourceRegistry<GLuint, HShader> g_Shaders;
	static inline qk::HandleResourceRegistry<GLuint, HShaderProgram> g_ShaderPrograms;

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



	Result<HShader, std::string> Shader_ctor(ShaderType type) {

		GLuint gl_shader = glCreateShader(ShaderType_cast(type));

		if (gl_shader == 0)
			return { "glCreateShader failed (invalid context or out of resources)"};


		HShader hShader = g_Shaders.insert(gl_shader);

		return hShader;
	}


	bool Shader_dtor(HShader& hShader)
	{
		QK_ASSERT(g_Shaders.valid(hShader));
		if (!g_Shaders.valid(hShader)) return 0;

		GLuint& gl_shader = g_Shaders.get(hShader);

		glDeleteShader(gl_shader);
		return 1;
	}

	bool Shader_valid(const HShader& hShader) {
		return g_Shaders.valid(hShader);
	}



	std::string Shader_upload(const HShader& hShader, const std::string& source) {
		QK_ASSERT(g_Shaders.valid(hShader));

		if (!g_Shaders.valid(hShader)) return "Invalid shader handle!";

		GLuint gl_shader = g_Shaders.get(hShader);



		const char* src = source.c_str();
		glShaderSource(gl_shader, 1, &src, nullptr);
		glCompileShader(gl_shader);
		GLint success;
		glGetShaderiv(gl_shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			GLint logLength = 0;
			glGetShaderiv(gl_shader, GL_INFO_LOG_LENGTH, &logLength);

			std::string log;
			log.resize((size_t)(logLength));

			glGetShaderInfoLog(gl_shader, logLength, nullptr, log.data());

			if (!log.empty() && log.back() == '\0')
				log.pop_back();

			return log;
		}
		return "";
	}





	Result<HShaderProgram, std::string>
	ShaderProgram_ctor(const HShader& hFragment, const HShader& hVertex){
		QK_ASSERT(g_Shaders.valid(hFragment));
		QK_ASSERT(g_Shaders.valid(hVertex));

		if (!g_Shaders.valid(hFragment)) return std::string{"Invalid fragment shader"};
		if (!g_Shaders.valid(hVertex)) return std::string{"Invalid vertex shader"};

		GLuint gl_fragShader = g_Shaders.get(hFragment);
		GLuint gl_vertShader = g_Shaders.get(hVertex);



		GLuint program = glCreateProgram();
		if (program == 0)
			return std::string{ "glCreateProgram() failed" };

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
			log.resize((size_t)(logLen));

			glGetProgramInfoLog(program, logLen, nullptr, log.data());

			if (!log.empty() && log.back() == '\0')
				log.pop_back();

			glDeleteProgram(program);

			return log;
		}

		glDetachShader(program, gl_vertShader);
		glDetachShader(program, gl_fragShader);


		return g_ShaderPrograms.insert(program);

	}

	bool ShaderProgram_dtor(HShaderProgram& hProgram){
		QK_ASSERT(g_ShaderPrograms.valid(hProgram));
		if (!g_ShaderPrograms.valid(hProgram)) return 0;
		glDeleteProgram(g_ShaderPrograms.get(hProgram));
		return 1;
	}

	bool ShaderProgram_bind(const HShaderProgram& hProgram){
		QK_ASSERT(g_ShaderPrograms.valid(hProgram));
		if (!g_ShaderPrograms.valid(hProgram)) return 0;
		glUseProgram(g_ShaderPrograms.get(hProgram));
		return 1;
	}
	bool ShaderProgram_valid(const HShaderProgram& hProgram) {
		return g_ShaderPrograms.valid(hProgram);
	}

	bool ShaderProgram_bind_uniform_block(const HShaderProgram& hProgram, const char* id, u8 slot) {
		QK_ASSERT(g_ShaderPrograms.valid(hProgram));
		if (!g_ShaderPrograms.valid(hProgram)) return 0;
		auto program = g_ShaderPrograms.get(hProgram);
		GLuint blockIndex = glGetUniformBlockIndex(program, "EditArgs");
		glUniformBlockBinding(program, blockIndex, 0);
	}



}