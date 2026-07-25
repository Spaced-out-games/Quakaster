#pragma once
#include "core.h"
#include <stdint.h>
#define QK_RHI_VERSION QK_API_VERSION(1,0,0)


namespace qk::gfx::rhi {


	// Retreives symbols for GLAD function calls
	int QK_API init(SymbolLoader loader);

	void draw_test();
}