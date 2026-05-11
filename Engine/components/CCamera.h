/// **************************************** QUAKASTER ENGINE **************************************** 
/// math/CCamera.h
/// Purpose: Defines a minimal Camera definition. Represents a projection matrix and nothing more.
/// **************************************************************************************************

#pragma once
#include "core.h"
#include <glm/matrix.hpp>                 // mat4 type

namespace qk::entity {

	class QK_API CCamera {
		float m_FovRadians;
		float m_Near;
		float m_Far;
		float m_AspectRatio;

	public:
		glm::mat4 projection() const;
		CCamera(float fov_degrees, float near, float far, float aspect_ratio);

		void set_fov(float fov_degrees);
		float fov() const;

		void set_near(float near);
		float near() const;

		void set_far(float far);
		float far() const;

		void set_aspect(float new_aspect);
		float aspect() const;

	};

}