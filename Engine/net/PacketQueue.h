#pragma once
#include "../utility/ByteStream.h"
#include "../utility/Serialize.h"
#include <unordered_map>

using qk::utility::ByteStream;

namespace qk::net {

	template <class packet_t>
	struct Channel_traits {
		//static constexpr u8 channel = 0;
	};



	struct PacketQueue {
		qk::utility::ByteStream m_Incoming;
		qk::utility::ByteStream m_Outgoing;


		template <class packet_t>
		bool push_incoming(const packet_t& packet) {
			return qk::serialize(m_Incoming, packet);
		}

		template <class packet_t>
		bool push_outgoing(const packet_t& packet) {
			return qk::serialize(m_Outgoing, packet);
		}


		template <class packet_t>
		void pop_incoming(packet_t& packet) {
			qk::serialize(m_Incoming, packet);
		}

		template <class packet_t>
		void pop_outgoing(packet_t& packet) {
			qk::deserialize(m_Outgoing, packet);
		}

		qk::utility::ByteStream& incoming() { return m_Incoming; }
		qk::utility::ByteStream& outgoing() { return m_Outgoing; }


	};



	class PacketQueueMap {
		std::unordered_map<u8, PacketQueue> m_Queues;
			
		public:
			void add_queue(u8 channel) {
				if (m_Queues.contains(channel)) return;

				m_Queues[channel] = PacketQueue{};
			}

	};


}