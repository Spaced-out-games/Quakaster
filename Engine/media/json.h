#pragma once
#include <nlohmann/json.hpp>

namespace qk::media {

	using json = nlohmann::json;

	// todo: define some kind of interface for this to work
	bool validatePath(const std::filesystem::path& path);


	std::string loadText(const std::filesystem::path& path);


	json loadJSON(const std::filesystem::path& path);

}