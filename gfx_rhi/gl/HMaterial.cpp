#include "HMaterial.h"


namespace qk::gfx::rhi {

	using Material_impl = Material_createinfo;

	HandleResourceRegistry< Material_impl, HMaterial> g_Materials;

	HMaterial Material_ctor(const Material_createinfo& MatInfo) {
		// first, validate
		if (!ShaderProgram_valid(MatInfo.program)) {
			throw std::runtime_error("Shader Program was invalid");
		}
		if (!DescriptorSet_valid(MatInfo.descriptor_set)) {
			throw std::runtime_error("Descriptor Set was invalid");
		}

		// find all the Uniform Buffer descriptors and initialize the uniform blocks

		




		return g_Materials.insert(MatInfo);
	}

	void Material_dtor(const HMaterial& hMaterial) {
		g_Materials.destroy(hMaterial);
	}

	bool Material_bind(const HMaterial& hMaterial) {

		Material_impl& mat = g_Materials.get(hMaterial);

		if(!ShaderProgram_bind(mat.program)) return 0;

		if (!DescriptorSet_bind(mat.descriptor_set)) return 0;

		return 1;

	}

	bool Material_valid(const HMaterial& hMaterial) {

		if (!g_Materials.valid(hMaterial)) return 0;

		Material_impl& mat = g_Materials.get(hMaterial);


		return DescriptorSet_valid(mat.descriptor_set) && ShaderProgram_valid(mat.program);

	}

}