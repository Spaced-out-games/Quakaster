/// **************************************** QUAKASTER ENGINE **************************************** 
/// net/Connection.h 
/// Purpose: Manages a connection and packet queues for a foreign endpoint.
/// **************************************************************************************************
#pragma once
#include "PacketQueue.h"
#include <enet/enet.h>
#include "core.h"



namespace qk::net {

	using ClientID = u32;

	/*
	 * Network Connection. Represents a foreign peer in network
	 * Stores a list of typed packet queues
	*/
	struct Connection: public PacketQueueMap {
		
		ENetPeer* m_pPeer = nullptr;
		ClientID m_ID = 0;
		

		Connection(ENetPeer* pPeer, ClientID id);

		~Connection();

		// Gets the queue from the specified channel.
		PacketQueue* operator[](size_t channel) {
			if (!m_Queues.contains(channel)) return nullptr;
			PacketQueue& queue = m_Queues.at(channel);

			return &queue;
		}

		// Sends all the packets queued for transmission
		bool flush();

		// Queues a packet for sending
		template <class packet_t>
		void send(const packet_t& value) {
			PacketQueueMap::push(value);
		}


	};

}
