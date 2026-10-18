#include <cassert>
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
		assert(hInstanceBuffer);
		add_partition();
	}

	// ---- private helpers --------------------------------------------------

	// Appends to the partition's logical state and records where the handle lives.
	template <class instance_t>
	void InstancePool<instance_t>::attach(u16 partition_index, u32 handle_sparse, const instance_t& payload) {
		auto& partition = m_Partitions.at(partition_index);
		const u32 local = partition.queue_insert(payload);

		auto& reverse = m_ReverseHandleLookup[partition_index];
		if (local >= reverse.size())
			reverse.resize(local + 1);
		reverse[local] = handle_sparse;

		if (handle_sparse >= m_Locations.size())
			m_Locations.resize(handle_sparse + 1);
		m_Locations[handle_sparse] = { local, partition_index };
	}

	// Swap-pops `local` out of the partition's logical state and queues the physical swap.
	// Does not touch the departing handle's Location; callers either kill the handle
	// or immediately overwrite it via attach().
	template <class instance_t>
	void InstancePool<instance_t>::detach(u16 partition_index, u32 local) {
		auto& partition = m_Partitions.at(partition_index);
		auto& reverse = m_ReverseHandleLookup[partition_index];

		assert(partition.new_size() > 0);
		const u32 back = partition.new_size() - 1;
		assert(local <= back);

		if (local != back) {
			// The back element moves into the hole.
			const u32 moved_handle_sparse = reverse[back];

			m_Locations[moved_handle_sparse].local_sparse = local;
			reverse[local] = moved_handle_sparse;

			partition.queue_swap(local, back);
		}

		partition.pop_back();
	}

	// ---- public API -------------------------------------------------------

	template <class instance_t>
	void InstancePool<instance_t>::insert(const InstancePoolTable_t& handle_table,
		const instance_t& payload,
		u16 target_partition,
		InstancePoolHandle_t handle) {
		assert(target_partition < m_Partitions.size());
		assert(handle_table.valid(handle));

		if (target_partition >= m_Partitions.size() || !handle_table.valid(handle))
			return;

		attach(target_partition, handle.sparse, payload);
	}

	// NOTE: the caller must call handle_table.remove(handle) AFTER this returns.
	template <class instance_t>
	void InstancePool<instance_t>::remove(const InstancePoolTable_t& handle_table, InstancePoolHandle_t handle) {
		if (!handle_table.valid(handle))
			return;

		assert(handle.sparse < m_Locations.size());
		const Location loc = m_Locations[handle.sparse];
		detach(loc.partition, loc.local_sparse);
	}

	template <class instance_t>
	void InstancePool<instance_t>::set(const InstancePoolTable_t& handle_table,
		InstancePoolHandle_t handle,
		const instance_t& value) {
		if (!handle_table.valid(handle))
			return;

		assert(handle.sparse < m_Locations.size());
		const Location loc = m_Locations[handle.sparse];
		m_Partitions.at(loc.partition).queue_set(value, loc.local_sparse);
	}

	template <class instance_t>
	void InstancePool<instance_t>::move(const InstancePoolTable_t& handle_table,
		InstancePoolHandle_t handle,
		const instance_t& initial_value,
		u16 new_partition) {
		assert(new_partition < m_Partitions.size());
		if (new_partition >= m_Partitions.size() || !handle_table.valid(handle))
			return;

		assert(handle.sparse < m_Locations.size());
		const Location loc = m_Locations[handle.sparse];

		// Same partition: just a write.
		if (loc.partition == new_partition) {
			m_Partitions.at(new_partition).queue_set(initial_value, loc.local_sparse);
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
	const InstancePoolPartition_t<instance_t>& InstancePool<instance_t>::get_partition(size_t index) const {
		return m_Partitions.at(index);
	}



	template <class instance_t>
	void InstancePool<instance_t>::flush() {
		size_t old_capacity = 0;
		size_t new_capacity = 0;

		for (const auto& partition : m_Partitions) {
			old_capacity += partition.capacity();
			new_capacity += partition.new_capacity();
		}

		if (old_capacity != new_capacity) {
			// Snapshot the old contents into a temporary buffer.
			rhi::HVertexBuffer old_storage = nullptr;
			if (old_capacity != 0) {
				old_storage = rhi::VertexBuffer_ctor(rhi::StaticDraw);
				rhi::VertexBuffer_alloc(old_storage, old_capacity * sizeof(instance_t));
				rhi::VertexBuffer_cpy(m_Storage, 0, old_storage, 0, old_capacity * sizeof(instance_t));
			}

			// m_Storage keeps the same handle but gets new backing memory.
			rhi::VertexBuffer_alloc(m_Storage, new_capacity * sizeof(instance_t));

			// Restore each partition into its new physical location.
			// Do all device-side copies BEFORE mapping the buffer.
			size_t old_offset = 0;
			size_t new_offset = 0;

			for (const auto& partition : m_Partitions) {
				const size_t old_cap = partition.capacity();
				const size_t new_cap = partition.new_capacity();

				if (old_cap != 0) {
					rhi::VertexBuffer_cpy(
						old_storage,
						old_offset * sizeof(instance_t),
						m_Storage,
						new_offset * sizeof(instance_t),
						old_cap * sizeof(instance_t)
					);
				}

				old_offset += old_cap;
				new_offset += new_cap;
			}

			if (old_storage) {
				rhi::Resource_cast(old_storage)->flag_for_deletion();
				rhi::VertexBuffer_free(old_storage);
			}
		}

		// Apply the queued ops through a single mapping.
		instance_t* pStorage = static_cast<instance_t*>(rhi::VertexBuffer_open(m_Storage, rhi::EBufferAccessFlags::WRITE));

		size_t offset = 0;
		for (auto& partition : m_Partitions) {
			partition.flush(pStorage + offset);
			offset += partition.new_capacity();
		}

		rhi::VertexBuffer_close(m_Storage);
	}

}