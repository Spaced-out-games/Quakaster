#pragma once
#include "../../glmplus.h"



struct Triangle {
    glm::vec3 points[3];
};

struct BSP_plane {
    BSP_plane(glm::vec3 normal, float distance) : normal(normal), distance(distance) {}
    BSP_plane(Triangle t);
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

    //std::vector<BSP_plane> create_planes(MeshLoadResult geometry);
    std::vector<BSP_plane> create_planes(std::vector<Triangle> geometry);
    std::vector<BSP_plane> create_planes(std::vector<glm::vec3> points, std::vector<unsigned int> indices);
};




BSP_plane::BSP_plane(Triangle t) {
    glm::vec3 edge_1 = t.points[1] - t.points[0];
    glm::vec3 edge_2 = t.points[2] - t.points[0];
    normal = glm::normalize(glm::cross(edge_1, edge_2));

    distance = glm::dot(normal, t.points[0]);
}

std::vector<BSP_plane> BSP_plane::create_planes(std::vector<glm::vec3> points, std::vector<unsigned int> indices) {
    return std::vector<BSP_plane>();
}
