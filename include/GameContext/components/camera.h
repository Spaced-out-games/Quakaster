#pragma once
#include <include/GameContext/components/transform.h>
#include <include/GameContext/server/interpreter/ConsoleInterpreter.h>
#include <include/GameContext/server/interpreter/InterpreterToken.h>
#include <span>
#include <include/GameContext/UI/console_message.h>
#include <include/GameContext/server/interpreter/console_function.h>
//#include <include/GameContext/resources/res_shader.h>
#include <include/GameContext/base/Component.h>


using namespace Quakaster::base;

namespace Quakaster::tags {
	struct target_camera : Tag {};
}


namespace Quakaster::components {

	struct Camera : Component, Transform
	{
		Camera(float fov = 90.0f, float near = 0.1f, float far = 1000.0f) : fov(fov), near(near), far(far) { move_to({ 1.0,2.0,5.0 }); }

		static inline Camera* target_camera = nullptr;

		float fov;
		float near;
		float far;
		//entt::handle target;

		static void set_target(entt::handle& entity) {
			entt::registry* registry = entity.registry(); // Get the registry from the entity

			// Remove target_camera tag from all entities
			auto view = registry->view<tags::target_camera>();
			for (entt::entity ent_ID : view) {
				registry->remove<tags::target_camera>(ent_ID);
			}

			// Add target_camera tag to the specified entity
			entity.emplace<tags::target_camera>();

			// Update the static pointer to point to the current camera instance
			target_camera = entity.try_get<Camera>();
		}

		static void set_target(Entity& entity) {
			//entt::registry* registry = entity.registry(); // Get the registry from the entity
			Scene& registry = entity.get_scene();


			// Remove target_camera tag from all entities
			auto view = registry.view<tags::target_camera>();
			for (entt::entity ent_ID : view) {
				registry.remove_component<tags::target_camera>(ent_ID);
			}

			// Add target_camera tag to the specified entity
			entity.add_component<tags::target_camera>();

			// Update the static pointer to point to the current camera instance
			target_camera = entity.try_get_component<Camera>();
		}

		// tag to discern which camera is the active camera. 
		

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
		/*
		void set_shader_uniforms(Shader& shader, entt::entity owner = entt::null, entt::registry* registry = nullptr)
		{
			if (registry)
			{
				if (registry->all_of<Transform>(owner))
				{
					shader->operator[]("u_view") = glm::inverse(get_matrix());


					// Meant to get the relative transform
					//shader->operator[]("u_view") = glm::inverse(get_matrix(owner, *registry));
				}

			}
			else
			{
				shader->operator[]("u_view") = glm::inverse(get_matrix());// *parent_transform;
			}
			shader->operator[]("u_proj") = get_projection_matrix();
		}
		*/


		void bind_convars(ConsoleInterpreter& interpreter)
		{
			interpreter.add_convar("fov", fov, false);
			interpreter.add_command("fov_desired", set_fov);
		}





	};


}

using Camera = Quakaster::components::Camera;

/*
			shader->operator[]("u_view") = glm::inverse(get_matrix());
			shader->operator[]("u_proj") = get_projection_matrix();
*/

