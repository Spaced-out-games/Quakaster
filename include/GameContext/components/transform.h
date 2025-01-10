#pragma once
#include <include/thirdparty/glm/glm.hpp> // For glm::vec3
#include <include/thirdparty/glm/gtc/quaternion.hpp> // For glm::quat

#include <include/thirdparty/glm/gtc/matrix_transform.hpp>
#include <include/thirdparty/glm/gtc/type_ptr.hpp>

#include <include/thirdparty/glm/mat4x4.hpp> // for glm::mat4
#include <math.h>



struct Transform {
    glm::mat4 matrix;

    Transform() : matrix(glm::mat4(1.0f)) {} // Initialize with the identity matrix

    // Getters
    const glm::vec3 get_position() const {
        return glm::vec3(matrix[3]); // Extract the translation from the matrix
    }

    const glm::mat4& get_matrix() const {
        return matrix;
    }

    // Setters
    void set_position(const glm::vec3& position) {
        matrix = glm::translate(glm::mat4(1.0f), position) * glm::mat4(glm::mat3(matrix)); // Preserve rotation and scale
    }

    void set_rotation(const glm::quat& rotation) {
        // Set rotation while preserving position
        matrix = glm::mat4_cast(rotation) * glm::translate(glm::mat4(1.0f), get_position()); // Preserve translation
    }

    // Rotate by pitch, yaw, and roll (in radians)
    void rotate(float pitch, float yaw, float roll) {
        glm::quat pitchQuat = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X
        glm::quat yawQuat = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));    // Rotate around Y
        glm::quat rollQuat = glm::angleAxis(roll, glm::vec3(0.0f, 0.0f, 1.0f));   // Rotate around Z

        // Combine the rotations and set the new rotation
        glm::quat newRotation = yawQuat * pitchQuat * rollQuat; // Apply in order: yaw, then pitch, then roll
        set_rotation(newRotation);
    }

    // Translate the transform by a given vector
    void translate(const glm::vec3& translation) {
        matrix = glm::translate(matrix, translation);
    }

    // Get the forward vector (negative Z in world space)
    glm::vec3 get_forward_vector() const {
        return glm::normalize(glm::vec3(matrix[2])); // Z-axis
    }

    // Get the right vector (X in world space)
    glm::vec3 get_right_vector() const {
        return glm::normalize(glm::vec3(matrix[0])); // X-axis
    }

    // Get the up vector (Y in world space)
    glm::vec3 get_up_vector() const {
        return glm::normalize(glm::vec3(matrix[1])); // Y-axis
    }
};
