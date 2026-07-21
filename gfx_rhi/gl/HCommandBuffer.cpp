#include "HCommandBuffer.h"
#include "rhi_globals.h"

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
        if(!hIndexBuffer)  
            return *this;

        m_DrawCommands.emplace_back(
            DrawCmd{BindIndexBufferCmd{hIndexBuffer}}
        );
        m_pResources.insert((Resource*)hIndexBuffer);
        return *this;
    }

    const std::unordered_set<Resource*>& CommandBuffer_recipe::dependencies() const {
        return m_pResources;
    }

    const std::vector<DrawCmd>& CommandBuffer_recipe::commands() const {
        return m_DrawCommands;
    }


    CommandBuffer_recipe& CommandBuffer_recipe::drawArrays(u32 offset, u32 count) {

        m_DrawCommands.emplace_back(
            DrawCmd{DrawArraysCmd{offset, count}}
        );

        return *this;
    }



    HCommandBuffer CommandBuffer_ctor(const CommandBuffer_recipe& recipe) {
        CommandBuffer_impl* pCmdBuf = new CommandBuffer_impl(recipe.dependencies(), recipe.commands());


        return pCmdBuf;
    }

    void CommandBuffer_dtor(const HCommandBuffer& hCmdBuf) {
        


    }

    void CommandBuffer_execute(const HCommandBuffer& hCmdBuf) {

        assert(hCmdBuf != nullptr);
        for (const auto& drawCmd : hCmdBuf->draw_commands) {

            const auto& bindVertexBufferCmd = drawCmd.bindVertexBufferCmd;
            const auto& bindIndexBufferCmd = drawCmd.bindIndexBufferCmd;
            const auto& bindPipelineCmd = drawCmd.bindPipelineCmd;
            const auto& drawArraysCmd = drawCmd.drawArraysCmd;
            const auto& type = drawCmd.type;

            
            switch (type)
            {
            case qk::gfx::rhi::EDrawCmdType::BindPipeline:
                Pipeline_bind(bindPipelineCmd.hPipeline);
                hCmdBuf->topology = bindPipelineCmd.hPipeline->topology;
                break;
            case qk::gfx::rhi::EDrawCmdType::BindVertexBuffer:
                VertexBuffer_bind(
                    bindVertexBufferCmd.hVertexBuffer,
                    bindVertexBufferCmd.slot,
                    bindVertexBufferCmd.offset,
                    bindVertexBufferCmd.stride
                );
                break;
            case qk::gfx::rhi::EDrawCmdType::BindIndexBuffer:
                IndexBuffer_bind(bindIndexBufferCmd.hIndexBuffer);
                break;
            case qk::gfx::rhi::EDrawCmdType::DrawArrays:
                glDrawArrays(PrimitiveType_cast(hCmdBuf->topology), drawArraysCmd.offset, drawArraysCmd.count);
                break;
            default:
                break;
            }
        }

    }

    CommandBuffer_impl::CommandBuffer_impl(const std::unordered_set<Resource*>& resources, const std::vector<DrawCmd>& commands): dependencies(resources), draw_commands(commands) {

        for (Resource* pDependency : dependencies) {
            pDependency->acquire();
        }


    }

    void CommandBuffer_impl::destroy() {
        for (Resource* pDependency : dependencies) {
            pDependency->release();
        }
    }

}