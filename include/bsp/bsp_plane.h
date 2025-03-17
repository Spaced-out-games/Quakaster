#pragma once
#include "../../glmplus.h"

struct BSP_plane {
    glm::vec3 normal;  // Plane normal
    float distance;    // Signed distance from origin

    // Checks if a point is on or in front of the plane
    bool IsIntersecting(const glm::vec3& point) const {
        return glm::dot(normal, point) >= distance;
    }

    // Finds the intersection of a ray (start + t * direction) with the plane
    glm::vec3 get_point_of_intersection(const glm::vec3& start, const glm::vec3& direction) const {
        float denom = glm::dot(normal, direction);
        if (glm::abs(denom) < 1e-6f) {
            return start; // No intersection, return start as a fallback
        }
        float t = (distance - glm::dot(normal, start)) / denom;
        return start + t * direction;
    }
};