#pragma once
#include "core.h"
#include "Handle.h"
#include <string>
#include "Result.h"

namespace qk::gfx::rhi {

	enum class ShaderType {
		Vertex,
		Fragment,
		Geometry,
		TesselationControl,
		TesselationEvaluation,
		Compute
	};


	struct HShaderTag {};
	using HShader = Handle<HShaderTag>;


	Result<HShader, std::string> Shader_ctor(ShaderType type);

	void Shader_dtor(HShader& hShader);

	std::string Shader_upload(const HShader& hShader, const std::string& source);



}