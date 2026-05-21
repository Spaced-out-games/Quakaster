#pragma once
#include <Handle.h>
#include <cstdint>

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

	using HIndexBuffer = HBuffer;
	using HVertexBuffer = HBuffer;
	using HUniformBuffer = HBuffer;
	using HStorageBuffer = HBuffer;

	// ============================================================
	// Core API
	// ============================================================

	HBuffer Buffer_ctor(BufferType type, BufferUsage usage = BufferUsage::StaticDraw);

	bool Buffer_dtor(const HBuffer& hBuf);

	bool Buffer_upload(
		const HBuffer& hBuf,
		const void* data,
		uint32_t count,
		uint32_t elem_size);

	bool Buffer_bind(const HBuffer& hBuf);

	bool Buffer_valid(const HBuffer& hBuf);

	bool Buffer_initialized(const HBuffer& hBuf);

	BufferType Buffer_type(const HBuffer& hBuf);

	// ============================================================
	// Index Buffer API
	// ============================================================

	inline HIndexBuffer IndexBuffer_ctor(BufferUsage usage = BufferUsage::StaticDraw) {
		return Buffer_ctor(BufferType::Index, usage);
	}

	inline bool IndexBuffer_dtor(const HIndexBuffer& hIndexBuffer) {
		return Buffer_dtor(hIndexBuffer);
	}

	inline bool IndexBuffer_bind(const HIndexBuffer& hIndexBuffer) {
		return Buffer_bind(hIndexBuffer);
	}

	inline bool IndexBuffer_upload(
		const HIndexBuffer& hIndexBuffer,
		const void* data,
		uint32_t count,
		uint32_t elem_size) {
		return Buffer_upload(hIndexBuffer, data, count, elem_size);
	}

	inline bool IndexBuffer_valid(const HIndexBuffer& hIndexBuffer) {
		return Buffer_valid(hIndexBuffer);
	}

	inline bool IndexBuffer_initialized(const HIndexBuffer& hIndexBuffer) {
		return Buffer_initialized(hIndexBuffer);
	}

	// ============================================================
	// Vertex Buffer API
	// ============================================================

	inline HVertexBuffer VertexBuffer_ctor(BufferUsage usage = BufferUsage::StaticDraw) {
		return Buffer_ctor(BufferType::Vertex, usage);
	}

	inline bool VertexBuffer_dtor(const HVertexBuffer& hVertexBuffer) {
		return Buffer_dtor(hVertexBuffer);
	}

	inline bool VertexBuffer_bind(const HVertexBuffer& hVertexBuffer) {
		return Buffer_bind(hVertexBuffer);
	}

	inline bool VertexBuffer_upload(
		const HVertexBuffer& hVertexBuffer,
		const void* data,
		uint32_t count,
		uint32_t elem_size) {
		return Buffer_upload(hVertexBuffer, data, count, elem_size);
	}

	inline bool VertexBuffer_valid(const HVertexBuffer& hVertexBuffer) {
		return Buffer_valid(hVertexBuffer);
	}

	inline bool VertexBuffer_initialized(const HVertexBuffer& hVertexBuffer) {
		return Buffer_initialized(hVertexBuffer);
	}

}