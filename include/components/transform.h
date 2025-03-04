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
    glm::mat4 get_matrix() const;
    
    


    // Get the forward vector (negative Z in OpenGL)
    glm::vec3 get_forward_vector() const;

    // Get the right vector (positive X)
    glm::vec3 get_right_vector() const;

    // Get the up vector (positive Y)
    glm::vec3 get_up_vector() const;

    // Set the rotation to look at a target
    void look_at(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    // Move the transform to a new position
    inline void move_to(const glm::vec3& new_position) {
        position = new_position;
    }

    // Translate the transform by a delta
    inline void move(const glm::vec3& delta_position) { position += delta_position; }

    // Rotate by delta pitch/yaw in radians
    inline void rotate(float deltaPitch, float deltaYaw);

    // Utility to get Euler angles for debugging or UI
    glm::vec3 get_euler_angles() const;
};



