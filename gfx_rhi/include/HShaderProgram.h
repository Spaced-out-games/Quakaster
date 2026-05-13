#pragma once
#include "core.h"
#include "Handle.h"
#include "HShader.h"
#include <string>
#include "Result.h"
#include "HShaderUniform.h"

namespace qk::gfx::rhi {




	struct HShaderProgramTag {};
	using HShaderProgram = Handle<HShaderProgramTag>;




	Result<HShaderProgram, std::string> ShaderProgram_ctor(const HShader& hFragment, const HShader& hVertex);

	void ShaderProgram_bind(const HShaderProgram& hProgram);

	void ShaderProgram_dtor(HShaderProgram& hProgram);

	Result<HShaderUniform, void_t> ShaderProgram_getUniform(const HShaderProgram& hProgram, const char* uniformName);


	// then you'll need EVEN MORE for uploading uniforms.


}