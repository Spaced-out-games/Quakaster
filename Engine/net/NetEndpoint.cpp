#include "NetEndpoint.h"

namespace qk::net {

	inline PacketType PacketType_cast(const ENetEvent& evt, u32 expected_size) {

		switch (evt.type)
		{
		case ENET_EVENT_TYPE_NONE:
			return STATUS_OKAY;
		case ENET_EVENT_TYPE_CONNECT:
			return STATUS_CONNECT;
		case ENET_EVENT_TYPE_RECEIVE:
			if (evt.packet->dataLength != expected_size)
				return ERROR_PACKET_INVALID_SIZE;
			return STATUS_RECEIVE;
		default:
			break;
		}
		return STATUS_OKAY;


	}



	NetEndpoint::NetEndpoint(bool* pSuccess) {

		m_Initialized = (enet_initialize() == 0);

		if (pSuccess) *pSuccess = m_Initialized;
	
	}

	NetEndpoint::~NetEndpoint() {
		if (m_Initialized)
			enet_deinitialize();
	}

	bool NetEndpoint::init(ENetAddress* pMyIPAddress, u32 num_peers, u8 num_channels) {

		assert(m_pEndpoint == nullptr);
		// nullptr, 1 outgoing connection, N channels, 0 out, 0 in for client
		// non-null, N connections, N channels, 0 out, 0 in for server
		m_pEndpoint = enet_host_create(pMyIPAddress, num_peers, num_channels, 0, 0);

		if (!m_pEndpoint)
			return false;

		return true;

	}
	
	bool NetEndpoint::connect(const char* pIPAddress, u16 port, u16 num_channels, u32 wait_ms) {

		assert(!m_pPeer);

		ENetAddress TheirIPAddress{
			.port = port
		};
		// this just converts the string to a u32 lol
		enet_address_set_host(&TheirIPAddress, pIPAddress);


		// 0 = initial mini-packet
		m_pPeer = enet_host_connect(m_pEndpoint, &TheirIPAddress, num_channels, 0);

		if (!m_pPeer) return false;

		// wait for a ping from the server

		if (enet_host_service(m_pEndpoint, &m_Event, wait_ms) > 0 && m_Event.type == ENET_EVENT_TYPE_CONNECT) {
			return true;
		}
		else {
			enet_peer_reset(m_pPeer);
			return false;
		}

	}

	// notice: make an enum to determine polling status:
	

	Packet_header NetEndpoint::receive_impl(void* pPayload, u32 capacity, u32 wait_ms) {

		Packet_header header;

		while (enet_host_service(m_pEndpoint, &m_Event, wait_ms) > 0) {

			switch (m_Event.type) {
				case STATUS_RECEIVE:

					if (!pPayload) return Packet_header{
						m_Event.peer,
						ERROR_OUT_OF_MEMORY,
						m_Event.packet->dataLength,
						m_Event.channelID
					};

					memcpy_s(pPayload, m_Event.packet->dataLength, m_Event.packet->data, m_Event.packet->dataLength);

					return Packet_header {
						m_Event.peer,
						STATUS_RECEIVE,
						m_Event.packet->dataLength,
						m_Event.channelID
					};
					

			}

		}


	}


	

	void NetEndpoint::disconnect() {
		enet_peer_disconnect(m_pPeer, 0);

		while (enet_host_service(m_pEndpoint, &m_Event, 3000) > 0) {
			switch (m_Event.type) {
				case ENET_EVENT_TYPE_RECEIVE:
					enet_packet_destroy(m_Event.packet);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					break;
			}
		}
	}

	bool NetEndpoint::send_impl(const void* pData, uint32_t size, uint8_t channel)
	{
		ENetPacket* packet = enet_packet_create(pData,size, ENET_PACKET_FLAG_RELIABLE);

		if (!packet) return false;

		auto result = enet_peer_send( m_pPeer, 0, packet);

		enet_host_flush(m_pEndpoint);

		return result == 0;
	}



}