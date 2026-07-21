#pragma once
// We have to forward-declare the pipeline stuff to prevent circular dependencies.
// #include <HPipeline.h>

#include <core.h>

// NOTICE: These are meant to be draw command descriptions! 

namespace qk::gfx::rhi {

    // Pipeline implementation
    struct Pipeline_impl;

    // Pipeline handle
    using HPipeline = Pipeline_impl*;

    
    struct Buffer_impl;
    using HVertexBuffer = Buffer_impl*;
    using HIndexBuffer = Buffer_impl*;




    enum class EDrawCmdType {
        BindPipeline,
        BindVertexBuffer,
        BindIndexBuffer,
        // BindDescriptorSet, // not implemented yet.
        DrawArrays
    };

    struct BindPipelineCmd {
        HPipeline hPipeline = nullptr;
    };
    
    struct BindVertexBufferCmd {
        HVertexBuffer hVertexBuffer = nullptr;
        u32 offset = 0;
        u32 stride = 0;
        u8 slot = 0;
    };
    
    struct BindIndexBufferCmd {
        HIndexBuffer hIndexBuffer = nullptr;
    };

    struct DrawArraysCmd {
        u32 offset = 0;
        u32 count = 0;
    };



    struct DrawCmd {
        EDrawCmdType type;

        union {
            BindPipelineCmd     bindPipelineCmd;
            BindVertexBufferCmd bindVertexBufferCmd;
            BindIndexBufferCmd  bindIndexBufferCmd;
            DrawArraysCmd       drawArraysCmd;
        };

        DrawCmd(BindPipelineCmd bindPipelineCmd);
        DrawCmd(BindVertexBufferCmd bindVertexBufferCmd);
        DrawCmd(BindIndexBufferCmd bindIndexBufferCmd);
        DrawCmd(DrawArraysCmd drawArraysCmd);

    };

}