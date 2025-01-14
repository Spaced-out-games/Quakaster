#pragma once
#include <include/GameContext/components/camera.h>
#include <include/GameContext/IO/controller.h>
#include <include/thirdparty/entt.hpp>
#include <math.h>
//#include <include/GameContext/GameContext.h>

extern float deltaTime;

struct ent_controller : public EventListener {
	//entt::handle target;
	Transform& target_transform;
	float pitch = 0.0;
	float yaw = 0.0;
	bool warp = 0;
	ent_controller(entt::dispatcher& dispatcher, Transform& transform): EventListener(dispatcher, ALL_EVENTS), target_transform(transform)//target(handle)
	{
		on_keyPress = [this](KeyPressEvent& evt)
		{

			// THIS MIGHT CAUSE SOME STUPID SHIT IF NOT CAMERA!!!
			//Transform& transform = target.get<Camera>(); // This was get<Transform>
			Transform& transform = target_transform;
			//glm::vec3& position = target.get<Transform>().position;

			switch (evt.code)
			{
			case SDLK_a:
				transform.move(-transform.get_right_vector() * deltaTime * 10.0f);
				break;
			case SDLK_d:
				transform.move(transform.get_right_vector() * deltaTime * 10.0f);
				break;
			case SDLK_w:
				transform.move(transform.get_forward_vector() * deltaTime * 10.0f);
				break;
			case SDLK_s:
				transform.move(-transform.get_forward_vector() * deltaTime * 10.0f);
				break;
			case SDLK_ESCAPE:
				break;

			default:
				break;
			}

		};
		on_keyRelease = [this](KeyReleaseEvent& evt) {};

		on_keyHold = [this](KeyHoldEvent& evt)
		{
			// THIS MIGHT CAUSE SOME STUPID SHIT IF NOT CAMERA!!!
			//Transform& transform = target.get<Camera>(); // This was get<Transform>
			Transform& transform = target_transform;
			//glm::vec3& position = target.get<Transform>().position;

			switch (evt.code)
			{
			case SDLK_a:
				transform.move(-transform.get_right_vector() * deltaTime * 10.0f);
				break;
			case SDLK_d:
				transform.move(transform.get_right_vector() * deltaTime * 10.0f);
				break;
			case SDLK_w:
				transform.move(transform.get_forward_vector() * deltaTime * 10.0f);
				break;
			case SDLK_s:
				transform.move(-transform.get_forward_vector() * deltaTime * 10.0f);
				break;
			case SDLK_ESCAPE:
				break;

			default:
				break;
			}
		};

		on_mouseMove = [this](MouseMoveEvent& evt)
		{

			// Calculate pitch and yaw deltas from mouse input
			float pitchDelta = 0.01f * -evt.yrel;
			float yawDelta = 0.01f * -evt.xrel;

			// Clamp pitch to avoid flipping (e.g., within [-89, 89] degrees)
			float maxPitch = glm::radians(89.0f);
			pitch = glm::clamp(pitch + pitchDelta, -maxPitch, maxPitch);

			// Wrap yaw to stay within [0, 360) degrees
			yaw += yawDelta;
			//yaw = glm::mod(yaw, glm::radians(360.0f));
			//if (yaw < 0.0f) yaw += glm::radians(360.0f);

			// Update the target transform's rotation
			Transform& targetTransform = target_transform;

			// Calculate pitch and yaw rotations using quaternions
			glm::quat pitchQuat = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X
			glm::quat yawQuat = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));     // Rotate around Y

			// Combine pitch and yaw rotations with the current rotation
			targetTransform.rotation = glm::normalize(yawQuat * pitchQuat);

			// Reset mouse position to the center of the window
			//SDL_SetRelativeMouseMode(SDL_FALSE);
			//if(warp) SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		};

	}
};