#include "HCommandBuffer.h"
#include "rhi_globals.h"
#include <cassert>

using qk::gfx::rhi::EDrawCmdType;

namespace qk::gfx::rhi {
    
    // commandbuffer builder.

    size_t CommandBuffer_recipe::size() const {
        return m_DrawCommands.size();
    }




    CommandBuffer_recipe& CommandBuffer_recipe::bindPipeline(const HPipeline& hPipeline) {
        if (!hPipeline)
            return *this;
        m_DrawCommands.emplace_back(
            DrawCmd{BindPipelineCmd{hPipeline}}
        );
        m_pResources.insert((Resource*)hPipeline);
        return *this;
    }

    CommandBuffer_recipe& CommandBuffer_recipe::bindVertexBuffer(const HVertexBuffer& hVertexBuffer, u8 slot, u32 offset, u32 stride) {
        if (!hVertexBuffer)
            return *this;
        m_DrawCommands.emplace_back(
            DrawCmd{BindVertexBufferCmd{hVertexBuffer,offset, stride, slot}}
        );
        m_pResources.insert((Resource*)hVertexBuffer);
        return *this;
    }

    CommandBuffer_recipe& CommandBuffer_recipe::bindIndexBuffer(const HIndexBuffer& hIndexBuffer) {
        assert(hIndexBuffer);


        m_DrawCommands.emplace_back(
            DrawCmd{BindIndexBufferCmd{hIndexBuffer}}
        );
        m_pResources.insert((Resource*)hIndexBuffer);
        return *this;
    }

    CommandBuffer_recipe& CommandBuffer_recipe::bindDescriptorSet(HDescriptorSet hDescriptorSet, u8 slot) {
        assert(slot < RHI_MAX_DESCRIPTORSETS_BOUND);
        assert(hDescriptorSet);

        m_DrawCommands.emplace_back(
            DrawCmd{ BindDescriptorSetCmd{hDescriptorSet, slot} }
        );
        m_pResources.insert((Resource*)hDescriptorSet);

        return *this;
    }

    const std::unordered_set<Resource*>& CommandBuffer_recipe::dependencies() const {
        return m_pResources;
    }

    const std::vector<DrawCmd>& CommandBuffer_recipe::commands() const {
        return m_DrawCommands;
    }


    CommandBuffer_recipe& CommandBuffer_recipe::drawArrays(u32 offset, u32 numVertices) {

        m_DrawCommands.emplace_back(
            DrawCmd{DrawArraysCmd{offset, numVertices}}
        );

        return *this;
    }

    CommandBuffer_recipe& CommandBuffer_recipe::drawArraysInstanced(u32 offset, u32 numVertices, u32 numInstances) {

        m_DrawCommands.emplace_back(
            DrawCmd{ DrawArraysInstancedCmd{offset, numVertices, numInstances} }
        );

        return *this;
    }


    CommandBuffer_impl::CommandBuffer_impl(const std::unordered_set<Resource*>& resources, const std::vector<DrawCmd>& commands) :
        dependencies(resources),
        draw_commands(commands),
        Resource("CommandBuffer_impl") {

        for (Resource* pDependency : dependencies) {
            pDependency->acquire();
        }


    }

    void CommandBuffer_impl::destroy() {
        for (Resource* pDependency : dependencies) {
            pDependency->release();
        }
    }

    Resource* Resource_cast(const HCommandBuffer& hCmdBuf) {
        return static_cast<Resource*>(hCmdBuf);
    }

    HCommandBuffer CommandBuffer_ctor(const CommandBuffer_recipe& recipe) {
        return new CommandBuffer_impl(recipe.dependencies(), recipe.commands());


    }

