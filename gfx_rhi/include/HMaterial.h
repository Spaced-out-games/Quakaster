#pragma once
#include <HDescriptorSet.h>
#include <Handle.h>
#include <HShader.h>


namespace qk::gfx::rhi {

	

	struct Material_createinfo {
		HShaderProgram program;
		HDescriptorSet descriptor_set;
	};


	struct HMaterial : Handle {};


	HMaterial Material_ctor(const Material_createinfo& MatInfo);

	void Material_dtor(const HMaterial& hMaterial);


	bool Material_bind(const HMaterial& hMaterial);
	
	bool Material_valid(const HMaterial& hMaterial);




}