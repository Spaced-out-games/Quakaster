#pragma once
#include "HShaderProgram.h"
#include "HVertexBuffer.h"
#include "ResourcePool.h"
#include "HVertexArray.h"
#include "HIndexBuffer.h"
#include "HTexture.h"
#include "utility/fnv1a.h"

#include <entt/core/hashed_string.hpp>

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

		template <size_t N>
		void registerShaderProgram(const char(&value)[N], HShaderProgram&& hShaderProgram) {
			m_ShaderPrograms.insert(qk::hash_fnv1a(value), std::move(hShaderProgram));
		}


		void registerVertexBuffer(uint64_t id, HVertexBuffer&& hVertexBuffer) {
			m_VertexBuffers.insert(id, std::move(hVertexBuffer));
		}

		template <size_t N>
		void registerVertexBuffer(const char(&value)[N], HVertexBuffer&& hVertexBuffer) {
			m_VertexBuffers.insert(qk::hash_fnv1a(value), std::move(hVertexBuffer));
		}

		void registerIndexBuffer(uint64_t id, HIndexBuffer&& hIndexBuffer) {
			m_IndexBuffers.insert(id, std::move(hIndexBuffer));
		}

		template <size_t N>
		void registerIndexBuffer(const char(&value)[N], HIndexBuffer&& hIndexBuffer) {
			m_IndexBuffers.insert(qk::hash_fnv1a(value), std::move(hIndexBuffer));
		}

		void registerVertexArray(uint64_t id, HVertexArray&& hVertexArray) {
			m_VertexArrays.insert(id, std::move(hVertexArray));
		}

		template <size_t N>
		void registerVertexArray(const char(&value)[N], HVertexArray&& hVertexArray) {
			m_VertexArrays.insert(qk::hash_fnv1a(value), std::move(hVertexArray));
		}

		void registerTexture(uint64_t id, HTexture&& hTexture) {
			m_Textures.insert(id, std::move(hTexture));
		}

		template <size_t N>
		void registerTexture(const char(&value)[N], HTexture&& hTexture) {
			m_Textures.insert(qk::hash_fnv1a(value), std::move(hTexture));
		}
	};

}