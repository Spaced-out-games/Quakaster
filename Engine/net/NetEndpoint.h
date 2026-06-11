#pragma once
#include "enet/enet.h"
#include <memory>
#include <vector>
#include <functional>
#include "core.h"
#include "PacketQueue.h"

struct ChatMessage {
	char msg[256]{};
	operator const char*() {
		return msg;
	}
};


namespace qk::net {






	



	template <>
	struct Channel_traits<ChatMessage> {
		static constexpr u8 channel = 0;
	};



	

	struct NetEndpoint {
		bool m_Initialized = false; // debug
		ENetHost* m_pEndpoint = nullptr;
		ENetEvent m_Event;
		//std::vector<ENetPeer*> m_Peers;
		ENetPeer* m_pPeer = nullptr;

		// returns 1 on success if pCode is 
		NetEndpoint(bool* pSuccess = nullptr);

		bool init(ENetAddress* pMyIPAddress, u32 num_peers = 1, u8 num_channels = 1);

		bool connect(const char* pIPAddress = "127.0.0.1", u16 port = 7777, u16 num_channels = 1, u32 wait_ms = 3000);

		void disconnect();

		Packet_header receive_impl(void* pPayload, u32 capacity, u32 wait_ms);

		bool send_impl(const void* pData, uint32_t size, uint8_t channel);






		//EndpointEvent pollEvent(u32 expected_size, u32 wait_ms = 100);


		//bool send(const void* pData, u32 size);

		~NetEndpoint();

	};
	

}