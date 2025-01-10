#pragma once
#include <include/GameContext/components/camera.h>
#include <include/GameContext/IO/controller.h>
#include <include/thirdparty/entt.hpp>
//#include <include/GameContext/GameContext.h>

extern float deltaTime;

struct ent_controller : public EventListener {
	entt::handle target;
	ent_controller(entt::dispatcher& dispatcher, entt::handle& handle): EventListener(dispatcher, ALL_EVENTS), target(handle)
	{
		on_keyPress = [this](KeyPressEvent& evt)
		{
			if (target.all_of<Transform>())
			{

				Transform& transform = target.get<Transform>();
				//glm::vec3& position = target.get<Transform>().position;
				std::cout << deltaTime;
				switch (evt.code)
				{
				case SDLK_a:
					transform.translate(transform.get_right_vector() * deltaTime * 10.0f);
					break;
				case SDLK_d:
					transform.translate(-transform.get_right_vector() * deltaTime * 10.0f);
					break;
				case SDLK_w:
					transform.translate(-transform.get_forward_vector() * deltaTime * 10.0f);
					break;
				case SDLK_s:
					transform.translate(transform.get_forward_vector() * deltaTime * 10.0f);
					//transform.position += transform.get_forward_vector() * deltaTime * 10.0f;
					break;

				default:
					break;
				}

			}
		};
		on_keyRelease = [this](KeyReleaseEvent& evt) {};

		on_keyHold = [this](KeyHoldEvent& evt)
		{
			if (target.all_of<Transform>())
			{

				Transform& transform = target.get<Transform>();
				//glm::vec3& position = target.get<Transform>().position;
				std::cout << deltaTime;
				switch (evt.code)
				{
				case SDLK_a:
					transform.translate(transform.get_right_vector() * deltaTime * 10.0f);
					break;
				case SDLK_d:
					transform.translate(-transform.get_right_vector() * deltaTime * 10.0f);
					break;
				case SDLK_w:
					transform.translate(-transform.get_forward_vector() * deltaTime * 10.0f);
					break;
				case SDLK_s:
					transform.translate(transform.get_forward_vector() * deltaTime * 10.0f);
					//transform.position += transform.get_forward_vector() * deltaTime * 10.0f;
					break;

				default:
					break;
				}

			}
		};

		on_mouseMove = [this](MouseMoveEvent& evt)
		{
			// Get FOV of owning entity
			float fov = 90.0f; // Default FOV
			if (target.all_of<Camera>())
			{
				fov = target.get<Camera>().fov;
			}

			// Convert FOV to radians for calculations
			float fovRadians = glm::radians(fov);

			// Calculate pitch from WINDOW_WIDTH, WINDOW_HEIGHT, and fovRadians
			float pitch = 0.01 * evt.yrel;
			float yaw = 0.01 * evt.xrel;

			

			// Use the yaw and pitch to update the camera direction or transform
			// Assuming you have a function to update the camera's orientation
			Transform& target_transform = target.get<Camera>();
			//target_transform.rotate(pitch, yaw, 0.0f);

//			target_transform.rotate(pitch, yaw, 0.0f);
		};

	}
};