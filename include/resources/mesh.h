#pragma once
#include "glew_surrogate.h"
#include <include/graphics/VAO.h>
#include <include/graphics/VBO.h>
#include <include/graphics/EBO.h>
#include <include/thirdparty/entt.hpp>
#include <include/resources/res_shader.h>
#include <include/components/camera.h>
#include "mesh_manager.h"

namespace Quakaster::components {
	/*
	struct Mesh
	{

		Mesh() {
			//default_constructed = 1;
		}

		~Mesh() {
			vao.~VAO();
			vbo.~VBO();
			ebo.~EBO();
		}




		template <typename vertex_t>
		Mesh(const std::vector<vertex_t>& vertices, const std::vector<uint32_t>& indices, std::string shader_name, std::string vertex_path, std::string fragment_path, GLenum primitive_type = GL_TRIANGLES) : shader(shader_name, vertex_path, fragment_path)
		{
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

			shader.init(shader_name, vertex_path, fragment_path);
		}

		struct system : ISystem
		{
			// No need for a universal initializer
			void init(Scene&) override {}

			// tick
			void tick(Scene& scene) override {
				auto view = scene.view<Mesh>();
				for (auto entity : view)
				{
					auto& mesh_component = scene.get_component<Mesh>(entity);

					auto& vao = mesh_component.vao;
					auto& vbo = mesh_component.vbo;
					auto& ebo = mesh_component.ebo;
					auto& shader = mesh_component.shader;



					shader->bind();

					// Bind texture if it's available, otherwise, unbind it
					if (scene.has_all_of<Texture>(entity)) { scene.get_component<Texture>(entity).bind(); }
					else { Texture::unbind(); }

					// set the model matrix if it's available. Might be better to just store a glm::mat4 and have objects simply update the matrix, then it's as easy as loading the matrix. This would make the code much more modular as well
					if (scene.has_all_of<Transform>(entity)) { shader->operator[]("u_model") = scene.get_component<Transform>(entity).get_matrix(); }


					else
					{
						shader->operator[]("u_model") = glm::mat4(1.0f);
					}

					vao.bind();

					glDrawElements(mesh_component.vao.primitive_type, ebo.get_index_count(), GL_UNSIGNED_INT, 0); // Draw using indices
					std::string context = "";
					// check_gl_error(context);
					//VAO::unbind();
					//EBO::unbind(); // Unbind the EBO after drawing

				}
			}
			// Destroy all meshes in the scene, clean up resources
			void destroy(Scene& scene) override {

			}
		};

		inline shader_handle operator[](const std::string& uniformName) {
			return shader->operator[](uniformName);
		}

		inline void bind() { vao.bind(); }
		inline void unbind() { VAO::unbind(); }
		inline void bind_shader() { shader->bind(); }
		private:
			VAO vao;
			VBO vbo;
			EBO ebo;
			Shader shader;

	};
	*/
}

//using Mesh = Quakaster::components::Mesh;