// bsp_node.h
// PURPOSE: BSP node. Holds a BSP_plane and pointer to both children nodes
#pragma once
// For custom pointer type
#include "bsp_index.h"
#include "bsp_plane.h"


struct BSP_Node {
	// N = {0}, D = 0, front -> empty, back -> empty
	BSP_Node() : plane(), front(), back() {}
	BSP_Plane plane;

	BSP_index front;
	BSP_index back;
};
