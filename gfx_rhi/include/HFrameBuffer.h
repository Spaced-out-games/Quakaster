#pragma once
#include <HTexture.h>


namespace qk::gfx::rhi {

	struct FrameBuffer_impl;

	using HFrameBuffer = FrameBuffer_impl*;





	// createinfo
	struct FrameBuffer_recipe {
		int _;
	};


	// lifetime
	HFrameBuffer FrameBuffer_ctor(const FrameBuffer_recipe& recipe);
	void FrameBuffer_dtor(const HFrameBuffer& recipe);
	void FrameBuffer_free(const HFrameBuffer& recipe);

	



	// debug
	void FrameBuffer_bind(const HFrameBuffer& hFrameBuffer);
	void FrameBuffer_unbind();



}