    void CommandBuffer_dtor(const HCommandBuffer& hCmdBuf) {
        assert(hCmdBuf);
        assert(hCmdBuf->refcount() == 0 && hCmdBuf->flagged());
        
        // since commandbuffers are entirely emulated in GL using standard types, 'destruction'
        // is trivial:

        for (auto* pResource : hCmdBuf->dependencies) {
            pResource->release();
        }

        hCmdBuf->dependencies.clear();
        hCmdBuf->draw_commands.clear();
        hCmdBuf->topology = PrimitiveTopology::Unknown;

    }

    void CommandBuffer_free(HCommandBuffer& hCmdBuf) {

        assert(hCmdBuf);
        assert(hCmdBuf->refcount() == 0 && hCmdBuf->flagged());

        delete hCmdBuf;
        hCmdBuf = nullptr;
    }





    static void DrawCmd_execute(
        const DrawCmd& drawCmd,
        HCommandBuffer hCmdBuf,
        DescriptorSetsContext& ctx,
        HDescriptorSet(&hDescriptorSets)[RHI_MAX_DESCRIPTORSETS_BOUND]
    )
    {
        const auto& bindVertexBufferCmd = drawCmd.bindVertexBufferCmd;
        const auto& bindIndexBufferCmd = drawCmd.bindIndexBufferCmd;
        const auto& bindPipelineCmd = drawCmd.bindPipelineCmd;
        const auto& drawArraysCmd = drawCmd.drawArraysCmd;
        const auto& drawArraysInstancedCmd = drawCmd.drawArraysInstancedCmd;
        const auto& type = drawCmd.type;

        switch (type)
        {
        case EDrawCmdType::BindPipeline:
            bindPipelineCmd.hPipeline->bind();
            hCmdBuf->topology = bindPipelineCmd.hPipeline->topology;
            break;

        case EDrawCmdType::BindVertexBuffer:
            bindVertexBufferCmd.hVertexBuffer->VertexBuffer_bind(bindVertexBufferCmd.slot,bindVertexBufferCmd.offset, bindVertexBufferCmd.stride);
            break;

        case EDrawCmdType::BindIndexBuffer:
            bindIndexBufferCmd.hIndexBuffer->IndexBuffer_bind();
            break;

        case EDrawCmdType::DrawArrays:
            // NOTE: The scene compiler might emit this command when the instancebuffer is empty, leading to UB.
            __debugbreak();
            glDrawArrays(
                PrimitiveType_cast(hCmdBuf->topology),
                drawArraysCmd.offset,
                drawArraysCmd.count
            );
            break;

        case EDrawCmdType::DrawArraysInstanced:
            glDrawArraysInstanced(
                PrimitiveType_cast(hCmdBuf->topology),
                drawArraysInstancedCmd.offset,
                drawArraysInstancedCmd.numVertices,
                drawArraysInstancedCmd.numInstances
            );
            break;

        case EDrawCmdType::BindDescriptorSet:
        {
            const auto& cmd = drawCmd.bindDescriptorSetCmd;

            assert(cmd.slot < RHI_MAX_DESCRIPTORSETS_BOUND);
            assert(cmd.set);

            hDescriptorSets[cmd.slot] = cmd.set;

            // rebuild context
            ctx = {};

            for (size_t i = 0; i < cmd.slot; ++i)
            {
                if (hDescriptorSets[i])
                    ctx.next(hDescriptorSets[i]);
            }

            hDescriptorSets[cmd.slot]->bind(ctx);
            break;
        }

        default:
            break;
        }
    }





    // Maybe consider moving this to implementation and wrapping it here instead!
    void CommandBuffer_execute(const HCommandBuffer& hCmdBuf)
    {
        assert(hCmdBuf);

        DescriptorSetsContext ctx{};
        HDescriptorSet hDescriptorSets[RHI_MAX_DESCRIPTORSETS_BOUND]{};

        for (const auto& drawCmd : hCmdBuf->draw_commands)
        {
            DrawCmd_execute(
                drawCmd,
                hCmdBuf,
                ctx,
                hDescriptorSets
            );
        }
    }



}