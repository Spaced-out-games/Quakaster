#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <include/thirdparty/glm/glm.hpp>
#include <imgui.h>
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h>
#include <include/GameContext/UI/console_message.h>
#include <include/GameContext/components/transform.h>


struct Renderer
{
	Renderer(ConsoleInterpreter& interpreter)
	{
		interpreter.add_convar("renderer_sample_mode", renderer_sample_mode, false);
		interpreter.add_command("r_sample_mode", Renderer::r_sample_mode_fn);
	}
	SDL_GLContext gl_context = nullptr;
	ImGuiContext* gui_context = nullptr;
	GLenum renderer_sample_mode = GL_LINEAR;

	
	
	// Console function for setting the render mode
	static void r_sample_mode_fn(console_message& msg, ConsoleInterpreter& interpreter, std::span<Token> args) {
		switch (args.size())
		{
		case 0:
			msg.message = "Defines what texture sampling method to use. Options are GL_LINEAR and GL_NEAREST";
			msg.color = console_color::ORANGE;
			return;
		case 1:
			if (args[0].token_type == TOKEN_IDENTIFIER)
			{
				std::string& key = args[0].snippet;
				if (key == "GL_LINEAR") { interpreter.set_convar("renderer_sample_mode", GL_LINEAR); msg.message = "set r_sample_mode to GL_LINEAR"; msg.color = console_color::DEFAULT_OKAY; return; }
				else if (key == "GL_NEAREST") { interpreter.set_convar("renderer_sample_mode", GL_NEAREST); msg.message = "set r_sample_mode to GL_LINEAR"; msg.color = console_color::DEFAULT_OKAY;  return; }
				else { msg.message = "INVALID KEY"; msg.color = console_color::DEFAULT_WARNING; return; }

			}
			else
			{
				msg.message = "Invalid input; expected GL_LINEAR or GL_NEAREST";
				msg.color = console_color::DEFAULT_WARNING;
				return;
			}
		default:
			break;
		}

	}
};