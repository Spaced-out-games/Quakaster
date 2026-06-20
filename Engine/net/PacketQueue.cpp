#include "PacketQueue.h"



namespace qk::net {

	ByteStream& PacketQueue::incoming() { return m_Incoming; }
	ByteStream& PacketQueue::outgoing() { return m_Outgoing; }
	size_t PacketQueueMap::size() const { return m_Queues.size(); }

}