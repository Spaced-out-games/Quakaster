#pragma once
// Include all the base 
#include <include/GameContext/base/Quakaster.h>

/// Basically an IDE hack. Allows you to basically wrap and name scopes within VS Code, my editor of choice.
/// It is nothing more than a no-op but tricks Intellisense into thinking it's a funciton when it's not
#define SECTION(x)

// Minimal C-style function pointer type
template <typename return_type, typename... argument_types>
using func_ptr_t = return_type(*)(argument_types...);

// Print a glm::vec3 to the console
std::ostream& operator<<(std::ostream& os, const glm::vec3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}



