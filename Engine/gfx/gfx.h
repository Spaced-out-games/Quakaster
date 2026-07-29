#pragma once
#include <HBuffer.h>
#include <HCommandBuffer.h>
#include <HDescriptorSet.h>
#include <HVertexLayout.h>
#include <HPipeline.h>
#include <HTexture.h>
#include <HShader.h>



namespace qk::gfx {

	static inline rhi::BufferRegistry		 GBuffers;

	static inline rhi::CommandBufferRegistry CommandBuffers;

	static inline rhi::DescriptorSetRegistry DescriptorSets;

	static inline rhi::VertexLayoutRegistry  VertexLayouts;

	static inline rhi::PipelineRegistry		 Pipelines;

	static inline rhi::TextureRegistry		 Textures;

	static inline rhi::ShaderProgramRegistry ShaderPrograms;

	inline void destroy_all_resources() {
		CommandBuffers.destroy_everything();
		DescriptorSets.destroy_everything();
		VertexLayouts.destroy_everything();
		Pipelines.destroy_everything();
		Textures.destroy_everything();
		ShaderPrograms.destroy_everything();
		GBuffers.destroy_everything();
	}

}