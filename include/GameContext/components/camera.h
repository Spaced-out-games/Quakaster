#pragma once
#include <include/GameContext/components/transform.h>
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h>
#include <include/GameContext/server/interpreter/InterpreterToken.h>
#include <span>
#include <include/GameContext/UI/console_message.h>
#include <include/GameContext/server/interpreter/console_function.h>
#include <include/GameContext/resources/res_shader.h>




struct Camera: public Transform
{
	Camera(entt::handle parent, float fov = 90.0f, float near = 0.01f, float far = 1000.0f) : fov(fov), near(near), far(far), target(parent) { move_to({ 0.0,0.0,5.0 }); }
	float fov;
	float near;
	float far;
	entt::handle target;
	
	static void set_fov(console_message& msg, ConsoleInterpreter& interpreter, std::span<Token> tokens)
	{
		switch (tokens.size())
		{
		case 0:
			msg = std::string("fov_desired = ") + std::to_string(interpreter.get_convar<float>("fov"));
			return;
		case 1:


			if (tokens[0].token_type == TYPE_FLOAT)
			{
				interpreter.set_convar<float>("fov", std::get<float>(tokens[0].value));
				msg = std::string("fov_desired =") + std::to_string(interpreter.get_convar<float>("fov"));
			}
			else if (tokens[0].token_type == TYPE_INTEGER)
			{
				interpreter.set_convar<float>("fov", (float)std::get<int>(tokens[0].value));
				msg = std::string("fov_desired =") + std::to_string(interpreter.get_convar<float>("fov"));
			}


			return;

		default:
			break;
		}
	}

	glm::mat4 get_projection_matrix()
	{
		const float aspect_ratio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT; // Replace with actual aspect ratio
		return glm::perspective(glm::radians(fov), aspect_ratio, near, far);
	}
	void set_shader_uniforms(Shader& shader)
	{


		// transform the camera by the parent entity's transform
		shader->operator[]("u_view") = glm::inverse(get_matrix());// *parent_transform;
		shader->operator[]("u_proj") = get_projection_matrix();

		//shader["u_view"] = get_matrix();
		//shader["u_proj"] = get_projection_matrix();
	}

	void bind_convars(ConsoleInterpreter& interpreter)
	{
		interpreter.add_convar("fov", fov, false);
		interpreter.add_command("fov_desired", set_fov);
	}
};


struct tag_target_camera {};