#include "LoadModel.h"
#include <iostream>
#include <format>



namespace qk::media {

	// TODO: handle empty checks.

	inline rhi::InputRate InputRate_cast(const std::string& v) {
		if (v == "vertex") return rhi::PerVertex;
		if (v == "instance") return rhi::PerInstance;
		return rhi::InputRate::Unknown;
	}

	inline rhi::EScalarType EScalarType_cast(const std::string& v) {
		if (v == "i8") return rhi::EScalarType::i8;
		if (v == "i16") return rhi::EScalarType::i16;
		if (v == "i32") return rhi::EScalarType::i32;
		if (v == "i64") return rhi::EScalarType::i64;

		if (v == "u8") return rhi::EScalarType::u8;
		if (v == "u16") return rhi::EScalarType::u16;
		if (v == "u32") return rhi::EScalarType::u32;
		if (v == "u64") return rhi::EScalarType::u64;

		if (v == "f32") return rhi::EScalarType::f32;
		if (v == "f64") return rhi::EScalarType::f64;

		return rhi::EScalarType::unknown;
	}

	inline rhi::EScalarMultiplicity EScalarMultiplicity_cast(const std::string& v) {
		if (v == "scalar") return rhi::Scalar;
		if (v == "vector") return rhi::Vector;
		if (v == "matrix") return rhi::Matrix;

		return rhi::EScalarMultiplicity::None;
	}

	inline rhi::EDescriptorType EDescriptorType_cast(const std::string& v) {
		if (v == "texture") return rhi::EDescriptorType::Texture;
		if (v == "uniform") return rhi::EDescriptorType::UniformBuffer;
		if (v == "storage") return rhi::EDescriptorType::StorageBuffer;
		return rhi::EDescriptorType::Unknown;
	}



	// ---------------------------------------------------------------------------------------------------
	// Vertex buffer layouts
	// ---------------------------------------------------------------------------------------------------

	Result<rhi::VertexAttribute_createinfo, std::string> VertexAttributeFromJSON(const json& j, u32& location) {
		assert(j.is_object() && j.contains("type"));

		// rename to `result`
		rhi::VertexAttribute_createinfo info = {};
		// location
		if (j.contains("location")) {
			if (!j["location"].is_number_unsigned()) {
				return std::string{ "`location` must be a uint. " };
			}

			info.location = j["location"];

		}
		else {
			info.location = location;
			location++;
		}

		// offset
		if (!j.contains("offset")) return std::string{ "`offset` is required!" };
		if (!j["offset"].is_number_unsigned()) return std::string{ "`offset` must be a uint." };

		info.offset = j["offset"];

		// type
		if (!j.contains("type")) return std::string{ "`type` is required!" };
		if (!j["type"].is_string()) return std::string{ "`type` must be a string" };

		info.component.type = EScalarType_cast(j["type"]);

		// multiplicity
		if (!j.contains("multiplicity")) return std::string{ "`multiplicity` is required!" };
		if (!j["multiplicity"].is_string()) return std::string{ "`multiplicity` must be a string!" };
		info.component.multiplicity = EScalarMultiplicity_cast(j["multiplicity"]);

		// count
		if (!j.contains("count")) return std::string{ "`count` is required!" };
		if (!j["count"].is_number_unsigned()) return std::string{ "`count` must be a uint." };
		info.component.count = j["count"];

		// normalize
		if (j.contains("normalize")) {
			const auto& norm = j["normalize"];
			if (!norm.is_boolean() || norm.is_number_unsigned()) return std::string{ "`normalize` must be a bool or uint." };
			info.component.normalize = (bool)norm;
		}

		return info;
	}
	

