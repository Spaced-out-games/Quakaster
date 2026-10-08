#pragma once
#include <Partition.h>
#include <Handle.h>
#include <HBuffer.h>
#include <core.h>


namespace qk::gfx {

	struct InstancePoolHandleMetadata {
		// local sparse index w.r.t the partition
		u32 local_sparse = 0;
		u16 partition = 0;

	};


	// we need these untemplated and outside the struct for the handle
	using InstancePoolTable_t = qk::HandleTable<InstancePoolHandleMetadata>;



	using InstancePoolHandle_t = InstancePoolTable_t::Handle;

	struct InstancePoolBase {
		// this needs to be here so std::unique_ptr knows how to destroy it.
		virtual ~InstancePoolBase() = 0;

		// we also need a virtual instance destructor as well
		virtual void remove(InstancePoolTable_t& handle_table, InstancePoolHandle_t hTarget) = 0;
	};

	// ~InstancePool<T>() is default-destroyed.
	inline InstancePoolBase::~InstancePoolBase() = default;



	template <class instance_t>
	struct InstancePool: InstancePoolBase {

		using InstancePoolPartition_t = qk::Partition<instance_t>;

		private:
		std::vector<InstancePoolPartition_t> m_Partitions;
		// todo: dependency inject this up an abstraction layer so we can have one handle map to multple slots at once
		//InstancePoolTable_t m_HandleTable;
		// Maps {partition, local_sparse_index} -> InstancePoolHandleMetadata index
		std::vector<std::vector<u32>> m_ReverseHandleLookup;
		qk::gfx::rhi::HVertexBuffer m_Storage = nullptr;

		public:


		void add_partition();

		bool valid(const InstancePoolTable_t& handle_table, InstancePoolHandle_t handle) const;

		InstancePool(qk::gfx::rhi::HVertexBuffer);


		// insert functionality if we already have a valid handle to link to
		void insert(const InstancePoolTable_t& handle_table, const instance_t& payload, u16 partition, InstancePoolHandle_t handle);
		InstancePoolHandle_t insert(InstancePoolTable_t& handle_table, const instance_t& target_payload, u16 target_partition);


		void remove(InstancePoolTable_t& handle_table, InstancePoolHandle_t hTarget) override;


		// flushes the command queue. Probably needs to be virtual
		void flush(InstancePoolTable_t& handle_table);


		// test override
		//void flush_test(InstancePoolTable_t& handle_table, std::vector<instance_t>& storage);


		void set(const InstancePoolTable_t& handle_table, InstancePoolHandle_t handle, const instance_t& value);



		// TODO: test inter-partition move
		
		void move(const InstancePoolTable_t& handle_table, InstancePoolHandle_t handle, const instance_t& initial_value, u16 partition);


	};


}

#include "InstancePool.inl"