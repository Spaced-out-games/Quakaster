#include "HDescriptorSet.h"
#include <glad/glad.h>

namespace qk::gfx::rhi {

	using Texture_impl = Texture_desc;

	using UniformBuffer_impl = UniformBuffer_desc;



	using DescriptorSet_impl = std::vector<Descriptor>;

	qk::HandleResourceRegistry<DescriptorSet_impl, HDescriptorSet> g_DescriptorSets;

	HDescriptorSet DescriptorSet_ctor(const DescriptorSet_createinfo& info) {
		return g_DescriptorSets.insert(info);
	}

	void DescriptorSet_dtor(const HDescriptorSet& hDescriptorSet) {

		QK_ASSERT(g_DescriptorSets.valid(hDescriptorSet));

		g_DescriptorSets.destroy(hDescriptorSet);

	}

	bool DescriptorSet_bind(const HDescriptorSet& hDescriptorSet) {
		QK_ASSERT(g_DescriptorSets.valid(hDescriptorSet));

		const auto& DescriptorSet = g_DescriptorSets.get(hDescriptorSet);

		for (auto& desc : DescriptorSet) {


			switch (desc.type)
			{
				case EDescriptorType::Texture:
					QK_ASSERT(Texture_valid(desc.texture.hTex));
					if (!Texture_valid(desc.texture.hTex)) { return 0; }
					Texture_bind(desc.texture.hTex, desc.binding);
					break;
				case EDescriptorType::UniformBuffer:
					const auto hUBuf = Handle_cast<HUniformBuffer>(desc.uniform.hBuf);
					QK_ASSERT(UniformBuffer_valid(Handle_cast<HUniformBuffer>(hUBuf)));
					if (!UniformBuffer_valid(hUBuf)) { return 0; }
					UniformBuffer_bind(hUBuf, desc.binding);
					break;
				case EDescriptorType::StorageBuffer:
					throw std::runtime_error("SSBO not implemented.");
					break;
				
			default:
				break;
			}
			

		}
		return 1;


	}


	bool DescriptorSet_valid(const HDescriptorSet& hDescriptorSet) {
		if (!g_DescriptorSets.valid(hDescriptorSet))
			return 0;

		const auto& DescriptorSet = g_DescriptorSets.get(hDescriptorSet);


		for (auto& desc : DescriptorSet) {


			switch (desc.type)
			{
			case EDescriptorType::Texture:
				if (!Texture_valid(desc.texture.hTex))
					return 0;
				break;
			case EDescriptorType::UniformBuffer:
				if(!Buffer_valid(Handle_cast<HBuffer>(desc.uniform.hBuf)))
					return 0;
				break;
			case EDescriptorType::StorageBuffer:
				return 0;
				break;
			
			default:
				break;
			}

		}

		return 1;

	}
	
	void DescriptorSet_bindProgram(const HDescriptorSet& hDescriptorSet, const HShaderProgram& hProgram) {
		if (!g_DescriptorSets.valid(hDescriptorSet)) return;

		const auto& set = g_DescriptorSets.get(hDescriptorSet);

		for (const auto& desc : set) {


			switch (desc.type) {
				case EDescriptorType::Texture:
					if (!Texture_valid(desc.texture.hTex))
						return;
				case EDescriptorType::UniformBuffer:
					const auto hUBuf = Handle_cast<HUniformBuffer>(desc.uniform.hBuf);
					if (!UniformBuffer_valid(hUBuf))
						return;
					ShaderProgram_bind_uniform_block(hProgram, desc.id.c_str(), desc.binding);
					break;
				case EDescriptorType::StorageBuffer:
					throw std::runtime_error("SSBO not implemented.");
				default:
					break;
			}




			
		}
	}




}