#pragma once
#include <Partition.h>
#include <Handle.h>
#include <HBuffer.h>
#include <core.h>


namespace qk::gfx {

    struct InstancePoolHandleMetadata {};

    using InstancePoolTable_t = qk::HandleTable<InstancePoolHandleMetadata>;
    using InstancePoolHandle_t = InstancePoolTable_t::Handle;


    template <class instance_t>
    using InstancePoolPartition_t = qk::Partition<instance_t>;


    template <class instance_t>
    struct InstancePool {

    private:
        struct Location { u32 local_sparse = 0; u16 partition = 0; };

        std::vector<InstancePoolPartition_t<instance_t>> m_Partitions;
        std::vector<std::vector<u32>> m_ReverseHandleLookup;   // {partition, local} -> handle.sparse
        std::vector<Location> m_Locations;                     // handle.sparse -> {partition, local}
        qk::gfx::rhi::HVertexBuffer m_Storage = nullptr;

        void attach(u16 partition, u32 handle_sparse, const instance_t& payload);
        void detach(u16 partition, u32 local);

    public:
        void add_partition();
        bool valid(const InstancePoolTable_t&, InstancePoolHandle_t) const;
        InstancePool(qk::gfx::rhi::HVertexBuffer);

        void insert(const InstancePoolTable_t&, const instance_t&, u16, InstancePoolHandle_t);
        void remove(const InstancePoolTable_t&, InstancePoolHandle_t);
        void set(const InstancePoolTable_t&, InstancePoolHandle_t, const instance_t&);
        void move(const InstancePoolTable_t&, InstancePoolHandle_t, const instance_t& initial_value, u16 partition);

        void flush();   // table param dropped, it was never used

        size_t num_partitions() const;

        // debug
        InstancePoolPartition_t<instance_t>& get_partition(size_t index);




    };




}


#include "InstancePool.inl"