#pragma once
#include <GL/glew.h>
#include <include/GameContext/graphics/VAO.h>
#include <include/GameContext/graphics/VBO.h>
#include <include/GameContext/graphics/EBO.h>
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/components/transform.h>
#include <include/GameContext/resources/res_shader.h>
#include <include/GameContext/resources/meshComponent.h>


// Code reused almost word-for-word from vector_visualizer
struct AABB
{
	AABB(glm::vec3& location): location(location)
	{
	}
	glm::vec3& location;
	glm::vec3 dimensions = {1.5,3.0,1.5};
	//glm::vec3 position;

	static void init()
	{
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

		//AABB::shader.init()
		AABB::mesh.init(vertices, indices, "AABB_shader", "resources/shaders/AABB.vert", "resources/shaders/AABB.frag");
	}

	static void draw_all(entt::registry& registry, Camera& camera)
	{
		//Shader& shader = AABB::mesh.shader;
		mesh.vao.bind();
		AABB::mesh.shader->bind();

		//show_uniforms(shader->program_ID);

		auto view = registry.view<AABB>();
		for (auto entity : view)
		{
			auto& visualizer = registry.get<AABB>(entity);
			// Consider moving this outside of the loop, should improve performance
			camera.set_shader_uniforms(AABB::mesh.shader);

			// std::cout << visualizer.transform.position.x;

			AABB::mesh.shader->operator[]("u_location") = registry.get<AABB>(entity).location;

			//shader->operator[]("u_proj") = glm::mat4(1.0);

			//shader->operator[]("u_view"); // FIX MEEEEEE
			
			AABB::mesh.shader->operator[]("u_dimensions") = registry.get<AABB>(entity).dimensions;

			glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);


		}
	}

private:

	//static Shader shader;

	// This will be excluded from meshComponent::draw_all, since this isn't registered in any entt::registry ; )
	static meshComponent mesh;
};




//Shader AABB::shader;
meshComponent AABB::mesh;