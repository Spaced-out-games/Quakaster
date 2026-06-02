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
	
	// uint32_t binding; // artificial construct in GL; inferred by array index
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

	

	//using VertexLayout_createinfo = std::vector<VertexAttributeBinding>;

	struct VertexLayout_recipe {
		std::vector<VertexAttributeBinding> bindings;
	};



}