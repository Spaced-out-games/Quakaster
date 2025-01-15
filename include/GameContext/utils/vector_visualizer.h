#pragma once
#include <GL/glew.h>
#include <include/GameContext/graphics/VAO.h>
#include <include/GameContext/graphics/VBO.h>
#include <include/GameContext/graphics/EBO.h>
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/components/transform.h>
#include <include/GameContext/resources/res_shader.h>
#include <include/GameContext/resources/meshComponent.h>


struct vector_visualizer
{
	glm::vec3 vector;

	static void init()
	{
		std::vector<default_vertex_t> vertices = {
			{{0.0,0.0,0.0}},
			{{1.0,0.0,0.0}},
			{{0.0,1.0,0.0}},
			{{0.0,0.0,1.0}}

		};
		std::vector<uint32_t> indices = {
			0,1,
			0,2,
			0,3
		};
		//mesh.init();

		//vector_visualizer::mesh.init(vertices, indices, "vector_visualizer_shader", "resources/shaders/vector_visualizer.vert", "resources/shaders/vector_visualizer.frag");
	}

	static void draw_all(entt::registry& registry, Camera& camera)
	{
		shader->bind();
		mesh.vao.bind();
		auto view = registry.view<vector_visualizer>();
		for (auto entity : view)
		{
			// Consider moving this outside of the loop, should improve performance
			camera.set_shader_uniforms(shader);

			if (registry.all_of<Transform>(entity))
			{
				shader->operator[]("u_model") = registry.get<Transform>(entity).get_matrix();
			}
			shader->operator[]("u_vector") = registry.get<vector_visualizer>(entity).vector;

			glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);


		}
	}

	private:

		static Shader shader;

		// This will be excluded from meshComponent::draw_all, since this isn't registered in any entt::registry ; )
		//static meshComponent mesh;
};




Shader vector_visualizer::shader;
//meshComponent vector_visualizer::mesh;