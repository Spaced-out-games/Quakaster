// triangle.h
// PURPOSE: define a probably unnecessary and bloated triangle structure

#pragma once
#include <glmplus.h>
#include <vector>
#include <set>

struct BSP_Plane;
struct SplitResult;

struct Triangle {
    glm::vec3 points[3];

    glm::vec3 normal() const {
        return glm::normalize(glm::cross(points[1] - points[0], points[2] - points[0]));
    }

    bool intersects(const glm::vec3& point) const {
        glm::vec3 v0 = points[1] - points[0];
        glm::vec3 v1 = points[2] - points[0];
        glm::vec3 v2 = point - points[0]; // Fixed typo

        float d00 = glm::dot(v0, v0);
        float d01 = glm::dot(v0, v1);
        float d11 = glm::dot(v1, v1);
        float d20 = glm::dot(v2, v0);
        float d21 = glm::dot(v2, v1);

        float denom = d00 * d11 - d01 * d01;
        float u = (d11 * d20 - d01 * d21) / denom;
        float v = (d00 * d21 - d01 * d20) / denom;

        return (u >= 0) && (v >= 0) && (u + v <= 1);
    }

    float area() const {
        return 0.5f * glm::length(glm::cross(points[1] - points[0], points[2] - points[0]));
    }

    glm::vec3 center() const {
        return (points[0] + points[1] + points[2]) / 3.0f;
    }

    // Splits a single triangle by a plane.
    SplitResult split_by_plane(BSP_Plane plane, Triangle triangle);

};

#include "bsp_plane.h"

struct SplitResult {
    std::vector<Triangle> front;
    std::vector<Triangle> back;
};


SplitResult Triangle::split_by_plane(BSP_Plane plane, Triangle triangle) {
    // set up empty partition lists
    SplitResult result;
    // is the triangle wholly in front of the plane? If so, move it to front and return
    if ( plane.point_is_in_front(triangle.points[0]) && plane.point_is_in_front(triangle.points[1]) && plane.point_is_in_front(triangle.points[2]) ) {
        // add triangle to front
        result.front.push_back(triangle);
        return result;
    }
    // Is the triangle is wholly behind?
    else if (!plane.point_is_in_front(triangle.points[0]) && !plane.point_is_in_front(triangle.points[1]) && !plane.point_is_in_front(triangle.points[2])) {
        // add triangle to front
        result.back.push_back(triangle);
        return result;
    }

    // find out which edges intersect, as well as their points of intersection
    plane.


}


