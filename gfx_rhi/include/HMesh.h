#pragma once
#include "HVertexLayout.h"
#include "HVertexArray.h"
#include "HMaterial.h"
#include "HBuffer.h"


namespace qk::gfx::rhi {

	struct Mesh_createinfo {
		std::vector<RVertexBuffer>	hVertexBuffers;
		RIndexBuffer				hIndexBuffer;
		VertexLayout_createinfo		layout_info;
		HMaterial					hMaterial;
	};

	struct HMesh : Handle {};

	HMesh Mesh_ctor(const Mesh_createinfo& createinfo);

	void Mesh_dtor(const HMesh& hMesh);

	HVertexArray Mesh_getVertexArray(const HMesh& hMesh);

	HMaterial Mesh_getMaterial(const HMesh& hMesh);

	RIndexBuffer Mesh_getIndexBuffer(const HMesh& hMesh);

	const std::vector<RVertexBuffer>& const Mesh_getVertexBuffers(const HMesh& hMesh);

	bool Mesh_valid(const HMesh& hMesh);



}