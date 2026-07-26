#include "rhi_globals.h"
#include <HPipeline.h>
#include <glad/glad.h>
#include "HShader.h"
#include "HVertexLayout.h"
#include "rhi_globals.h"
#include "HShader.h"
#include <array>

namespace qk::gfx::rhi {



	Pipeline_impl::Pipeline_impl(const Pipeline_recipe& recipe) : Resource("Pipeline_impl") {

		// initialize the vao
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		

		// initialize shaders
		str err;
		const std::span<const ShaderModule> modules = recipe.shader_modules;

		program = new ShaderProgram_impl(modules, err);
		assert(program);
		program->acquire();

		// copy the topology
		topology = recipe.topology;


		
		// apply the layout. Allocates memory for this for now
		hLayout = VertexLayout_ctor(recipe.layout);
		hLayout->acquire();
		hLayout->apply();


		// also copy the strides
		u8 slot = 0; // Once we add slots indices to the bindings, update this!!!
		for (const auto& binding : recipe.layout.bindings) {
			strides[slot] = binding.stride;
		}

	}

	void Pipeline_impl::destroy() {
		if (!vao) return;
		glDeleteVertexArrays(1, &vao);
		// todo: fix shaders not being deleted here! Probably should just decrease the refcount instead of full on deletion!
		assert(program);
		program->release();
		program->flag_for_deletion();
		ShaderProgram_dtor(program);
		ShaderProgram_free(program);

		assert(hLayout);
		hLayout->release();
		hLayout->flag_for_deletion();
		VertexLayout_dtor(hLayout);
		VertexLayout_free(hLayout);

	}


	void Pipeline_impl::bind() const {
		glBindVertexArray(vao);
		ShaderProgram_bind(program);


	}




	/// API


	HPipeline Pipeline_ctor(const Pipeline_recipe& recipe) {
		return new Pipeline_impl(recipe);
	}

	void Pipeline_dtor(const HPipeline& hPipeline) {
		assert(hPipeline);
		assert(hPipeline->refcount() == 0 && hPipeline->flagged());

		hPipeline->destroy();

	}

	void Pipeline_free(HPipeline& hPipeline) {
		assert(hPipeline);
		assert(hPipeline->refcount() == 0 && hPipeline->flagged());


		delete hPipeline;

		hPipeline = nullptr;

	}

	bool Pipeline_bind(const HPipeline& hPipeline) {
		assert(hPipeline);
		if (!hPipeline) return false;
		hPipeline->bind();
		return true;
	}

	Resource* Resource_cast(const HPipeline& hPipeline) {
		return static_cast<Resource*>(hPipeline);
	}


}