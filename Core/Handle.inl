

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
	size_t HandleTable<T>::sparse_size() const {
		return m_Entries.size();
	}


	template <class T>
	bool HandleTable<T>::valid(Handle handle) const {
		if (handle.sparse >= m_Entries.size())
			return false;


		return m_Entries[handle.sparse].alive &&
			(handle.generation == m_Entries[handle.sparse].generation);

	}

	template <class T>
	bool HandleTable<T>::valid(u32 sparse) const {
		if (sparse >= m_Entries.size())
			return false;

		return  m_Entries[sparse].alive;

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
	void HandleTable<T>::remove(u32 sparse) {
		// valid index first of all...
		if (sparse >= m_Entries.size())
			return;

		// get the entry
		Entry& entry = m_Entries[sparse];

		// don't double-delete.
		if (!entry.alive)
			return;

		entry.alive = false;
		entry.payload = {};

		m_Free.push_back(sparse);
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
	const T* HandleTable<T>::try_get_sparse(u32 sparse) const {
		if (sparse >= m_Entries.size())
			return nullptr;

		auto& entry = m_Entries[sparse];

		if (entry.alive)
			return &entry.payload;

		return nullptr;

	}










	template <class T>
	T* HandleTable<T>::try_get_sparse(u32 sparse) {
		if (sparse >= m_Entries.size())
			return nullptr;

		auto& entry = m_Entries[sparse];

		if (entry.alive)
			return &entry.payload;

		return nullptr;

	}



	// Swaps payloads for two live sparse indices.
	// LOW LEVEL: does not validate generations.
	template <class T>
	void HandleTable<T>::swap(u32 sparse_a, u32 sparse_b) {

		// validate
		QK_ASSERT(sparse_a < m_Entries.size(), "sparse_a was too big");
		QK_ASSERT(m_Entries[sparse_a].alive, "sparse_a wasn't alive");

		QK_ASSERT(sparse_b < m_Entries.size(), "sparse_b was too big");
		QK_ASSERT(m_Entries[sparse_b].alive, "sparse_b wasn't alive");





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

	// iterator methods

	template <class T>
	typename HandleTable<T>::iterator& HandleTable<T>::iterator::operator++() {
		QK_ASSERT(pTable, "pTable was nullptr");
		do {
			++current_sparse;
		} while (current_sparse < pTable->sparse_size());// && !pTable->valid(current_sparse));

		return *this;
	}


	template <class T>
	u32 HandleTable<T>::iterator::operator*() {
		QK_ASSERT(pTable, "pTable was nullptr");

		return current_sparse;
	}

	template <class T>
	bool HandleTable<T>::iterator::operator==(const iterator& other) const {
		return pTable == other.pTable &&
			current_sparse == other.current_sparse;
	}

	template <class T>
	bool HandleTable<T>::iterator::operator!=(const iterator& other) const {
		return !(*this == other);
	}

	template <class T>
	HandleTable<T>::iterator HandleTable<T>::begin() {
		size_t i = 0;
		while (i < sparse_size() && !valid(i))
			++i;

		return { this, i };
	}

	template <class T>
	HandleTable<T>::iterator HandleTable<T>::end() {
		return { this, sparse_size() };
	}




}