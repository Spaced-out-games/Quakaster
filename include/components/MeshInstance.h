#pragma once
#include <GL/glew.h>
//#include <include/resources/mesh_manager.h>

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

	void submit(const glm::mat4& new_matrix);

	glm::mat4 get_matrix();

	// Very important destructor
	~MeshInstance() {};

};



void MeshInstance::submit(const glm::mat4& new_matrix) {
	MeshManager::submit(vao, transform_index, new_matrix);
}

glm::mat4 MeshInstance::get_matrix() {
	return MeshManager::get_matrix(vao, transform_index);
}