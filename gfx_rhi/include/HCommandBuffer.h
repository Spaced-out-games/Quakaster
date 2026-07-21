#pragma once
#include "DrawCommand.h"
#include "HPipeline.h"
#include <vector>
#include <unordered_set>
#include <span>

namespace qk::gfx::rhi {

    struct CommandBuffer_impl;
    using HCommandBuffer = CommandBuffer_impl*;
    class CommandBuffer_recipe;
    HCommandBuffer CommandBuffer_ctor(const CommandBuffer_recipe& recipe);

    void CommandBuffer_dtor(const HCommandBuffer& hCmdBuf);

    void CommandBuffer_execute(const HCommandBuffer& hCmdBuf);

    
    class CommandBuffer_recipe {
        std::vector<DrawCmd> m_DrawCommands;

        // keeps track of all the resources in use so we can put locks on them.
        std::unordered_set<Resource*> m_pResources;


        public:

            size_t size() const;

            CommandBuffer_recipe& bindPipeline(const HPipeline& hPipeline);

            CommandBuffer_recipe& bindVertexBuffer(const HVertexBuffer&, u8 slot, u32 offset = 0, u32 stride = 0);

            CommandBuffer_recipe& bindIndexBuffer(const HIndexBuffer& hIndexBuffer);

            // CommandBuffer_recipe& bindDescriptorSet(const HDescriptorSet& hDescriptorSet);

            CommandBuffer_recipe& drawArrays(u32 offset, u32 count);

            // get a list of all the resources this commandbuffer recipe is dependent on.
            const std::unordered_set<Resource*>& dependencies() const;

            const std::vector<DrawCmd>& commands() const;

    };


    

}