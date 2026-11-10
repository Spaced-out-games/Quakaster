#include "rhi_globals.h"
#include <HPipeline.h>
#include <glad/glad.h>
#include "HShader.h"
#include "VertexLayoutDesc.h"
#include "rhi_globals.h"
#include "HShader.h"
#include <array>
#include <spdlog/spdlog.h>

namespace qk::gfx::rhi {

    bool apply_vertexlayout(const VertexLayout_recipe& layout)
    {
        for (const auto& buffer_binding : layout.bindings)
        {
            const GLuint bindingIndex = buffer_binding.slot;
            const GLuint divisor = InputRate_cast(buffer_binding.input_rate);

            spdlog::info(
                "VertexLayout apply: binding {} ({}, {} attribute(s))",
                bindingIndex,
                buffer_binding.input_rate == InputRate::PerInstance
                ? "PerInstance"
                : "PerVertex",
                buffer_binding.attributes.size()
            );

            // The divisor belongs to the binding, not the attributes.
            glVertexBindingDivisor(
                bindingIndex,
                divisor
            );

            for (const auto& attribute : buffer_binding.attributes)
            {
                const GLenum type = EScalarType_cast(attribute.type);

                /*
                 * Integer attributes:
                 *
                 * glVertexAttribIFormat() preserves the integer type and
                 * therefore corresponds to integer shader inputs.
                 *
                 * glVertexAttribFormat() converts the attribute to a
                 * floating-point shader input, optionally normalizing it.
                 */
                if (EScalarType_isInteger(attribute.type) &&
                    !attribute.normalize)
                {
                    glVertexAttribIFormat(
                        attribute.location,
                        attribute.count,
                        type,
                        attribute.offset
                    );
                }
                else
                {
                    glVertexAttribFormat(
                        attribute.location,
                        attribute.count,
                        type,
                        attribute.normalize ? GL_TRUE : GL_FALSE,
                        attribute.offset
                    );
                }

                glVertexAttribBinding(
                    attribute.location,
                    bindingIndex
                );

                glEnableVertexAttribArray(
                    attribute.location
                );


                str normalize_str = attribute.normalize ? " (normalized)" : "";

                spdlog::info("  attribute {},  binding {}: {}[{}]{} @ {}",
                    attribute.location,
                    bindingIndex,
                    to_string(attribute.type),
                    attribute.count,
                    normalize_str,
                    attribute.offset
                );

            }
        }

        return true;
    }




	Pipeline_impl::Pipeline_impl(const Pipeline_recipe& recipe) : Resource("Pipeline_impl") {

		// initialize the vao
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		

		// initialize shaders
		const std::span<const ShaderModule> modules = recipe.shader_modules;

		program = new ShaderProgram_impl(modules);
		QK_ASSERT(program != nullptr, "ShaderProgram_impl() returned nullptr");
		program->acquire();

		// copy the topology
		topology = recipe.topology;


		
		// apply the layout. Allocates memory for this for now

        apply_vertexlayout(recipe.layout);


		// also copy the strides
		for (const auto& binding : recipe.layout.bindings) {
			strides[binding.slot] = binding.stride;
		}

	}

	void Pipeline_impl::destroy() {
		if (!vao) return;
		glDeleteVertexArrays(1, &vao);
		// todo: fix shaders not being deleted here! Probably should just decrease the refcount instead of full on deletion!
		QK_ASSERT(program, "program was nullptr");
		program->release();
		program->flag_for_deletion();
		ShaderProgram_dtor(program);
		ShaderProgram_free(program);


	}


	void Pipeline_impl::bind() const {
		glBindVertexArray(vao);
		program->bind();
	}




	/// API


	HPipeline Pipeline_ctor(const Pipeline_recipe& recipe) {
		return new Pipeline_impl(recipe);
	}

	void Pipeline_dtor(const HPipeline& hPipeline) {
		QK_ASSERT(hPipeline != nullptr, "hPipeline was nullptr");
		QK_ASSERT(hPipeline->refcount() == 0 && hPipeline->flagged(), "Attepted to destroy locked resource");

		hPipeline->destroy();

	}

	void Pipeline_free(HPipeline& hPipeline) {
		QK_ASSERT(hPipeline, "hPipeline was nullptr");
		QK_ASSERT(hPipeline->refcount() == 0 && hPipeline->flagged(), "Attepted to destroy locked resource");


		delete hPipeline;

		hPipeline = nullptr;

	}


	Resource* Resource_cast(const HPipeline& hPipeline) {
		return static_cast<Resource*>(hPipeline);
	}


}