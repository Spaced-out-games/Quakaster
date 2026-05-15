#pragma once
#include "core.h"
#include <gui_widget_rhi.h>
#include <string>
// temporary; for testing.
#include <glm/glm.hpp>
namespace qk::gfx::gui {

	struct QK_API Widget {

		int m_Z = 0;

		// also the avenue for logic.
		virtual void render() = 0;

	};


	// move elsewhere
	struct QK_API Vec3EditWidget : Widget {



		std::string m_Label;
		glm::vec3* m_pTarget = nullptr;
		float m_Min;
		float m_Max;

		Vec3EditWidget(const std::string& label, glm::vec3* pTarget, float min, float max) {
			m_Label = label;
			m_pTarget = pTarget;
			m_Min = min;
			m_Max = max;
		}

		
		void render() override {
			if (!m_pTarget) return;
			qk::gfx::rhi::gui::slider_float3(m_Label.c_str(), &m_pTarget->x, m_Min, m_Max, "%.3f");

		}
	};
	
}