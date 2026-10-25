#pragma once
#include <enet/enet.h>
#include <entt/entity/entity.hpp>


namespace qk::net_v2 {

	struct Peer {
		ENetPeer* m_pPeer = nullptr;
		entt::entity m_IncomingPackets = entt::null;
		entt::entity m_OutgoingPackets = entt::null;
	};


}