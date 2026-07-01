#include "CCamera.h"
#include <glm/gtc/matrix_transform.hpp>   // perspective


#define QK_CAMERA_ELIPSON 0.0001f

namespace qk {

	glm::mat4 CCamera::projection() const {
		return glm::perspective(m_FovRadians, m_AspectRatio, m_Near, m_Far);
	}
	CCamera::CCamera(float fov_degrees, float near, float far, float aspect_ratio) {
		set_fov(fov_degrees);
		set_near(near);
		set_far(far);
		set_aspect(aspect_ratio);
	}

	void CCamera::set_fov(float fov_degrees) {
		m_FovRadians = glm::radians(fov_degrees);
	}

	float CCamera::fov() const {
		return glm::degrees(m_FovRadians);
	}

	void CCamera::set_near(float near) {
		m_Near = std::max(m_Near, QK_CAMERA_ELIPSON);

	}

	float CCamera::near() const {
		return m_Near;
	}

	void CCamera::set_far(float far) {
		m_Far = std::max(m_Far, m_Near + QK_CAMERA_ELIPSON);
	}
	float CCamera::far() const {
		return m_Far;
	}

	void CCamera::set_aspect(float new_aspect) {
		m_AspectRatio = new_aspect;
	}

	float CCamera::aspect() const {
		return m_AspectRatio;
	}



}
