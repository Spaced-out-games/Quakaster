// mesh_manager.h

#pragma once
#include <include/graphics/VAO.h>
#include <include/graphics/VBO.h>
#include <include/graphics/EBO.h>
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <include/resources/res_shader.h>
/// <summary>
/// Defines per-mesh state information
/// </summary>
struct MeshInfo {
	// Number of references
	uint32_t ref_count = 0; 
	// Upon destruction of a Mesh, does it auto-destroy when ref_count = 0?
	bool cache = 0;
	// insert the first instance in the beginning
	uint32_t next_transform_index = 0; 

	// make sure to keep a VBO with the matrices on standby
	GLuint instance_matrices = 0;

	Shader shader;
};

struct MeshInstance {
	void draw();
	void bind();
	void destroy_instance();
	void submit_transform(const glm::mat4& new_orientation);
	private:
		// cannot be constructed except by MeshManager
		friend class MeshManager;
		MeshInstance() = default;
		MeshInstance(GLuint vao, uint32_t transform_index) : vao(vao), transform_index(transform_index) {}

		GLuint vao;
		uint32_t transform_index;

};


// might be better as a namespace
struct MeshManager {
	static inline std::unordered_map<std::string, GLuint> name_map;
	static inline std::unordered_map<GLuint, MeshInfo> info_map;

	static MeshInstance load(const std::string& name_or_path) {
		if (name_map.find(name_or_path) != name_map.end()) {
			return MeshInstance(); // Missing semicolon added
		}
		// No implementation on actual file loading (yet)
		return MeshInstance(); // Prevent compiler warning/error
	}

	// flexible for all vertex types you could possibly want. vertex_t must implement vertex_t::set_pointers()
	template <typename vertex_t>
	static MeshInstance load(const std::string& name_or_path, std::vector<vertex_t>& vertices, std::vector<uint32_t> indices = {}) {
		// if indices array is empty, don't add an EBO
	}
};

