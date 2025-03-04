#pragma once
#include <include/components/transform.h>
#include <include/server/interpreter/ConsoleInterpreter.h>
#include <include/server/interpreter/InterpreterToken.h>
#include <span>
#include <include/UI/console_message.h>
#include <include/server/interpreter/console_function.h>
//#include <include/resources/res_shader.h>
#include <include/base/Component.h>





namespace Quakaster::components {

	struct Camera : Component
	{
		// Denotes that the owning entity is the camera that we need to use to draw.
		struct target_camera_tag: Tag {};

		Transform& owner_transform;

		Camera(Transform& owner_transform, float fov = 90.0f, float near = 0.1f, float far = 1000.0f) : fov(fov), near(near), far(far), owner_transform(owner_transform) {

			owner_transform.move_to({ 1.0,2.0,5.0 });

		}


		glm::vec3 offset = { 0.0,0.0,0.0 }; // Might be better to make this an optional component
		//entt::handle target;

		static void set_target(entt::handle& entity) {
			entt::registry* registry = entity.registry(); // Get the registry from the entity

			// Remove target_camera tag from all entities
			auto view = registry->view<Camera::target_camera_tag>();
			for (entt::entity ent_ID : view) {
				registry->remove<Camera::target_camera_tag>(ent_ID);
			}

			// Add target_camera tag to the specified entity
			entity.emplace<Camera::target_camera_tag>();

			// Update the static pointer to point to the current camera instance
			target_camera = entity.try_get<Camera>();
		}

		static void set_target(Entity& entity) {
			//entt::registry* registry = entity.registry(); // Get the registry from the entity
			Scene& registry = entity.get_scene();


			// Remove target_camera tag from all entities
			auto view = registry.view<Camera::target_camera_tag>();
			for (entt::entity ent_ID : view) {
				registry.remove_component<Camera::target_camera_tag>(ent_ID);
			}

			// Add target_camera tag to the specified entity
			entity.add_component<Camera::target_camera_tag>();

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


		void bind_convars(ConsoleInterpreter& interpreter)
		{
			interpreter.add_convar("fov", fov, false);
			interpreter.add_command("fov_desired", set_fov);
		}

		glm::mat4 get_matrix() const {
			glm::mat4 matrix = glm::translate(glm::mat4(1.0f), owner_transform.position + offset);
			return matrix * glm::mat4_cast(owner_transform.rotation);
		}
		static Camera* get_target_camera() { return target_camera; }

		private:
			static inline Camera* target_camera = nullptr;
			bool owns_transform = false;
			
			float fov;
			float near;
			float far;

	};




}

struct Cam_offset: glm::vec3 {};


using Camera = Quakaster::components::Camera;

/*
			shader->operator[]("u_view") = glm::inverse(get_matrix());
			shader->operator[]("u_proj") = get_projection_matrix();
*/

