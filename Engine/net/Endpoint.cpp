#include "Endpoint.h"
#include "spdlog/spdlog.h"
#include <memory>
namespace qk::net {


	bool Endpoint::init(const char* hostname, u16 port, u32 max_recipients, u8 max_channels, u32 in_bandwidth, u32 out_bandwidth) {
		if (!m_Initialized) {
			m_Initialized = enet_initialize() == 0;
			if (!m_Initialized) return false;
		}

		spdlog::debug("ENet initialized.");

		if (!m_pEndpoint) {
			ENetAddress MyAddress = {
				//.host = address,
				.port = port
			};

			if (!hostname) {
				MyAddress.host = ENET_HOST_ANY;
			}
			else {
				enet_address_set_host(&MyAddress, hostname);
			}

			m_pEndpoint = enet_host_create(&MyAddress, max_recipients, max_channels, in_bandwidth, out_bandwidth);

			if (!m_pEndpoint) return false;

		}

		spdlog::info("Endpoint initialized.");


		return true;
	}

	void Endpoint::shutdown() {
		if (m_Initialized && m_pEndpoint)
			enet_host_destroy(m_pEndpoint);
	}


	void Endpoint::flush() {
		ENetEvent evt;
		Connection* pConnection = nullptr;
		u8 channel = 0;
		PacketQueue* pQueue = nullptr;
		ClientID id;
		while (enet_host_service(m_pEndpoint, &evt, 0) > 0) {

			switch (evt.type) {
				case ENET_EVENT_TYPE_CONNECT:
					
					id = (ClientID)evt.data;

					if (!m_Connections.contains(id)) {
						m_Connections[id] = std::make_unique<Connection>(evt.peer, id);
					}



					

					pConnection = m_Connections.at(id).get();

					// so we know how to give the disconnect event
					evt.peer->data = (void*)pConnection;

					

					if (on_connect)
						on_connect(*this, pConnection);
					break;

				case ENET_EVENT_TYPE_DISCONNECT:

					pConnection = (Connection*)evt.peer->data;

					id = pConnection->m_ID;

					assert(m_Connections.contains(id));


					// call the callback first
					if (on_disconnect)
						on_disconnect(*this, pConnection);

					if (m_Connections.contains(id)) {
						m_Connections.erase(id);
					}
					break;
				case ENET_EVENT_TYPE_RECEIVE:


					// find the connection
					pConnection = (Connection*)evt.peer->data;

					if (!pConnection) break;

					id = pConnection->m_ID;
					
					assert(m_Connections.contains(id));

					channel = evt.channelID;

					// get the right queue
					pQueue = (*pConnection)[channel];

					// save the packet to that queue
					if (pQueue)
						pQueue->incoming().write(evt.packet->data, evt.packet->dataLength);

					// call the callback
					if (on_receive)
						on_receive(*this, pConnection);

					// remove the packet
					enet_packet_destroy(evt.packet);

					break;


			}

		}

		for (const auto& [id, pConnection] : m_Connections) {
			pConnection->flush();
		}

		enet_host_flush(m_pEndpoint);
		
	}

	bool Endpoint::connect(const char* hostname, u16 port, u8 max_channels) {
		ENetAddress TheirAddress = {
			.port = port
		};
		enet_address_set_host(&TheirAddress, hostname);

		ENetPeer* pPeer = enet_host_connect(m_pEndpoint, &TheirAddress, max_channels, (enet_uint32)m_ID);


		return pPeer != nullptr;



	}

	void Endpoint::disconnect(ClientID id) {
		if (!m_Connections.contains(id))
			return;

		m_Connections.erase(id);

	}

	Endpoint::~Endpoint() {
		shutdown();
	}


}