#pragma once
#include "HTexture.h"
#include "HBuffer.h"
#define RHI_MAX_DESCRIPTORSETS_BOUND 4

namespace qk::gfx::rhi {

    // Descriptor type
    enum class EDescriptorType {
        Texture,
        UniformBuffer,
        Unknown
        //StorageBuffer
    };

    // Used to forward-declare descriptor set structure.
    struct DescriptorDeclaration {
        EDescriptorType type;
        // bindings are auto-generated sequentially
    };



    using DescriptorSetLayout = std::vector<DescriptorDeclaration>;
	struct DescriptorSet_impl;

	using HDescriptorSet = DescriptorSet_impl*;


    HDescriptorSet DescriptorSet_ctor(const DescriptorSetLayout& recipe);

    void DescriptorSet_dtor(const HDescriptorSet& hDescriptorSet);

    void DescriptorSet_free(HDescriptorSet& hDescriptorSet);

	void DescriptorSet_setResource(HDescriptorSet hDescriptorSet, u32 binding, HTexture hTexture);
	void DescriptorSet_setResource(HDescriptorSet hDescriptorSet, u32 binding, HUniformBuffer hUniformBuffer);


    Resource* Resource_cast(const HDescriptorSet& hDescriptorSet);

    using DescriptorSetRegistry = ResourceRegistry<DescriptorSet_impl, DescriptorSet_dtor, DescriptorSet_free>;


    // Helper structures for batching.



    struct DescriptorDefinition {
        #ifdef _DEBUG
            EDescriptorType type;
        #endif
        union {
            // generic pointer to whatever the fuck
            void* pDescriptor;
            HTexture hTexture;
            HUniformBuffer hUniformBuffer;
        };

        DescriptorDefinition(EDescriptorType type, void* hDescriptor) {

            switch (type)
            {
            case qk::gfx::rhi::EDescriptorType::Texture:
                hTexture = (HTexture)hDescriptor;
                break;
            case qk::gfx::rhi::EDescriptorType::UniformBuffer:
                hUniformBuffer = (HUniformBuffer)hDescriptor;
                break;
            }

            QK_ASSERT(false, "Invalid EDescriptorType");

        }

        bool operator==(const DescriptorDefinition& other) const {
            
            #ifdef _DEBUG
                return type == other.type && pDescriptor == other.pDescriptor;
            #else
                    return pDescriptor == other.pDescriptor;
            #endif // _DEBUG

        }


    };

    // A fully defined DescriptorSet
    struct DescriptorSetDefinition {
        HDescriptorSet hDescriptorSet = nullptr;
        u8 slot = -1;
    };


}