#pragma once
#include <Handle.h>
#include <stdint.h>
#include "HTexture.h"
#include "HBuffer.h"
#include "HShader.h"
#include "Result.h"
#include "core.h"


namespace qk::gfx::rhi {
	/*
		NOTE TO SELF: In the Vulkan spec, descriptors are effectively just polymorphic
		pointers to buffers/textures (including vertex/indexbuffers) with some metadata

		A descriptor set is just an array of these with no repeats.

		A descriptor pool is an array of descriptor sets.


		For now, since IndexBuffers and VertexBuffers are automatically bound in GL, we'll just ignore those
		from the rhi's DescriptorSet specification. 

	*/


	enum class EDescriptorType {
		Unknown,
		Texture,
		UniformBuffer,
		StorageBuffer
	};

	// todo: move ids into the descriptor. UniformBuffer was the only one with it inside
	struct Texture_desc{
		HTexture	hTex;
	};

	struct UniformBuffer_desc {
		RUniformBuffer hBuf;
		// NOTE: this has NOT been made memory safe! Consider using std::string instead!
		// need more metadata that isn't supplied in the implementation
	};

	// Not implemented in GL 3.3
	struct StorageBuffer_desc {
		RBuffer hBuf;
	};



	
	 
	struct Descriptor {
		EDescriptorType type;
		u8 binding = 0;
		str id;

		union {
			Texture_desc		texture;
			UniformBuffer_desc	uniform;
			StorageBuffer_desc	storage;
		};

	};

	// note: bind index is derived from the index in the descriptor set.
	using DescriptorSet_createinfo = std::vector<Descriptor>;


	struct HDescriptorSet: Handle {};


	HDescriptorSet DescriptorSet_ctor(const DescriptorSet_createinfo& info);

	void DescriptorSet_dtor(const HDescriptorSet& hDescriptorSet);

	bool DescriptorSet_bind(const HDescriptorSet& hDescriptorSet);

	bool DescriptorSet_valid(const HDescriptorSet& hDescriptorSet);

	void DescriptorSet_bindProgram(const HDescriptorSet& hDescriptorSet, const RShaderProgram& hProgram);


}