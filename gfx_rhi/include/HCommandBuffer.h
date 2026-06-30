#pragma once

//#include "HVertexArray.h"

namespace qk::gfx::rhi {

    // What to draw with
    enum class PrimitiveTopology {
        Unknown,
        Triangle,
        Lines,
        Points,
        TriangleStrip,
        LineStrip
    };

    // Which channels to clear
    enum ClearChannels : uint32_t {
        Color = 1u << 0,
        Depth = 1u << 1,
        Stencil = 1u << 2
    };

    inline constexpr ClearChannels operator|(ClearChannels a, ClearChannels b) {
        return (ClearChannels)((uint32_t)a | (uint32_t)b);
    }

    inline constexpr ClearChannels operator&(ClearChannels a, ClearChannels b) {
        return (ClearChannels)((uint32_t)a & (uint32_t)b);
    }

    inline constexpr ClearChannels operator^(ClearChannels a, ClearChannels b) {
        return (ClearChannels)((uint32_t)a ^ (uint32_t)b);
    }

    inline constexpr ClearChannels operator~(ClearChannels a) {
        return (ClearChannels)(~(uint32_t)a);
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


    class CommandBuffer_recipe {

    };


}