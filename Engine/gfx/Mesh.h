#pragma once
#include <HVertexArray.h>
#include <HVertexBuffer.h>
#include <HVertexBufferLayout.h>
#include "Material.h"


namespace qk::gfx {

	
	struct Mesh {
		rhi::HVertexArray m_hVertexArray;
		rhi::HVertexBuffer m_hVertexBuffer;
		//HIndexBuffer m_hIndexBuffer;

		Material m_Material;


		template <class vertex_t>
		void init(	const std::vector<vertex_t>& vertices, const rhi::HVertexBufferLayout& hLayout,
					const std::string& vertSrc, const std::string& fragSrc, rhi::BufferUsage usage) {
			assert(!m_hVertexArray.valid());
			assert(!m_hVertexBuffer.valid());

			m_hVertexArray = rhi::VertexArray_ctor();
			rhi::VertexArray_bind(m_hVertexArray);

			m_hVertexBuffer = rhi::VertexBuffer_ctor();

			rhi::VertexBuffer_upload(m_hVertexBuffer, vertices.data(), 
				vertices.size(), sizeof(vertex_t), usage
			);

			rhi::VertexArray_applyLayout(m_hVertexArray, m_hVertexBuffer, hLayout);

			m_Material.init(vertSrc, fragSrc);

		}

		template <class T>
		void pushUniform(const std::string& name, T* data) {
			m_Material.pushUniform<T>(name, data);
		}

		void bind() const {
			rhi::VertexArray_bind(m_hVertexArray);
			m_Material.bind();
		}

		void update_uniforms() const {
			m_Material.update();
		}

	};


}