
#include<span>

namespace qk {

	template <class T>
	Partition<T>::Partition() {
		// safe small reservation
		reserve(1);
	}


	template <class T>
	void Partition<T>::reserve(u32 size) {
		// always reserve a power of two
		m_NextCapacity = std::max(m_NextCapacity, std::bit_ceil(size));
	}

	template <class T>
	u32 Partition<T>::size() const {
		return m_Size;
	}

	template <class T>
	u32 Partition<T>::capacity() const {
		return m_Capacity;
	}

	template <class T>
	u32 Partition<T>::new_size() const {
		return m_NextSize;
	}
	
	
	template <class T>
	u32 Partition<T>::new_capacity() const {
		return m_NextCapacity;
	}


	template <class T>
	void Partition<T>::queue_swap(u32 offset_a, u32 offset_b) {


		// ensure it's in logical bounds
		assert(offset_a <= m_NextSize);
		assert(offset_b <= m_NextSize);


		m_Queue.push_back({
			.swap = {
				.offset_a = offset_a,
				.offset_b = offset_b
			},
			.type = EPartitionRequestType::SWAP
			});
	}

	template <class T>
	void Partition<T>::queue_set(const T& payload, u32 offset) {
		// FYI: This isn't strictly < nextsize because it's fine to set out of logical bounds
		assert(offset <= m_NextSize);

		m_Queue.push_back({
			.set = {
				.payload = payload,
				.offset = offset
			},
			.type = EPartitionRequestType::SET
			});

	}


	
	template <class T>
	u32 Partition<T>::queue_insert(const T& payload) {
		if (m_NextSize == m_NextCapacity) {
			m_NextCapacity = m_NextCapacity
				? m_NextCapacity << 1
				: 1;
		}

		u32 offset = m_NextSize++;
		queue_set(payload, offset);

		return offset;
	}

	template <class T>
	u32 Partition<T>::pop_back() {
		assert(m_NextSize > 0);
		return m_NextSize--;
	}

	template <class T>
	void Partition<T>::flush(T* pBase) {
		assert(pBase);

		// temporary, for debugging purposes. Intentional off by one to make sure I'm not reading past
		std::span<T> temp(pBase, 5);

		for (const auto& query : m_Queue) {
			switch (query.type) {
			case EPartitionRequestType::SWAP:
				std::swap(
					pBase[query.swap.offset_a],
					pBase[query.swap.offset_b]
				);
				// DEBUG: set pBase[query.swap.offset_b] to -1u in order to invalidate
				//pBase[query.swap.offset_b] = -1u;
				break;

			case EPartitionRequestType::SET:
				pBase[query.set.offset] = query.set.payload;
				break;
			}
		}

		m_Capacity = m_NextCapacity;
		m_Size = m_NextSize;

		m_Queue.clear();
	}

	/*
	template <class T>
	void Partition<T>::remove(u32 offset) {
		if (offset >= m_NextSize || m_NextSize == 0)
			return;

		u32 back = m_NextSize - 1;

		if (back != offset) {
			queue_swap(offset, back);
		}

		auto popped = pop_back();
		assert(back == popped - 1);


		return;
	}*/
	
	template <class T>
	u32 Partition<T>::remove(u32 offset) {
		if (offset >= m_NextSize || m_NextSize == 0)
			return NULL_OFFSET;

		u32 back = m_NextSize - 1;

		if (back != offset) {
			queue_swap(offset, back);
		}

		auto popped = pop_back();
		assert(back == popped - 1);


		return back;
	}

	template <class T>
	size_t Partition<T>::queue_size() const {
		return m_Queue.size();
	}
	


}