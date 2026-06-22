/// **************************************** QUAKASTER ENGINE **************************************** 
/// media/LoadModel.h (needs moved to gfx/ )
/// Purpose: Loads JSON-based graphics pipelines
/// **************************************************************************************************
#pragma once
#include <HMaterial.h>
#include <VertexLayoutDesc.h>
#include <unordered_map>
#include <Result.h>
#include "json.h"
#include "core.h" // for str

using namespace qk::gfx;

using qk::Result;

namespace qk::media {

	// todo: give actual allocated assets instead.

	using VertexLayoutMap = std::unordered_map<str, rhi::VertexLayout_recipe>;

	using MaterialLayoutMap = std::unordered_map<str, rhi::Material_recipe>;

	using DescriptorSetLayoutMap = std::unordered_map<str, rhi::DescriptorSet_createinfo>;


	// Expects ["layouts"] 
	Result<VertexLayoutMap, str> VertexLayoutsFromJSON(const json& j);

	// Expects ["materials"]
	Result <MaterialLayoutMap, str> MaterialsFromJSON(const json& j);

	// Expects ["descriptor_sets"]
	Result<DescriptorSetLayoutMap, str> DescriptorSetsFromJSON(const json& j);


}