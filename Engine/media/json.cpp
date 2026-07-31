#include "json.h"
#include "spdlog/spdlog.h"
#include <fstream>


namespace qk::media {

    std::pair<size_t, size_t> get_row_col(const std::string& text, size_t byte_pos)
    {
        size_t row = 1;
        size_t col = 1;

        for (size_t i = 0; i < byte_pos && i < text.size(); ++i) {
            if (text[i] == '\n') {
                ++row;
                col = 1;
            }
            else {
                ++col;
            }
        }

        return { row, col };
    }



    bool validatePath(const std::filesystem::path& path) {
        // no validity checks for now
        return 1;
    }


    std::string loadText(const std::filesystem::path& path) {

        auto canonical_path = std::filesystem::canonical(path);

        if (!validatePath(canonical_path)) {
            spdlog::error("Invalid path: `{}`", canonical_path.string());
        }
        std::ifstream file(canonical_path);
        if (!file) {
            spdlog::critical("Failed to open file `{}`", canonical_path.string());
            throw std::runtime_error("Failed to open file");
        }
        std::stringstream ss;
        ss << file.rdbuf();

        return ss.str();


    }


    json loadJSON(const std::filesystem::path& path)
    {
        if (path.extension() != ".json" && path.extension() != ".jsonc") {
            spdlog::critical("file `{}` is not a .json or .jsonc file!", path.string());
            throw std::runtime_error("Not a JSON file");
        }

        std::string content = loadText(path);

        bool allow_comments = true;// (path.extension() == ".jsonc"); // strict
        json j;
        try {
            j = json::parse(content, nullptr, true, allow_comments);
            return j;
        }
        catch (const json::parse_error& e) {
            auto [row, col] = get_row_col(content, e.byte);
            spdlog::critical(
                "Failed to parse JSON at {}:{}: {} (id={})",
                row, col, std::string(e.what()), e.id
            );
            throw;
        }

        return {};


    }

    void flatten_json(const json& in, json& out, const str& separator, const str& prefix) {
        str new_key;
        for (const auto& [k, v] : in.items()) {
            if (prefix.empty()) new_key = k;
            else new_key = prefix + separator + k;
            
            if (v.is_array()) {
                out[new_key] = json::array();
                for (const auto& item : v) {
                    if (item.is_object()) {
                        flatten_json(item, out[k], separator, "");
                    }
                }
            } else if (v.is_object()) {
                flatten_json(v, out, separator, new_key);
            } else  {
                out[new_key] = v;

            }
        }
    }
}