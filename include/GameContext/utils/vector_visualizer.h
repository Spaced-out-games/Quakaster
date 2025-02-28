#pragma once
#include <GL/glew.h>
#include <include/GameContext/graphics/VAO.h>
#include <include/GameContext/graphics/VBO.h>
#include <include/GameContext/graphics/EBO.h>
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/components/transform.h>
#include <include/GameContext/resources/res_shader.h>
#include <include/GameContext/resources/mesh.h>

using namespace Quakaster;

struct vector_visualizer: public Transform
{
	vector_visualizer(glm::vec3& inspected_vector, Transform& transform): vector(inspected_vector), transform(transform)
	{
	}
	glm::vec3& vector;
	Transform& transform;

	static void init()
	{
		std::vector<default_vertex_t> vertices = {
			{{0.0,0.0,0.0}},
			{{1.0,0.0,0.0}},
			{{0.0,1.0,0.0}},
			{{0.0,0.0,1.0}},
			{{0.577f,0.577f,0.577f} },


		};
		std::vector<uint32_t> indices = {
			0,1,
			0,2,
			0,3,
			0,4
		};
		//vector_visualizer::shader.init()
		vector_visualizer::mesh.init(vertices, indices, "vector_visualizer_shader", "resources/shaders/vector_visualizer.vert", "resources/shaders/vector_visualizer.frag");
	}

	static void draw_all(entt::registry& registry, components::Camera& camera)
	{
		Shader& shader = vector_visualizer::mesh.shader;
		mesh.vao.bind();
		shader->bind();

		//show_uniforms(shader->program_ID);

		auto view = registry.view<vector_visualizer>();
		for (auto entity : view)
		{
			auto& visualizer = registry.get<vector_visualizer>(entity);
			// Consider moving this outside of the loop, should improve performance
			camera.set_shader_uniforms(shader);

			// std::cout << visualizer.transform.position.x;

			shader->operator[]("u_model") = glm::translate(glm::mat4(1.0f), visualizer.transform.position + glm::vec3{0.0,-1.0f,0.0});

			//shader->operator[]("u_proj") = glm::mat4(1.0);

			//shader->operator[]("u_view"); // FIX MEEEEEE
			shader->operator[]("u_vector") = registry.get<vector_visualizer>(entity).vector;

			glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);


		}
	}

	private:

		//static Shader shader;

		// This will be excluded from mesh::draw_all, since this isn't registered in any entt::registry ; )
		static Quakaster::components::mesh mesh;
};




//Shader vector_visualizer::shader;
Quakaster::components::mesh vector_visualizer::mesh;