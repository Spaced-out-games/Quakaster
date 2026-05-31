#include "HMesh.h"


namespace qk::gfx::rhi {

	struct Mesh_meta {
		std::vector<RVertexBuffer>	hVertexBuffers;
		RIndexBuffer				hIndexBuffer;
		HVertexArray				hVertexArray;
		HMaterial					hMaterial;

	};

	static inline HandleResourceRegistry<Mesh_meta, HMesh> g_Meshes;


	HMesh Mesh_ctor(const Mesh_createinfo& createinfo) {

		Mesh_meta meta;

		// first, make the VertexArray
		meta.hVertexArray = rhi::VertexArray_ctor("placeholder");
		rhi::VertexArray_bind(meta.hVertexArray);

		


		// then add the vertex buffers
		meta.hVertexBuffers = createinfo.hVertexBuffers;

		// bind them for convenience
		for (const auto& hVertBuf : meta.hVertexBuffers) {
			QK_ASSERT(VertexBuffer_valid(hVertBuf));
			VertexBuffer_bind(hVertBuf);
		}
		meta.hIndexBuffer = createinfo.hIndexBuffer;


		// and the index buffer...
		if (IndexBuffer_valid(meta.hIndexBuffer)) IndexBuffer_bind(meta.hIndexBuffer);

		// then the layout
		HVertexLayout hLayout = rhi::VertexLayout_ctor(createinfo.layout_info);
		rhi::VertexLayout_apply(hLayout);


		// just use the same material.
		meta.hMaterial = createinfo.hMaterial;



		return g_Meshes.insert(meta);
	}

	void Mesh_dtor(const HMesh& hMesh) {
		g_Meshes.destroy(hMesh);
	}

	HVertexArray Mesh_getVertexArray(const HMesh& hMesh) {
		if (Mesh_valid(hMesh)) return g_Meshes.get(hMesh).hVertexArray;
		return {};
	}

	HMaterial Mesh_getMaterial(const HMesh& hMesh) {
		if (Mesh_valid(hMesh)) return g_Meshes.get(hMesh).hMaterial;
		return {};
	}

	RIndexBuffer Mesh_getIndexBuffer(const HMesh& hMesh) {
		if (Mesh_valid(hMesh)) return g_Meshes.get(hMesh).hIndexBuffer;
		return {};
	}

	const std::vector<RVertexBuffer>& const Mesh_getVertexBuffers(const HMesh& hMesh) {
		QK_ASSERT(Mesh_valid(hMesh));
		return g_Meshes.get(hMesh).hVertexBuffers;

	}


	bool Mesh_valid(const HMesh& hMesh) {
		return g_Meshes.valid(hMesh);
	}





}