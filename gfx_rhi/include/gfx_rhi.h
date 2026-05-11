#pragma once
#include "core.h"
#include <stdint.h>

namespace qk::gfx::rhi {
	enum class ClearChannels : uint32_t {
		COLOR =		1u << 0,
		DEPTH =		1u << 1,
		STENCIL =	1u << 2
	};

	inline constexpr ClearChannels operator|(ClearChannels a, ClearChannels b) {
		return static_cast<ClearChannels>(
			static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
			);
	}

	inline constexpr ClearChannels operator&(ClearChannels a, ClearChannels b) {
		return static_cast<ClearChannels>(
			static_cast<uint32_t>(a) & static_cast<uint32_t>(b)
			);
	}

	inline constexpr ClearChannels operator^(ClearChannels a, ClearChannels b) {
		return static_cast<ClearChannels>(
			static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b)
			);
	}

	inline constexpr ClearChannels operator~(ClearChannels a) {
		return static_cast<ClearChannels>(
			~static_cast<uint32_t>(a)
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
	int QK_API init(void* procLoader);

	void clear(float r, float g, float b, float a, ClearChannels channels);

	void clear();

}