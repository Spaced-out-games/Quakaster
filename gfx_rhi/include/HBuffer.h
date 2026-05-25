#pragma once
#include <Handle.h>
#include <cstdint>
#include "core.h"
using qk::Handle;

namespace qk::gfx::rhi {

	enum class BufferUsage {
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

	struct HBuffer : Handle {};



	// ============================================================
	// Core API
	// ============================================================

	HBuffer Buffer_ctor(BufferType type, BufferUsage usage = BufferUsage::StaticDraw);

	bool Buffer_dtor(const HBuffer& hBuf);

	bool Buffer_upload(
		const HBuffer& hBuf,
		const void* data,
		u32 count,
		u32 elem_size);

	bool Buffer_valid(const HBuffer& hBuf);

	bool Buffer_initialized(const HBuffer& hBuf);

	BufferType Buffer_type(const HBuffer& hBuf);

	// ============================================================
	// Index Buffer API
	// ============================================================

	// using HIndexBuffer = HBuffer;

	struct HIndexBuffer: Handle {};

	inline HIndexBuffer IndexBuffer_ctor(BufferUsage usage = BufferUsage::StaticDraw) {
		return Handle_cast<HIndexBuffer>(Buffer_ctor(BufferType::Index, usage));
	}

	inline bool IndexBuffer_dtor(const HIndexBuffer& hIndexBuffer) {
		return Buffer_dtor(Handle_cast<HBuffer>(hIndexBuffer));
	}

	bool IndexBuffer_bind(const HIndexBuffer& hBuf);

	inline bool IndexBuffer_upload(
		const HIndexBuffer& hIndexBuffer,
		const void* data,
		u32 count,
		u32 elem_size) {
		return Buffer_upload(Handle_cast<HBuffer>(hIndexBuffer), data, count, elem_size);
	}

	inline bool IndexBuffer_valid(const HIndexBuffer& hIndexBuffer) {
		return Buffer_valid(Handle_cast<HBuffer>(hIndexBuffer));
	}

	inline bool IndexBuffer_initialized(const HIndexBuffer& hIndexBuffer) {
		return Buffer_initialized(Handle_cast<HBuffer>(hIndexBuffer));
	}

	// ============================================================
	// Vertex Buffer API
	// ============================================================


	struct HVertexBuffer : Handle {};

	inline HVertexBuffer VertexBuffer_ctor(BufferUsage usage = BufferUsage::StaticDraw) {
		return Handle_cast<HVertexBuffer>(Buffer_ctor(BufferType::Vertex, usage));
	}

	inline bool VertexBuffer_dtor(const HVertexBuffer& hVertexBuffer) {
		return Buffer_dtor(Handle_cast<HBuffer>(hVertexBuffer));
	}

	bool VertexBuffer_bind(const HVertexBuffer& hBuf);

	inline bool VertexBuffer_upload(
		const HVertexBuffer& hVertexBuffer,
		const void* data,
		u32 count,
		u32 elem_size) {
		return Buffer_upload(Handle_cast<HBuffer>(hVertexBuffer), data, count, elem_size);
	}

	inline bool VertexBuffer_valid(const HVertexBuffer& hVertexBuffer) {
		return Buffer_valid(Handle_cast<HBuffer>(hVertexBuffer));
	}

	inline bool VertexBuffer_initialized(const HVertexBuffer& hVertexBuffer) {
		return Buffer_initialized(Handle_cast<HBuffer>(hVertexBuffer));
	}

	// ============================================================
	// Uniform Buffer API
	// ============================================================


	struct HUniformBuffer : Handle {};


	inline HUniformBuffer UniformBuffer_ctor() {
		return Handle_cast<HUniformBuffer>(Buffer_ctor(BufferType::Uniform, BufferUsage::DynamicDraw));
	}

	inline bool UniformBuffer_dtor(const HUniformBuffer& hUniformBuffer) {
		return Buffer_dtor(Handle_cast<HBuffer>(hUniformBuffer));
	}
	// TODO: deprecate uniform buffer bindings at this layer of the RHI once descriptor sets and
	// friends are well-defined and proven to work. Might be worth removing 'slot'
	bool UniformBuffer_bind(const HUniformBuffer& hBuf, u8 slot);

	inline bool UniformBuffer_upload(
		const HUniformBuffer& hUniformBuffer,
		const void* data,
		u32 count,
		u32 elem_size) {
		return Buffer_upload(Handle_cast<HBuffer>(hUniformBuffer), data, count, elem_size);
	}


	inline bool UniformBuffer_valid(const HUniformBuffer& hUniformBuffer) {
		return Buffer_valid(Handle_cast<HBuffer>(hUniformBuffer));
	}

	inline bool UniformBuffer_initialized(const HUniformBuffer& hUniformBuffer) {
		return Buffer_initialized(Handle_cast<HBuffer>(hUniformBuffer));
	}




	// ============================================================
	// Storage Buffer API
	// ============================================================
	// using HStorageBuffer = HBuffer;





	// ============================================================
	// Instance Buffer API
	// ============================================================

	struct HInstanceBuffer : Handle {};


	inline HInstanceBuffer InstanceBuffer_ctor() {
		return Handle_cast<HInstanceBuffer>(Buffer_ctor(BufferType::Vertex, BufferUsage::StreamDraw));
	}

	inline bool InstanceBuffer_dtor(const HInstanceBuffer& hInstanceBuffer) {
		return Buffer_dtor(Handle_cast<HBuffer>(hInstanceBuffer));
	}

	bool InstanceBuffer_bind(const HInstanceBuffer& hBuf);


	inline bool InstanceBuffer_upload(const HInstanceBuffer& hInstanceBuffer, const void* data, u32 count, u32 elem_size) {
		return Buffer_upload(Handle_cast<HBuffer>(hInstanceBuffer), data, count, elem_size);
	}


	// TODO: deprecate any future buffer bindings at this layer of the RHI once descriptor sets and
	// friends are well-defined and proven.






}