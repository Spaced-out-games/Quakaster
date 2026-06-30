#include "rhi_globals.h"
#include <HPipeline.h>
#include <glad/glad.h>
#include "HShader.h"
#include "HVertexLayout.h"
#include "rhi_globals.h"
#include "HShader.h"
#include <array>

namespace qk::gfx::rhi {

	struct Pipeline_impl : qk::Resource {
		HShaderProgram				program;
		PrimitiveTopology			topology;
		GLuint						vao;
		std::unordered_map<u8, u32> strides;
		//CullMode cull;
		//DepthState depth;
		//BlendState blend;

		Pipeline_impl(const Pipeline_recipe& recipe);

		bool destroy() override {
			if (program)
				program->release();
			if (vao)
				glDeleteVertexArrays(1, &vao);
			return true;
		}


		void bind() const;

	};

	Pipeline_impl::Pipeline_impl(const Pipeline_recipe& recipe) {

		// initialize the vao
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		

		// initialize shaders
		str err;
		const std::span<const ShaderModule> modules = recipe.shader_modules;

		program = new ShaderProgram_impl(modules, err);
		assert(program);

		// copy the topology
		topology = recipe.topology;



		// apply the layout. Allocates memory for this for now
		HVertexLayout hLayout = VertexLayout_ctor(recipe.layout);
		hLayout->apply();

		// also copy the strides
		u8 slot = 0; // Once we add slots indices to the bindings, update this!!!
		for (const auto& binding : recipe.layout.bindings) {
			strides[slot] = binding.stride;
		}

	}

	void Pipeline_impl::bind() const {
		glBindVertexArray(vao);
		ShaderProgram_bind(program);


	}




	/// API


	HPipeline Pipeline_ctor(const Pipeline_recipe& recipe) {
		return new Pipeline_impl(recipe);
	}

	void Pipeline_dtor(HPipeline& pipeline) {
		assert(pipeline);
		if (!pipeline) return;
		delete pipeline; // would be better to use a try_delete interface
		pipeline = nullptr;

	}

	bool Pipeline_bind(const HPipeline& pipeline) {
		assert(pipeline);
		if (!pipeline) return false;
		pipeline->bind();
	}




}