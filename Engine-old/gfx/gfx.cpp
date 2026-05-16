#include "gfx.h"
#include <stdexcept>
#include "spdlog/spdlog.h"
#include "GL/glew.h"


namespace qk::gfx {
	void init() {
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			throw std::runtime_error("Failed to initialize GLEW");
		}
		spdlog::info("gfx initialized successfully!...");
		spdlog::info("OpenGL Version: {}", (const char*)glGetString(GL_VERSION));
		spdlog::info("Renderer: {}", (const char*)glGetString(GL_RENDERER));
		spdlog::info("Vendor: {}", (const char*)glGetString(GL_VENDOR));
	}

	void close() {
		// There is no GlewTerminate() 
		// or similiar but for consistency...
		spdlog::info("gfx shutting down...");

	}


	void test() {
		glClearColor(0.1f, 0.6f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void drawArrays(unsigned int mode, int first, int count) {
		glDrawArrays(mode, first, (GLsizei)count);

	}
}