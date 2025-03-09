#pragma once
#include "glew_surrogate.h"
#include <include/graphics/VAO.h>
#include <include/graphics/VBO.h>
#include <include/graphics/EBO.h>
#include <include/thirdparty/entt.hpp>
#include <include/components/transform.h>
#include <include/resources/res_shader.h>
#include <include/resources/mesh.h>
#include <include/components/Scalar.h>



namespace Quakaster::components {



	// Code reused almost word-for-word from vector_visualizer
	struct AABB: Component
	{
		AABB(const glm::vec3& location) : location(location) {}



		

		struct system: ISystem {
			void init(Scene& scene) override {
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

				AABB::mesh.init(vertices, indices, "AABB_shader", "resources/shaders/AABB.vert", "resources/shaders/AABB.frag");
			}

			void tick(Scene& scene) override {
				if (!Camera::get_target_camera()) return;

				mesh.bind();
				AABB::mesh.bind_shader();

				auto view = scene.view<AABB>();
				for (auto entity : view)
				{
					auto& visualizer = scene.get_component<AABB>(entity);
					AABB::mesh["u_location"] = visualizer.location;
					AABB::mesh["u_dimensions"] = visualizer.dimensions;

					glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
				}
			}
			
			void destroy(Scene& scene) override {
				// free the mesh
				AABB::mesh.~Mesh();
			}
		};


	private:
		static inline Mesh mesh;
		const glm::vec3& location;
		glm::vec3 dimensions = { 1.5, 3.0, 1.5 };
	};

}

using AABB = Quakaster::components::AABB;
