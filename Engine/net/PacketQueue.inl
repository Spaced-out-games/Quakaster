

namespace qk::net {
	template <class packet_t>
	bool PacketQueue::push_incoming(const packet_t& packet) {
		return qk::serialize(m_Incoming, packet);
	}

	template <class packet_t>
	bool PacketQueue::push_outgoing(const packet_t& packet) {
		return qk::serialize(m_Outgoing, packet);
	}

	template <class packet_t>
	bool PacketQueue::pop_incoming(packet_t& packet) {
		return qk::deserialize(m_Incoming, packet);
	}

	template <class packet_t>
	bool PacketQueue::pop_outgoing(packet_t& packet) {
		return qk::deserialize(m_Outgoing, packet);
	}


	template <class packet_t>
	void PacketQueueMap::add_queue() {
		constexpr u8 channel = Channel_traits<packet_t>::channel;

		if (m_Queues.contains(channel)) return;

		m_Queues[channel] = PacketQueue{};
	}

	template <class packet_t>
	bool PacketQueueMap::push(const packet_t& value) {
		constexpr u8 channel = Channel_traits<packet_t>::channel;

		if (!m_Queues.contains(channel)) add_queue<packet_t>();

		return m_Queues.at(channel).push_outgoing(value);

	}

	template <class packet_t>
	bool PacketQueueMap::pop(packet_t& value) {
		constexpr u8 channel = Channel_traits<packet_t>::channel;

		if (!m_Queues.contains(channel)) add_queue<packet_t>();

		return m_Queues.at(channel).pop_incoming(value);

	}


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