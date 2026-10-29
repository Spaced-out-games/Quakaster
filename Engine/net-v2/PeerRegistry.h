#pragma once
#include <entt/entity/registry.hpp>
#include <entt/entity/entity.hpp>
#include <unordered_map>
#include "Peer.h"
#include "Connection.h"
#include <core.h>


namespace qk::net_v2 {

	using ClientID = u32;


	using PeerRegistry = std::unordered_map<ClientID, Peer>;


	template<Protocol Protocol_t>
	struct PacketRegistry  {
		
		entt::registry m_PacketStorage;

		PeerRegistry m_PeerRegistry;



	};


}