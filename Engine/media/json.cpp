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
    // this needs to be inline on the same in/out to reduce memory churn in large files
    void flatten_json(const json& in, json& out, const str& separator, const str& prefix) {
        str new_key;
        for (const auto& [k, v] : in.items()) {
            if (prefix.empty()) new_key = k;
            else new_key = prefix + separator + k;
            
            if (v.is_array()) {
                out[new_key] = v;
                for (const auto& item : out[new_key]) {
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

    JSONArrayClassification classify(const json& j) {
        if (j.is_null() || j.empty()) return JSONArrayClassification::Empty;
        if (!j.is_array()) return JSONArrayClassification::NotAnArray;

        bool has_bool = false;
        bool has_int = false;
        bool has_uint = false;
        bool has_float = false;
        bool has_array = false;
        bool has_object = false;
        bool has_string = false;

        for (const auto& element : j) {
            if (element.is_number_float())
                has_float = true;
            else if (element.is_number_unsigned())
                has_uint = true;
            else if (element.is_number_integer())
                has_int = true;
            else if (element.is_boolean())
                has_bool = true;
            else if (element.is_string())
                has_string = true;
            else if (element.is_array())
                has_array = true;
            else if (element.is_object())
                has_object = true;
        }
        // exclusively array of arrays
        if (has_array && !has_object && !has_bool && !has_int && !has_uint && !has_float && !has_string)
            return JSONArrayClassification::Array;

        // exclusively array of objects
        else if (has_object && !has_array && !has_bool && !has_int && !has_uint && !has_float && !has_string)
            return JSONArrayClassification::Object;
        // exclusively string array
        else if (has_string && !has_object && !has_array && !has_bool && !has_int && !has_uint && !has_float)
            return JSONArrayClassification::String;

        if (has_float)
            return JSONArrayClassification::Float;

        else if (has_int)
            return JSONArrayClassification::Int;

        else if (has_uint)
            return JSONArrayClassification::UInt;

        else if (has_bool)
            return JSONArrayClassification::Bool;

        return JSONArrayClassification::Heterogeneous;



    }
}