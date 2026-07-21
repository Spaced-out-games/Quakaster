#include "HDescriptorSet.h"
#include "HTexture.h"
#include "HBuffer.h"
#include "rhi_globals.h"
namespace qk::gfx::rhi {


	
	struct DescriptorSetVTableEntry {
		EDescriptorType type = EDescriptorType::Unknown;
		u32 index = 0;
	};

	using DescriptorSetVTable = std::vector<DescriptorSetVTableEntry>;


	struct DescriptorSet_impl {
		std::vector<HTexture> m_Textures;
		std::vector<HUniformBuffer> m_UniformBuffers;

		// Maps virtual descriptor binding to the real descriptor index
		DescriptorSetVTable m_DescriptorVTable;


		//todo: move to implementation file
		DescriptorSet_impl(const DescriptorSet_recipe& recipe) {
			using enum EDescriptorType;


			for (const DescriptorBinding& binding : recipe) {
				switch (binding.type)
				{
					case Texture:
						m_DescriptorVTable.emplace_back(Texture, m_Textures.size());
						m_Textures.push_back(nullptr);
						break;
					case UniformBuffer:
						m_DescriptorVTable.emplace_back(UniformBuffer, m_UniformBuffers.size());
						m_UniformBuffers.push_back(nullptr);
						break;
					default:
						break;
				}
			}
		}

	};

	// this is just an empty layout
	HDescriptorSet DescriptorSet_ctor(const DescriptorSet_recipe& recipe) {

		return new DescriptorSet_impl(recipe);
		

	}

	// todo: move implementations into member functions
	void DescriptorSet_setResource(HDescriptorSet hDescriptorSet, u32 binding, HTexture hTexture) {
		assert(hDescriptorSet && hTexture);

		if (binding >= hDescriptorSet->m_DescriptorVTable.size()) return;
		const auto& details = hDescriptorSet->m_DescriptorVTable[binding];
		if (details.type != EDescriptorType::Texture) return;


		assert(hDescriptorSet->m_Textures.size() > details.index);
		auto& slot = hDescriptorSet->m_Textures[details.index];

		if (slot)
			slot->release();

		slot = hTexture;

		slot->acquire();

	}

	void DescriptorSet_setResource(HDescriptorSet hDescriptorSet, u32 binding, HUniformBuffer hUniformBuffer) {
		assert(hDescriptorSet && hUniformBuffer);

		if (binding >= hDescriptorSet->m_DescriptorVTable.size()) return;
		const auto& details = hDescriptorSet->m_DescriptorVTable[binding];
		if (details.type != EDescriptorType::UniformBuffer) return;


		assert(hDescriptorSet->m_UniformBuffers.size() > details.index);
		auto& slot = hDescriptorSet->m_UniformBuffers[details.index];

		if (slot)
			slot->release();

		slot = hUniformBuffer;

		slot->acquire();

	}


	void DescriptorSet_dtor(HDescriptorSet hDescriptorSet);


}