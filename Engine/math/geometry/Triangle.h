#pragma once
#include "../core/linalg.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>  // for glm::normalize, glm::cross, glm::length

using namespace glm;

namespace qk {

    struct Triangle {
        vec3 a, b, c;
    };

    inline vec3 normal(const Triangle& triangle) {
        vec3 edge1 = triangle.b - triangle.a;
        vec3 edge2 = triangle.c - triangle.a;

        vec3 n = glm::cross(edge1, edge2);

        float len = glm::length(n);
        if (len < 1e-6f) {
            // Degenerate triangle, return zero vector
            return vec3(0.0f);
        }

        return n / len; // normalize safely
    }

}