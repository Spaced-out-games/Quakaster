/// **************************************** QUAKASTER ENGINE **************************************** 
/// media/json.h
/// Purpose: Defines an interface to load and work with JSON files
/// **************************************************************************************************
#pragma once
#include <nlohmann/json.hpp>
#include "core.h"

namespace qk::media {

	using json = nlohmann::json;

	// Validates a path to ensure it is in the whitelist and in a relative path. Not implemented as of 06/20/26
	bool validatePath(const std::filesystem::path& path);

	// Loads a text file
	std::string loadText(const std::filesystem::path& path);

	// Loads a json file
	json loadJSON(const std::filesystem::path& path);

	void flatten_json(const json& in, json& out, const str& separator = "/", const str& prefix = "");


	enum class JSONArrayClassification {
		Empty,
		NotAnArray,
		Bool,
		Int,
		UInt,
		Float,
		String,
		Object,
		Array,
		Heterogeneous
	};

	JSONArrayClassification classify(const json& j);

}