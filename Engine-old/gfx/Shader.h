/// **************************************** QUAKASTER ENGINE **************************************** 
/// gfx/Shader.h
/// Purpose: Represents shader source code, ready for compilation
/// **************************************************************************************************

#pragma once
#include <string>
#include "../core.h"
#include "types.h"

namespace qk::gfx {

	using ShaderType = unsigned int;


	struct QK_API Shader {



		private:
			std::string m_Source;
			ShaderType	m_Type;


		public:

			Shader(const std::string& source, ShaderType type);

			Handle compile() const;


	};
}