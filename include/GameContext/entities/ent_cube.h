#pragma once
#include <include/GameContext/base/Entity.h>
#include <include/GameContext/base/Scene.h>
#include <include/GameContext/resources/res_texture.h>
#include <include/GameContext/resources/mesh.h>
//#include <include/thirdparty/glm/vec3.hpp>



//struct Scale : glm::vec3 {};
// cube entity

namespace Quakaster::entities {
	struct ent_cube : Entity {
		//static mesh cube_mesh; // this is legal because it's static

		struct tag : public Tag {};

		//static void init()

		ent_cube(Scene& scene) : Entity(scene) {

			// Initialize the vertices and indices of the cube
			std::vector<default_vertex_t> vertices = {
				{{0.0f, 0.0f, 0.0f}},  // Vertex 0: Bottom-left-back
				{{100.0f, 0.0f, 0.0f}},  // Vertex 1: Bottom-right-back
				{{100.0f, 1.0f, 0.0f}},  // Vertex 2: Top-right-back
				{{0.0f, 1.0f, 0.0f}},  // Vertex 3: Top-left-back
				{{0.0f, 0.0f, 100.0f}},  // Vertex 4: Bottom-left-front
				{{100.0f, 0.0f, 100.0f}},  // Vertex 5: Bottom-right-front
				{{100.0f, 1.0f, 100.0f}},  // Vertex 6: Top-right-front
				{{0.0f, 1.0f, 100.0f}},  // Vertex 7: Top-left-front
			};
			std::vector<uint32_t> indices = {
				// Back face
				0, 2, 1,  // Flipped 2 and 1
				0, 3, 2,  // Flipped 3 and 2
				// Front face
				5, 6, 4,  // Flipped 6 and 4
				6, 7, 4,  // Flipped 7 and 4
				// Left face
				0, 7, 3,  // Flipped 7 and 3
				0, 4, 7,  // Flipped 4 and 7
				// Right face
				1, 6, 5,  // Flipped 6 and 5
				1, 2, 6,  // Flipped 2 and 6
				// Top face
				3, 6, 2,  // Flipped 6 and 2
				3, 7, 6,  // Flipped 7 and 6
				// Bottom face
				0, 1, 5,  // Flipped 1 and 5
				0, 5, 4   // Flipped 5 and 4
			};
			add_component<ent_cube::tag>();
			add_component<Mesh>(
				vertices,
				indices,
				"default_shader",
				"resources/shaders/default.vert",
				"resources/shaders/default.frag"
				);
			add_component<Transform>();
			add_component<Texture>("resources/images/atlas.png");
		}

		static inline ent_cube& cast(Entity& ent) {
			// Assuming that `ent` is guaranteed to be of type `ent_cube`.
			return static_cast<ent_cube&>(ent);
		}



		static inline ent_cube* try_cast(Entity& entity) {
			return dynamic_cast<ent_cube*>(&entity);
		}


		// getters, setters, actions
		inline Transform& transform() { return get_component<Transform>(); }
		inline Mesh& mesh() { return get_component<Mesh>(); }
		inline Texture& texture() { return get_component<Texture>(); }


		// Operations forwarded from Transform

		inline void move(const glm::vec3& delta_pos) { transform().move(delta_pos); }
		inline void move_to(const glm::vec3& new_pos) { transform().move_to(new_pos); }
		inline glm::vec3 get_forward_vector() { return transform().get_forward_vector(); }
		inline glm::vec3 get_up_vector() { return transform().get_up_vector(); }
		inline glm::vec3 get_right_vector() { return transform().get_right_vector(); }
		inline glm::vec3 get_euler_angles() { return transform().get_euler_angles(); }
		inline glm::mat4 get_matrix() { return transform().get_matrix(); }
		inline void look_at(const glm::vec3& target) { transform().look_at(target); }

		// Additional operation to look at the target
		inline void look_at(Entity& target) {
			if (target.has<Transform>()) transform().look_at(target.get_component<Transform>().position);
			else
			{
				__debugbreak();
			}
		}

	};
}

using ent_cube = Quakaster::entities::ent_cube;