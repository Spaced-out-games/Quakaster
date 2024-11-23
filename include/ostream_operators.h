//#ifndef STREAM_OPERATORS_HPP
//#define STREAM_OPERATORS_HPP


#pragma once
#include <iostream>
#include <glm/glm.hpp>


// Overload for glm::vec2
inline std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

// Overload for glm::vec3
inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

// Overload for glm::vec4
inline std::ostream& operator<<(std::ostream& os, const glm::vec4& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return os;
}

// Overload for glm::mat4 (for matrix printing)
inline std::ostream& operator<<(std::ostream& os, const glm::mat4& mat) {
    os << "[\n";
    for (int i = 0; i < 4; ++i) {
        os << "  (";
        for (int j = 0; j < 4; ++j) {
            os << mat[i][j];
            if (j < 3) os << ", ";
        }
        os << ")";
        if (i < 3) os << ",\n";
    }
    os << "\n]";
    return os;
}
