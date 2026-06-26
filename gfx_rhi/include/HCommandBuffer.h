#pragma once

#include "HMaterial.h"
#include "HVertexArray.h"

namespace qk::gfx::rhi {

    // What to draw with
    enum PrimitiveTopology {
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

    // null command description
    struct NullCommand_desc {};

    // Clear command description
    struct ClearCommand_desc {
        float r, g, b, a;
        ClearChannels channels;
    };

    // DrawArrays draw call description
    struct DrawArrays_desc {
        HMaterial material;
        HVertexArray mesh;
        PrimitiveTopology topology;
        int offset;
        size_t num_vertices;
    };

    // 
    enum class DrawCommandType : u8 {
        Null,
        Clear,
        DrawArrays
    };

    // draw call description
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

    // Recipe for command buffer
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

    // Commandbuffer implementation
    struct CommandBuffer_impl;

    // Commandbuffer handle
    using HCommandBuffer = CommandBuffer_impl*;

    // Commandbuffer constructor
    HCommandBuffer CommandBuffer_ctor(const CommandBuffer_recipe& desc);

    // Commandbuffer destructor
    void CommandBuffer_dtor(HCommandBuffer& hCmdBuf);

    // Commandbuffer validity check
    bool CommandBuffer_valid(const HCommandBuffer& hCmdBuf);

    // Execute commandbuffer. 
    bool CommandBuffer_execute(const HCommandBuffer& hCmdBuf);

}