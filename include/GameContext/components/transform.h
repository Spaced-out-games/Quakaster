#pragma once
#include <include/thirdparty/glm/glm.hpp> // For glm::vec3
#include <include/thirdparty/glm/gtc/quaternion.hpp> // For glm::quat
#include <include/thirdparty/glm/gtc/matrix_transform.hpp>
#include <include/thirdparty/glm/gtc/type_ptr.hpp>
#include <include/thirdparty/glm/mat4x4.hpp> // For glm::mat4
#include <cmath>

struct Transform {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); // Position of the transform
    glm::quat rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)); // Rotation as a quaternion

    // Get the transformation matrix
    glm::mat4 get_matrix() const {
        glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
        return matrix * glm::mat4_cast(rotation);
    }

    glm::mat4 get_matrix(entt::entity entity, entt::registry scene) const
    {
        glm::mat4 parent_transform = scene.get<Transform>(entity).get_matrix();
        return Transform::get_matrix() * parent_transform;
    }

    glm::mat4 get_matrix(entt::handle owner) const {
        glm::mat4 matrix = glm::mat4(1.0f); // Initialize the matrix to identity

        // Check if the owner has a Transform component
        if (owner.all_of<Transform>()) {
            // Get the root transform
            Transform& other = owner.get<Transform>();

            // Calculate the world position by combining the current position with the root position
            glm::vec3 world_position = other.position + position;

            // Create the world transformation matrix
            matrix = glm::translate(glm::mat4(1.0f), world_position) * glm::mat4_cast(other.rotation * rotation);
        }
        else {
            // If there's no root transform, just return the local transform
            matrix = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation);
        }

        return matrix; // Return the final world transformation matrix
    }

    // Get the forward vector (negative Z in OpenGL)
    glm::vec3 get_forward_vector() const {
        return glm::normalize(rotation * glm::vec3(0.0f, 0.0f, -1.0f));
    }

    // Get the right vector (positive X)
    glm::vec3 get_right_vector() const {
        return glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
    }

    // Get the up vector (positive Y)
    glm::vec3 get_up_vector() const {
        return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
    }

    // Set the rotation to look at a target
    void look_at(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f)) {
        glm::vec3 direction = glm::normalize(target - position);
        if (glm::length(direction) == 0.0f) return; // Avoid singularities
        glm::quat targetRotation = glm::quatLookAt(direction, up);
        rotation = glm::normalize(targetRotation);
    }

    // Move the transform to a new position
    void move_to(const glm::vec3& new_position) {
        position = new_position;
    }

    // Translate the transform by a delta
    void move(const glm::vec3& delta_position) {
        position += delta_position;
    }

    // Rotate by delta pitch/yaw in radians
    void rotate_from_mouse(float delta_pitch, float delta_yaw, float sensitivity = 0.1f) {
        // Scale the deltas by sensitivity
        delta_pitch *= sensitivity;
        delta_yaw *= sensitivity;

        // Calculate forward vector pitch (to clamp)
        glm::vec3 forward = get_forward_vector();
        float pitchAngle = glm::degrees(glm::asin(forward.y));

        // Clamp pitch to avoid flipping
        if ((pitchAngle > 89.0f && delta_pitch < 0) || (pitchAngle < -89.0f && delta_pitch > 0)) {
            delta_pitch = 0;
        }

        // Update yaw and pitch
        glm::quat yawRotation = glm::angleAxis(delta_yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat pitchRotation = glm::angleAxis(delta_pitch, get_right_vector());

        rotation = glm::normalize(yawRotation * rotation);
        rotation = glm::normalize(rotation * pitchRotation);
    }

    // Utility to get Euler angles for debugging or UI
    glm::vec3 get_euler_angles() const {
        return glm::eulerAngles(rotation);
    }
};
