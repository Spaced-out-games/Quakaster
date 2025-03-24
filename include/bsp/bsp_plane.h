// bsp_plane.h
// PURPOSE: Define a plane equation for use by BSP trees.
#pragma once
#include <glmplus.h>
#include "triangle.h"

struct BSP_Plane {

    // Default constructor
    BSP_Plane();

    // Direct instantiation
    BSP_Plane(glm::vec3 normal, float distance);

    // Constructor from triangle
    BSP_Plane(const Triangle& triangle);

    // Normal vector
    glm::vec3 normal = { 0.0f, 0.0f, 0.0f };

    // Signed distance from the origin
    float distance = 0.0f;

    inline bool point_is_in_front(glm::vec3 point) const;

    inline float distance_from_plane(glm::vec3 point) const;

    // Find the nearest point to the origin point that lies on the plane
    inline glm::vec3 project_to_plane(glm::vec3 origin) const;

    // Cast a ray from origin to plane using direction
    inline glm::vec3 project_to_plane(glm::vec3 origin, glm::vec3 direction) const;


    // Does a line segment intersect the plane?
    inline bool intersects(glm::vec3 start, glm::vec3 end);
};

BSP_Plane::BSP_Plane() : normal({ 0.0f, 0.0f, 0.0f }), distance(0.0) {}

BSP_Plane::BSP_Plane(glm::vec3 normal, float distance) : normal(normal), distance(distance) {}

// Constructor from a Triangle
BSP_Plane::BSP_Plane(const Triangle& triangle) {
    normal = triangle.normal();  // Compute normal from triangle
    distance = glm::dot(normal, triangle.points[0]); // Compute distance using plane equation
}

float BSP_Plane::distance_from_plane(glm::vec3 point) const {
    return glm::dot(normal, point) - distance;
}

bool BSP_Plane::point_is_in_front(glm::vec3 point) const {
    return distance_from_plane(point) > 0;
}

// Find the nearest point to the origin point that lies on the plane
glm::vec3 BSP_Plane::project_to_plane(glm::vec3 origin) const {
    float d = distance_from_plane(origin);
    return origin - d * normal;  // Project the point onto the plane
}

// Cast a ray from origin to the plane using direction
glm::vec3 BSP_Plane::project_to_plane(glm::vec3 origin, glm::vec3 direction) const {
    float d = distance_from_plane(origin);
    float t = -d / glm::dot(normal, direction);  // Find intersection along direction
    return origin + t * direction;  // Inductive linear interpolation
}

bool BSP_Plane::intersects(glm::vec3 start, glm::vec3 end) {
    float d_start = distance_from_plane(start);
    float d_end = distance_from_plane(end);

    // If either point lies exactly on the plane, consider it as intersecting. Might want to add some floating
    if (d_start == 0.0f || d_end == 0.0f) {
        return true; // Intersection at the plane
    }

    // Check for opposite signs indicating intersection
    return (d_start * d_end < 0);
}

