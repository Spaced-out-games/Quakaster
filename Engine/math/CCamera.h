/// **************************************** QUAKASTER ENGINE **************************************** 
/// math/CCamera.h
/// Purpose: Minimal header-only Camera. Represents a projection matrix and nothing more.
/// **************************************************************************************************

#pragma once

#include "core/linalg.h"
#include <glm/gtc/matrix_transform.hpp> // glm::perspective
#include <algorithm>              // std::max

namespace qk {

    class QK_API CCamera {
        float m_FovRadians;
        float m_Near;
        float m_Far;
        float m_AspectRatio;

    public:
        // ------------------- Constructors -------------------
        CCamera(float fov_degrees = 90.0f, float near = 0.1f, float far = 1000.0f, float aspect_ratio = 16.0f / 9.0f)
            : m_FovRadians(glm::radians(fov_degrees))
            , m_Near(std::max(near, 0.0001f))
            , m_Far(std::max(far, near + 0.0001f))
            , m_AspectRatio(aspect_ratio)
        {}

        // ------------------- Projection Matrix -------------------

        inline qk::mat4 projection() const {
            return glm::perspective(m_FovRadians, m_AspectRatio, m_Near, m_Far);
        }

        // ------------------- FOV -------------------

        inline void set_fov(float fov_degrees) { m_FovRadians = glm::radians(fov_degrees); }
        inline float fov() const { return glm::degrees(m_FovRadians); }

        // ------------------- Near -------------------

        inline void set_near(float near_val) { m_Near = std::max(near_val, 0.0001f); }
        inline float near() const { return m_Near; }

        // ------------------- Far -------------------

        inline void set_far(float far_val) { m_Far = std::max(far_val, m_Near + 0.0001f); }
        inline float far() const { return m_Far; }

        // ------------------- Aspect -------------------

        inline void set_aspect(float new_aspect) { m_AspectRatio = new_aspect; }
        inline float aspect() const { return m_AspectRatio; }
    };

} // namespace qk