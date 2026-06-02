#include "rhi_globals.h"
#include <HPipeline.h>
#include <glad/glad.h>
#include "HShader.h"
#include "HVertexLayout.h"
#include "rhi_globals.h"


namespace qk::gfx::rhi {

	struct Pipeline_impl: qk::Resource {
		// Resources
		RShaderProgram		hProgram = nullptr;
		//DescriptorSet_impl	descriptors;
		GLuint				vao = 0;
		std::vector<HDescriptorSet> sets;
		std::vector<Descriptor_impl> descriptors;

		PrimitiveTopology		topology = Triangle;
		//CullMode				cull_mode;
		//DepthState			depth_state;
		//BlendState			blend_state;

		Pipeline_impl();
		~Pipeline_impl();
		Pipeline_impl(const Pipeline_recipe& recipe);

		void bind() const;

		

	};

	Pipeline_impl::Pipeline_impl(const Pipeline_recipe& recipe) {

		// make the VAO
		glGenVertexArrays(1, &vao);

		// bind it
		glBindVertexArray(vao);


		// set up the vertex layout
		RVertexLayout hLayout = VertexLayout_ctor(recipe.layout);
		VertexLayout_apply(hLayout);

		// make the program.
		auto program = ShaderProgram_ctor(recipe.vert, recipe.frag);

		if (!program) {
			throw std::runtime_error(program.error());
		}

		// copy the descriptor set handles
		sets = recipe.sets;

		// since this is GL, copy the descriptors and validate
		for (const auto& set : sets) {
			
			if (!DescriptorSet_valid(set)) throw std::runtime_error("Provided an invalid Descriptor set");



		}






	}



}