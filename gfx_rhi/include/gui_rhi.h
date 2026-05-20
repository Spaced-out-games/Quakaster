#pragma once


namespace qk::gfx::rhi::gui {

	void init(const void* window);

	void begin_frame(float DeltaTime);

	void end_frame();

	void render();

	void shutdown();



	
}