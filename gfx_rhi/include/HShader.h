#pragma once
//#include "Uniform.h" we need this!!!
#include <Handle.h>
#include <Result.h>
#include <rhi_Resource.h>
#include <string>
#include <core.h>

using qk::Result;

namespace qk::gfx::rhi {

	enum class ShaderType {
		Vertex,
		Fragment,
		Geometry
	};

	// ------------------------------------------------ SHADER ------------------------------------------------- //



	struct HShader : Handle {};


	Result<HShader, std::string> Shader_ctor(ShaderType type);
	std::string Shader_upload(const HShader& hShader, const std::string& source);
	bool Shader_dtor(HShader& hShader);
	bool Shader_valid(const HShader& hShader);


	// --------------------------------------------- SHADER PROGRAM --------------------------------------------- //


	//struct HShaderProgram : Handle {};

	/*
	Result<HShaderProgram, std::string>
	ShaderProgram_ctor(const HShader& hFragment, const HShader& hVertex);
	bool ShaderProgram_dtor(HShaderProgram& hProgram);
	bool ShaderProgram_bind(const HShaderProgram& hProgram);
	bool ShaderProgram_valid(const HShaderProgram& hProgram);
	bool ShaderProgram_bind_uniform_block(const HShaderProgram& hProgram, const char* id, u8 slot);
	*/

	struct HUniformBlock : Handle {};


	struct RShaderProgram: Resource {};


	Result<RShaderProgram, std::string>
	ShaderProgram_ctor(const HShader& hFragment, const HShader& hVertex, bool);
	bool ShaderProgram_dtor(RShaderProgram& hProgram);
	bool ShaderProgram_bind(const RShaderProgram& hProgram);
	bool ShaderProgram_valid(const RShaderProgram& hProgram);
	bool ShaderProgram_bind_uniform_block(const RShaderProgram& hProgram, const char* id, u8 slot);




}