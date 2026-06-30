#pragma once
#include "core.h"
#include "Handle.h"
#include "Scalar.h"

using qk::Handle;

namespace qk::gfx::rhi {


	enum InputRate : uint8_t {
		PerVertex,
		PerInstance,
		Unknown
	};
	


	struct VertexAttribute {
		// component
		EScalarMultiplicity multiplicity = EScalarMultiplicity::Scalar;
		bool                normalize;
		uint32_t            count;
		EScalarType         type;

		uint32_t location;
		uintptr_t offset;

	};
	
	struct VertexAttributeBinding {
		InputRate input_rate;
		uint32_t stride;
		u8 slot = 0;

		std::vector<VertexAttribute> attributes;

	};


	


	struct VertexLayout_recipe {
		std::vector<VertexAttributeBinding> bindings;
	};



}