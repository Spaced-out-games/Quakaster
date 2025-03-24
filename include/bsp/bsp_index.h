// bsp_index.h
// PURPOSE: Defines a utility atomic type for indexing BSPs, defining materials, and leaves.
#pragma once
#include <stdint.h>
#include <string>
#include <stdexcept>

using BSP_index_primitive_t = uint32_t;

struct BSP_index {
	// Default constructor is empty
	BSP_index(): isLeaf(true), contents(0) {}

	// In most cases, we will be defining leaves, at least on the front end. Geometry 
	BSP_index(BSP_index_primitive_t material_type) : isLeaf(true), contents(material_type) {};

	// Whether or not a left/right node is actually a leaf node.
	BSP_index_primitive_t isLeaf : 1;

	// The contents of the node. If leaf, this is the material index (0 means empty, >0 is a material index)
	BSP_index_primitive_t contents : (sizeof(BSP_index_primitive_t) * 8) - 1;




	bool is_solid();
	bool is_empty();


	// returns contents if leaf, 0 otherwise
	BSP_index_primitive_t material();

	std::string to_string() const;
};

bool BSP_index::is_solid() {
	// It's a solid node if contents != 0 and it's a leaf node
	return isLeaf && contents;
}

bool BSP_index::is_empty() {
	// It's an empty node if contents == 0 and it's a leaf node. 

	return isLeaf && !contents;
}

BSP_index_primitive_t BSP_index::material() {
	// Since it's a leaf node, just report the correct material index
	// if it's not a leaf, it's pointing to another node, and so technically no material (ie, it's material is the empty 'material')

	return isLeaf ? contents : 0;
}

std::string BSP_index::to_string() const {
	#ifdef _DEBUG
	if (isLeaf) {
		return contents ? "Leaf with material #" + std::to_string(contents)
			: "Empty leaf";
	}
	return "->Node(" + std::to_string(contents) + ")";
	#else
	// just throw an error so I know to go up the stack frame and delete all calls if I forget
	throw std::runtime_error("unsupported feature in debug builds");
	#endif
}