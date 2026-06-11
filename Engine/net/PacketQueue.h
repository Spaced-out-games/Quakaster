#pragma once
#include <unordered_map>
#include <enet/enet.h>
#include <type_traits>
#include <queue>
#include "core.h"
#include <cstring>


namespace qk::net {

	// packets and packet queues

	template <class payload_t>
	struct Channel_traits {

	};


	enum PacketType : u32 {
		STATUS_OKAY,
		STATUS_CONNECT,
		STATUS_DISCONNECT,
		STATUS_RECEIVE,
		// Successfully got a packet but it is too large
		ERROR_PACKET_INVALID_SIZE,
		ERROR_OUT_OF_MEMORY

	};

	struct Packet_header {
		// IConnection* pConnection; // when we get that ball rolling
		ENetPeer* pFrom = nullptr;
		PacketType	type = STATUS_OKAY;
		size_t		size = 0;
		u8			channel = 0;

	};

	template <class payload_t>	
	struct Packet: Packet_header {
		payload_t payload;
	};

	struct IPacketQueue {
		virtual void upload(
			const void* pPayload,
			size_t PayloadSize,
			ENetPeer* pPeer,
			PacketType type,
			u8 channel
		) = 0;

		virtual ~IPacketQueue() = default;
	};

	template <class payload_t>
	class PacketQueue : IPacketQueue {
		static_assert(std::is_trivially_copyable_v<payload_t>, "Packet payloads must be trivially copyable");


		std::deque<Packet<payload_t>> m_Packets;


		void upload(const void* pPayload, size_t PayloadSize, ENetPeer* pPeer, PacketType type, u8 channel) override {

			assert(sizeof(payload_t) == PayloadSize);
			assert(Channel_traits<payload_t>::channel == channel);
			
			if (sizeof(payload_t) != PayloadSize)
				return;
			
			Packet<payload_t> pkt;
			pkt.pFrom = pPeer;
			pkt.size = PayloadSize;
			pkt.type = type;
			pkt.channel = channel;

			if (pPayload) {
				memcpy(&pkt.payload, pPayload, PayloadSize);
			}
			

			m_Packets.push_back(pkt);

		}

		std::deque<Packet<payload_t>>& queue() {
			return m_Packets;
		}

		const std::deque<Packet<payload_t>>& queue() {
			return m_Packets;
		}

	};

	class PacketQueueMap {
		std::unordered_map<u8, IPacketQueue*> m;
	};




}