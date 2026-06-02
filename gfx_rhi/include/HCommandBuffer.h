#pragma once

#include "HMaterial.h"
#include "HVertexArray.h"

namespace qk::gfx::rhi {

    enum PrimitiveTopology {
        Triangle,
        Lines,
        Points,
        TriangleStrip,
        LineStrip
    };

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

    struct NullCommand_desc {};

    struct ClearCommand_desc {
        float r, g, b, a;
        ClearChannels channels;
    };

    struct DrawArrays_desc {
        RMaterial material;
        HVertexArray mesh;
        PrimitiveTopology topology;
        int offset;
        size_t num_vertices;
    };

    enum class DrawCommandType : u8 {
        Null,
        Clear,
        DrawArrays
    };

    struct DrawCommand_desc {

        DrawCommandType type;

        union {
            NullCommand_desc null;
            ClearCommand_desc clear;
            DrawArrays_desc drawArrays;
        };

        DrawCommand_desc()
            : null()
            , type(DrawCommandType::Null)
        {}

        DrawCommand_desc(const NullCommand_desc& desc)
            : null(desc)
            , type(DrawCommandType::Null)
        {}

        DrawCommand_desc(const ClearCommand_desc& desc)
            : clear(desc)
            , type(DrawCommandType::Clear)
        {}

        DrawCommand_desc(const DrawArrays_desc& desc)
            : drawArrays(desc)
            , type(DrawCommandType::DrawArrays)
        {}
    };

    class CommandBuffer_recipe {

        std::vector<DrawCommand_desc> m_Commands;

    public:

        CommandBuffer_recipe& push(const DrawCommand_desc& cmd) {
            m_Commands.push_back(cmd);
            return *this;
        }

        const std::vector<DrawCommand_desc>& commands() const {
            return m_Commands;
        }
    };

    struct CommandBuffer_impl;

    using HCommandBuffer = CommandBuffer_impl*;

    HCommandBuffer CommandBuffer_ctor(const CommandBuffer_recipe& desc);

    void CommandBuffer_dtor(HCommandBuffer& hCmdBuf);

    bool CommandBuffer_valid(const HCommandBuffer& hCmdBuf);

    bool CommandBuffer_execute(const HCommandBuffer& hCmdBuf);

}