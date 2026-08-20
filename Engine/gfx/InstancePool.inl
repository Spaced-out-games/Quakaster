#pragma once

namespace qk::gfx {

	// -------------------------------------------------------------------------
	// SparseMapping
	// -------------------------------------------------------------------------

	template <class Tag_t>
	u32 SparseMapping<Tag_t>::find_sparse() {
		if (m_FreeList.empty())
			return (u32)m_Sparse.size();

		u32 sparse = m_FreeList.back();
		m_FreeList.pop_back();

		return sparse;
	}


	template <class Tag_t>
	typename SparseMapping<Tag_t>::SparseMappingHandle
	SparseMapping<Tag_t>::insert(u32 dense, u16 partition) {
		u32 sparse_index = find_sparse();
		m_DenseToSparseLookup.push_back(sparse_index);
		if (sparse_index == m_Sparse.size()) {
			m_Sparse.emplace_back(dense, 0, partition);
		}
		else {
			m_Sparse[sparse_index] = {
				.dense = dense,
				.generation = (u16)m_Sparse[sparse_index].generation + 1u,
				.partition = partition
			};
		}

		return SparseMappingHandle{
			.sparse = sparse_index,
			.generation = m_Sparse[sparse_index].generation
		};
	}


	template <class Tag_t>
	bool SparseMapping<Tag_t>::valid(
		SparseMappingHandle handle
	) const {
		if (handle.sparse >= m_Sparse.size())
			return false;

		const auto& node = m_Sparse[handle.sparse];

		return node.dense != SPARSEMAPPING_NULL_INDEX &&
			node.generation == handle.generation;
	}


	template <class Tag_t>
	bool SparseMapping<Tag_t>::remove(SparseMappingHandle hTarget, u32 with, u32& sparse) {
		if (!valid(hTarget))
			return false;
		assert(with < m_DenseToSparseLookup.size());
		sparse = m_DenseToSparseLookup[with];
		m_Sparse[hTarget.sparse].dense = SPARSEMAPPING_NULL_INDEX;
		m_FreeList.push_back(hTarget.sparse);


		return true;
	}


	template <class Tag_t>
	const typename SparseMapping<Tag_t>::SparseMappingNode*
		SparseMapping<Tag_t>::at(
			SparseMappingHandle hTarget
		) const {
		if (!valid(hTarget))
			return nullptr;

		return &m_Sparse[hTarget.sparse];
	}


	template <class Tag_t>
	const typename SparseMapping<Tag_t>::SparseMappingNode*
		SparseMapping<Tag_t>::operator[](
			SparseMappingHandle hTarget
			) const {
		return at(hTarget);
	}


	// -------------------------------------------------------------------------
	// InstancePoolQueue
	// -------------------------------------------------------------------------

	template <class instance_t>
	void InstancePoolQueue<instance_t>::insert(
		u32 dense,
		const instance_t& payload,
		u32 partition
	) {
		m_Queue.emplace_back(InstancePoolQuery<instance_t>{
			.type = EInstancePoolQueryType::INSERT,
				.insert = {
					.payload = payload,
					.dense = dense,
					.partition = partition,
			}
		});
	}


	template <class instance_t>
	void InstancePoolQueue<instance_t>::remove(
		u32 dense,
		u32 with,
		u32 partition
		
	) {
		m_Queue.emplace_back(InstancePoolQuery<instance_t>{
			.type = EInstancePoolQueryType::REMOVE,
				.remove = {
					.dense = dense,
					.with = with,
					.partition = partition
			}
		});
	}


	template <class instance_t>
	void InstancePoolQueue<instance_t>::set(
		u32 dense,
		const instance_t& payload,
		u32 partition
	) {
		m_Queue.emplace_back(InstancePoolQuery<instance_t>{
			.type = EInstancePoolQueryType::SET,
				.set = {
					.payload = payload,
					.dense = dense,
					.partition = partition
			}
		});
	}


	template <class instance_t>
	void InstancePoolQueue<instance_t>::move(
		u32 src_dense,
		u32 src_partition,
		u32 dest_dense,
		u32 dest_partition
	) {
		m_Queue.emplace_back(InstancePoolQuery<instance_t>{
			.type = EInstancePoolQueryType::MOVE,
				.move = {
					.src_dense = src_dense,
					.src_partition = src_partition,
					.dest_dense = dest_dense,
					.dest_partition = dest_partition
			}
		});
	}


	template <class instance_t>
	auto InstancePoolQueue<instance_t>::begin() {
		return m_Queue.begin();
	}


	template <class instance_t>
	auto InstancePoolQueue<instance_t>::end() {
		return m_Queue.end();
	}

}