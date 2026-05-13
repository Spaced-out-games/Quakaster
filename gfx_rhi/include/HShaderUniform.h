#pragma once
#include "core.h"
#include "Handle.h"
#include "HShader.h"
#include <string>
#include "Result.h"

using namespace qk;

namespace qk::gfx::rhi {

	struct HShaderUniformTag {};
	using HShaderUniform = Handle<HShaderUniformTag>;

}