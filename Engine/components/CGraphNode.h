/// **************************************** QUAKASTER ENGINE **************************************** 
/// components/CGraphNode.h
/// Purpose: Defines a scene graph node type, within the entt ecosystem
/// **************************************************************************************************
#pragma once

#include <entt/entity/entity.hpp>

namespace qk::entity {

	// Used to define an entity heirarchy
	struct CGraphNode {
		entt::entity parent = entt::null;
		entt::entity prev = entt::null;
		entt::entity next = entt::null;
		entt::entity first = entt::null;
		entt::entity last = entt::null;

		struct DirtyFlag {
			entt::entity old_parent = entt::null;
		};

	};



}

