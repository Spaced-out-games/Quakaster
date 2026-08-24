

namespace qk {

	template <class T>
	HandleTable<T>::Handle HandleTable<T>::insert(const T & payload) {


		if (m_Free.empty()) {
			m_Entries.emplace_back(payload, 0, true);
			return { ((u32)m_Entries.size()) - 1u, 0u };
		}

		u32 sparse = m_Free.back();
		m_Free.pop_back();

		u16 generation = m_Entries[sparse].generation + 1;

		m_Entries[sparse] = {
			.payload = payload,
			.generation = generation,
			.alive = true
		};

		return { sparse, generation };



	}

	template <class T>
	bool HandleTable<T>::valid(Handle handle) const {
		if (handle.sparse >= m_Entries.size())
			return false;

		const auto& entry = m_Entries[handle.sparse];

		return entry.alive && (handle.generation == entry.generation);

	}

	template <class T>
	void HandleTable<T>::remove(Handle handle) {
		if (!valid(handle))
			return;

		Entry& entry = m_Entries[handle.sparse];

		entry.alive = false;
		entry.payload = {};

		m_Free.push_back(handle.sparse);
	}
	template <class T>
	const T* HandleTable<T>::try_get(Handle handle) const {
		if (handle.sparse >= m_Entries.size())
			return nullptr;

		auto& entry = m_Entries[handle.sparse];

		if (entry.alive && (handle.generation == entry.generation))
			return &entry.payload;

		return nullptr;

	}

	template <class T>
	T* HandleTable<T>::try_get(Handle handle) {
		if (handle.sparse >= m_Entries.size())
			return nullptr;

		auto& entry = m_Entries[handle.sparse];

		if (entry.alive && (handle.generation == entry.generation))
			return &entry.payload;

		return nullptr;

	}

	// Swaps payloads for two live sparse indices.
	// LOW LEVEL: does not validate generations.
	template <class T>
	void HandleTable<T>::swap(u32 sparse_a, u32 sparse_b) {

		// validate
		assert(sparse_a < m_Entries.size() && m_Entries[sparse_a].alive);
		assert(sparse_b < m_Entries.size() && m_Entries[sparse_b].alive);


		std::swap(m_Entries[sparse_a].payload, m_Entries[sparse_b].payload);

	}

	template <class T>
	void HandleTable<T>::swap(Handle a, Handle b) {
		if (!valid(a) || !valid(b))
			return;

		swap(a.sparse, b.sparse);

	}

	// helper
	template <class T>
	void HandleTable<T>::set(Handle handle, const T& value) {
		T* pPayload = try_get(handle);

		if (pPayload == nullptr) return;

		*pPayload = value;

	}




}