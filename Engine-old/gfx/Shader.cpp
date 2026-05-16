#include "Shader.h"
#include "GL/glew.h"
#include "spdlog/spdlog.h"

namespace qk::gfx {

	Handle Shader::compile() const{
        GLuint shader = glCreateShader(m_Type);
        const char* src = m_Source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

            std::string log(logLength, '\0');
            glGetShaderInfoLog(shader, logLength, nullptr, log.data());

            spdlog::error("Shader compile error:\n{}", log);

            glDeleteShader(shader);
            return NULL_HANDLE;
        }

        return (Handle)shader;
	}

    Shader::Shader(const std::string& source, ShaderType type)
        : m_Source(source), m_Type(type) {} 



}