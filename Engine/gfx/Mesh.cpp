#include "Mesh.h"


// Mesh ID
namespace qk::gfx {


	
	u64 MeshID::key() const {
		return (u64(pipeline_id) << 48) |
			(u64(vertexbuffer_group_id) << 32) |
			(u64(indexbuffer_group_id) << 16) |
			u64(descriptor_group_id);
	}

	MeshID::operator u64() const {
		return key();
	}

	bool MeshID::operator> (MeshID other) {
		return key() > other.key();
	}
	bool MeshID::operator==(const MeshID& other) const {
		return key() == other.key();
	}




}