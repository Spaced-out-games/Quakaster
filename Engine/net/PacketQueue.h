#pragma once
#include "../utility/ByteStream.h"
#include "../utility/Serialize.h"
#include <unordered_map>

using qk::utility::ByteStream;

namespace qk::net {

	template <class packet_t>
	struct Channel_traits;


	// Stores the incoming and outgoing packets for one channel.
	struct PacketQueue {

		ByteStream m_Incoming;
		ByteStream m_Outgoing;

		// Push a packet to the incoming queue
		template <class packet_t>
		bool push_incoming(const packet_t& packet);

		// Push a packet to the outgoing queue
		template <class packet_t>
		bool push_outgoing(const packet_t& packet);

		/* 
		* Pop a packet from the incoming queue, writing to `packet`.
		* Returns true if `packet` was successfully written to
		*/ 
		template <class packet_t>
		bool pop_incoming(packet_t& packet);

		/*
		* Pop a packet from the incoming queue, writing to `packet`.
		* Returns true if `packet` was successfully written to
		*/
		template <class packet_t>
		bool pop_outgoing(packet_t& packet);
		
		// Reference to incoming queue
		ByteStream& incoming();

		// Reference to outgoing queue
		ByteStream& outgoing();

	};


	// Maps a channel to a packetqueue, and by consequence, a type to its queue.
	class PacketQueueMap {
		protected:
		public:

		// type erased queues
		std::unordered_map<u8, PacketQueue> m_Queues;
			
			template <class packet_t>
			void add_queue();

			size_t size() const;

			template <class packet_t>
			bool push(const packet_t& value);

			template <class packet_t>
			bool pop(packet_t& value);

	};


	template <class T>
	PacketQueueMap& operator<<(PacketQueueMap& map, const T& value);

	template <class T>
	PacketQueueMap& operator>>(PacketQueueMap& map, T& value);



}


#include "PacketQueue.inl"