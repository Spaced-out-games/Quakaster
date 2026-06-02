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



	struct VertexAttribute_recipe {
		uint32_t location;
		ComponentDesc component;
		uintptr_t offset;

	};
	
	struct VertexAttributeBinding {
		InputRate input_rate; // the divisor
		uint32_t stride;

		std::vector<VertexAttribute_recipe> attributes;

	};

	using VertexBinding = u8;

	struct VertexAttributePreset {
		EScalarType type;
		uint32_t count;
		EScalarMultiplicity multiplicity;
		bool normalized;
	};

	


	struct VertexLayout_recipe {
		str label;
		std::vector<VertexAttributeBinding> bindings;
	};



}