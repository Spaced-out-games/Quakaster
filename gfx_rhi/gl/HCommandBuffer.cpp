#include "HCommandBuffer.h"
#include <glad/glad.h>
#include <Resource.h>
#include <stdexcept>
#include "rhi_globals.h"

namespace qk::gfx::rhi {


    bool DrawCommand_execute(const DrawCommand_desc& cmd)
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

            if (!VertexArray_bind(cmd.drawArrays.mesh))
                return false;

            if (!Material_bind(cmd.drawArrays.material))
                return false;

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



    CommandBuffer_impl::CommandBuffer_impl(const CommandBuffer_recipe& desc) {
        commands = desc.commands();
    }



    bool CommandBuffer_impl::destroy() {
        return true;
    }

    bool CommandBuffer_impl::execute() {
        for (const auto& cmd : commands)
        {
            if (!DrawCommand_execute(cmd)) return false;
        }
        return true;
    }


    




    // API impleementation


    HCommandBuffer CommandBuffer_ctor(const CommandBuffer_recipe& desc)
    {

        CommandBuffer_impl* pCmds = new CommandBuffer_impl(desc);

        return pCmds;

    }

    void CommandBuffer_dtor(HCommandBuffer& hCmdBuf)
    {
        if (!hCmdBuf)return;

        delete hCmdBuf;

        hCmdBuf = nullptr;
    }

    bool CommandBuffer_valid(const HCommandBuffer& hCmdBuf)
    {
        return true;//g_CommandBuffers.valid(hCmdBuf);
    }

    bool CommandBuffer_execute(const HCommandBuffer& hCmdBuf)
    {
        if (!hCmdBuf) return false;

        return hCmdBuf->execute();

    }

}