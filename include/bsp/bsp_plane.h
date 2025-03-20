#pragma once
#include "../../glmplus.h"
#include <vector>
#include <numeric>
#include <functional>
/*
struct Triangle {
    glm::vec3 points[3];
};

using BSP_ptr_primitive_t = uint32_t;

struct BSP_ptr {
    BSP_ptr_primitive_t isLeaf : 1;      // Flag to indicate if this is a leaf node
    BSP_ptr_primitive_t contents : sizeof(uint32_t) - 1; // Contents, index or material index

    // Constructor
    BSP_ptr() : isLeaf(0), contents(0) {} // Default to not a leaf, empty
};

// Represents a plane in the BSP
struct BSP_plane {
    BSP_plane(glm::vec3 normal, float distance) : normal(normal), distance(distance) {}
    BSP_plane(Triangle t);
    glm::vec3 normal;  // Plane normal
    float distance;    // Signed distance from origin

    bool IsIntersecting(const glm::vec3& point) const {
        return glm::dot(normal, point) >= distance;
    }

    glm::vec3 get_point_of_intersection(const glm::vec3& start, const glm::vec3& direction) const {
        float denom = glm::dot(normal, direction);
        if (glm::abs(denom) < 1e-6f) {
            return start; // No intersection, return start as a fallback
        }
        float t = (distance - glm::dot(normal, start)) / denom;
        return start + t * direction;
    }

    static std::vector<BSP_plane> create_planes(std::vector<glm::vec3> points, std::vector<unsigned int> indices);
};

// Triangle to BSP_plane conversion
BSP_plane::BSP_plane(Triangle t) {
    glm::vec3 edge_1 = t.points[1] - t.points[0];
    glm::vec3 edge_2 = t.points[2] - t.points[0];
    normal = glm::normalize(glm::cross(edge_1, edge_2));
    distance = glm::dot(normal, t.points[0]);
}

// Create BSP_planes from vertices and indices
std::vector<BSP_plane> BSP_plane::create_planes(std::vector<glm::vec3> points, std::vector<unsigned int> indices) {
    std::vector<BSP_plane> planes;
    planes.reserve(indices.size() / 3);

    Triangle tri;

    for (size_t i = 0; i < indices.size(); i += 3) {
        tri.points[0] = points[indices[i + 0]];
        tri.points[1] = points[indices[i + 1]];
        tri.points[2] = points[indices[i + 2]];
        planes.emplace_back(tri);
    }

    return planes;
}

// Represents a node in the BSP tree
struct BSPNode {
    BSP_plane plane;        // The plane for this node
    BSP_ptr front;         // Pointer information for the front child
    BSP_ptr back;          // Pointer information for the back child
    std::vector<Triangle> triangles; // Store triangles for this node

    BSPNode(const BSP_plane& p)
        : plane(p), front(), back() {} // Initialize pointers to not a leaf
};

class BSP_Tree {
public:
    BSP_Tree(const std::vector<BSP_plane>& planes) {
        rootIndex = BuildBSP(planes);
    }

    bool IsPointInBSP(const glm::vec3& point) const {
        return IsPointInBSPRecursive(rootIndex, point);
    }

private:
    std::vector<BSPNode> nodes; // Vector to hold BSP nodes
    int rootIndex; // Index of the root node

    int BuildBSP(const std::vector<BSP_plane>& planes) {
        std::vector<int> planeIndices(planes.size());
        std::iota(planeIndices.begin(), planeIndices.end(), 0);

        // Helper function to build the BSP recursively
        std::function<int(std::vector<int>&)> build = [&](std::vector<int>& indices) -> int {
            if (indices.empty()) {
                return -1; // Base case: no planes left
            }

            int nodeIndex = nodes.size();
            nodes.emplace_back(planes[indices[0]]); // Create a new node with the first plane

            // Classify planes into front and back
            std::vector<int> frontPlanes;
            std::vector<int> backPlanes;

            for (size_t i = 1; i < indices.size(); ++i) {
                int currentIndex = indices[i];
                const BSP_plane& currentPlane = planes[currentIndex];

                // Classify the plane based on its position relative to the root plane
                if (nodes[nodeIndex].plane.IsIntersecting(currentPlane.normal)) {
                    frontPlanes.push_back(currentIndex);
                }
                else {
                    backPlanes.push_back(currentIndex);
                }
            }

            // Recursively build the front and back BSP trees and store their indices
            nodes[nodeIndex].front.contents = build(frontPlanes); // Set front contents
            nodes[nodeIndex].back.contents = build(backPlanes);   // Set back contents

            // Set leaf status
            nodes[nodeIndex].front.isLeaf = (frontPlanes.empty() && nodes[nodeIndex].front.contents == -1);
            nodes[nodeIndex].back.isLeaf = (backPlanes.empty() && nodes[nodeIndex].back.contents == -1);

            return nodeIndex; // Return the index of the constructed node
        };

        return build(planeIndices); // Start the BSP construction
    }

    bool IsPointInBSPRecursive(int nodeIndex, const glm::vec3& point) const {
        if (nodeIndex < 0) {
            return false; // Base case: No node means no collision
        }

        const BSPNode& node = nodes[nodeIndex];

        // Check if the point is in front of the plane
        if (node.plane.IsIntersecting(point)) {
            // If front is a leaf
            if (node.front.isLeaf) {
                return false; // No triangles in an empty leaf
            }
            return IsPointInBSPRecursive(node.front.contents, point); // Traverse the front child
        }
        else {
            // If back is a leaf
            if (node.back.isLeaf) {
                for (const Triangle& triangle : node.triangles) {
                    if (IsPointInTriangle(triangle, point)) {
                        return true; // Collision detected
                    }
                }
                return false; // No collision in an empty leaf
            }
            return IsPointInBSPRecursive(node.back.contents, point); // Continue to back child
        }
    }

    bool IsPointInTriangle(const Triangle& triangle, const glm::vec3& point) const {
        // Barycentric coordinates method
        glm::vec3 v0 = triangle.points[1] - triangle.points[0];
        glm::vec3 v1 = triangle.points[2] - triangle.points[0];
        glm::vec3 v2 = point - triangle.points[0];

        float dot00 = glm::dot(v0, v0);
        float dot01 = glm::dot(v0, v1);
        float dot02 = glm::dot(v0, v2);
        float dot11 = glm::dot(v1, v1);
        float dot12 = glm::dot(v1, v2);

        float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

        // Check if the point is inside the triangle
        return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
    }
};

std::vector<BSP_plane> CreateCubePlanes() {
    std::vector<BSP_plane> planes;

    // Positive X Plane: x = 1
    planes.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    // Negative X Plane: x = -1
    planes.emplace_back(glm::vec3(-1.0f, 0.0f, 0.0f), -1.0f);
    // Positive Y Plane: y = 1
    planes.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
    // Negative Y Plane: y = -1
    planes.emplace_back(glm::vec3(0.0f, -1.0f, 0.0f), -1.0f);
    // Positive Z Plane: z = 1
    planes.emplace_back(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);
    // Negative Z Plane: z = -1
    planes.emplace_back(glm::vec3(0.0f, 0.0f, -1.0f), -1.0f);

    return planes;
}

*/