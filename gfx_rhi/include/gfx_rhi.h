#pragma once
#include "core.h"
#include <stdint.h>

namespace qk::gfx::rhi {

	enum ClearChannels : uint32_t {
		Color =		1u << 0,
		Depth =		1u << 1,
		Stencil =	1u << 2
	};

	inline constexpr ClearChannels operator|(ClearChannels a, ClearChannels b) {
		return (ClearChannels)(
			(uint32_t)(a) | (uint32_t)(b)
			);
	}

	inline constexpr ClearChannels operator&(ClearChannels a, ClearChannels b) {
		return (ClearChannels)(
			(uint32_t)(a) & (uint32_t)(b)
			);
	}

	inline constexpr ClearChannels operator^(ClearChannels a, ClearChannels b) {
		return (ClearChannels)(
			(uint32_t)(a) ^ (uint32_t)(b)
			);
	}

	inline constexpr ClearChannels operator~(ClearChannels a) {
		return (ClearChannels)(
			~(uint32_t)(a)
			);
	}

	inline ClearChannels& operator|=(ClearChannels& a, ClearChannels b) {
		a = a | b;
		return a;
	}

	inline ClearChannels& operator&=(ClearChannels& a, ClearChannels b) {
		a = a & b;
		return a;
	}

	inline ClearChannels& operator^=(ClearChannels& a, ClearChannels b) {
		a = a ^ b;
		return a;
	}
	// something like glfwGetProcAddress
	int QK_API init(void* hLoader);

	void clear(float r, float g, float b, float a, ClearChannels channels);

	void clear();

}