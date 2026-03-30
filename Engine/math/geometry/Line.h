#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>  // glm::normalize, glm::cross, glm::length

using namespace glm;

namespace qk {

    struct Line {
        vec3 a, b;
    };

    inline float length(const Line& line) {
        return glm::length(line.b - line.a);
    }

    inline vec3 direction(const Line& line) {
        vec3 dir = line.b - line.a;
        float len = glm::length(dir);
        if (len < 1e-6f) return vec3(0.0f); // degenerate line
        return dir / len;
    }

}