#pragma once
#include <SDL.h>
#include "glew_surrogate.h"
#include <include/thirdparty/glm/glm.hpp>
#include <imgui.h>
#include <include/server/interpreter/ConsoleInterpreter.h>
#include <include/UI/console_message.h>
#include <include/components/transform.h>


struct Renderer
{
	Renderer()
	{
		//interpreter.add_convar("renderer_sample_mode", renderer_sample_mode, false);
		//interpreter.add_convar("renderer_wireframe", renderer_wireframe, false);

		//interpreter.add_command("r_sample_mode", Renderer::r_sample_mode_fn);
		//interpreter.add_command("r_wireframe", Renderer::r_wireframe_fn);

		init();
		// interpreter.add_command("r_wireframe", );
	}
	SDL_GLContext gl_context = nullptr;
	ImGuiContext* gui_context = nullptr;
	inline static GLenum renderer_sample_mode = GL_LINEAR;
	int renderer_wireframe = 0;

	static void enable_face_culling()
	{
		glEnable(GL_CULL_FACE);
	}
	static void disable_face_culling()
	{
		glDisable(GL_CULL_FACE);
	}

	static void set_cull_mode(GLenum mode = GL_CCW)
	{
		glCullFace(mode); // Cull back faces (default)



	}

	void init() {
		//glEnable(GL_CULL_FACE);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		glLineWidth(4.0f);
	}
	
};