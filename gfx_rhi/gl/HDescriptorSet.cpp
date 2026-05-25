#include "HDescriptorSet.h"
#include <glad/glad.h>

namespace qk::gfx::rhi {

	using Texture_impl = Texture_desc;

	using UniformBuffer_impl = UniformBuffer_desc;



	using DescriptorSet_impl = std::vector<Descriptor>;

	qk::HandleResourceRegistry<DescriptorSet_impl, HDescriptorSet> g_DescriptorSets;

	HDescriptorSet DescriptorSet_ctor(const DescriptorSet_createinfo& info) {
		return g_DescriptorSets.insert(info);
	}

	void DescriptorSet_dtor(const HDescriptorSet& hDescriptorSet) {

		QK_ASSERT(g_DescriptorSets.valid(hDescriptorSet));

		g_DescriptorSets.destroy(hDescriptorSet);

	}

	bool DescriptorSet_bind(const HDescriptorSet& hDescriptorSet) {

		QK_ASSERT(g_DescriptorSets.valid(hDescriptorSet));

		const auto& DescriptorSet = g_DescriptorSets.get(hDescriptorSet);

		//u8 binding = 0;
		bool result = 0;
		for (auto& descriptor : DescriptorSet) {

			descriptor.visit([&result](auto&& desc) {
				using T = std::decay_t<decltype(desc)>;

				if constexpr (std::is_same_v<T, Texture_desc>) {
					QK_ASSERT(Texture_valid(desc.hTex));
					if (!Texture_valid(desc.hTex)) { result = 0; return; }
					Texture_bind(desc.hTex, desc.binding);
				}
				else if constexpr (std::is_same_v<T, UniformBuffer_desc>) {
					const auto hUBuf = Handle_cast<HUniformBuffer>(desc.hBuf);
					QK_ASSERT(UniformBuffer_valid(Handle_cast<HUniformBuffer>(desc.hBuf)));
					if (!UniformBuffer_valid(hUBuf)) { result = 0; return; }
					UniformBuffer_bind(hUBuf, desc.binding);
				}
				else if constexpr (std::is_same_v<T, StorageBuffer_desc>) {
					// QK_ASSERT(StorageBuffer_valid(desc.hBuf));
					//if (!StorageBuffer_valid(desc.hBuf)) { result = 0; return; }

					throw std::runtime_error("SSBO not implemented.");
				}

				result = 1;

			}
			);

			if (!result) return 0;

			//descriptor.
		}

		return 1;

	}


	bool DescriptorSet_valid(const HDescriptorSet& hDescriptorSet) {
		if (!g_DescriptorSets.valid(hDescriptorSet)) return 0;

		const auto& DescriptorSet = g_DescriptorSets.get(hDescriptorSet);

		bool result = 0;

		for (auto& descriptor : DescriptorSet) {

				
			descriptor.visit([&result](auto&& desc) {
				using T = std::decay_t<decltype(desc)>;

			if constexpr (std::is_same_v<T, Texture_desc>) {
				if (!Texture_valid(desc.hTex)) { result = 0; return; }
				}
				else if constexpr (std::is_same_v<T, UniformBuffer_desc>) {
					const auto hUBuf = Handle_cast<HUniformBuffer>(desc.hBuf);
					if (!UniformBuffer_valid(hUBuf)) { result = 0; return; }
				}
				else if constexpr (std::is_same_v<T, StorageBuffer_desc>) {
					//if (!StorageBuffer_valid(desc.hBuf)) { result = 0; return; }

					throw std::runtime_error("SSBO not implemented.");
				}

				result = 1;
				
			}
			);

			if (!result) return 0;

			//descriptor.
		}

		return 1;

	}
	
	void DescriptorSet_bindProgram(const HDescriptorSet& hDescriptorSet, const HShaderProgram& hProgram) {
		if (!g_DescriptorSets.valid(hDescriptorSet)) return;

		const auto& set = g_DescriptorSets.get(hDescriptorSet);

		for (const auto& desc : set) {
			desc.visit([&](auto&& desc) {
				using T = std::decay_t<decltype(desc)>;

			if constexpr (std::is_same_v<T, Texture_desc>) {
				if (!Texture_valid(desc.hTex)) { return; }
			}
			else if constexpr (std::is_same_v<T, UniformBuffer_desc>) {
				const auto& hUBuf = Handle_cast<HUniformBuffer>(desc.hBuf);
				if (!UniformBuffer_valid(hUBuf)) {

					return;
					
				}
				ShaderProgram_bind_uniform_block(hProgram, desc.id, desc.binding);

			}
			else if constexpr (std::is_same_v<T, StorageBuffer_desc>) {
				//if (!StorageBuffer_valid(desc.hBuf)) { result = 0; return; }

				throw std::runtime_error("SSBO not implemented.");
			}


			}
			);
		}
	}




}