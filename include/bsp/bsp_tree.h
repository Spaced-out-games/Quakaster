// bsp_tree.h
// PURPOSE: Defines a BSP-tree
#pragma once
#include "bsp_node.h"
#include <vector>

/*
Custom Binary Space Partitioning tree implementation. Instead of storing triangles, it defines geometry purely geometrically using the leaf planes
*/
struct BSP_tree {
	
	// builds the tree
	void build(std::vector<Triangle>&);



	// Checks if a point is in a solid leaf node
	bool is_solid(glm::vec3);
	
	
	
	std::vector<BSP_Node> tree;
	int root_index = -1;



};


void BSP_tree::build(std::vector<Triangle>& triangles) {
	

	// 1. choose a polygon from the list
	Triangle& target = triangles.at(0); // just for ease of use

	// 2. make a node N in the BSP tree...
	tree.emplace_back();
	// ...and add P to the list of polygons for that node
	if (node_triangle_lists.size() < tree.size()) {
		node_triangle_lists.push_back(target)
	}
}