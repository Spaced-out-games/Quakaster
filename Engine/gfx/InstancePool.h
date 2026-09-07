#pragma once
#include <Partition.h>
#include <Handle.h>
#include <HBuffer.h>
#include <core.h>


namespace qk::gfx {

	struct InstancePoolHandleMetadata {
		u32 local_sparse = 0;
		u16 partition = 0;

	};

	template <class instance_t>
	struct InstancePool {

		using Table_t = qk::HandleTable<InstancePoolHandleMetadata>;
		using Partition_t = qk::Partition<instance_t>;
		using Handle_t = Table_t::Handle;

		private:
		std::vector<Partition_t> m_Partitions;
		Table_t m_HandleTable;
		// Maps {partition, local_sparse_index} -> InstancePoolHandleMetadata index
		std::vector<std::vector<u32>> m_ReverseHandleLookup;
		qk::gfx::rhi::HVertexBuffer m_Storage = nullptr;

		public:


		void add_partition();

		bool valid(Handle_t handle) const;

		InstancePool();


		Handle_t insert(const instance_t& target_payload, u16 target_partition);

		void remove(Handle_t hTarget);



		void flush();


		// test override
		void flush_test(std::vector<instance_t>& storage);


		void set(Handle_t handle, const instance_t& value);
		

	};


}

#include "InstancePool.inl"