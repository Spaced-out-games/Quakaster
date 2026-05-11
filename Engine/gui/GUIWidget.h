#pragma once
#include "core.h"
#include <gui_widget_rhi.h>

// temporary; for testing.
#include <glm/glm.hpp>
namespace qk::gfx::gui {

	struct QK_API Widget {

		int z = 0;

		// also the avenue for logic.
		virtual void render() = 0;

	};

	struct QK_API Vec3EditWidget : Widget {
		glm::vec3* target = nullptr;
		glm::vec3 min;
		glm::vec3 max;

		
		void render() override {
			if (!target) return;
			qk::gfx::rhi::gui::slider_float3("location", &target->x, -1e6f, 1e6f, "%.3f");

		}
	};

}