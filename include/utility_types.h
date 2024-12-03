#pragma once


// Templated function pointer. Easy to read and understand, lol
template <typename return_t, typename... arg_t>
using func_ptr_t = return_t(*)(arg_t...);

#include <string>

struct SourceLocation {
    std::string file;      // Name of the file
    int line;              // Line number
    std::string function;  // Function name

    SourceLocation(const std::string& f, int l, const std::string& func)
        : file(f), line(l), function(func) {}
};

#define SOURCE_LOCATION SourceLocation(__FILE__, __LINE__, __func__)


std::string get_error_details_string(const SourceLocation& location) {
    std::string fullMessage = "\nFile: " + location.file +
        "\nLine: " + std::to_string(location.line) +
        "\nFunction: " + location.function;
    return fullMessage;
}
