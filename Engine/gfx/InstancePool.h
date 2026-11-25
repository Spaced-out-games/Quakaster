#pragma once
#include <Handle.h>
#include <HBuffer.h>
#include <core.h>

#include <algorithm>
#include <bit>
#include <type_traits>
#include <vector>


namespace qk::gfx {

    struct InstancePoolHandleMetadata {};

    using InstancePoolTable_t = qk::HandleTable<InstancePoolHandleMetadata>;
    using InstancePoolHandle_t = InstancePoolTable_t::Handle;


    // A contiguous run of instances. The CPU-side vector is the source of truth;
    // the GPU buffer is only ever written to, never read back.
    //
    // size()/capacity() describe what was committed to the GPU by the last flush().
    // new_size()/new_capacity() describe the pending (CPU-side) state.
    template <class instance_t>
    class InstancePoolPartition {
        static_assert(std::is_trivially_copyable_v<instance_t>,
            "instance_t must be trivially copyable (it is memcpy'd into a mapped buffer)");

        std::vector<instance_t> m_Data;

        u32 m_CommittedSize = 0;
        u32 m_CommittedCapacity = 0;
        u32 m_WantedCapacity = 1;   // power of two, grow-only

        // Half-open range [m_DirtyMin, m_DirtyMax) of elements that need uploading.
        u32 m_DirtyMin = ~0u;
        u32 m_DirtyMax = 0;

        void mark(u32 i) {
            m_DirtyMin = std::min(m_DirtyMin, i);
            m_DirtyMax = std::max(m_DirtyMax, i + 1);
        }

    public:
        // ---- committed (GPU-visible) state, as of the last flush
        u32 size() const { return m_CommittedSize; }
        u32 capacity() const { return m_CommittedCapacity; }

        // ---- pending (CPU) state
        u32 new_size() const { return static_cast<u32>(m_Data.size()); }
        u32 new_capacity() const { return m_WantedCapacity; }

        const instance_t* data() const { return m_Data.data(); }

        // Appends and returns the local index it landed at.
        u32 push(const instance_t& value) {
            m_Data.push_back(value);
            const u32 index = new_size() - 1;
            if (new_size() > m_WantedCapacity)
                m_WantedCapacity = std::bit_ceil(new_size());
            mark(index);
            return index;
        }

        void set(u32 index, const instance_t& value) {
            QK_ASSERT(index < m_Data.size(), "index was out of bounds");
            m_Data[index] = value;
            mark(index);
        }

        // Moves the back element into `index` and shrinks by one.
        // The caller is responsible for patching whatever tracked the moved element.
        void swap_remove(u32 index) {
            QK_ASSERT(index < m_Data.size(), "index was out of bounds");
            const u32 back = new_size() - 1;
            if (index != back) {
                m_Data[index] = m_Data[back];
                mark(index);
            }
            m_Data.pop_back();
        }

        // ---- used by InstancePool::flush
        bool dirty() const { return m_DirtyMin < m_DirtyMax; }
        u32  dirty_min() const { return m_DirtyMin; }
        u32  dirty_max() const { return m_DirtyMax; }
        void mark_all_dirty() { m_DirtyMin = 0; m_DirtyMax = new_size(); }

        void commit() {
            m_CommittedSize = new_size();
            m_CommittedCapacity = m_WantedCapacity;
            m_DirtyMin = ~0u;
            m_DirtyMax = 0;
        }
    };


    template <class instance_t>
    using InstancePoolPartition_t = InstancePoolPartition<instance_t>;


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

        // Uploads whatever changed since the last flush.
        void flush();

        size_t num_partitions() const;

        // debug
        InstancePoolPartition_t<instance_t>& get_partition(size_t index);

    };

}


#include "InstancePool.inl"