#include "LoadPipeline.h"

using qk::gfx::rhi::EScalarType;
using qk::gfx::rhi::InputRate;
using qk::gfx::rhi::ShaderType;
using qk::gfx::rhi::PrimitiveTopology;
using qk::gfx::rhi::EDescriptorType;
using qk::gfx::rhi::DescriptorBinding;
namespace qk::media {




	inline EScalarType EScalarType_fromString(const str& value) {
		if (value == "i8") return EScalarType::i8;
		else if (value == "i16") return EScalarType::i16;
		else if (value == "i32") return EScalarType::i32;
		else if (value == "i64") return EScalarType::i64;

		else if (value == "u8") return EScalarType::u8;
		else if (value == "u16") return EScalarType::u16;
		else if (value == "u32") return EScalarType::u32;
		else if (value == "u64") return EScalarType::u64;

		else if (value == "f32") return EScalarType::f32;
		else if (value == "f64") return EScalarType::f64;
		else assert(false);
	}

	inline ShaderType ShaderType_fromString(const str& value) {
		if (value == "vertex") return ShaderType::Vertex;
		else if (value == "fragment") return ShaderType::Fragment;
		else if (value == "geometry") return ShaderType::Geometry;
		else if (value == "compute") return ShaderType::Compute;
		else assert(false);
	}

	inline PrimitiveTopology PrimitiveTopology_fromString(const str& value) {
		if (value == "triangle") return PrimitiveTopology::Triangle;
		else if (value == "lines") return PrimitiveTopology::Lines;
		else if (value == "points") return PrimitiveTopology::Points;
		else if (value == "triangle_strip") return PrimitiveTopology::TriangleStrip;
		else if (value == "line_strip") return PrimitiveTopology::LineStrip;
		else assert(false);
	}

	inline EDescriptorType EDescriptorType_fromString(const str& value) {
		if (value == "texture") return EDescriptorType::Texture;
		else if (value == "uniform_buffer") return EDescriptorType::UniformBuffer;
		else
			assert(false);
	}


	bool VertexAttributeFromJSON(const json& j, VertexAttribute& attribute) {
		if (!j.is_object()) return false;


		if (j.contains("location") && j["location"].is_number_unsigned())
			attribute.location = j["location"];
		else
			return false;

		if (j.contains("offset") && j["offset"].is_number_unsigned())
			attribute.offset = j["offset"];
		else
			return false;

		if (j.contains("type") && j["type"].is_string()) {
			attribute.type = EScalarType_fromString(j["type"]);
			if (attribute.type == EScalarType::unknown) return false;
		}
		else
			attribute.type = EScalarType::f32; // we can *probably* infer this

		if (j.contains("count") && j["count"].is_number_unsigned())
			attribute.count = j["count"];
		else
			return false;

		if (j.contains("normalize") && j["normalize"].is_number_unsigned())
			attribute.normalize = j["normalize"];
		else
			attribute.normalize = false;

		return true;


	}

	bool VertexAttributeBindingFromJSON(const json& j, VertexAttributeBinding& binding) {

		if (j.contains("input_rate") && j["input_rate"].is_string()) {
			if (j["input_rate"] == "per-vertex")
				binding.input_rate = InputRate::PerVertex;
			else if (j["input_rate"] == "per-instance")
				binding.input_rate = InputRate::PerInstance;
			else
				return false;
		}
		else {
			return false;
		}

		if (j.contains("stride") && j["stride"].is_number_unsigned())
			binding.stride = j["stride"];
		else
			return false;

		VertexAttribute vert_attribute;
		bool attribute_valid = true;

		if (j.contains("attributes") && j["attributes"].is_array()) {
			for (const auto& attribute : j["attributes"]) {
				if (!VertexAttributeFromJSON(attribute, vert_attribute))
					return false;
				binding.attributes.push_back(vert_attribute);
			}
		} else
			return false;

		if (j.contains("slot") && j["slot"].is_number_unsigned())
			binding.slot = j["slot"];
		else
			return false;

		return true;

	}

	bool ShaderModuleFromJSON(const json& j, ShaderModule& module) {
		if (!j.is_object()) return false;

		if (j.contains("type") && j["type"].is_string()) {
			module.type = ShaderType_fromString(j["type"]);
			if (module.type == ShaderType::Unknown) return false;
		}

		if (j.contains("path") && j["path"].is_string())
			module.source = loadText(j["path"]);
		else if (j.contains("source") && j["source"].is_string())
			module.source = j["source"];
		else
			return false;

		return true;


	}

	bool VertexLayout_recipeFromJSON(const json& j, VertexLayout_recipe& layout) {
		if (!j.is_array()) return false;
		
		VertexAttributeBinding binding;


		for (const auto& buffer_binding : j) {

			if (VertexAttributeBindingFromJSON(buffer_binding, binding))
				layout.bindings.push_back(binding);
			else
				return false;

		}
		

		return true; // for now
	}

	bool Pipeline_recipeFromJSON(const json& j, Pipeline_recipe& pipeline) {
		if (!j.is_object()) return false;


		if (j.contains("topology") && j["topology"].is_string())
			pipeline.topology = PrimitiveTopology_fromString(j["topology"]);
		else
			pipeline.topology = PrimitiveTopology::Triangle;

		if (j.contains("layout") && j["layout"].is_array()) {
			if (!VertexLayout_recipeFromJSON(j["layout"], pipeline.layout))
				return false;
		} else
			return false;

		ShaderModule shader_module;

		if (j.contains("shader_modules") && j["shader_modules"].is_array()) {
			for (const auto& module : j["shader_modules"]) {

				if (ShaderModuleFromJSON(module, shader_module))
					pipeline.shader_modules.push_back(shader_module);
				else
					return false;

			}
		}

		return true;

	}

	bool DescriptorBindingFromJSON(const json& j, DescriptorBinding& out) {
		if (!j.is_object()) return false;

		// for now, it's dead simple - just push the type

		if (j.contains("type") && j["type"].is_string()) {
			out.type = EDescriptorType_fromString(j["type"]);
			return true;
		}

		return false;

	}

	bool DescriptorSetFromJSON(const json& j, DescriptorSet_recipe& out) {
		if (!j.is_array()) return false;
		DescriptorBinding binding;
		for (const auto& item : j) {
			bool status = DescriptorBindingFromJSON(item, binding);
			if (status)
				out.push_back(binding);
			else
				return false;
		}
		return true;
	}

	bool loadPipelinesFromJSON(
		const json& j,
		std::unordered_map<str, Pipeline_recipe>& pipelines
	) {
		if (!j.is_object()) return false;

		if (!j.contains("pipelines") || !j["pipelines"].is_object()) return false;

		Pipeline_recipe pipeline_recipe;
		for (const auto& [pipeline_name, pipeline_details] : j["pipelines"].items()) {
			if (Pipeline_recipeFromJSON(pipeline_details, pipeline_recipe)) {
				pipelines[pipeline_name] = pipeline_recipe;
			} else return false;
		}




	}

	bool loadDescriptorSetsFromJSON(
		const json& j,
		std::unordered_map<str, DescriptorSet_recipe>& out)
	{
		if (!j.is_object())
			return false;

		if (!j.contains("descriptor_sets"))
			return false;

		const auto& sets = j["descriptor_sets"];

		if (!sets.is_object())
			return false;

		for (const auto& [name, info] : sets.items())
		{
			assert(!out.contains(name));

			DescriptorSet_recipe recipe;

			if (!DescriptorSetFromJSON(info, recipe))
				return false;

			out.emplace(name, std::move(recipe));
		}

		return true;
	}


}