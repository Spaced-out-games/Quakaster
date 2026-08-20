#include "InstancePool.h"
#include <cassert>

namespace qk::gfx {

	InstancePoolPartition::InstancePoolPartition(
		size_t initial_capacity,
		size_t initial_offset
	) {
		// Should be a power of two.
		assert(initial_capacity != 0 && (initial_capacity & (initial_capacity - 1)) == 0);

		m_Capacity = initial_capacity;

		// Canary in a coal mine: this is an alignment assumption,
		// not necessarily a requirement for it to work.
		assert((initial_offset % 4) == 0);

		m_Offset = initial_offset;
	}


	size_t InstancePoolPartition::current_size() const {
		return m_Size;
	}

	size_t InstancePoolPartition::resolve(size_t index) const {
		return m_Offset + index;
	}


	size_t InstancePoolPartition::current_capacity() const {
		return m_Capacity;
	}


	size_t InstancePoolPartition::future_size() const {
		return m_Size + m_SizeDelta;
	}


	size_t InstancePoolPartition::future_capacity() const {
		return std::bit_ceil(future_size());
	}


	void InstancePoolPartition::insert() {
		++m_SizeDelta;
	}


	void InstancePoolPartition::remove() {
		// Prevent underflow.
		assert(future_size() > 0);
		--m_SizeDelta;
	}


	size_t InstancePoolPartition::offset() const {
		return m_Offset;
	}


	void InstancePoolPartition::flush() {
		m_Size = future_size();
		reserve(m_Size);
		m_SizeDelta = 0;
	}


	void InstancePoolPartition::reserve(size_t min_size) {
		if (min_size > m_Capacity)
			m_Capacity = std::bit_ceil(min_size);
	}


	void InstancePoolPartition::offset(size_t new_offset) {
		assert((new_offset % 4) == 0);
		m_Offset = new_offset;
	}

}