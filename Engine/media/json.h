#pragma once
#include <nlohmann/json.hpp>

namespace qk::media {

	using json = nlohmann::json;


	bool validate_path(const std::filesystem::path& path);


	std::string load_text(const std::filesystem::path& path);


	json load_json(const std::filesystem::path& path);

}