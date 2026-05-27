#pragma once
#include <HMaterial.h>
#include <VertexLayoutDesc.h>
#include <unordered_map>
#include <Result.h>
#include "json.h"

using namespace qk::gfx;

using qk::Result;

namespace qk::media {

	// todo: give actual allocated assets instead.

	using VertexLayoutMap = std::unordered_map<std::string, rhi::VertexLayout_createinfo>;

	using MaterialLayoutMap = std::unordered_map<std::string, rhi::Material_createinfo>;


	// Expects ["layouts"] 
	Result<VertexLayoutMap, std::string> VertexLayoutsFromJSON(const json& j);

	// Expects ["materials"]
	Result <MaterialLayoutMap, std::string> MaterialsFromJSON(const json& j);


}