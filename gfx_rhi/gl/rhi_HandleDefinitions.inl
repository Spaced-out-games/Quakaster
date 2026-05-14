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

namespace qk {

	template<>
	struct handle_traits<HVertexBufferLayoutTag> {
		using value_type = VertexBufferLayoutDesc*;
	};

	template<>
	struct handle_traits<HVertexBufferTag> {
		using value_type = GLuint;
	};

	template<>
	struct handle_traits<HIndexBufferTag> {
		using value_type = GLuint;
	};

	template<>
	struct handle_traits<HVertexArrayTag> {
		using value_type = GLuint;
	};

	template<>
	struct handle_traits<HTextureTag> {
		using value_type = GLuint;
	};

	template<>
	struct handle_traits<HShaderTag> {
		using value_type = GLuint;
	};

	template<>
	struct handle_traits<HShaderProgramTag> {
		using value_type = GLuint;
	};

	template<>
	struct handle_traits<HShaderUniformTag> {
		using value_type = GLint;
	};



	

}