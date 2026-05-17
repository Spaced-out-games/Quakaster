#pragma once
#include <unordered_map>
#include <fnv1a.h>
#include "Material.h"
#include "Mesh.h"

namespace qk::gfx {


	class Renderer {
		std::unordered_map<fnv1a_hash_t, Material>	m_MaterialPool;
		std::unordered_map<fnv1a_hash_t, Mesh>		m_MeshPool;
	};


}