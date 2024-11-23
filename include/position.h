#pragma once
#include "glm.hpp"
#include "entity.h"

// 2D position
using pos2D = glm::vec2;

// 3D position
using pos3D = glm::vec3;

// 2D velocity
using vel2D = glm::vec2;

// 3D velocity
using vel3D = glm::vec3;

// 2D transformation matrix
using transform2D = glm::mat3;

// 3D transformation matrix
using transform3D = glm::mat4;

using point_cloud2D = std::vector<pos2D>;

using point_cloud3D = std::vector<pos3D>;



namespace movable_2D
{
	bool is_valid(entt::entity entity) { return scene.all_of<pos2D, vel2D>(entity); }

	void add_components(entt::entity entity)
	{
		// Make sure it has a position
		if (!scene.any_of<pos2D>(entity)) { scene.emplace<pos2D>(entity); }

		// Make sure it has a velocity
		if (!scene.any_of<vel2D>(entity)) { scene.emplace<pos2D>(entity); }




	}

	void update(entt::entity entity, float deltaTime)
	{
		scene.get<pos2D>(entity) += scene.get<vel2D>(entity) * deltaTime;
	}
}