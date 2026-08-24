#pragma once
#include "core.h"
#include <vector>
#include "Scalar.h"


namespace qk::gfx::rhi {


	// Determines the rate by which a vertex attribute is given to the shader
	enum InputRate : uint8_t {
		PerVertex,
		PerInstance,
		Unknown
	};
	

	// Defines the structure of how to read an individual interleaved attribute in a vertex buffer
	struct VertexAttribute {
		// component
		EScalarMultiplicity multiplicity = EScalarMultiplicity::Scalar;
		bool                normalize;
		uint32_t            count;
		EScalarType         type;

		uint32_t location;
		uintptr_t offset;

	};
	
	// Defines the structure of one vertex buffer
	struct VertexAttributeBinding {
		InputRate input_rate;
		uint32_t stride;
		u8 slot = 0;

		std::vector<VertexAttribute> attributes;

	};


	

	// Vertex buffer layout create info
	struct VertexLayout_recipe {
		std::vector<VertexAttributeBinding> bindings;
	};


}