#pragma once
#include <vector>
#include <core.h>


namespace qk {


	constexpr u32 SparseSet_NULL_INDEX = UINT32_MAX;



	template <class payload_t>
	struct SparseSet {


		struct SparseSetNode {
			u32 dense = SparseSet_NULL_INDEX;
			u16 generation = 0;
			bool alive = false;
		};

		struct SparseSetHandle {
			u32 sparse = SparseSet_NULL_INDEX;
			u16 generation = 0;
		};


	//private:

		// sparse mapping
		std::vector<SparseSetNode> m_Sparse;

		// O(1) reverse lookup
		std::vector<u32>			   m_SparseReverseLookup;

		// dead nodes
		std::vector<u32>			   m_FreeList;

		// dense storage.
		std::vector<payload_t> m_Dense;

	public:
		bool valid(SparseSetHandle handle) const;

		u32 sparse(u32 dense) const;

		bool alive(u32 sparse) const;

		u32 dense(u32 sparse) const;

		size_t free() const;

		size_t capacity() const;

		size_t size() const;

		void resize(size_t size);


		payload_t* try_get(SparseSetHandle handle);


		const payload_t* try_get(SparseSetHandle handle) const;

		// Generates a handle to dense[i]
		SparseSetHandle insert(payload_t payload, u32 dense);

		// Swaps the dense indices between two sparse nodes.
		void swap_dense(u32 sparse_a, u32 sparse_b);

		// deletes a node by sparse ID
		void erase(u32 sparse);

	};


}

#include "SparseSet.inl"