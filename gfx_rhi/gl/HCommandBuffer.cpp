#include "HCommandBuffer.h"
#include <glad/glad.h>
#include <Resource.h>
#include <stdexcept>

namespace qk::gfx::rhi {

    struct CommandBuffer_impl : qk::Resource {
        std::vector<DrawCommand_desc> commands;
    };

    static inline ResourceRegistry<CommandBuffer_impl>
        g_CommandBuffers;

    static inline u64 g_CommandBufferCounter = 0;

    inline GLbitfield ClearChannels_cast(
        qk::gfx::rhi::ClearChannels channels)
    {
        GLbitfield result = 0;

        if ((channels & ClearChannels::Color) == ClearChannels::Color)
            result |= GL_COLOR_BUFFER_BIT;

        if ((channels & ClearChannels::Depth) == ClearChannels::Depth)
            result |= GL_DEPTH_BUFFER_BIT;

        if ((channels & ClearChannels::Stencil) == ClearChannels::Stencil)
            result |= GL_STENCIL_BUFFER_BIT;

        return result;
    }

    inline GLuint PrimitiveType_cast(
        PrimitiveTopology type)
    {
        switch (type)
        {
        case PrimitiveTopology::Triangle:
            return GL_TRIANGLES;

        case PrimitiveTopology::Lines:
            return GL_LINES;

        case PrimitiveTopology::Points:
            return GL_POINTS;

        case PrimitiveTopology::TriangleStrip:
            return GL_TRIANGLE_STRIP;

        case PrimitiveTopology::LineStrip:
            return GL_LINE_STRIP;

        default:
            return GL_TRIANGLES;
        }
    }

    bool DrawCommand_execute(
        const DrawCommand_desc& cmd)
    {
        switch (cmd.type)
        {
        case DrawCommandType::Null:
            return true;

        case DrawCommandType::Clear:

            glClearColor(
                cmd.clear.r,
                cmd.clear.g,
                cmd.clear.b,
                cmd.clear.a);

            glClear(
                ClearChannels_cast(
                    cmd.clear.channels));

            return true;

        case DrawCommandType::DrawArrays:

            if (!VertexArray_bind(
                cmd.drawArrays.mesh))
            {
                return false;
            }

            if (!Material_bind(
                cmd.drawArrays.material))
            {
                return false;
            }

            glDrawArrays(
                PrimitiveType_cast(
                    cmd.drawArrays.topology),
                cmd.drawArrays.offset,
                static_cast<GLsizei>(
                    cmd.drawArrays.num_vertices));

            return true;

        default:
            throw std::runtime_error(
                "Unhandled DrawCommandType");
        }
    }

    HCommandBuffer CommandBuffer_ctor(
        const CommandBuffer_recipe& desc)
    {
        CommandBuffer_impl impl;
        impl.commands = desc.commands();

        str label =
            "__CommandBuffer_" +
            std::to_string(
                g_CommandBufferCounter++);

        return g_CommandBuffers.insert(
            label,
            std::move(impl));
    }

    void CommandBuffer_dtor(
        HCommandBuffer& hCmdBuf)
    {
        if (!hCmdBuf)
            return;

        g_CommandBuffers.destroy(
            hCmdBuf);

        hCmdBuf = nullptr;
    }

    bool CommandBuffer_valid(
        const HCommandBuffer& hCmdBuf)
    {
        return g_CommandBuffers.valid(
            hCmdBuf);
    }

    bool CommandBuffer_execute(
        const HCommandBuffer& hCmdBuf)
    {
        if (!hCmdBuf)
            return false;

        if (!g_CommandBuffers.valid(
            hCmdBuf))
        {
            return false;
        }

        for (const auto& cmd :
            hCmdBuf->commands)
        {
            if (!DrawCommand_execute(cmd))
                return false;
        }

        return true;
    }

}