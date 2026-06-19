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

			size_t size() {
				return m_Queues.size();
			}
			template <class packet_t>
			bool push(const packet_t& value) {
				constexpr u8 channel = Channel_traits<packet_t>::channel;

				if (!m_Queues.contains(channel)) add_queue(channel);

				return m_Queues.at(channel).push_outgoing(value);

			}

			template <class packet_t>
			bool pop(packet_t& value) {
				constexpr u8 channel = Channel_traits<packet_t>::channel;

				if (!m_Queues.contains(channel)) add_queue(channel);

				return m_Queues.at(channel).pop_incoming(value);

			}

	};


	template <class T>
	PacketQueueMap& operator<<(PacketQueueMap& map, const T& value) {
		map.push(value);
		return map;
	}

	template <class T>
	PacketQueueMap& operator>>(PacketQueueMap& map, T& value) {
		map.pop(value);

		return map;
	}



}