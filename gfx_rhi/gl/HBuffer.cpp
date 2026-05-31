#include "HBuffer.h"
#include <stdexcept>
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



	//static inline qk::HandleResourceRegistry<BufferEntry, HBuffer> g_Buffers;
	static rhi::ResourceCache<BufferEntry, RBuffer> g_Rbuffers;
	


	// new!
	RBuffer Buffer_ctor(const str_view& label, BufferType type, BufferUsage usage) {
		BufferEntry entry = {
			.type = type,
			.usage = usage,
			.initialized = false
		};
		glGenBuffers(1, &entry.hBuffer);
		return g_Rbuffers.insert(label, entry);

	}



	str_view Buffer_name(const RBuffer& hUniformBuffer) {
		return g_Rbuffers.label(hUniformBuffer);
	}


	// new!
	bool Buffer_dtor(const RBuffer& rBuf) {
		QK_ASSERT(g_Rbuffers.valid(rBuf));
		auto* pEntry = g_Rbuffers.try_get(rBuf);

		if (!pEntry) return false;

		glDeleteBuffers(1, &pEntry->hBuffer);
		g_Rbuffers.destroy(rBuf);
		return 1;

	}





	// new!
	bool Buffer_upload(const RBuffer& rBuf, const void* pIndices, u32 count, u32 size) {
		//QK_ASSERT(g_Rbuffers.valid(rBuf));
		auto* pEntry = g_Rbuffers.try_get(rBuf);
		if (!pEntry) return 0;
		pEntry->elem_count = count;
		pEntry->elem_size = size;
		pEntry->initialized = true;
		glBindBuffer(BufferType_cast(pEntry->type), pEntry->hBuffer);
		glBufferData(BufferType_cast(pEntry->type), count * size, pIndices, BufferUsage_cast(pEntry->usage));

		return 1;


	}

	// new!
	bool Buffer_valid(const RBuffer& rBuf) {
		auto res = g_Rbuffers.valid(rBuf);
		return res;
	}


	// new!
	bool Buffer_initialized(const RBuffer& hBuf) {
		auto* pEntry = g_Rbuffers.try_get(hBuf);
		if (!pEntry) return false;
		return pEntry->initialized;
	}


	
	bool VertexBuffer_bind(const RVertexBuffer& hVBuf) {
		RBuffer rBuf(hVBuf);

		auto* pEntry = g_Rbuffers.try_get(rBuf);

		if (!pEntry || !pEntry->initialized || pEntry->type != BufferType::Vertex) return false;

		glBindBuffer(GL_ARRAY_BUFFER, pEntry->hBuffer);

		return true;
	}




	bool IndexBuffer_bind(const RIndexBuffer& hIBuf) {
		RBuffer rBuf(hIBuf);

		auto* pEntry = g_Rbuffers.try_get(rBuf);

		if (!pEntry || !pEntry->initialized || pEntry->type != BufferType::Index) return false;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pEntry->hBuffer);

		return true;

	}

	// these should be moved to descriptors tbh


	bool UniformBuffer_bind(const RUniformBuffer& hUBuf, uint8_t slot) {
		RBuffer rBuf(hUBuf);

		auto* pEntry = g_Rbuffers.try_get(rBuf);

		if (!pEntry || !pEntry->initialized || pEntry->type != BufferType::Uniform) return false;
		
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, pEntry->hBuffer);

		return true;

	}



	



	BufferType Buffer_type(const RBuffer& hBuf) {
		auto* pEntry = g_Rbuffers.try_get(hBuf);
		if (!pEntry) return BufferType::Unknown;
		return pEntry->type;
	}

}