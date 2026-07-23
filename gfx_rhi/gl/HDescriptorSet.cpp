#include "HDescriptorSet.h"
#include "HTexture.h"
#include "HBuffer.h"
#include "rhi_globals.h"
namespace qk::gfx::rhi {



	DescriptorSet_impl::DescriptorSet_impl(const DescriptorSet_recipe& recipe) {
		using enum EDescriptorType;


		for (const DescriptorBinding& binding : recipe) {
			switch (binding.type)
			{
			case Texture:
				m_DescriptorVTable.emplace_back(Texture, m_Textures.size());
				m_Textures.emplace_back(nullptr, m_Textures.size());
				break;
			case UniformBuffer:
				m_DescriptorVTable.emplace_back(UniformBuffer, m_UniformBuffers.size());
				m_UniformBuffers.emplace_back(nullptr, m_UniformBuffers.size());
				break;
			default:
				break;
			}
		}
	}

	void DescriptorSetsContext::next(HDescriptorSet hDescriptorSet) {
		TexOffset += hDescriptorSet->m_Textures.size();
		UniformBufferOffset += hDescriptorSet->m_UniformBuffers.size();
	}



	void DescriptorSet_impl::destroy() {
		for (auto& TexSlot : m_Textures) {
			if (TexSlot.hTex) TexSlot.hTex->release();
		}

		for (auto& BufSlot : m_UniformBuffers) {
			if (BufSlot.hUniformBuffer) BufSlot.hUniformBuffer->release();
		}
	}


	DescriptorSet_impl::~DescriptorSet_impl() {
		destroy();
	}

	void DescriptorSet_impl::setResource(u32 binding, HTexture hTexture) {
		assert(hTexture);
		assert(binding < m_DescriptorVTable.size());
		
		const auto& details = m_DescriptorVTable[binding];

		assert(details.type == EDescriptorType::Texture);
		assert(m_Textures.size() > details.index);
		auto& slot = m_Textures[details.index];

		if (slot.hTex)
			slot.hTex->release();

		slot.hTex = hTexture;

		slot.hTex->acquire();
	}

	void DescriptorSet_impl::setResource(u32 binding, HUniformBuffer hUniformBuffer) {
		assert(hUniformBuffer);
		assert(binding < m_DescriptorVTable.size());
		
		const auto& details = m_DescriptorVTable[binding];
		
		
		assert(details.type == EDescriptorType::UniformBuffer);
		assert(m_UniformBuffers.size() > details.index);


		auto& slot = m_UniformBuffers[details.index];

		if (slot.hUniformBuffer)
			slot.hUniformBuffer->release();

		slot.hUniformBuffer = hUniformBuffer;

		slot.hUniformBuffer->acquire();
	}
	void DescriptorSet_impl::bind(DescriptorSetsContext& ctx) const {

		for (auto& TexSlot : m_Textures) {
			if(TexSlot.hTex)
				TexSlot.hTex->bind(TexSlot.slot + ctx.TexOffset);
		}

		for (auto& UniformBufSlot : m_UniformBuffers) {
			if(UniformBufSlot.hUniformBuffer)
				UniformBufSlot.hUniformBuffer->UniformBuffer_bind(UniformBufSlot.slot + ctx.UniformBufferOffset);
		}

		ctx.TexOffset += m_Textures.size();
		ctx.UniformBufferOffset += m_UniformBuffers.size();

	}



	

	// this is just an empty layout
	HDescriptorSet DescriptorSet_ctor(const DescriptorSet_recipe& recipe) {

		return new DescriptorSet_impl(recipe);
		

	}

	// todo: move implementations into member functions
	void DescriptorSet_setResource(HDescriptorSet hDescriptorSet, u32 binding, HTexture hTexture) {
		return hDescriptorSet->setResource(binding, hTexture);

	}

	void DescriptorSet_setResource(HDescriptorSet hDescriptorSet, u32 binding, HUniformBuffer hUniformBuffer) {
		return hDescriptorSet->setResource(binding, hUniformBuffer);


	}


	void DescriptorSet_dtor(HDescriptorSet hDescriptorSet);


}