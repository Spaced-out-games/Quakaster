// mesh_manager.h

#pragma once
#include <include/graphics/VAO.h>
#include <include/graphics/VBO.h>
#include <include/graphics/EBO.h>
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <include/resources/res_shader.h>
#define TINYOBJLOADER_IMPLEMENTATION

#include <include/thirdparty/tiny_obj_loader.h>
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



struct MeshInstance;

struct MeshLoadResult {
	std::vector<glm::vec3> points;
	std::vector<unsigned int> indices;
};

struct MeshManager {
	// these will be made private at a later date
	static inline std::unordered_map<std::string, GLuint> name_map;
	static inline std::unordered_map<GLuint, MeshInfo> info_map;

	static glm::mat4 get_matrix(GLuint vao, size_t instance_index);
	static void submit(GLuint vao, size_t instance_index, const glm::mat4& matrix);

	template <typename vertex_t = default_vertex_t, typename instance_t = glm::mat4>
	static inline void generate_mesh(
		const std::string& name,
		const std::vector<vertex_t>& vertices,
		const std::vector<uint32_t>& indices,
		std::string shader_name,
		std::string vertex_path,
		std::string fragment_path,
		uint32_t num_instances = 2,
		GLenum primitive_type = GL_TRIANGLES
	);

	static inline MeshInstance get_mesh(const std::string& name);
	static inline tinyobj::ObjReader reader;
	static inline tinyobj::ObjReaderConfig config;

	static MeshLoadResult load_mesh(std::string path);

	static void load(std::string path);


	struct system : ISystem {
		virtual void init(QKScene& scene) {}
		virtual void tick(QKScene&) {
			for (const auto& pair : MeshManager::info_map) {

				// bind the shader
				pair.second.shader->bind();
				// and texture
				if(pair.second.texture) pair.second.texture->bind();


				// Bind the VAO
				glBindVertexArray(pair.first);
				// draw em
				glDrawElementsInstanced(pair.second.primitive_type, pair.second.index_count, GL_UNSIGNED_INT, nullptr, pair.second.next_instance_index);

			}
		}
		// TODO: delete all meshes
		virtual void destroy(QKScene& scene) {};
	};

};


#include <include/components/MeshInstance.h>

void MeshManager::submit(GLuint vao, size_t instance_index, const glm::mat4& matrix) {
	glBindVertexArray(vao);
	// get the right vbo


	info_map[vao].instance_matrix_buffer.bind();
	glBufferSubData(GL_ARRAY_BUFFER, instance_index * sizeof(glm::mat4), sizeof(glm::mat4), &matrix);
}


template <typename vertex_t = default_vertex_t, typename instance_t = glm::mat4>
static inline void MeshManager::generate_mesh(
	const std::string& name,
	const std::vector<vertex_t>& vertices,
	const std::vector<uint32_t>& indices,
	std::string shader_name,
	std::string vertex_path,
	std::string fragment_path,
	uint32_t num_instances,
	GLenum primitive_type
) {
	// check for the existence of this mesh already existing, but we will skip it for now
	if (name_map.find(name) != name_map.end()) {
		return;
	}
	std::vector<instance_t> modelMatrices(num_instances);



	if constexpr (!std::is_same_v<instance_t, glm::mat4>) {
		for (auto& instance : modelMatrices) {
			instance = instance_t();
		}
	}
	else {
		for (auto& instance : modelMatrices) {
			instance = glm::mat4(1.0f);
		}
	}

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
	info_map[vao].texture = new Texture(std::string("resources/images/seamlessTextures/100_1174_seamless.JPG"));

	// initialize instance buffer
	info_map[vao].instance_matrix_buffer.init(modelMatrices);
	info_map[vao].instance_matrix_buffer.bind();

	// and its pointers
	if constexpr (std::is_same_v<instance_t, glm::mat4>) {
		for (vertex_pointer_index = 0; vertex_pointer_index < 4; vertex_pointer_index++) {
			glVertexAttribPointer(vertex_pointer_index, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(vertex_pointer_index * 4 * sizeof(float)));
			glEnableVertexAttribArray(vertex_pointer_index);
			glVertexAttribDivisor(vertex_pointer_index, 1); // Update per instance
			std::cout << vertex_pointer_index;
		}
	}



	// bind vbo
	info_map[vao].vbo.init(vertices);
	info_map[vao].vbo.bind();

	// bind ebo
	info_map[vao].ebo.init(indices);
	info_map[vao].ebo.bind();

	if constexpr (std::is_same_v<vertex_t, glm::vec3>) {
		VBO::add_vec3_pointer(vertex_pointer_index, sizeof(glm::vec3), 0);
	}
	else {
		vertex_t::set_pointers(vertex_pointer_index); // set the pointers for the custom vertex type

	}



	//if constexpr (std::is_same_v<

	// unbind the VAO only
	VAO::unbind();


}


glm::mat4 MeshManager::get_matrix(GLuint vao, size_t instance_index) {
	if (info_map.find(vao) == info_map.end()) {
		throw std::runtime_error("VAO not found in MeshManager!");
	}

	MeshInfo& info = info_map[vao];
	glm::mat4 matrix;

	info.instance_matrix_buffer.bind();
	glGetBufferSubData(GL_ARRAY_BUFFER, instance_index * sizeof(glm::mat4), sizeof(glm::mat4), &matrix);

	return matrix;
}
inline MeshInstance MeshManager::get_mesh(const std::string& name) {
	// check for the existence of this mesh already existing, but we will skip it for now
	if (name_map.find(name) != name_map.end()) {
		GLuint& vao = name_map[name];
		MeshInfo& info = info_map[vao];
		MeshInstance instance(vao, info.next_instance_index++);
		//info.last_instance = &instance;
		return instance;

	}
	throw std::runtime_error("Mesh does not exist here"); // shorthand, for now
}

MeshLoadResult MeshManager::load_mesh(std::string path) {

	MeshLoadResult mesh_result;


	DevMsg("Loading file...");
	if (!reader.ParseFromFile(path, config)) {
		std::string message = "tinyobj failed to load " + path;
		DevMsg(message); // log the error

	}


	// Retrieve the model data
	const auto& attrib = reader.GetAttrib();
	const auto& shapes = reader.GetShapes();
	const auto& materials = reader.GetMaterials();
	
	std::vector<glm::vec3> vertices;
	for (size_t v = 0; v < attrib.vertices.size(); v += 3) {
		mesh_result.points.push_back({ attrib.vertices[v] , attrib.vertices[v + 1], attrib.vertices[v + 2] });

	}


	for (const auto& shape : shapes) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
			size_t num_verts = size_t(shape.mesh.num_face_vertices[f]);
			for (size_t v = 0; v < num_verts; v++) {
				unsigned int idx = shape.mesh.indices[index_offset + v].vertex_index;
				mesh_result.indices.push_back(idx);
			}
			index_offset += num_verts;
		}
	}


	return mesh_result;

}

void MeshManager::load(std::string path) {
	MeshLoadResult result = load_mesh(path);
	generate_mesh(path, result.points, result.indices, "default_shader",
		"resources/shaders/default.vert",
		"resources/shaders/default.frag");
}