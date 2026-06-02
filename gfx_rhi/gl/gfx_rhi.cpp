#include "gfx_rhi.h"
#include <glad/glad.h>


namespace qk::gfx::rhi {
	int init(SymbolLoader loader) {

		int result = gladLoadGLLoader((GLADloadproc)loader);
		
		#ifdef _DEBUG
			//glEnable(GL_PROGRAM_POINT_SIZE);
		#endif
		return result;
	}
}