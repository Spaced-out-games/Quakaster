#pragma once
#include "glew_surrogate.h"
#include <include/graphics/VAO.h>
#include <include/graphics/VBO.h>
#include <include/graphics/EBO.h>
#include <include/thirdparty/entt.hpp>
#include <include/components/transform.h>
#include <include/resources/res_shader.h>
#include <glmplus.h>
#include <include/resources/mesh_manager.h>



	// Code reused almost word-for-word from vector_visualizer
struct AABB
{
	friend struct AABB_system;

	AABB(float width, float height, float depth): scale(width, height, depth) {
		mesh = MeshManager::get_mesh("AABB");
	}

	private:
		MeshInstance mesh;
		glm::vec3 scale;
};


struct AABB_system : ISystem {
	void init(QKScene& scene) override {
		std::vector<default_vertex_t> vertices = {
			{{-0.5f, -0.5f, -0.5f}},  // Vertex 0: Bottom-left-back
			{{ 0.5f, -0.5f, -0.5f}},  // Vertex 1: Bottom-right-back
			{{ 0.5f,  0.5f, -0.5f}},  // Vertex 2: Top-right-back
			{{-0.5f,  0.5f, -0.5f}},  // Vertex 3: Top-left-back
			{{-0.5f, -0.5f,  0.5f}},  // Vertex 4: Bottom-left-front
			{{ 0.5f, -0.5f,  0.5f}},  // Vertex 5: Bottom-right-front
			{{ 0.5f,  0.5f,  0.5f}},  // Vertex 6: Top-right-front
			{{-0.5f,  0.5f,  0.5f}},  // Vertex 7: Top-left-front
		};
		std::vector<uint32_t> indices = {
			// Bottom face
			0, 1,  // Edge between Vertex 0 and Vertex 1
			1, 2,  // Edge between Vertex 1 and Vertex 2
			2, 3,  // Edge between Vertex 2 and Vertex 3
			3, 0,  // Edge between Vertex 3 and Vertex 0

			// Top face
			4, 5,  // Edge between Vertex 4 and Vertex 5
			5, 6,  // Edge between Vertex 5 and Vertex 6
			6, 7,  // Edge between Vertex 6 and Vertex 7
			7, 4,  // Edge between Vertex 7 and Vertex 4

			// Vertical edges connecting bottom and top faces
			0, 4,  // Edge between Vertex 0 and Vertex 4
			1, 5,  // Edge between Vertex 1 and Vertex 5
			2, 6,  // Edge between Vertex 2 and Vertex 6
			3, 7   // Edge between Vertex 3 and Vertex 7
		};

		MeshManager::generate_mesh(
			"AABB",
			vertices,
			indices,
			"AABB_shader",
			"resources/shaders/AABB.vert",
			"resources/shaders/AABB.frag",
			1024,
			GL_LINES

		);
	}

	void tick(QKScene& scene) override {
		auto view = scene.view<AABB, Transform>();
		for (auto entity : view) {
			glm::mat4 transform = Matrix{}
				.translate(scene.get<Transform>(entity).position)
				.scale(scene.get<AABB>(entity).scale);


				




			scene.get<AABB>(entity).mesh.submit(transform);
		}
	}

	void destroy(QKScene&) {}
};
