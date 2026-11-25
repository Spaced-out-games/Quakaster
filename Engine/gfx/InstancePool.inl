#include <cassert>
#include <cstring>
#include <vector>

namespace qk::gfx {

	template <class instance_t>
	void InstancePool<instance_t>::add_partition() {
		m_Partitions.emplace_back();
		m_ReverseHandleLookup.emplace_back();
	}

	template <class instance_t>
	bool InstancePool<instance_t>::valid(const InstancePoolTable_t& handle_table, InstancePoolHandle_t handle) const {
		return handle_table.valid(handle);
	}

	template <class instance_t>
	InstancePool<instance_t>::InstancePool(qk::gfx::rhi::HVertexBuffer hInstanceBuffer)
		: m_Storage(hInstanceBuffer) {
		QK_ASSERT(hInstanceBuffer, "hInstanceBuffer was nullptr");
		//add_partition();
	}

	// ---- private helpers --------------------------------------------------

	// Appends to the partition and records where the handle lives.
	template <class instance_t>
	void InstancePool<instance_t>::attach(u16 partition_index, u32 handle_sparse, const instance_t& payload) {
		auto& partition = m_Partitions.at(partition_index);
		const u32 local = partition.push(payload);

		auto& reverse = m_ReverseHandleLookup[partition_index];
		if (local >= reverse.size())
			reverse.resize(local + 1);
		reverse[local] = handle_sparse;

		if (handle_sparse >= m_Locations.size())
			m_Locations.resize(handle_sparse + 1);
		m_Locations[handle_sparse] = { local, partition_index };
	}

	// Swap-pops `local` out of the partition.
	// Does not touch the departing handle's Location; callers either kill the handle
	// or immediately overwrite it via attach().
	template <class instance_t>
	void InstancePool<instance_t>::detach(u16 partition_index, u32 local) {
		auto& partition = m_Partitions.at(partition_index);
		auto& reverse = m_ReverseHandleLookup[partition_index];

		QK_ASSERT(partition.new_size() > 0, "Partition.new_size() must be > 0");
		const u32 back = partition.new_size() - 1;
		QK_ASSERT(local <= back, "local > back");

		if (local != back) {
			// The back element moves into the hole.
			const u32 moved_handle_sparse = reverse[back];

			m_Locations[moved_handle_sparse].local_sparse = local;
			reverse[local] = moved_handle_sparse;
		}

		partition.swap_remove(local);
		reverse.pop_back();   // keeps the reverse table parallel to the partition
	}

	// ---- public API -------------------------------------------------------

	template <class instance_t>
	void InstancePool<instance_t>::insert(const InstancePoolTable_t& handle_table,
		const instance_t& payload,
		u16 target_partition,
		InstancePoolHandle_t handle) {
		QK_ASSERT(target_partition < m_Partitions.size(), "target_partition was out of bounds");

		if (target_partition >= m_Partitions.size() || !handle_table.valid(handle))
			return;

		attach(target_partition, handle.sparse, payload);
	}

	// NOTE: the caller must call handle_table.remove(handle) AFTER this returns.
	template <class instance_t>
	void InstancePool<instance_t>::remove(const InstancePoolTable_t& handle_table, InstancePoolHandle_t handle) {
		if (!handle_table.valid(handle))
			return;

		QK_ASSERT(handle.sparse < m_Locations.size(), "handle.sparse was out of bounds");

		const Location loc = m_Locations[handle.sparse];
		detach(loc.partition, loc.local_sparse);
	}

	template <class instance_t>
	void InstancePool<instance_t>::set(const InstancePoolTable_t& handle_table,
		InstancePoolHandle_t handle,
		const instance_t& value) {
		if (!handle_table.valid(handle))
			return;

		QK_ASSERT(handle.sparse < m_Locations.size(), "handle.sparse was out of bounds");
		const Location loc = m_Locations[handle.sparse];
		m_Partitions.at(loc.partition).set(loc.local_sparse, value);
	}

	template <class instance_t>
	void InstancePool<instance_t>::move(const InstancePoolTable_t& handle_table,
		InstancePoolHandle_t handle,
		const instance_t& initial_value,
		u16 new_partition) {
		QK_ASSERT(new_partition < m_Partitions.size(), "new_partition was out of bounds");
		if (new_partition >= m_Partitions.size() || !handle_table.valid(handle))
			return;

		QK_ASSERT(handle.sparse < m_Locations.size(), "handle.sparse was out of bounds");
		const Location loc = m_Locations[handle.sparse];

		// Same partition: just a write.
		if (loc.partition == new_partition) {
			m_Partitions.at(new_partition).set(loc.local_sparse, initial_value);
			return;
		}

		detach(loc.partition, loc.local_sparse);
		attach(new_partition, handle.sparse, initial_value); // overwrites m_Locations[handle.sparse]
	}

	template <class instance_t>
	size_t InstancePool<instance_t>::num_partitions() const {
		return m_Partitions.size();
	}

	template <class instance_t>
	InstancePoolPartition_t<instance_t>& InstancePool<instance_t>::get_partition(size_t index) {
		return m_Partitions.at(index);
	}



	template <class instance_t>
	void InstancePool<instance_t>::flush() {
		if (m_Partitions.empty())
			return;

		size_t total_capacity = 0;
		bool relayout = false;
		bool any_dirty = false;

		for (const auto& partition : m_Partitions) {
			total_capacity += partition.new_capacity();
			relayout |= (partition.new_capacity() != partition.capacity());
			any_dirty |= partition.dirty();
		}

		// A capacity change moves every later partition, so reallocate the buffer
		// and re-upload everything from the CPU copies. No GPU-side snapshot needed.
		// (m_Storage keeps the same handle but gets new, uninitialized backing memory.)
		if (relayout) {
			rhi::VertexBuffer_alloc(m_Storage, total_capacity * sizeof(instance_t));

			for (auto& partition : m_Partitions)
				partition.mark_all_dirty();

			any_dirty = true;
		}

		// Upload only the dirty span of each partition, through a single write-only mapping.
		// Nothing here reads from the mapped memory.
		if (any_dirty) {
			instance_t* pStorage = static_cast<instance_t*>(
				rhi::VertexBuffer_open(m_Storage, rhi::EBufferAccessFlags::WRITE));
			QK_ASSERT(pStorage, "failed to map the instance buffer");

			size_t offset = 0;
			for (auto& partition : m_Partitions) {
				const u32 lo = partition.dirty_min();
				// swap_remove can leave dirty_max past the end; clamp it.
				const u32 hi = std::min(partition.dirty_max(), partition.new_size());

				if (lo < hi) {
					std::memcpy(
						pStorage + offset + lo,
						partition.data() + lo,
						(hi - lo) * sizeof(instance_t));
				}

				offset += partition.new_capacity();
			}

			rhi::VertexBuffer_close(m_Storage);
		}

		// Always commit: popping the back element changes the size without dirtying anything.
		for (auto& partition : m_Partitions)
			partition.commit();
	}

}
