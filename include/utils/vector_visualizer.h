#pragma once
#include <GL/glew.h>
#include <include/graphics/VAO.h>
#include <include/graphics/VBO.h>
#include <include/graphics/EBO.h>
#include <include/thirdparty/entt.hpp>
#include <include/components/transform.h>
#include <include/resources/res_shader.h>
#include <glmplus.h>
#include <include/resources/mesh_manager.h>

struct vector_visualizer
{
	MeshInstance mesh;
	vector_visualizer() {
		mesh = MeshManager::get_mesh("vector_visualizer");
	}
		
		
	struct system : ISystem {
		void init(QKScene& scene) override {
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
				
			MeshManager::generate_mesh("vector_visualizer",vertices, indices, "vector_visualizer_shader", "resources/shaders/vector_visualizer.vert", "resources/shaders/vector_visualizer.frag",256, GL_LINES);


		}

		void tick(QKScene& scene) override {
			auto view = scene.view<vector_visualizer, Transform>();
			for (auto entity : view) {
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), scene.get<Transform>(entity).position + scene.get<Camera>(entity).owner_transform.get_forward_vector());

				glm::vec3 scalar = { 1.0,1.0,1.0 };

				if (scene.has<Scalar>(entity)) scalar = scene.get<Scalar>(entity);

				if (scene.has<MoveState>(entity)) transform = glm::scale(transform, scene.get<MoveState>(entity).velocity() * scalar);

				scene.get<vector_visualizer>(entity).mesh.submit(transform);
			}
		}
			
		void destroy(QKScene& scene) override {

		}
	};





private:


};
