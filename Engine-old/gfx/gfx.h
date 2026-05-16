/// **************************************** QUAKASTER ENGINE **************************************** 
/// gfx/gfx.h
/// Purpose: Defines some fundamental wrappers for managing the gfx module lifetime.
/// **************************************************************************************************


#pragma once
#include "../core.h"
#include "types.h"


namespace qk::gfx {

	// Initializes GLEW  ¯\_(ツ)_/¯
	void QK_API init();

	// Disables GLEW (lie) (real)
	void QK_API close();

	// test glClear
	void QK_API test();

	void QK_API drawArrays(unsigned int mode, int first, int count);
}