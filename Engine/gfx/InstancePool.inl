



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
	InstancePool<instance_t>::InstancePool(qk::gfx::rhi::HVertexBuffer hInstanceBuffer): m_Storage(hInstanceBuffer) {
		assert(hInstanceBuffer);
		add_partition();
	}

	template <class instance_t>
	void InstancePool<instance_t>::insert(const InstancePoolTable_t& handle_table, const instance_t& target_payload, u16 target_partition, InstancePoolHandle_t handle) {
		assert(target_partition < m_Partitions.size());


		// get the partition
		auto& partition = m_Partitions.at(target_partition);

		// get the metadata for the new handle
		InstancePoolHandleMetadata* pMeta = (InstancePoolHandleMetadata*)handle_table.try_get(handle);

		// this shouldn't ever be false but just in case I wrote it wrong...
		assert(pMeta);

		// allocate a slot in the partition
		u32 sparse_index = partition.queue_insert(target_payload);


		// update the metadata of the newly created node
		pMeta->local_sparse = sparse_index;
		pMeta->partition = target_partition;

		// let the reverse lookup map know wassup.
		auto& reverse = m_ReverseHandleLookup[target_partition];

		if (sparse_index == reverse.size())
			reverse.emplace_back();

		reverse[sparse_index] = handle.sparse;

		return;

	}


	template <class instance_t>
	InstancePoolHandle_t InstancePool<instance_t>::insert(InstancePoolTable_t& handle_table, const instance_t& target_payload, u16 target_partition) {
		assert(target_partition < m_Partitions.size());

		// create a new handle
		InstancePoolHandle_t handle = handle_table.insert(InstancePoolHandleMetadata{});

		insert(handle_table, target_payload, target_partition, handle);
		

		return handle;

	}







	

	template <class instance_t>
	void InstancePool<instance_t>::remove(InstancePoolTable_t& handle_table, InstancePoolHandle_t hTarget) {
		


		// get the details we need for deletion

		const InstancePoolHandleMetadata* pTargetMetadata = handle_table.try_get(hTarget);

		if (!pTargetMetadata)
			return;

		const u16 target_partition = pTargetMetadata->partition;
		const u32 target_index_sparse = pTargetMetadata->local_sparse;

		// get the partition of the target
		InstancePoolPartition_t& partition = m_Partitions.at(target_partition);
		auto& reverse = m_ReverseHandleLookup[target_partition];


		const u32 back_index = partition.new_size() - 1;
		const u32 back_handle_sparse = reverse[back_index];

		const u32 target_handle_sparse = hTarget.sparse;


		// all const up to this point in practice


		// swap - pop if not at end
		if (target_index_sparse != back_index) {
			// Handle occupying the physical back slot.

			// Swap the metadata associated with the two physical objects.
			handle_table.swap( // this mutates
				target_handle_sparse,
				back_handle_sparse
			);


			// The back object moved into target_index_sparse.
			auto* pMovedMetadata = handle_table.try_get_sparse(back_handle_sparse); // this mutates

			pMovedMetadata->local_sparse = target_index_sparse;

			// Reverse lookup follows the moved object.
			reverse[target_index_sparse] = back_handle_sparse;

			// Queue physical swap.
			partition.queue_swap(target_index_sparse, back_index);
		}

		// Not strictly necessary..moreso for poisoning to mark invariants clearly. Remove in release!!! Cheeky!
		// partition.queue_swap(back_index, back_index);

		// Shrink logical partition.	
		partition.pop_back();

		// Invalidate target's handle slot.
		handle_table.remove(target_handle_sparse);
	}

	template <class instance_t>
	void InstancePool<instance_t>::set(const InstancePoolTable_t& handle_table, InstancePoolHandle_t handle, const instance_t& value) {
		auto* pMetadata = handle_table.try_get(handle);

		if (!pMetadata)
			return;

		InstancePoolPartition_t& partition = m_Partitions.at(pMetadata->partition);

		partition.queue_set(
			value,
			pMetadata->local_sparse
		);
	}

	template <class instance_t>
	void InstancePool<instance_t>::move(const InstancePoolTable_t& handle_table, InstancePoolHandle_t handle, const instance_t& initial_value, u16 partition) {
		
		assert(partition < m_Partitions.size());

		if (partition >= m_Partitions.size())
			return;

		auto* pMetaData = handle_table.try_get(handle);

		if (!pMetaData) return;

		Partition& old_partition = m_Partitions.at(pMetaData->partition);

		Partition& new_partition = m_Partitions.at(partition);

		// delete from old
		old_partition.remove(pMetaData->local_sparse);

		// add to new partition
		pMetaData->local_sparse = new_partition.queue_insert(initial_value);
		pMetaData->partition = partition;


	}



	template <class instance_t>
	void InstancePool<instance_t>::flush(InstancePoolTable_t& handle_table) {
		size_t old_capacity = 0;
		size_t new_capacity = 0;

		for (const auto& partition : m_Partitions) {
			old_capacity += partition.capacity();
			new_capacity += partition.new_capacity();
		}

		if (old_capacity != new_capacity) {
			// Temporary buffer owns a separate RHI resource.
			rhi::HVertexBuffer old_storage = rhi::VertexBuffer_ctor(rhi::StaticDraw);
			rhi::VertexBuffer_alloc(
				old_storage,
				old_capacity * sizeof(instance_t)
			);

			// Preserve old contents.
			rhi::VertexBuffer_cpy(
				m_Storage,
				0,
				old_storage,
				0,
				old_capacity * sizeof(instance_t)
			);

			// m_Storage itself remains the same handle.
			rhi::VertexBuffer_alloc(
				m_Storage,
				new_capacity * sizeof(instance_t)
			);

			instance_t* pStorage = static_cast<instance_t*>(
				rhi::VertexBuffer_open(m_Storage, rhi::EBufferAccessFlags::WRITE)
				);

			size_t offset = 0;

			for (auto& partition : m_Partitions) {
				const size_t old_partition_capacity = partition.capacity();
				const size_t new_partition_capacity = partition.new_capacity();

				// Restore the old physical partition contents into its
				// new physical location.
				if (old_partition_capacity != 0) {
					rhi::VertexBuffer_cpy(
						old_storage,
						/* old byte offset */ 0,
						m_Storage,
						/* new byte offset */ offset * sizeof(instance_t),
						old_partition_capacity * sizeof(instance_t)
					);
				}

				partition.flush(pStorage + offset);

				offset += new_partition_capacity;
			}

			rhi::VertexBuffer_close(m_Storage);
			rhi::VertexBuffer_free(old_storage);

			return;
		}

		instance_t* pStorage = static_cast<instance_t*>(
			rhi::VertexBuffer_open(m_Storage, rhi::EBufferAccessFlags::WRITE)
			);

		size_t offset = 0;

		for (auto& partition : m_Partitions) {
			partition.flush(pStorage + offset);
			offset += partition.new_capacity();
		}

		rhi::VertexBuffer_close(m_Storage);
	}

	/* DEPRECATED
	template <class instance_t>
	void InstancePool<instance_t>::flush_test(InstancePoolTable_t& handle_table, std::vector<instance_t>& storage) {
		size_t old_capacity = 0;
		size_t new_capacity = 0;

		for (const auto& partition : m_Partitions) {
			old_capacity += partition.capacity();
			new_capacity += partition.new_capacity();
		}

		// Capacity changed: preserve the old backing storage while rebuilding.
		if (old_capacity != new_capacity) {
			std::vector<instance_t> old_storage = storage;

			storage.resize(new_capacity);

			size_t old_offset = 0;
			size_t new_offset = 0;

			for (auto& partition : m_Partitions) {
				const size_t old_partition_capacity = partition.capacity();
				const size_t new_partition_capacity = partition.new_capacity();

				std::copy(
					old_storage.begin() + old_offset,
					old_storage.begin() + old_offset + old_partition_capacity,
					storage.begin() + new_offset
				);

				partition.flush(storage.data() + new_offset);

				old_offset += old_partition_capacity;
				new_offset += new_partition_capacity;
			}

			return;
		}

		// No resize: apply queued operations directly.
		size_t offset = 0;

		for (auto& partition : m_Partitions) {
			partition.flush(storage.data() + offset);
			offset += partition.new_capacity();
		}
	}
	*/










}