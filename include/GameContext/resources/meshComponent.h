#pragma once
#include <GL/glew.h>
#include <include/GameContext/graphics/VAO.h>
#include <include/GameContext/graphics/VBO.h>
#include <include/GameContext/graphics/EBO.h>
#include <include/thirdparty/entt.hpp>
#include <include/GameContext/resources/res_shader.h>
#include <include/GameContext/components/camera.h>

struct meshComponent
{

	//meshComponent() {
	//	default_constructed = 1;
	//}


	template <typename vertex_t>
	meshComponent(const std::vector<vertex_t>& vertices, const std::vector<uint32_t>& indices, std::string shader_name, std::string vertex_path, std::string fragment_path, GLenum primitive_type = GL_TRIANGLES) : shader(shader_name, vertex_path, fragment_path)
	{
		std::cout << "test";
		init<vertex_t>(vertices, indices, shader_name, vertex_path, fragment_path, primitive_type);
	}


	template <typename vertex_t>
	void init(const std::vector<vertex_t>& vertices, const std::vector<uint32_t>& indices, std::string shader_name, std::string vertex_path, std::string fragment_path, GLenum primitive_type = GL_TRIANGLES)
	{
		vao.init();
		vao.bind();
		
		vbo.init(vertices);
		vbo.bind();

		ebo.init(indices);
		ebo.bind();

		//shader = res_shader::load(shader_name, vertex_path, fragment_path);

		vertex_t::set_pointers();

		// Unbind only the VAO since unbinding the others removes the relation
		VAO::unbind();


	}


	static void draw_all(entt::registry& registry, Camera& camera)
	{
		auto view = registry.view<meshComponent>();
		for (auto entity : view)
		{
			auto& mesh_component = registry.get<meshComponent>(entity);
			
			auto& vao = mesh_component.vao;
			auto& vbo = mesh_component.vbo;
			auto& ebo = mesh_component.ebo;
			auto& shader = mesh_component.shader;

			

			shader->bind();
			camera.set_shader_uniforms(shader);

			if (registry.all_of<Transform>(entity))
			{
				shader->operator[]("u_model") = registry.get<Transform>(entity).get_matrix();
			}

			vao.bind();

			glDrawElements(mesh_component.vao.primitive_type, ebo.get_index_count(), GL_UNSIGNED_INT, 0); // Draw using indices
			std::string context = "";
			check_gl_error(context);
			//VAO::unbind();
			//EBO::unbind(); // Unbind the EBO after drawing

		}
	}
	void draw(Camera& camera)
	{

	}

	bool default_constructed = 0;
	VAO vao;
	VBO vbo;
	EBO ebo;
	Shader shader;
};