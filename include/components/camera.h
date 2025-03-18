#pragma once
#include <include/components/transform.h>
#include <include/server/interpreter/ConsoleInterpreter.h>
#include <span>
#include <include/UI/console_message.h>
#include <include/base/Component.h>

Convar fov_desired("fov_desired", 90.0f);
Convar cl_near("cl_near", 0.1f);
Convar cl_far("cl_far", 100000.0f);

struct Camera : QKComponent
{
	// Denotes that the owning entity is the camera that we need to use to draw.
	struct target_camera_tag: Tag {};

		
	Camera(Transform& owner_transform, bool client_camera = true) : owner_transform(owner_transform) {
		if (client_camera) {
			fov = &(fov_desired.m_Value.m_Float);
			near = &(cl_near.m_Value.m_Float);
			far = &(cl_far.m_Value.m_Float);


		}

	}



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

	static void set_target(QKEntity& entity) {
		//entt::registry* registry = entity.registry(); // Get the registry from the entity
		QKScene& registry = entity.get_scene();


		// Remove target_camera tag from all entities
		auto view = registry.view<Camera::target_camera_tag>();
		for (entt::entity ent_ID : view) {
			registry.remove_component<Camera::target_camera_tag>(ent_ID);
		}

		// Add target_camera tag to the specified entity
		entity.add<Camera::target_camera_tag>();

		// Update the static pointer to point to the current camera instance
		target_camera = entity.try_get<Camera>();
	}

	// tag to discern which camera is the active camera. 
		


	glm::mat4 get_projection_matrix()
	{
		const float aspect_ratio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT; // Replace with actual aspect ratio
		return glm::perspective(glm::radians(*fov), aspect_ratio, *near, *far);
	}




	glm::mat4 get_matrix() const {
		glm::mat4 matrix = glm::translate(glm::mat4(1.0f), owner_transform.position + offset);
		return matrix * glm::mat4_cast(owner_transform.rotation);
	}
	static Camera* get_target_camera() { return target_camera; }

	friend class QKComponent;



	Transform& owner_transform;


	private:
		static inline Camera* target_camera = nullptr;
		glm::vec3 offset = { 0.0,0.0,0.0 }; // Might be better to make this an optional component

		float* fov = nullptr;
		float* near = nullptr;
		float* far = nullptr;

};




/*
			shader->operator[]("u_view") = glm::inverse(get_matrix());
			shader->operator[]("u_proj") = get_projection_matrix();
*/

