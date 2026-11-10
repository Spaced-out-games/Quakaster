#include "HDescriptorSet.h"
#include "HTexture.h"
#include "HBuffer.h"
#include "rhi_globals.h"
namespace qk::gfx::rhi {



	DescriptorSet_impl::DescriptorSet_impl(const DescriptorSetLayout& recipe) : Resource("DescriptorSet_impl") {
		using enum EDescriptorType;


		for (const DescriptorDeclaration& binding : recipe) {
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

		m_Textures.clear();

		for (auto& BufSlot : m_UniformBuffers) {
			if (BufSlot.hUniformBuffer) BufSlot.hUniformBuffer->release();
		}

		m_UniformBuffers.clear();

	}

	void DescriptorSet_impl::setResource(u32 binding, HTexture hTexture) {
		QK_ASSERT(hTexture != nullptr, "hTexture was nullptr");
		QK_ASSERT(binding < m_DescriptorVTable.size(), "Bind index out of range");
		
		const auto& details = m_DescriptorVTable[binding];

		QK_ASSERT(details.type == EDescriptorType::Texture, "attempted to assign a texture descriptor to a non-texture descriptor slot");
		QK_ASSERT(details.index < m_Textures.size(), "descriptor index out of range");
		auto& slot = m_Textures[details.index];

		if (slot.hTex)
			slot.hTex->release();

		slot.hTex = hTexture;

		slot.hTex->acquire();
	}

	void DescriptorSet_impl::setResource(u32 binding, HUniformBuffer hUniformBuffer) {
		QK_ASSERT(hUniformBuffer != nullptr, "hUniformBuffer was nullptr");
		QK_ASSERT(binding < m_DescriptorVTable.size(), "descriptor index out of range");
		
		const auto& details = m_DescriptorVTable[binding];
		
		
		QK_ASSERT(details.type == EDescriptorType::UniformBuffer, "Attempted to assign a uniform buffer to a non-uniformbuffer descriptor slot");
		QK_ASSERT(details.index < m_UniformBuffers.size(), "descriptor index out of range");


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



	Resource* Resource_cast(const HDescriptorSet& hDescriptorSet) {
		return static_cast<Resource*>(hDescriptorSet);
	}

	// this is just an empty layout
	HDescriptorSet DescriptorSet_ctor(const DescriptorSetLayout& recipe) {

		return new DescriptorSet_impl(recipe);
		
	}

	void DescriptorSet_dtor(const HDescriptorSet& hDescriptorSet) {

		QK_ASSERT(hDescriptorSet->refcount() == 0 && hDescriptorSet->flagged(), "");

		hDescriptorSet->destroy();

	}

	void DescriptorSet_free(HDescriptorSet& hDescriptorSet) {
		QK_ASSERT(hDescriptorSet->refcount() == 0 && hDescriptorSet->flagged(), "");

		delete hDescriptorSet;

		hDescriptorSet = nullptr;

	}

	// todo: move implementations into member functions
	void DescriptorSet_setResource(HDescriptorSet hDescriptorSet, u32 binding, HTexture hTexture) {
		return hDescriptorSet->setResource(binding, hTexture);

	}

	void DescriptorSet_setResource(HDescriptorSet hDescriptorSet, u32 binding, HUniformBuffer hUniformBuffer) {
		return hDescriptorSet->setResource(binding, hUniformBuffer);


	}





}