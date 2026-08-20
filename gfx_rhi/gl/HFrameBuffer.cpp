#include <HFrameBuffer.h>
#include "rhi_globals.h"


namespace qk::gfx::rhi {

	FrameBuffer_impl::FrameBuffer_impl(const FrameBuffer_recipe& recipe): Resource("FrameBuffer_impl") {
		glGenFramebuffers(1, &framebuffer);
	}


	// lifetime
	HFrameBuffer FrameBuffer_ctor(const FrameBuffer_recipe& recipe) {
		return new FrameBuffer_impl(recipe);
	}
	void FrameBuffer_dtor(const HFrameBuffer& recipe) {

	}
	void FrameBuffer_free(const HFrameBuffer& recipe) {

	}




	// debug
	void FrameBuffer_bind(const HFrameBuffer& hFrameBuffer) {
		glBindFramebuffer(GL_FRAMEBUFFER, hFrameBuffer->framebuffer);
	}

	void FrameBuffer_unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


}