#pragma once

#include <type_traits>
#include <vector>
#include <HBuffer.h>
#include <core.h>
#include <bit>

using qk::gfx::rhi::HVertexBuffer;

namespace qk::gfx {

	constexpr u32 SPARSEMAPPING_NULL_INDEX = UINT32_MAX;
	constexpr u16 SPARSEMAPPING_NULL_GENERATION = UINT16_MAX;


	template <class Tag_t>
	struct SparseMapping {

		struct SparseMappingNode {
			u32 dense = SPARSEMAPPING_NULL_INDEX;
			u16 generation = SPARSEMAPPING_NULL_GENERATION;
			u16 partition;
		};

		struct SparseMappingHandle {
			u32 sparse = SPARSEMAPPING_NULL_INDEX;
			u16 generation = 0;
		};

	private:
		std::vector<u32>			   m_DenseToSparseLookup;
		std::vector<u32>			   m_FreeList;
		std::vector<SparseMappingNode> m_Sparse;

		u32 find_sparse();

	public:
		SparseMappingHandle insert(u32 dense, u16 partition = 0);

		bool valid(SparseMappingHandle handle) const;
		bool remove(SparseMappingHandle hTarget, u32 with, u32& sparse);

		// Pointer to vector storage is asking for trouble, but the intended
		// usage should be sandboxed anyways.
		const SparseMappingNode* at(SparseMappingHandle hTarget) const;

		// Pointer to vector storage is asking for trouble, but the intended
		// usage should be sandboxed anyways.
		const SparseMappingNode* operator[](SparseMappingHandle hTarget) const;
	};


	enum class EInstancePoolQueryType : u8 {
		INSERT,
		REMOVE,
		SET,
		MOVE
	};


	template <class instance_t>
	struct InstancePoolQuery {
		static_assert(std::is_trivially_constructible_v<instance_t>);

		EInstancePoolQueryType type;

		// Guide: dense is the dense index relative to partition's m_Offset!

		union {
			struct {
				instance_t payload;
				u32 dense;
				u32 partition;
			} insert;

			struct {
				instance_t payload;
				u32 dense;
				u32 partition;
			} set;

			struct {
				u32 dense;

				// the dense index we are swapping with
				u32 with;
				u32 partition;
			} remove;

			// Unsupported for now.
			struct {
				u32 src_dense;
				u32 src_partition;
				u32 dest_dense;
				u32 dest_partition;
			} move;
		};
	};


	template <class instance_t>
	class InstancePoolQueue {
		static_assert(std::is_trivially_constructible_v<instance_t>);

		std::vector<InstancePoolQuery<instance_t>> m_Queue;

	public:
		void insert(u32 dense, const instance_t& payload, u32 partition = 0);

		void remove(
			u32 dense,
			u32 with,
			u32 partition = 0

		);

		void set(
			u32 dense,
			const instance_t& payload,
			u32 partition = 0
		);

		void move(
			u32 src_dense,
			u32 src_partition,
			u32 dest_dense,
			u32 dest_partition
		);

		auto begin();
		auto end();
	};


	// u32 resolve_dense() = partition.offset + dense (as slot index!!!)

	class InstancePoolPartition {
		size_t m_Size = 0;
		size_t m_Capacity = 0;
		size_t m_Offset = 0;
		i64 m_SizeDelta = 0;

	public:
		InstancePoolPartition(size_t initial_capacity, size_t initial_offset);

		// Size before a flush()
		size_t current_size() const;

		// Transforms an index from local space to buffer space
		size_t resolve(size_t index) const;

		// Capacity before a flush
		size_t current_capacity() const;

		// Size after a flush()
		size_t future_size() const;

		// Capacity after a flush()
		size_t future_capacity() const;

		void insert();
		void remove();

		size_t offset() const;

		void flush();

		void reserve(size_t min_size);

		void offset(size_t new_offset);
	};

	// temporary, for intellisense readability purposes
	using instance_t = int;

	struct InstancePool {
		struct InstanceTag {};
		
		using Map_t = SparseMapping<InstanceTag>;
		using Handle_t = Map_t::SparseMappingHandle;
		
		std::vector<InstancePoolPartition>			m_Partitions;
		Map_t										m_Sparse;
		InstancePoolQueue<instance_t>				m_Queue;

		Handle_t insert(const instance_t& initial_value, u32 partition = 0) {
			assert(partition < m_Partitions.size());
			// find a slot in the appropriate partition
			u32 dense = m_Partitions[partition].future_size();
			m_Partitions[partition].insert();

			// insert in the sparse set
			Handle_t handle = m_Sparse.insert(dense, partition);

			m_Queue.insert(dense, initial_value, partition);
			return handle;
		}

		void remove(Handle_t hTarget) {
			if (!m_Sparse.valid(hTarget))
				return;

			// get the partition associated with the handle
			u16 partition = m_Sparse[hTarget]->partition;
			u32 dense = m_Sparse[hTarget]->dense;


			// this shouldn't ever trigger if you appropriately partition the buffer, even at partition == 0
			assert(partition < m_Partitions.size());

			const u32 with = (m_Partitions[partition].future_size() - 1);

			// remove from the sparse set
			u32 sparse;
			m_Sparse.remove(hTarget, with, sparse);

			// add remove command to the queue
			m_Queue.remove(dense, partition);

		}

		void set(Handle_t hTarget, const instance_t& value) {
			if (!m_Sparse.valid(hTarget))
				return;
			
			u32 dense = m_Sparse[hTarget]->dense;
			u16 partition = m_Sparse[hTarget]->partition;

			m_Queue.set(dense, value, partition);

		}

		



	};


}

#include "InstancePool.inl"