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


	struct Texture_desc{
		HTexture	hTex;
	};

	struct UniformBuffer_desc {
		HUniformBuffer hBuf;
		const char* id = nullptr;
		// need more metadata that isn't supplied in the implementation
	};

	// Not implemented in GL 3.3
	struct StorageBuffer_desc {
		HBuffer hBuf;
	};



	
	 
	struct Descriptor {
		EDescriptorType type;
		u8 binding = 0;

		union {
			Texture_desc		texture;
			UniformBuffer_desc	uniform;
			StorageBuffer_desc	storage;
		};
	};




	// TODO: Transfer to union. 
	//using Descriptor = Variant<Texture_desc, UniformBuffer_desc>;

	



	// note: bind index is derived from the index in the descriptor set.
	using DescriptorSet_createinfo = std::vector<Descriptor>;


	/*
	struct DescriptorSet_createinfo {
		std::vector<Descriptor> descriptors;
	};
	*/

	struct HDescriptorSet: Handle {};


	HDescriptorSet DescriptorSet_ctor(const DescriptorSet_createinfo& info);

	void DescriptorSet_dtor(const HDescriptorSet& hDescriptorSet);

	bool DescriptorSet_bind(const HDescriptorSet& hDescriptorSet);

	bool DescriptorSet_valid(const HDescriptorSet& hDescriptorSet);

	void DescriptorSet_bindProgram(const HDescriptorSet& hDescriptorSet, const HShaderProgram& hProgram);


	/*
		TODO: Work on developing a DescriptorSetLayout so that we don't have to bind in-order. In case you forget,
		a binding is conceptually just a u8 + Descriptor:

		struct DescriptorBinding {
			uint8_t slot;
			Descriptor descriptor;
		}

		then a descriptor set, at least in a GL implementation, can just be a vector of those OR a side-by side vector:

		std::vector<Descriptor> descriptors;
		std::vector<u8>			bindings;


		neither one is wrong.
	
	
	
	*/


}