#pragma once
#include "core.h"

namespace qk::gfx::rhi::gui {

	// Sets up imgui with a window
	void init(const HWindow window);

	// Starts an imgui frame
	void begin_frame(float DeltaTime);

	// ends an imgui frame
	void end_frame();

	// renders the imgui frame
	void render();

	// shuts down imgui
	void shutdown();
	
}