#pragma once
#include <HDescriptorSet.h>
#include <Handle.h>
#include <HShader.h>


namespace qk::gfx::rhi {

	

	struct Material_createinfo {
		HShaderProgram program;
		HDescriptorSet descriptor_set;
		Material_createinfo() = default;
		Material_createinfo(HShaderProgram hProgram, HDescriptorSet hDescriptorSet):
			program(hProgram), descriptor_set(hDescriptorSet) {}
		Material_createinfo(const str& vertSrc, const str& fragSrc, DescriptorSet_createinfo desc) {

			// ============================================================
			// Vertex Shader
			// ============================================================

			auto vsResult = rhi::Shader_ctor(rhi::ShaderType::Vertex);


			rhi::HShader vertexShader = vsResult.value();



			str vsError = rhi::Shader_upload(vertexShader, vertSrc);

			if (!vsError.empty())
			{
				printf("%s\n", vsError.c_str());
				std::abort();
			}

			// ============================================================
			// Fragment Shader
			// ============================================================

			auto fsResult = rhi::Shader_ctor(rhi::ShaderType::Fragment);



			rhi::HShader fragmentShader = fsResult.value();


			str fsError =
				rhi::Shader_upload(fragmentShader, fragSrc);

			if (!fsError.empty())
			{
				printf("%s\n", fsError.c_str());
				std::abort();

			}

			// ============================================================
			// Shader Program
			// ============================================================

			auto progResult = rhi::ShaderProgram_ctor(fragmentShader, vertexShader);

			if (!progResult)
			{
				printf("[SHADER_PROGRAM_LINK] %s\n", progResult.error().c_str());
				std::abort();

			}

			program = progResult.value();


			// ============================================================
			// Descriptor set
			// ============================================================
			descriptor_set = rhi::DescriptorSet_ctor(desc);

		}
	};


	struct HMaterial : Handle {};


	HMaterial Material_ctor(const Material_createinfo& MatInfo);

	void Material_dtor(const HMaterial& hMaterial);


	bool Material_bind(const HMaterial& hMaterial);
	
	bool Material_valid(const HMaterial& hMaterial);




}