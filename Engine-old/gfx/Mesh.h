#pragma once
#include "../core.h"
#include "VertexBufferLayout.h"
#include "TextureBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"


namespace qk::gfx {

	struct VertexGroup {
		uint32_t index_offset;
		uint32_t index_count;
	};


	using LOD_t = uint8_t;
	constexpr LOD_t LOD_CULL = 255;

	// 
	template <uint8_t num_LODs = 8>
	struct TLODTable {
		// we need room for billboards and culling
		static_assert(num_LODs < 254);

		float LOD_area_cutoffs[num_LODs];
		float billboard_area_cutoffs;
		float cull_area_cutoff;

		VertexGroup LOD_partitions[num_LODs + 1];

		// Binary searches for the LOD since it's probably in cache lines anyways.
		LOD_t calc_LOD(float area) const {
			// Cull first
			if (area < cull_area_cutoff)
				return LOD_CULL;

			// Billboard
			if (area < billboard_area_cutoffs)
				return num_LODs; // last slot = billboard

			// Regular LODs
			for (LOD_t i = num_LODs - 1; i != 255; --i) {
				if (area >= LOD_area_cutoffs[i])
					return i;
			}

			// Fallback (small but not billboard)
			return num_LODs - 1;
		}
	};

	using LODTable = TLODTable<8>;



	// Basic LOD-less meshes (for now)
	class MeshInstancePool_impl {
		// object
		VertexArray			m_VertexArray;

		// vertex data
		VertexBuffer_impl	m_Geometry;


		// instance data
		VertexBuffer_impl	m_InstanceData;
		
		protected:

		void init_impl(const VertexBufferLayout& vertex_layout, const VertexBufferLayout& instance_layout) {

			m_VertexArray.init();
			m_VertexArray.bind();
			
			m_Geometry.init_impl();
			m_Geometry.bind_impl();
			m_VertexArray.apply(m_Geometry, vertex_layout);

			m_InstanceData.init_impl();
			m_InstanceData.bind_impl();
			m_VertexArray.apply(m_InstanceData, instance_layout);


		}

		void upload_geometry_impl(const void* vertices, size_t vertex_count, size_t vertex_size) {
			m_Geometry.upload_impl(vertices, vertex_count, vertex_size, GL_STATIC_DRAW);
		}
		void upload_instances_impl(const void* instances, size_t instance_count, size_t instance_size) {
			m_InstanceData.upload_impl(instances, instance_count, instance_size, GL_STREAM_DRAW);
		}

	};

	template <class vertex_t, class instance_t>
	class MeshInstancePool: public MeshInstancePool_impl {


		std::vector<instance_t> m_Instances;
		public:

			void init(const VertexBufferLayout& vertex_layout, const VertexBufferLayout& instance_layout) {
				init_impl(vertex_layout, instance_layout);
			}

			void upload_geometry(const vertex_t* vertices, size_t vertex_count) {
				upload_geometry_impl(vertices, vertex_count, sizeof(vertex_t));
			}

			void upload_instances() {
				upload_instances_impl(m_Instances.data(), m_Instances.size(), sizeof(instance_t));
			}

			// accessors for m_Instances



	};

	
}