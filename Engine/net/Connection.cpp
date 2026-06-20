#include "Connection.h"


namespace qk::net {
	Connection::Connection(ENetPeer* pPeer, ClientID id) :
		m_pPeer(pPeer), m_ID(id) {}

	bool Connection::flush() {
		ENetPacket* pkt = nullptr;
		for (auto& [channel, queue] : m_Queues) {
			auto& outgoing = queue.m_Outgoing;

			pkt = enet_packet_create(outgoing.data(), outgoing.size(), ENET_PACKET_FLAG_RELIABLE);

			if (enet_peer_send(m_pPeer, channel, pkt) < 0)
			{
				enet_packet_destroy(pkt);
				return false;
			}

			outgoing.clear();

		}

		return true;


	}


	Connection::~Connection() {
		if (!m_pPeer) return;
		enet_peer_disconnect(m_pPeer, 0);
	}

}