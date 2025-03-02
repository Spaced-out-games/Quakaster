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

namespace Quakaster::components {

	struct vector_visualizer : public Transform
	{
		vector_visualizer(glm::vec3& inspected_vector, Transform& transform, float scalar = 1.0f) : vector(inspected_vector), transform(transform), scalar(scalar)
		{
		}
		glm::vec3& vector;
		Transform& transform;
		float scalar;

		struct system : ISystem {
			void init(Scene& scene) override {
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
				

				vector_visualizer::mesh.init(vertices, indices, "vector_visualizer_shader", "resources/shaders/vector_visualizer.vert", "resources/shaders/vector_visualizer.frag");
			}

			void tick(Scene& scene) override {
				if (!Camera::target_camera) return;

				Shader& shader = vector_visualizer::mesh.shader;
				mesh.vao.bind();
				shader->bind();

				//show_uniforms(shader->program_ID);
				auto view = scene.view<vector_visualizer>();
				for (auto entity : view)
				{
					auto& visualizer = scene.get_component<vector_visualizer>(entity);
					glm::vec3 scaled_vector = visualizer.transform.position * visualizer.scalar;

					// Consider moving this outside of the loop, should improve performance
					//Camera::target_camera->set_shader_uniforms(shader);

					// std::cout << visualizer.transform.position.x;

					shader->operator[]("u_model") = glm::translate(glm::mat4(1.0f), visualizer.transform.position + glm::vec3{ 0.0,-1.0f,0.0 });

					//shader->operator[]("u_proj") = glm::mat4(1.0);

					//shader->operator[]("u_view"); // FIX MEEEEEE
					shader->operator[]("u_vector") = scene.get_component<vector_visualizer>(entity).vector * visualizer.scalar;

					glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);


				}
			}
			void destroy(Scene& scene) override {

			}
		};





	private:

		//static Shader shader;

		// This will be excluded from mesh::draw_all, since this isn't registered in any entt::registry ; )
		static Mesh mesh;
	};
}

using vector_visualizer = Quakaster::components::vector_visualizer;


//Shader vector_visualizer::shader;
Mesh vector_visualizer::mesh;