	Result <rhi::VertexAttributeBinding, std::string> VertexAttributeBindingFromJSON(const json& j) {
		u32 location = 0;
		assert(j.is_object());
		assert(j.contains("stride"));
		rhi::VertexAttributeBinding result;
		

		// input rate
		if (j.contains("input_rate") && j["input_rate"].is_string())
			result.input_rate = InputRate_cast(j["input_rate"]);
		else
			result.input_rate = rhi::PerVertex;

		// stride
		if (!j.contains("stride")) return std::string{ "Missing `stride` value!" };

		if (!j["stride"].is_number_unsigned()) return std::string{ "`stride` must be a uint." };

		result.stride = j["stride"];

		if (!j.contains("attributes")) return std::string{ "No attributes!" };

		const auto& attribute_list = j["attributes"];

		for (const auto& attribute_contents : attribute_list) {

			const auto& attr_result = VertexAttributeFromJSON(attribute_contents, location);

			if (attr_result.has_error()) return std::string{ attr_result.error() };

			result.attributes.push_back(attr_result.value());

		}

		return result;

	}


	// ---------------------------------------------------------------------------------------------------
	// Vertex buffer layouts
	// ---------------------------------------------------------------------------------------------------


	Result<VertexLayoutMap, std::string> VertexLayoutsFromJSON(const json& j) {

		assert(j.is_object());

		// more asserts are needed!
		

		VertexLayoutMap result;
		for (const auto& [layout_name, layout_info] : j.items()) {

			for (const auto& binding_info : layout_info["bindings"]) {
				const auto& layout_res = VertexAttributeBindingFromJSON(binding_info);
				if (layout_res.has_error()) return std::string{ layout_res.error() };
				result[layout_name].push_back(layout_res.value());

			}
		}
		

		return result;

 	}





	Result<rhi::Descriptor, std::string> DescriptorFromJSON(size_t id, const json& j) {
		if (j.empty()) return std::string{ std::format("Descriptor id={} was empty", id) };

		if (!j.contains("type")) return std::string{std::format("Descriptor id={} did not have a type", id)};

		return std::string{};
	}




	Result<rhi::DescriptorSet_createinfo, std::string> DescriptorSetFromJSON(const std::string& id, const json& j) {
		if (j.empty()) return std::string{ std::format("Descriptor set `{}` was empty", id) };
		
		return std::string{};
	}




	Result<rhi::DescriptorSet_createinfo, std::string> DescriptorSetsFromJSON(const json& j) {
		return std::string{};
	}
























	// ---------------------------------------------------------------------------------------------------
	// Materials
	// ---------------------------------------------------------------------------------------------------


	Result<rhi::Material_createinfo, std::string> MaterialFromJSON(const std::string& mat_name, const json& j) {

		if (j.empty()) return std::string{ std::format("Material `{}` was empty.", mat_name) };

		// get the vertex shader source code
		std::string vertSrc = "";
		if (j.contains("vert_shader_path")) {
			if (!j["vert_shader_path"].is_string()) return std::string{ "key `vert_shader_path` must be a string." };
			vertSrc = loadText(j["vert_shader_path"]);
		}
		else if (j.contains("vert_shader_source")) {
			if (!j["vert_shader_path"].is_string()) return std::string{ "key `vert_shader_source` must be a string." };
			vertSrc = j["vert_shader_path"];

		}
		else {
			return std::string{ "Material must contain vertex shader source code or path" };
		}


		// get the frag shader source code
		std::string fragSrc = "";
		if (j.contains("frag_shader_path")) {
			if (!j["frag_shader_path"].is_string()) return std::string{ "key `frag_shader_path` must be a string." };
			fragSrc = loadText(j["frag_shader_path"]);
		}
		else if (j.contains("frag_shader_source")) {
			if (!j["frag_shader_path"].is_string()) return std::string{ "key `frag_shader_source` must be a string." };
			fragSrc = j["frag_shader_path"];

		}
		else {
			return std::string{ "Material must contain vertex shader source code or path" };
		}
	}




	Result <MaterialLayoutMap, std::string> MaterialsFromJSON(const json& j) {

		assert(j.contains("mat_solid"));

		if (!j.is_object()) return std::string{ "All Material descriptions must be objects." };

		MaterialLayoutMap result;

		for (const auto& [mat_name, mat_attributes] : j.items()) {
			
			const auto& material = MaterialFromJSON(mat_name, mat_attributes);

			if (material) {
				result[mat_name] = material.value();
			}
			else {
				return std::string{ material.error() };
			}

		}

		return result;


	}

























































}