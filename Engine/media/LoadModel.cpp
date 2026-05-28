#include "LoadModel.h"
#include <iostream>
#include <format>

using std::format;


namespace qk::media {

	// TODO: handle empty checks.

	inline rhi::InputRate InputRate_cast(const str& v) {
		if (v == "vertex") return rhi::PerVertex;
		if (v == "instance") return rhi::PerInstance;
		return rhi::InputRate::Unknown;
	}

	inline rhi::EScalarType EScalarType_cast(const str& v) {
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

	inline rhi::EScalarMultiplicity EScalarMultiplicity_cast(const str& v) {
		if (v == "scalar") return rhi::Scalar;
		if (v == "vector") return rhi::Vector;
		if (v == "matrix") return rhi::Matrix;

		return rhi::EScalarMultiplicity::None;
	}

	inline rhi::EDescriptorType EDescriptorType_cast(const str& v) {
		if (v == "texture") return rhi::EDescriptorType::Texture;
		if (v == "uniform") return rhi::EDescriptorType::UniformBuffer;
		if (v == "storage") return rhi::EDescriptorType::StorageBuffer;
		return rhi::EDescriptorType::Unknown;
	}



	// ---------------------------------------------------------------------------------------------------
	// Vertex buffer layouts
	// ---------------------------------------------------------------------------------------------------

	Result<rhi::VertexAttribute_createinfo, str> VertexAttributeFromJSON(const json& j, u32& location) {
		assert(j.is_object() && j.contains("type"));

		// rename to `result`
		rhi::VertexAttribute_createinfo info = {};
		// location
		if (j.contains("location")) {
			if (!j["location"].is_number_unsigned()) {
				return str{ "`location` must be a uint. " };
			}

			info.location = j["location"];

		}
		else {
			info.location = location;
			location++;
		}

		// offset
		if (!j.contains("offset")) return str{ "`offset` is required!" };
		if (!j["offset"].is_number_unsigned()) return str{ "`offset` must be a uint." };

		info.offset = j["offset"];

		// type
		if (!j.contains("type")) return str{ "`type` is required!" };
		if (!j["type"].is_string()) return str{ "`type` must be a string" };

		info.component.type = EScalarType_cast(j["type"]);

		// multiplicity
		if (!j.contains("multiplicity")) return str{ "`multiplicity` is required!" };
		if (!j["multiplicity"].is_string()) return str{ "`multiplicity` must be a string!" };
		info.component.multiplicity = EScalarMultiplicity_cast(j["multiplicity"]);

		// count
		if (!j.contains("count")) return str{ "`count` is required!" };
		if (!j["count"].is_number_unsigned()) return str{ "`count` must be a uint." };
		info.component.count = j["count"];

		// normalize
		if (j.contains("normalize")) {
			const auto& norm = j["normalize"];
			if (!norm.is_boolean() || norm.is_number_unsigned()) return str{ "`normalize` must be a bool or uint." };
			info.component.normalize = (bool)norm;
		}

		return info;
	}
	

	Result <rhi::VertexAttributeBinding, str> VertexAttributeBindingFromJSON(const json& j) {
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
		if (!j.contains("stride")) return str{ "Missing `stride` value!" };

		if (!j["stride"].is_number_unsigned()) return str{ "`stride` must be a uint." };

		result.stride = j["stride"];

		if (!j.contains("attributes")) return str{ "No attributes!" };

		const auto& attribute_list = j["attributes"];

		for (const auto& attribute_contents : attribute_list) {

			const auto& attr_result = VertexAttributeFromJSON(attribute_contents, location);

			if (attr_result.has_error()) return str{ attr_result.error() };

			result.attributes.push_back(attr_result.value());

		}

		return result;

	}


	// ---------------------------------------------------------------------------------------------------
	// Vertex buffer layouts
	// ---------------------------------------------------------------------------------------------------


	Result<VertexLayoutMap, str> VertexLayoutsFromJSON(const json& j) {

		assert(j.is_object());

		// more asserts are needed!
		

		VertexLayoutMap result;
		for (const auto& [layout_name, layout_info] : j.items()) {

			for (const auto& binding_info : layout_info["bindings"]) {
				const auto& layout_res = VertexAttributeBindingFromJSON(binding_info);
				if (layout_res.has_error()) return str{ layout_res.error() };
				result[layout_name].push_back(layout_res.value());

			}
		}
		

		return result;

 	}






























	// ---------------------------------------------------------------------------------------------------
	// Materials
	// ---------------------------------------------------------------------------------------------------


	Result<rhi::Material_createinfo, str> MaterialFromJSON(const str& mat_name, const json& j) {

		if (j.empty()) return str{ format("Material `{}` was empty.", mat_name) };

		// get the vertex shader source code
		str vertSrc = "";
		if (j.contains("vert_shader_path")) {
			if (!j["vert_shader_path"].is_string()) return str{ "key `vert_shader_path` must be a string." };
			vertSrc = loadText(j["vert_shader_path"]);
		}
		else if (j.contains("vert_shader_source")) {
			if (!j["vert_shader_path"].is_string()) return str{ "key `vert_shader_source` must be a string." };
			vertSrc = j["vert_shader_path"];

		}
		else {
			return str{ "Material must contain vertex shader source code or path" };
		}


		// get the frag shader source code
		str fragSrc = "";
		if (j.contains("frag_shader_path")) {
			if (!j["frag_shader_path"].is_string()) return str{ "key `frag_shader_path` must be a string." };
			fragSrc = loadText(j["frag_shader_path"]);
		}
		else if (j.contains("frag_shader_source")) {
			if (!j["frag_shader_path"].is_string()) return str{ "key `frag_shader_source` must be a string." };
			fragSrc = j["frag_shader_path"];

		}
		else {
			return str{ "Material must contain vertex shader source code or path" };
		}

		return rhi::Material_createinfo{ vertSrc, fragSrc, {} };

	}




	Result <MaterialLayoutMap, str> MaterialsFromJSON(const json& j) {

		assert(j.contains("mat_solid"));

		if (!j.is_object()) return str{ "All Material descriptions must be objects." };

		MaterialLayoutMap result;

		for (const auto& [mat_name, mat_attributes] : j.items()) {
			
			const auto& material = MaterialFromJSON(mat_name, mat_attributes);

			if (material) {
				result[mat_name] = material.value();
			}
			else {
				return str{ material.error() };
			}

		}

		return result;


	}


	Result<rhi::Descriptor, str> DescriptorFromJSON(const json& j, u32 index, const str& name) {
		assert(j.is_object());
		assert(j.contains("type") && j["type"].is_string());
		assert(j.contains("binding") && j["binding"].is_number_unsigned());

		if (!j.is_object())
			return str{format("Descriptor set `{}`, index {}: Descriptor is a non-object.", name, index)};

		if (!j.contains("type"))
			return str{ format("Descriptor set `{}`, index {}: Descriptor does not have required `type` field.", name, index) };
		if (!j["type"].is_string())
			return str{ format("Descriptor set `{}`, index {}: Descriptor has a non-string `type` field.", name, index) };

		if (!j.contains("binding"))
			return str{ format("Descriptor set `{}`, index {}: Descriptor does not have required `binding` field.", name, index) };
		if (!j["binding"].is_number_unsigned())
			return str{ format("Descriptor (set `{}`, index {}): Descriptor has a non-uint `type` field.", name, index) };

		if (!j.contains("id"))
			return str{ format("Descriptor set `{}`, index {}: Descriptor does not have required `id` field.", name, index) };
		if (!j["id"].is_string())
			return str{ format("Descriptor (set `{}`, index {}): Descriptor has a non-string `id` field.", name, index) };


		rhi::Descriptor result = {};

		result.type = EDescriptorType_cast(j["type"]);
		result.binding = j["binding"];

		switch (result.type)
		{
			case rhi::EDescriptorType::Texture:

				return str{ "Texture not implemented yet." };









			case rhi::EDescriptorType::UniformBuffer:

				result.uniform.hBuf = rhi::UniformBuffer_ctor();
				result.id = str{j["id"]}.c_str();
				return result;



			case rhi::EDescriptorType::StorageBuffer:
				return str{ "SSBO not supported." };

			default:
				return str{ "Unsupported descriptor type" };
		}


	}


	Result<rhi::DescriptorSet_createinfo, str> DescriptorSetFromJSON(const json& j, const str& name) {

		std::cout << j;


		assert(j.is_array());
		u32 i = 0;
		rhi::DescriptorSet_createinfo descriptor_set = {};
		for (const auto& descriptor : j) {
			auto desc = DescriptorFromJSON(descriptor, i, name);

			if (!desc) return str{ desc.error() };
			descriptor_set.push_back(desc.value());

			i++;
		}

		return descriptor_set;


	}


	Result<DescriptorSetLayoutMap, str> DescriptorSetsFromJSON(const json& j) {
		
		
		
		assert(j.is_object());
		assert(!j.empty());

		assert(j.contains("CameraDataDescriptorSet"));







		DescriptorSetLayoutMap result;


		for (const auto& [descriptorSetName, descriptors] : j.items()) {
			
			auto res = DescriptorSetFromJSON(descriptors, descriptorSetName);

			if (!res) return str{ res.error() };

			result[descriptorSetName] = res.value();

		}

		return result;


	}






















































}