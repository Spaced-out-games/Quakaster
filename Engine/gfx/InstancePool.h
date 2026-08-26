#pragma once
#include <Partition.h>
#include <Handle.h>
#include <HBuffer.h>
#include <core.h>

using qk::gfx::rhi::HVertexBuffer;

namespace qk::gfx {

	struct InstancePoolHandleEntry {
		u16 partition = 0;

	};

	template <class T>
	struct InstancePool {
		HVertexBuffer m_hInstanceBuffer = nullptr;

		using Handle_t = HandleTable<InstancePoolHandleEntry>::Handle;

		HandleTable<InstancePoolHandleEntry> m_HandleMap;


		InstancePool() {
			m_hInstanceBuffer = rhi::VertexBuffer_ctor();
		}

		Handle_t insert(const T& payload)



	};


}

#include "InstancePool.inl"