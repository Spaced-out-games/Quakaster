#pragma once
#include "core.h"

namespace qk::gfx::rhi::gui {

	void init(const HWindow window);

	void begin_frame(float DeltaTime);

	void end_frame();

	void render();

	void shutdown();



	
}