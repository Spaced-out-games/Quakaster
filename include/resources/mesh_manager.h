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

	// VBO to the model matrix list
	GLuint instance_VBO = 0;

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

private:
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
		GLuint vbo = info_map[vao].instance_VBO;


		glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
		uint32_t num_instances = 1, // allocate matrices for 1024 instances
		GLenum primitive_type = GL_TRIANGLES
	) {
		// check for the existence of this mesh already existing, but we will skip it for now
		if (name_map.find(name) != name_map.end()) {
			GLuint& vao = name_map[name];
			MeshInfo& info = info_map[vao];
			return MeshInstance(vao, info.next_instance_index++);

		}


		// generate the VAO
		GLuint vertex_pointer_index = 0;
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// add a new entry to the maps
		check_gl_error("b");
		// set up the hash maps and mesh counts
		info_map[vao] = MeshInfo{}; // default MeshInfo for now
		MeshInfo& info = info_map[vao];
		info.vertex_count = vertices.size();
		info.index_count = indices.size();
		name_map[name] = vao;

		check_gl_error("c");


		// identity matrices
		std::vector<glm::mat4> modelMatrices(num_instances, glm::mat4(1.0f));

		// bind the instance buffer VBO
		info.instance_matrix_buffer.init(modelMatrices);
		info.instance_matrix_buffer.bind();

		check_gl_error("d");

		for (unsigned int i = 0; i < 4; i++) {
			glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(i * 4 * sizeof(float)));
			glEnableVertexAttribArray(3 + i);
			glVertexAttribDivisor(3 + i, 1); // Update per instance
		}

		check_gl_error("e");


		info.capacity = num_instances;

		// bind vbo
		info.vbo.init(vertices);
		info.vbo.bind();

		check_gl_error("e");
		// bind ebo
		info.ebo.init(indices);
		info.ebo.bind();

		check_gl_error("f");

		vertex_t::set_pointers(vertex_pointer_index); // set the pointers for the custom vertex type

		check_gl_error("g");

		// we are rendering this type of primitive
		info.primitive_type = primitive_type;
		info.shader.init(shader_name, vertex_path, fragment_path); // FIX ME LATER, AND ACTUALLY, PERHAPS REMOVE!!!
		info.texture = new Texture(std::string("resources/images/atlas.png")); // FIX ME LATER, AND ACTUALLY, PERHAPS REMOVE!!!

		check_gl_error("h");

		// unbind the VAO only
		VAO::unbind();

		check_gl_error("i");

		return MeshInstance(vao, info.next_instance_index++);


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