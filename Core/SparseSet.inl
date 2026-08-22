



namespace qk {


	template <class payload_t>
	bool SparseSet<payload_t>::valid(SparseSetHandle handle) const {
		if (handle.sparse >= m_Sparse.size())
			return false;

		const auto& node = m_Sparse[handle.sparse];

		return node.dense != SparseSet_NULL_INDEX &&
			node.generation == handle.generation && node.alive;




	}

	template <class payload_t>
	u32 SparseSet<payload_t>::sparse(u32 dense) const {
		if (dense >= m_SparseReverseLookup.size())
			return SparseSet_NULL_INDEX;

		return m_SparseReverseLookup[dense];
	}

	template <class payload_t>
	bool SparseSet<payload_t>::alive(u32 sparse) const {
		if (sparse >= m_Sparse.size())
			return false;

		const auto& node = m_Sparse[sparse];

		return node.alive;
	}


	template <class payload_t>
	u32 SparseSet<payload_t>::dense(u32 sparse) const {
		if (!alive(sparse))
			return SparseSet_NULL_INDEX;

		return m_Sparse[sparse].dense;

	}

	// Generates a handle to dense[i]
	template <class payload_t>
	SparseSet<payload_t>::SparseSetHandle SparseSet<payload_t>::insert(payload_t payload, u32 dense) {
		// warn of double-insertion attempt
		assert(
			dense >= m_SparseReverseLookup.size() ||
			!alive(m_SparseReverseLookup[dense])
		);

		u32 sparse_id = 0;
		if (!m_FreeList.empty()) {
			sparse_id = m_FreeList.back();
			m_FreeList.pop_back();
			m_Sparse[sparse_id].dense = dense;
			u16 generation = m_Sparse[sparse_id].generation + 1;
			m_Sparse[sparse_id].generation = generation;
			m_Sparse[sparse_id].alive = true;
			m_SparseReverseLookup[dense] = sparse_id;
			m_Dense[dense] = payload;



			return SparseSetHandle{
				.sparse = sparse_id,
				.generation = generation
			};

		}

		sparse_id = m_Sparse.size();

		m_Sparse.emplace_back(dense, 0, true);
		m_SparseReverseLookup.emplace_back(sparse_id);
		m_Dense.push_back(payload);
		return SparseSetHandle{
			.sparse = sparse_id,
			.generation = 0
		};


	}

	template <class payload_t>
	void SparseSet<payload_t>::resize(size_t size) {
		m_Dense.resize(size);
		m_SparseReverseLookup.resize(size);
	}

	template <class payload_t>
	payload_t* SparseSet<payload_t>::try_get(SparseSetHandle handle) {
		if (!valid(handle))
			return nullptr;

		return m_Dense.data() + m_Sparse[handle.sparse].dense;
	}

	template <class payload_t>
	const payload_t* SparseSet<payload_t>::try_get(SparseSetHandle handle) const {
		if (!valid(handle))
			return nullptr;

		return m_Dense.data() + m_Sparse[handle.sparse].dense;
	}



	template <class payload_t>
	size_t SparseSet<payload_t>::free() const {
		return m_FreeList.size();
	}

	template <class payload_t>
	size_t SparseSet<payload_t>::capacity() const {
		return m_Sparse.size();
	}

	template <class payload_t>
	size_t SparseSet<payload_t>::size() const {
		return m_SparseReverseLookup.size();
	}

	// Swaps the dense indices between two sparse nodes.
	template <class payload_t>
	void SparseSet<payload_t>::swap_dense(u32 sparse_a, u32 sparse_b)
	{
		assert(sparse_a < m_Sparse.size());
		assert(sparse_b < m_Sparse.size());

		if (sparse_a == sparse_b)
			return;

		const u32 dense_a = m_Sparse[sparse_a].dense;
		const u32 dense_b = m_Sparse[sparse_b].dense;

		assert(dense_a != SparseSet_NULL_INDEX);
		assert(dense_b != SparseSet_NULL_INDEX);

		m_Sparse[sparse_a].dense = dense_b;
		m_Sparse[sparse_b].dense = dense_a;

		std::swap(
			m_SparseReverseLookup[dense_a],
			m_SparseReverseLookup[dense_b]
		);

		std::swap(m_Dense[dense_a], m_Dense[dense_b]);


	}

	// deletes a node by sparse ID
	template <class payload_t>
	void SparseSet<payload_t>::erase(u32 sparse)
	{

		if (sparse >= m_Sparse.size())
			return;

		u32 dense = m_Sparse[sparse].dense;

		if (dense == SparseSet_NULL_INDEX)
			return;
		// add it to the freelist
		m_FreeList.push_back(sparse);

		// invalidate the handle with this sparse index
		m_Sparse[sparse].alive = false;

	}




}