// mesh_manager.h

#pragma once
#include <include/graphics/VAO.h>
#include <include/graphics/VBO.h>
#include <include/graphics/EBO.h>
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <include/resources/res_shader.h>
#include <include/thirdparty/tiny_obj_loader.h>
/// <summary>
/// Defines per-mesh state information
/// </summary>
struct MeshInfo {
	MeshInfo() = default;

	// the model info. VAO is in the hash maps
	VBO vbo;
	EBO ebo;

	// make sure to keep a VBO with the matrices on standby
	VBO instance_matrix_buffer;


	// What type of mesh this is
	GLenum primitive_type = GL_TRIANGLES;


	// Number of references to this resource
	uint32_t ref_count = 0;
	// Upon destruction of a Mesh, does it auto-destroy when ref_count = 0?
	bool cache = 0;

	// Which index the next instance of this mesh will have in the model matrix array
	uint32_t next_instance_index = 0;



	// the shader // TODO: make it use deffered rendering; make albedo_shader, etc etc eventually
	Shader shader; //

	// the texture // TODO: make it use deffered rendering; make albedo_texture, etc etc eventually
	Texture* texture = nullptr; //

	// self explanatory
	size_t vertex_count = 0; //
	// self explanatory
	size_t index_count = 0; //

	// Number of instances allocated in the instance VBO
	size_t capacity = 0;

};

struct MeshInstance {

	void bind() const { glBindVertexArray(vao); }
	static void unbind() { glBindVertexArray(0); }


	// delete copy constructors...might change this in the future for convenience
	//MeshInstance(const MeshInstance&) = delete;
	//MeshInstance& operator=(const MeshInstance&) = delete;

	// delete move constructors
	//MeshInstance(MeshInstance&&) = delete;
	//MeshInstance& operator(MeshInstance&&) = delete;

//private:
	// cannot be constructed except by MeshManager
	friend class MeshManager;
	MeshInstance() = default;
	MeshInstance(GLuint vao, uint32_t transform_index) : vao(vao), transform_index(transform_index) {}

	GLuint vao = 0; // no harm no foul copy of it
	uint32_t transform_index = 0; // okay maybe some foul

};


struct MeshManager {
	// these will be made private at a later date
	static inline std::unordered_map<std::string, GLuint> name_map;
	static inline std::unordered_map<GLuint, MeshInfo> info_map;


	static void submit(GLuint vao, size_t instance_index, const glm::mat4& matrix) {

		glBindVertexArray(vao);
		// get the right vbo


		info_map[vao].instance_matrix_buffer.bind();
		glBufferSubData(GL_ARRAY_BUFFER, instance_index * sizeof(glm::mat4), sizeof(glm::mat4), &matrix);
	}

	template <typename vertex_t = default_vertex_t>
	static MeshInstance generate_mesh(
		const std::string& name,
		const std::vector<vertex_t>& vertices,
		const std::vector<uint32_t>& indices,
		std::string shader_name,
		std::string vertex_path,
		std::string fragment_path,
		uint32_t num_instances = 5,
		GLenum primitive_type = GL_TRIANGLES
	) {
		// check for the existence of this mesh already existing, but we will skip it for now
		if (name_map.find(name) != name_map.end()) {
			GLuint& vao = name_map[name];
			MeshInfo& info = info_map[vao];
			return MeshInstance(vao, info.next_instance_index++);

		}
		std::vector<glm::mat4> modelMatrices(num_instances, glm::mat4(1.0f));
		GLuint vertex_pointer_index;
		// init the vao
		GLuint vao;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// set up human-readible associations
		name_map[name] = vao;
		info_map[vao].index_count = indices.size();
		info_map[vao].vertex_count = vertices.size();
		info_map[vao].primitive_type = primitive_type;
		info_map[vao].capacity = num_instances;

		// might want to remove these
		info_map[vao].shader.init(shader_name, vertex_path, fragment_path);
		info_map[vao].texture = new Texture(std::string("resources/images/atlas.png"));
		
		// initialize instance buffer
		info_map[vao].instance_matrix_buffer.init(modelMatrices);
		info_map[vao].instance_matrix_buffer.bind();

		// and its pointers
		for (vertex_pointer_index = 0; vertex_pointer_index < 4; vertex_pointer_index++) {
			glVertexAttribPointer(vertex_pointer_index, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(vertex_pointer_index * 4 * sizeof(float)));
			glEnableVertexAttribArray(vertex_pointer_index);
			glVertexAttribDivisor(vertex_pointer_index, 1); // Update per instance
			std::cout << vertex_pointer_index;
		}

		


		// bind vbo
		info_map[vao].vbo.init(vertices);
		info_map[vao].vbo.bind();

		// bind ebo
		info_map[vao].ebo.init(indices);
		info_map[vao].ebo.bind();


		vertex_t::set_pointers(vertex_pointer_index); // set the pointers for the custom vertex type




		// unbind the VAO only
		VAO::unbind();

		return MeshInstance(vao, info_map[vao].next_instance_index++);


	}

};

void draw_all_meshes() {
	for (const auto& pair : MeshManager::info_map) {

		// bind the shader
		pair.second.shader->bind();
		// and texture
		pair.second.texture->bind();

		// don't set u_model this time.

		// Bind the VAO
		glBindVertexArray(pair.first);
		// draw em
		glDrawElementsInstanced(pair.second.primitive_type, pair.second.index_count, GL_UNSIGNED_INT, nullptr, pair.second.next_instance_index);

	}
}