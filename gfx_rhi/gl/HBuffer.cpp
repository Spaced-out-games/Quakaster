#include "HBuffer.h"
#include "glad/glad.h"
#include "core.h"

namespace qk::gfx::rhi {

	inline GLuint BufferUsage_cast(BufferUsage usage) {
		switch (usage)
		{
		case BufferUsage::StaticDraw:  return GL_STATIC_DRAW;
		case BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
		case BufferUsage::StreamDraw:  return GL_STREAM_DRAW;
		}

		throw std::runtime_error("Invalid BufferUsage");
	}

	inline GLenum BufferType_cast(BufferType type) {
		switch (type)
		{
		case BufferType::Vertex:  return GL_ARRAY_BUFFER;
		case BufferType::Index:   return GL_ELEMENT_ARRAY_BUFFER;
		case BufferType::Uniform: return GL_UNIFORM_BUFFER;
		}

		throw std::runtime_error("Invalid BufferType");
	}
	

	struct BufferEntry {
		GLuint			hBuffer		= 0;
		BufferType		type		= BufferType::Unknown;
		BufferUsage		usage		= BufferUsage::StaticDraw;
		uint32_t		elem_size	= 0;
		uint32_t		elem_count	= 0;
		bool initialized = false;
	};



	static inline qk::HandleResourceRegistry<BufferEntry, HBuffer> g_Buffers;


	HBuffer Buffer_ctor(BufferType type, BufferUsage usage) {

		BufferEntry entry = {
			.type = type,
			.usage = usage,
			.initialized = false
		};

		glGenBuffers(1, &entry.hBuffer);
		return g_Buffers.insert(entry);
	}

	bool Buffer_dtor(const HBuffer& hBuf) {
		QK_ASSERT(g_Buffers.valid(hBuf));
		if (!g_Buffers.valid(hBuf))
			return false;

		auto& entry = g_Buffers.get(hBuf);

		glDeleteBuffers(1, &entry.hBuffer);
		g_Buffers.destroy(hBuf);

		return true;
	}



	bool Buffer_upload(const HBuffer& hBuf, const void* pIndices, uint32_t count, uint32_t elem_size) {
		QK_ASSERT(g_Buffers.valid(hBuf));
		if (!g_Buffers.valid(hBuf)) return 0;
		auto& entry = g_Buffers.get(hBuf);
		entry.elem_count = count;
		entry.elem_size = elem_size;

		glBindBuffer(BufferType_cast(entry.type), entry.hBuffer);
		glBufferData(BufferType_cast(entry.type), count * elem_size, pIndices, BufferUsage_cast(entry.usage));
		entry.initialized = true;
		return 1;
	}

	bool Buffer_valid(const HBuffer& hBuf) {
		return g_Buffers.valid(hBuf);
	}

	bool Buffer_initialized(const HBuffer& hBuf) {
		if (!g_Buffers.valid(hBuf))
			return false;

		return g_Buffers.get(hBuf).initialized;
	}




	bool Buffer_bind(const HBuffer& hBuf) {
		QK_ASSERT(Buffer_initialized(hBuf));
		if (!Buffer_initialized(hBuf)) return 0;
		auto& entry = g_Buffers.get(hBuf);

		glBindBuffer(BufferType_cast(entry.type), entry.hBuffer);
		return 1;
	}



	BufferType Buffer_type(const HBuffer& hBuf) {
		if (!g_Buffers.valid(hBuf)) return BufferType::Unknown;
		return g_Buffers.get(hBuf).type;
	}

}