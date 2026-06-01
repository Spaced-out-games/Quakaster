#include "HMaterial.h"
#include <format>


namespace qk::gfx::rhi {


	struct Material_impl {
		HShaderProgram program;
		HDescriptorSet descriptor_set;
	};

	HandleResourceRegistry< Material_impl, HMaterial> g_Materials;

	
	static inline u32 generated_material_index = 0;

	Material_recipe::Material_recipe(
		const str& name,
		const str& vertName,
		const str& vertSrc,
		const str& fragName, 
		const str& fragSrc,
		const DescriptorSet_createinfo& setRecipe
	):
		matName(name),
		vertName(vertName),
		vertSrc(vertSrc),
		fragName(fragName),
		fragSrc(fragSrc),
		set_recipe(setRecipe)
	{}


	Material_recipe::Material_recipe(
		const str& vertSrc,
		const str& fragSrc
	):
		matName(std::format("<generated_material_{}>", generated_material_index++)),
		vertName(matName + "::vert"),
		vertSrc(vertSrc),
		fragName(matName + "::frag"),
		fragSrc(fragSrc)
	{
	}




	Result<Material_impl, str> make_material(const Material_recipe& mat_recipe, const DescriptorSet_createinfo& set_recipe) {

		// Vertex shader
		auto vert = rhi::Shader_ctor(rhi::ShaderType::Vertex);
		if (!vert) return str{ vert.error() };
		auto vertUploadCode = rhi::Shader_upload(vert.value(), mat_recipe.vertSrc);
		if (!vertUploadCode.empty()) return vertUploadCode;


		// Frag shader
		auto frag = rhi::Shader_ctor(rhi::ShaderType::Fragment);
		if (!frag) return str{ frag.error() };
		auto fragUploadCode = rhi::Shader_upload(frag.value(), mat_recipe.fragSrc);
		if (!fragUploadCode.empty()) return fragUploadCode;

		// Program
		auto program = rhi::ShaderProgram_ctor(frag.value(), vert.value());
		if (!program) return program.error();


		// Descriptor set
		HDescriptorSet set = rhi::DescriptorSet_ctor(set_recipe);



		Material_impl m = {
			.program = program.value(),
			.descriptor_set = set
		};

		return m;

	}






	Result<HMaterial, str> Material_ctor(const Material_recipe& MatInfo) {
		auto mat_res = make_material(MatInfo, MatInfo.set_recipe);

		if (!mat_res) return str{ mat_res.error() };

		const auto& mat = mat_res.value();

		// first, validate
		if (!ShaderProgram_valid(mat.program)) {
			throw std::runtime_error("Shader Program was invalid");
		}
		if (!DescriptorSet_valid(mat.descriptor_set)) {
			throw std::runtime_error("Descriptor Set was invalid");
		}

		DescriptorSet_bindProgram(mat.descriptor_set, mat.program);

		return g_Materials.insert(mat);
	}

	void Material_dtor(const HMaterial& hMaterial) {
		g_Materials.destroy(hMaterial);
	}

	bool Material_bind(const HMaterial& hMaterial) {

		QK_ASSERT(g_Materials.valid(hMaterial));
		if (!g_Materials.valid(hMaterial)) return 0;
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