#pragma once
#include <GL/glew.h>
#include <include/GameContext/graphics/VAO.h>
#include <include/GameContext/graphics/VBO.h>
#include <include/GameContext/graphics/EBO.h>
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/components/transform.h>
#include <include/GameContext/resources/res_shader.h>

struct vector_visualizer: public Transform
{
	static std::vector<default_vertex_t> vertices;
	static std::vector<uint32_t> indices;
	static res_shader shader; // Not initialized until the first construction to prevent errors with OpenGL

	static void init()
	{
		vector_visualizer::shader.compile_and_link("resources/shaders/vector_visualizer.vert", "resources/shaders/vector_visualizer.frag");
	}

	vector_visualizer()
	{
		vao.bind();


		vbo.init(vertices);
		//vbo.bind();

		ebo.init(indices);
		ebo.bind();
		default_vertex_t::set_pointers();


		VAO::unbind();

		//VBO::unbind();
		EBO::unbind();
		

	}
	static void draw_all(entt::registry& registry, Camera& camera)
	{
		auto view = registry.view<vector_visualizer>();
		for (auto entity : view) {
			vector_visualizer& visualizer = registry.get<vector_visualizer>(entity);

			if (registry.all_of<Transform>(entity))
			{
				shader["u_model"] = registry.get<Transform>(entity).get_matrix();
			}
			else
			{
				shader["u_model"] = glm::mat4(1.0f);
			}


			// bind the shader

			shader.bind();
			visualizer.vao.bind();
			visualizer.vbo.bind();
			visualizer.ebo.bind();

			glDrawElements(GL_LINES, visualizer.ebo.get_index_count(), GL_UNSIGNED_INT, 0);


			VAO::unbind();
			EBO::unbind(); // Unbind the EBO after drawing


		}
	}



	glm::vec3 vector;
	
	VAO vao;
	VBO vbo;
	EBO ebo;
	
};


std::vector<default_vertex_t> vector_visualizer::vertices = {
	{{0.0,0.0,0.0}},
	{{1.0,0.0,0.0}},
	{{0.0,1.0,0.0}},
	{{0.0,0.0,1.0}}

};
std::vector<uint32_t> vector_visualizer::indices = {
	0,1,
	0,2,
	0,3
};
res_shader vector_visualizer::shader;