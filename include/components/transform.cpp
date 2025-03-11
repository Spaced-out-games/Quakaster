//=================================== transform.cpp ===================================//
//
//
//
//
//
//=====================================================================================//


#include "transform.h"

inline void Transform::rotate(float deltaPitch, float deltaYaw) {
    // Update the target transform's rotation using quaternions
    glm::quat pitchQuat = glm::angleAxis(deltaPitch, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X
    glm::quat yawQuat = glm::angleAxis(deltaYaw, glm::vec3(0.0f, 1.0f, 0.0f));     // Rotate around Y
    rotation = glm::normalize(yawQuat * rotation * pitchQuat);
}

glm::vec3 Transform::get_euler_angles() const {
    return glm::eulerAngles(rotation);
}

void Transform::look_at(const glm::vec3& target, const glm::vec3& up) {
    glm::vec3 direction = glm::normalize(target - position);
    if (glm::length(direction) == 0.0f) return; // Avoid singularities
    glm::quat targetRotation = glm::quatLookAt(direction, up);
    rotation = glm::normalize(targetRotation);
}

glm::vec3 Transform::get_up_vector() const {
    return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Transform::get_right_vector() const {
    return glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 Transform::get_forward_vector() const {
    return glm::normalize(rotation * glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::mat4 Transform::get_matrix() const {
    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
    return matrix * glm::mat4_cast(rotation);
}