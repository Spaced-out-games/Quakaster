#pragma once
#include <include/thirdparty/glm/glm.hpp> // For glm::vec3
#include <include/thirdparty/glm/gtc/quaternion.hpp> // For glm::quat
#include <include/thirdparty/glm/mat4x4.hpp> // for glm::mat4

struct Transform {
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    // Getters
    const glm::vec3& get_position() const { return position; }
    const glm::quat& get_rotation() const { return rotation; }

    // Returns the transformation matrix (position + rotation)
    glm::mat4 get_matrix() const {
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotate = glm::mat4_cast(rotation);
        return translate * rotate;
    }

    // Get the forward vector (negative Z in local space)
    glm::vec3 get_forward_vector() const {
        return glm::normalize(rotation * glm::vec3(0.0f, 0.0f, -1.0f));
    }

    // Get the up vector (Y in local space)
    glm::vec3 get_up_vector() const {
        return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
    }

    // Get the right vector (X in local space)
    glm::vec3 get_right_vector() const {
        return glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
    }

    // Setters
    void set_position(const glm::vec3& new_position) { position = new_position; }

    // Set rotation using Euler angles (pitch, yaw, roll in radians)
    void set_rotation(const glm::vec3& euler_angles) {
        rotation = glm::quat(euler_angles);
    }
};


