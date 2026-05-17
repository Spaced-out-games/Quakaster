#pragma once
#include "HVertexBufferLayout.h"
#include "HVertexBufferLayout.h"
#include "VertexBufferLayoutDesc.h"
#include "HShaderProgram.h"
#include "HDescriptorSet.h"
#include "HVertexArray.h"
#include "HVertexBuffer.h"
#include "HIndexBuffer.h"
#include "glad/glad.h"
#include "HTexture.h"
#include "HShader.h"

using namespace qk::gfx::rhi;
namespace qk::gfx::rhi {
	struct CommandBuffer;
}
namespace qk {

	template<>
	struct ResourceTraits<HVertexBufferLayoutTag> {
		using value_type = VertexBufferLayoutDesc*;
	};

	template<>
	struct ResourceTraits<HVertexBufferTag> {
		using value_type = GLuint;
	};

	template<>
	struct ResourceTraits<HIndexBufferTag> {
		using value_type = GLuint;
	};

	template<>
	struct ResourceTraits<HVertexArrayTag> {
		using value_type = GLuint;
	};

	template<>
	struct ResourceTraits<HTextureTag> {
		using value_type = GLuint;
	};

	template<>
	struct ResourceTraits<HShaderTag> {
		using value_type = GLuint;
	};

	template<>
	struct ResourceTraits<HShaderProgramTag> {
		using value_type = GLuint;
	};

	template<>
	struct ResourceTraits<HShaderUniformTag> {
		using value_type = GLint;
	};



	

}