#include "gfx_rhi.h"
#include <glad/glad.h>


namespace qk::gfx::rhi {


    static void APIENTRY gl_debug_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* user_param)
    {
        if (type == GL_DEBUG_TYPE_ERROR) {
            std::fprintf(stderr,
                "[OpenGL ERROR] id=%u: %s\n",
                id, message);
        }
        else if (severity == GL_DEBUG_SEVERITY_HIGH) {
            std::fprintf(stderr,
                "[OpenGL HIGH] id=%u: %s\n",
                id, message);
        }
    }

    void enable_gl_debugging()
    {
        GLint flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

        if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT)) {
            std::fprintf(stderr, "No OpenGL debug context!\n");
            return;
        }

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_callback, nullptr);
    }



	int init(SymbolLoader loader) {

		int result = gladLoadGLLoader((GLADloadproc)loader);
		
		#ifdef _DEBUG
			//glEnable(GL_PROGRAM_POINT_SIZE);
		#endif

        enable_gl_debugging();

		return result;
	}



	void gl_clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

}