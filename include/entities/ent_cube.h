#pragma once
#include <include/base/Entity.h>
#include <include/base/Scene.h>
#include <include/resources/res_texture.h>
#include <include/components/AABB.h>
#include <include/resources/mesh_manager.h>



struct ent_cube : QKEntity {
	//static mesh cube_mesh; // this is legal because it's static

	struct tag : Tag {};

	struct system : ISystem {
		// just initializes the cube mesh
		void init(QKScene&) {

			std::vector<default_vertex_t> vertices = {
				{{-0.5f, -0.5f, -0.5f}},  // Vertex 0: Bottom-left-back
				{{ 0.5f, -0.5f, -0.5f}},  // Vertex 1: Bottom-right-back
				{{ 0.5f,  0.5f, -0.5f}},  // Vertex 2: Top-right-back
				{{-0.5f,  0.5f, -0.5f}},  // Vertex 3: Top-left-back
				{{-0.5f, -0.5f,  0.5f}},  // Vertex 4: Bottom-left-front
				{{ 0.5f, -0.5f,  0.5f}},  // Vertex 5: Bottom-right-front
				{{ 0.5f,  0.5f,  0.5f}},  // Vertex 6: Top-right-front
				{{-0.5f,  0.5f,  0.5f}}   // Vertex 7: Top-left-front
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



			MeshManager::generate_mesh(
				"cube",
				vertices,
				indices,
				"default_shader",
				"resources/shaders/default.vert",
				"resources/shaders/default.frag"
			);
		}
		void tick(QKScene&) {}
		void destroy(QKScene&) {}
	};


	//static void init()

	ent_cube(QKScene& scene, glm::mat4 matrix) : QKEntity(scene) {
		//add<Transform>();

		add<MeshInstance>("cube");
		get<MeshInstance>().submit(matrix);
			
		//add<AABB>(scale.x, scale.y, scale.z);
		add<ent_cube::tag>();

	}

	static inline ent_cube& cast(QKEntity& ent) {
		// Assuming that `ent` is guaranteed to be of type `ent_cube`.
		return static_cast<ent_cube&>(ent);
	}



	static inline ent_cube* try_cast(QKEntity& entity) {
		return dynamic_cast<ent_cube*>(&entity);
	}


	// getters, setters, actions
	inline Transform& transform() { return get<Transform>(); }
	inline MeshInstance& mesh() { return get<MeshInstance>(); }
	inline AABB& aabb() { return get<AABB>(); }

	inline void rescale(glm::vec3 scale) {
		// mesh().submit(glm::scale)
	}

};