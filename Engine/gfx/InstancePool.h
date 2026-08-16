#pragma once
#include <unordered_map> // replace with a better one when you get the chance
#include <HBuffer.h>
#include <vector>
#include <core.h>

using qk::gfx::rhi::HVertexBuffer;
using qk::gfx::rhi::VertexBuffer_ctor;
using qk::gfx::rhi::VertexBuffer_dtor;
using qk::gfx::rhi::VertexBuffer_free;
using qk::gfx::rhi::VertexBuffer_upload;
using qk::gfx::rhi::BufferUsage;
using qk::gfx::rhi::Resource_cast;

namespace qk::gfx {

	constexpr u32 INVALID_INSTANCE_ID = UINT32_MAX;
	constexpr u16 INVALID_GENERATION_ID = UINT16_MAX;

	constexpr u32 INITIAL_INSTANCEPOOL_SIZE = 64;


	struct InstanceID {
		u32 sparse_index = INVALID_INSTANCE_ID;
		u16 generation = INVALID_GENERATION_ID;
	};


	template <class instance_t>
	class InstancePool {

		struct SparseNode {
			u32 dense = INVALID_INSTANCE_ID;
			u16 generation = 0;
		};


		std::vector<instance_t> m_Dense;

		// sparse index -> dense index + generation
		std::unordered_map<u32, SparseNode> m_Sparse;

		// dense index -> sparse index
		std::vector<u32> m_DenseToSparse;

		// Reusable sparse indices.
		std::vector<u32> m_FreeSparseIndices;

		HVertexBuffer m_InstanceBuffer = nullptr;

		u32 m_NextSparseIndex = 0;


		static u16 next_generation(u16 generation) {
			++generation;

			// UINT16_MAX is reserved as the invalid generation.
			if (generation == INVALID_GENERATION_ID)
				generation = 0;

			return generation;
		}


	public:

		InstancePool() {
			m_Dense.reserve(INITIAL_INSTANCEPOOL_SIZE);
			m_DenseToSparse.reserve(INITIAL_INSTANCEPOOL_SIZE);

			m_InstanceBuffer = VertexBuffer_ctor(BufferUsage::DynamicDraw);
			Resource_cast(m_InstanceBuffer)->acquire();
		}


		~InstancePool() {
			// This should never trigger since it's set during construction but you never know XD
			assert(m_InstanceBuffer);
			Resource_cast(m_InstanceBuffer)->release();
			VertexBuffer_dtor(m_InstanceBuffer);
			VertexBuffer_free(m_InstanceBuffer);
		}


		size_t size() const {
			return m_Dense.size();
		}


		const HVertexBuffer buffer() const {
			return m_InstanceBuffer;
		}


		void upload() {
			VertexBuffer_upload(m_InstanceBuffer, m_Dense);
		}


		InstanceID insert(const instance_t& instance) {

			u32 sparse_index;

			if (!m_FreeSparseIndices.empty()) {

				// Reuse a previously freed sparse index.
				sparse_index = m_FreeSparseIndices.back();
				m_FreeSparseIndices.pop_back();

			}
			else {

				// Allocate a new sparse index.
				sparse_index = m_NextSparseIndex++;

				m_Sparse.emplace(
					sparse_index,
					SparseNode{}
				);
			}


			SparseNode& node = m_Sparse.at(sparse_index);

			const u32 dense_index =
				static_cast<u32>(m_Dense.size());


			node.dense = dense_index;

			m_Dense.push_back(instance);
			m_DenseToSparse.push_back(sparse_index);


			return InstanceID{
				.sparse_index = sparse_index,
				.generation = node.generation
			};
		}


		void remove(InstanceID id) {

			auto it = m_Sparse.find(id.sparse_index);

			if (it == m_Sparse.end())
				return;


			SparseNode& node = it->second;

			// Reject stale handles.
			if (node.generation != id.generation)
				return;


			const u32 dense_index = node.dense;
			const u32 last_index =
				static_cast<u32>(m_Dense.size() - 1);


			if (dense_index != last_index) {

				// Move the last instance into the removed slot.
				m_Dense[dense_index] =
					std::move(m_Dense[last_index]);


				// Find the sparse ID belonging to the moved instance.
				const u32 moved_sparse_index =
					m_DenseToSparse[last_index];


				// Update dense -> sparse mapping.
				m_DenseToSparse[dense_index] =
					moved_sparse_index;


				// Update sparse -> dense mapping.
				m_Sparse.at(moved_sparse_index).dense =
					dense_index;
			}


			m_Dense.pop_back();
			m_DenseToSparse.pop_back();


			// Invalidate this sparse node.
			node.dense = INVALID_INSTANCE_ID;
			node.generation = next_generation(node.generation);


			// Make the sparse index available for reuse.
			m_FreeSparseIndices.push_back(id.sparse_index);
		}


		instance_t* at(InstanceID id) {

			auto it = m_Sparse.find(id.sparse_index);

			if (it == m_Sparse.end())
				return nullptr;


			const SparseNode& node = it->second;


			// Stale or invalid handle.
			if (node.generation != id.generation)
				return nullptr;


			if (node.dense == INVALID_INSTANCE_ID)
				return nullptr;


			return &m_Dense[node.dense];
		}


		const instance_t* at(InstanceID id) const {

			auto it = m_Sparse.find(id.sparse_index);

			if (it == m_Sparse.end())
				return nullptr;


			const SparseNode& node = it->second;


			if (node.generation != id.generation)
				return nullptr;


			if (node.dense == INVALID_INSTANCE_ID)
				return nullptr;


			return &m_Dense[node.dense];
		}
	};

}