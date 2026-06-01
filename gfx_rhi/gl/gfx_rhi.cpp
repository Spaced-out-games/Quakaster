#include "gfx_rhi.h"
#include <glad/glad.h>


namespace qk::gfx::rhi {
	int init(SymbolLoader loader) {
		return gladLoadGLLoader((GLADloadproc)loader);
	}
}