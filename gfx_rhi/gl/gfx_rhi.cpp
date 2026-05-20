#include "gfx_rhi.h"
#include <glad/glad.h>


namespace qk::gfx::rhi {
	static inline ClearChannels g_ClearChannels;
	float						g_ClearColor[4];


	GLbitfield ClearChannels_cast(qk::gfx::rhi::ClearChannels channels) {
		GLbitfield result = 0;

		if ((channels & qk::gfx::rhi::ClearChannels::Color) == qk::gfx::rhi::ClearChannels::Color)
			result |= GL_COLOR_BUFFER_BIT;

		if ((channels & qk::gfx::rhi::ClearChannels::Depth) == qk::gfx::rhi::ClearChannels::Depth)
			result |= GL_DEPTH_BUFFER_BIT;

		if ((channels & qk::gfx::rhi::ClearChannels::Stencil) == qk::gfx::rhi::ClearChannels::Stencil)
			result |= GL_STENCIL_BUFFER_BIT;

		return result;
	}

	int init(void* hLoader) {
		return gladLoadGLLoader((GLADloadproc)hLoader);
	}

	void clear(float r, float g, float b, float a, ClearChannels channels) {
		g_ClearChannels = channels;
		g_ClearColor[0] = r; g_ClearColor[1] = g;
		g_ClearColor[2] = b; g_ClearColor[3] = a;

		glClearColor(r, g, b, a);
		glClear(ClearChannels_cast(g_ClearChannels));
		

	}

	void clear() {
		glClear(ClearChannels_cast(g_ClearChannels));
	}
}