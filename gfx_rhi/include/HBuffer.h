#pragma once
//#include <Handle.h>
#include <cstdint>
#include "core.h"
#include <rhi_Resource.h>
//using qk::Handle;



namespace qk::gfx::rhi {

	enum BufferUsage {
		StaticDraw,
		DynamicDraw,
		StreamDraw
	};

	enum class BufferType {
		Unknown,
		Vertex,
		Index,
		Uniform,
		Storage
	};


	struct RBuffer : Resource {};


	// ============================================================
	// Core API
	// ============================================================

	RBuffer Buffer_ctor(const str_view& label, BufferType type, BufferUsage usage = BufferUsage::StaticDraw);
	
	str_view Buffer_name(const RBuffer& hUniformBuffer);

	bool Buffer_dtor(const RBuffer& rBuf);
	bool Buffer_upload(
		const RBuffer& hBuf,
		const void* data,
		u32 count,
		u32 elem_size);

	bool Buffer_upload(const RBuffer& rBuf, const void* pIndices, u32 count, u32 size);

	bool Buffer_valid(const RBuffer& rBuf);


	bool Buffer_initialized(const RBuffer& hBuf);

	BufferType Buffer_type(const RBuffer& hBuf);

	// ============================================================
	// Index Buffer API
	// ============================================================



	struct RIndexBuffer : Resource {};
	

	inline RIndexBuffer IndexBuffer_ctor(str_view label, BufferUsage usage = BufferUsage::StaticDraw) {
		return RIndexBuffer(Buffer_ctor(label, BufferType::Index, usage));
	}



	bool IndexBuffer_bind(const RIndexBuffer& hBuf);

	inline bool IndexBuffer_upload(
		const RIndexBuffer& hIndexBuffer,
		const void* data,
		u32 count,
		u32 elem_size) {
		return Buffer_upload(RBuffer(hIndexBuffer), data, count, elem_size);
	}

	inline bool IndexBuffer_valid(const RIndexBuffer& hIndexBuffer) {
		return Buffer_valid(RBuffer(hIndexBuffer));
	}

	inline bool IndexBuffer_initialized(const RIndexBuffer& hIndexBuffer) {
		return Buffer_initialized(RBuffer(hIndexBuffer));
	}

	// ============================================================
	// Vertex Buffer API
	// ============================================================


	struct RVertexBuffer : Resource {};



	inline RVertexBuffer VertexBuffer_ctor(str_view label, BufferUsage usage = BufferUsage::StaticDraw) {
		return RVertexBuffer(Buffer_ctor(label, BufferType::Vertex, usage));
	}


	inline bool VertexBuffer_dtor(const RVertexBuffer& hVertexBuffer) {
		return Buffer_dtor(RBuffer(hVertexBuffer));
	}



	bool VertexBuffer_bind(const RVertexBuffer& hBuf);

	bool VertexBuffer_bind(const RVertexBuffer& hVBuf);


	inline bool VertexBuffer_upload(
		const RVertexBuffer& hVertexBuffer,
		const void* data,
		u32 count,
		u32 elem_size) {
		
		return Buffer_upload(RBuffer(hVertexBuffer), data, count, elem_size);
	}



	template <class vertex_t>
	inline bool VertexBuffer_upload(const RVertexBuffer& hVertexBuffer, const std::vector<vertex_t>& vertices) {
		return VertexBuffer_upload(hVertexBuffer, vertices.data(), vertices.size(), sizeof(vertex_t));
	}

	inline bool VertexBuffer_valid(const RVertexBuffer& hVertexBuffer) {
		return Buffer_valid(RBuffer(hVertexBuffer));
	}

	inline bool VertexBuffer_initialized(const RVertexBuffer& hVertexBuffer) {
		return Buffer_initialized(RBuffer(hVertexBuffer));
	}

	template <class vertex_t>
	RVertexBuffer VertexBuffer_ctor(str_view label, BufferUsage usage, const std::vector<vertex_t>& vertices) {
		RVertexBuffer hVertexBuffer = VertexBuffer_ctor(label, usage);
		VertexBuffer_upload(hVertexBuffer, vertices);
		return hVertexBuffer;
	}


	// ============================================================
	// Uniform Buffer API: needs converted into a raw descriptor tbh
	// ============================================================


	struct RUniformBuffer : Resource {};


	inline RUniformBuffer UniformBuffer_ctor(str_view label) {
		return RUniformBuffer(Buffer_ctor(label, BufferType::Uniform, BufferUsage::DynamicDraw));
	}

	inline bool UniformBuffer_dtor(const RUniformBuffer& hUniformBuffer) {
		return Buffer_dtor(RBuffer(hUniformBuffer));
	}
	// TODO: deprecate uniform buffer bindings at this layer of the RHI once descriptor sets and
	// friends are well-defined and proven to work. Might be worth removing 'slot'
	bool UniformBuffer_bind(const RUniformBuffer& hUBuf, uint8_t slot);


	inline bool UniformBuffer_upload(
		const RUniformBuffer& hUniformBuffer,
		const void* data,
		u32 count,
		u32 elem_size) {
		return Buffer_upload(RBuffer(hUniformBuffer), data, count, elem_size);
	}


	inline bool UniformBuffer_valid(const RUniformBuffer& hUniformBuffer) {
		return Buffer_valid(RBuffer(hUniformBuffer));
	}

	inline bool UniformBuffer_initialized(const RUniformBuffer& hUniformBuffer) {
		return Buffer_initialized(RBuffer(hUniformBuffer));
	}





}