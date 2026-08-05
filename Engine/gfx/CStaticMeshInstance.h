#pragma once
#include <HBuffer.h>
#include <math/geometric/Mat4.h>
#include <emhash/hash_table6.hpp>

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

using qk::gfx::rhi::HVertexBuffer;
using qk::gfx::rhi::VertexBuffer_upload;

namespace qk::gfx {

	struct CStaticMeshInstance {
		HVertexBuffer hVertexBuffer;
	};

	template <class Instance_t>
	using InstanceStagingBuffer_t = emhash6::HashMap<HVertexBuffer, std::vector<Instance_t>>;


	void clearStaticMeshInstanceBuffers(InstanceStagingBuffer_t<Mat4>& batchmap) {
		for (auto& it : batchmap) {
			it.second.clear();
		}
	}


	void genStaticMeshInstanceBuffers(InstanceStagingBuffer_t<Mat4>& batchmap, const entt::registry& registry) {
		auto view = registry.view<CStaticMeshInstance, CMat4>();

		

		for (auto entity : view)
		{
			auto& mesh = view.get<CStaticMeshInstance>(entity);
			auto& model = view.get<CMat4>(entity);

			batchmap[mesh.hVertexBuffer].push_back(model);
		}
	}

	void uploadStaticMeshInstanceBuffers(const InstanceStagingBuffer_t<Mat4>& batchmap) {
		for (const auto& it : batchmap) {
			VertexBuffer_upload(it.first, it.second);
		}
	}
	


}