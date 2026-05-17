#pragma once
#include "HShaderProgram.h"
#include "HVertexBuffer.h"
#include "ResourcePool.h"
#include "HVertexArray.h"
#include "HIndexBuffer.h"
#include <string_view>
#include "HTexture.h"
#include "fnv1a.h"


using namespace qk::gfx::rhi;

namespace qk::gfx {

	struct GraphicsResourceRegistry {
		ShaderProgramRegistry m_ShaderPrograms;
		VertexBufferRegistry m_VertexBuffers;
		IndexBufferRegistry m_IndexBuffers;
		VertexArrayRegistry m_VertexArrays;
		TextureRegistry m_Textures;

		void registerShaderProgram(uint64_t id, HShaderProgram&& hShaderProgram) {
			m_ShaderPrograms.insert(id, std::move(hShaderProgram));
		}


		void registerShaderProgram(std::string_view label, HShaderProgram&& hShaderProgram) {
			m_ShaderPrograms.insert(hash_fnv1a(label), std::move(hShaderProgram));
		}


		void registerVertexBuffer(uint64_t id, HVertexBuffer&& hVertexBuffer) {
			m_VertexBuffers.insert(id, std::move(hVertexBuffer));
		}
		void registerVertexBuffer(std::string_view label, HVertexBuffer&& hVertexBuffer) {
			m_VertexBuffers.insert(hash_fnv1a(label), std::move(hVertexBuffer));
		}

		void registerIndexBuffer(uint64_t id, HIndexBuffer&& hIndexBuffer) {
			m_IndexBuffers.insert(id, std::move(hIndexBuffer));
		}

		
		void registerIndexBuffer(std::string_view label, HIndexBuffer&& hIndexBuffer) {
			m_IndexBuffers.insert(hash_fnv1a(label), std::move(hIndexBuffer));
		}

		void registerVertexArray(uint64_t id, HVertexArray&& hVertexArray) {
			m_VertexArrays.insert(id, std::move(hVertexArray));
		}

		void registerVertexArray(std::string_view label, HVertexArray&& hVertexArray) {
			m_VertexArrays.insert(hash_fnv1a(label), std::move(hVertexArray));
		}

		void registerTexture(uint64_t id, HTexture&& hTexture) {
			m_Textures.insert(id, std::move(hTexture));
		}

		void registerTexture(std::string_view label, HTexture&& hTexture) {
			m_Textures.insert(hash_fnv1a(label), std::move(hTexture));
		}
	};

}