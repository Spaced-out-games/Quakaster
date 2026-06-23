#include "rhi_globals.h"
#include <HPipeline.h>
#include <glad/glad.h>
#include "HShader.h"
#include "HVertexLayout.h"
#include "rhi_globals.h"
#include "HShader.h"
#include <array>

namespace qk::gfx::rhi {

	struct Pipeline_impl: qk::Resource {
		std::array<HDescriptorSet, 4> descriptor_sets;
		RShaderProgram				program;
		PrimitiveTopology			topology;
		//VertexLayout_recipe			layout;
		GLuint						vao;
		//CullMode cull;
		//DepthState depth;
		//BlendState blend;
		
		Pipeline_impl(const Pipeline_recipe& recipe);

		bool destroy() override {
			if(program)
				program->release();
			for (auto& set : descriptor_sets) {
				if(set)
					set->release();
			}
			if (vao)
				glDeleteVertexArrays(1, &vao);
			return true;
		}


		void bind() const;

	};

	Pipeline_impl::Pipeline_impl(const Pipeline_recipe& recipe) {

		descriptor_sets = recipe.descriptor_sets;
		program = recipe.program;
		topology = recipe.topology;
		
		// initialize the vao
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		

		// Vertex layout
		for (const auto& binding : recipe.layout.bindings) {
			for (const auto& attr : binding.attributes) {
				if (EScalarType_isInteger(attr.component.type) && attr.component.normalize) {
					glVertexAttribIPointer(
						attr.location, attr.component.count,
						EScalarType_cast(attr.component.type), binding.stride,
						(const void*)attr.offset
					);
				}
				else
				{
					glVertexAttribPointer(
						attr.location,
						attr.component.count,
						EScalarType_cast(attr.component.type),
						attr.component.normalize,
						binding.stride,
						(const void*)attr.offset
					);
				}

				glEnableVertexAttribArray(
					attr.location
				);

				glVertexAttribDivisor(
					attr.location,
					InputRate_cast(binding.input_rate)
				);
			}
		}




	}

	void Pipeline_impl::bind() const {
		glBindVertexArray(vao);
		ShaderProgram_bind(program);

		// We still need a way to offset each descriptor set.
		for (const auto& descriptor : descriptor_sets) {
			descriptor->bind();
		}

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